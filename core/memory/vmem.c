/**
 ** Copyright (c) 2017 "Ian Laird"
 ** Research Project Operating System (rpos) - https://github.com/en0/rpos
 ** 
 ** This file is part of rpos
 ** 
 ** rpos is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 ** 
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 ** 
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

/*
 * DISCLAIMER: I have no idea how this is going to work with more then 1 task.
 * Infact i know it is not, but since i don't know what i am going to need in
 * that case, i am going to keep this code in place untill i better understand
 * what i am missing.
 */

#include <string.h>
#include <kconfig.h>
#include <rpos/utils.h>
#include <rpos/bitmap.h>

#include "../interrupt/fault.h"
#include "pmem.h"
#include "vmem.h"

#define VMEM_PF_P_MASK    0b00001
#define VMEM_PF_RW_MASK   0b00010
#define VMEM_PF_US_MASK   0b00100
#define VMEM_PF_RSVD_MASK 0b01000
#define VMEM_ID_RSVD_MASK 0b10000

#define BITMAP_ARR_LEN (COMPUTE_BITMAP_WORD_CNT(0x100000))

bitmap_word_t g_vmem_map_data[BITMAP_ARR_LEN];
bitmap_info_t g_vmem_map = { BITMAP_ARR_LEN, g_vmem_map_data };

uint32_t* g_vmem_page_dir;
uint32_t* g_vmem_page_tbl = VIRT_ADDR_PGPTE;

/* PROTO */
void _lock_region(void* addr, size_t size);
void _initialize_map_region(vmem_map_info_t* region);
void _page_fault_handler(regs_t* reg);

#define is_pf_access_violation(e) is_flag_set(e, VMEM_PF_P_MASK)
#define is_pf_user_mode(e) is_flag_set(e, VMEM_PF_US_MASK)

static inline uint32_t get_cr3() {
    uint32_t addr;
    asm volatile( "mov %%cr2, %0" : "=r"(addr));
    return addr;
}

static inline void _invalidate_page_table_entry(uint32_t* addr) {
   asm volatile(
        "invlpg (%0)"
        : 
        :"r"(addr)
        : "memory");
}

static inline void _invalidate_page_table() {
    asm volatile(
        "mov %cr3, %eax;"
        "mov %eax, %cr3;" );
}

void printPDE(uint32_t *v) {
    
#ifdef PROFILE_DEBUG

    uint32_t dir_index = (uint32_t)v >> 22;
    //uint32_t tbl_index = _vmap_enabled ? (uint32_t)v >> 12 : ((uint32_t)v >> 12) & 0x3FF;
    //uint32_t* tbl = _vmap_enabled ? _vmap_tbl : (uint32_t*)(g_vmem_page_dir[dir_index] & 0xFFFFF000);
    uint32_t tbl_index = ((uint32_t)v >> 12) & 0x3FF;
    uint32_t* tbl = (uint32_t*)(g_vmem_page_dir[dir_index] & 0xFFFFF000);

    dbglogf("(%p)->vmap_dir[%i] = %p\n", v, dir_index, g_vmem_page_dir[dir_index]);
    if((g_vmem_page_dir[dir_index] & VMEM_FLG_PRESENT) == VMEM_FLG_PRESENT) {
        dbglogf(" PDE> PT Physical Address: %p\n", g_vmem_page_dir[dir_index] & 0xFFFFF000);
        dbglogf(" PDE> FLAGS: %s%s%s%s%s%s%s%s (%b)\n", 
            is_flag_set(g_vmem_page_dir[dir_index], VMEM_FLG_PRESENT) ? "PRESENT " : "",
            is_flag_set(g_vmem_page_dir[dir_index], VMEM_FLG_WRITABLE) ? "WRITEABLE " : "",
            is_flag_set(g_vmem_page_dir[dir_index], VMEM_FLG_USER) ? "USER " : "",
            is_flag_set(g_vmem_page_dir[dir_index], VMEM_FLG_WRITETHROUGH) ? "WRITETHROUGH " : "",
            is_flag_set(g_vmem_page_dir[dir_index], VMEM_FLG_NOCACHE) ? "NOCACHE " : "",
            is_flag_set(g_vmem_page_dir[dir_index], VMEM_FLG_ACCESSED) ? "ACCESSED " : "",
            is_flag_set(g_vmem_page_dir[dir_index], VMEM_FLG_LARGE) ? "LARGE " : "",
            is_flag_set(g_vmem_page_dir[dir_index], VMEM_FLG_GLOBAL) ? "GLOBAL " : "",
            g_vmem_page_dir[dir_index] & 0x00000FFF
        );

        dbglogf(" PTE> PG Physical Address: %p\n", tbl[tbl_index] & 0xFFFFF000);
        dbglogf(" PTE> FLAGS: %s%s%s%s%s%s%s%s (%b)\n", 
            is_flag_set(tbl[tbl_index], VMEM_FLG_PRESENT) ? "PRESENT " : "",
            is_flag_set(tbl[tbl_index], VMEM_FLG_WRITABLE) ? "WRITEABLE " : "",
            is_flag_set(tbl[tbl_index], VMEM_FLG_USER) ? "USER " : "",
            is_flag_set(tbl[tbl_index], VMEM_FLG_WRITETHROUGH) ? "WRITETHROUGH " : "",
            is_flag_set(tbl[tbl_index], VMEM_FLG_NOCACHE) ? "NOCACHE " : "",
            is_flag_set(tbl[tbl_index], VMEM_FLG_ACCESSED) ? "ACCESSED " : "",
            is_flag_set(tbl[tbl_index], VMEM_FLG_DIRTY) ? "DIRTY " : "",
            is_flag_set(tbl[tbl_index], VMEM_FLG_GLOBAL) ? "GLOBAL " : "",
            tbl[tbl_index] & 0x00000FFF
        );
        /*
        */
    } else {
        dbglogf(" PDE> PT NOT Present\n");
    }

    dbglogf("\n");

#endif

}

void vmem_initialize_paging(vmem_map_info_t* regions, size_t map_cnt) {

    uint32_t dir_index = addr_to_directory_index(VIRT_ADDR_PGPDE);

    g_vmem_page_dir = pmem_allocate_frame();
    memset(g_vmem_page_dir, 0x00, 1024*sizeof(uint32_t));

    g_vmem_page_dir[dir_index] = 
        (uint32_t)g_vmem_page_dir | VMEM_FLG_PRESENT | VMEM_FLG_WRITABLE | VMEM_FLG_GLOBAL;

    // Fill all page directory entries with a valid reference to a page table.
    uint32_t i, *tbl;
    for(i = 0; i < dir_index; i++) {
        tbl = pmem_allocate_frame();
        g_vmem_page_dir[i] = (uint32_t)tbl | VMEM_FLG_PRESENT | VMEM_FLG_WRITABLE;
        memset(tbl, 0x00, 0x1000);
    }

    // Lock the memory map range
    _lock_region(g_vmem_page_tbl, 0xFFFFFFFF - (uint32_t)g_vmem_page_tbl);

    for(i = 0; i < map_cnt; i++)
        _initialize_map_region(&regions[i]);

    install_fault_handler(FAULT_PF, _page_fault_handler);

    asm volatile("mov %0, %%cr3;" : : "r"(g_vmem_page_dir));
    g_vmem_page_dir = VIRT_ADDR_PGPDE;
}

void _lock_region(void* addr, size_t size) {

    uint32_t first_frame = addr_to_frame(addr);
    uint32_t last_frame = addr_to_frame((addr+size-1));

    for(;first_frame <= last_frame; first_frame++)
        bitmap_add(&g_vmem_map, first_frame);
}

void _initialize_map_region(vmem_map_info_t* region) {

    /*
     * This function only works while we still have the identity mapping. It
     * will not work once paging is enabled and should not be called directly.
     *
     * This function will add the given vmem_map_info to a page.
     *
     * Arguments:
     * - region : A map_info structure containing the addresses and size of the
     *   region to map.
     *
     * Returns:
     * - None
     */

    uint32_t dir_index, tbl_index, *tbl;
    uint32_t p = align4(region->phys_addr);
    uint32_t v = align4(region->virt_addr);
    uint32_t v_end = ((uint32_t)region->virt_addr + region->byte_len);

    for(; v <= v_end; v=next_frame(v), p=next_frame(p)) {

        // If NULL is passed as the phys_addr, we will allocate space.
        if(region->phys_addr == NULL)
            p = (uint32_t)pmem_allocate_frame();

        // Lookup the physical table location
        dir_index = addr_to_directory_index(v);
        tbl_index = addr_to_table_index(v);
        tbl = (uint32_t*)(g_vmem_page_dir[dir_index] & 0xFFFFF000);

        // Add the page to the page table.
        tbl[tbl_index & 0x3ff] = 
            ((uint32_t)p & 0xFFFFF000) | region->flags | VMEM_FLG_PRESENT;

        // Lock the virtual address
        bitmap_add(&g_vmem_map, addr_to_frame(v));

        dbglogf("MAP REGION { %p => %p, %i Bytes }\n", v, p, region->byte_len);
    }
}

void* vmem_allocate_kernel_heap_page() {
    uint32_t frame;

    if(bitmap_find_first_absent(&g_vmem_map, addr_to_frame(VIRT_ADDR_HEAP), &frame)) {
        bitmap_add(&g_vmem_map, frame);
        return (void*)frame_to_addr(frame);
    }

    return NULL;
}

void vmem_lock_region(void* addr, size_t size) {

    uint32_t first_frame = addr_to_frame(addr);
    uint32_t last_frame = addr_to_frame((addr+size-1));

    for(;first_frame <= last_frame; first_frame++)
        bitmap_add(&g_vmem_map, first_frame);
}

void vmem_free_page(void* addr, size_t size) {
    uint32_t first_frame = addr_to_frame(addr);
    uint32_t last_frame = addr_to_frame((addr+size-1));

    for(;first_frame <= last_frame; first_frame++)
        bitmap_rem(&g_vmem_map, first_frame);
}

void _map_single_frame(void* v, void* p, uint32_t flags) {

    uint32_t tbl_index = addr_to_table_index(v);

    if(p == NULL)
        p = pmem_allocate_frame();

    g_vmem_page_tbl[tbl_index] =
        align4(p) | flags | VMEM_FLG_PRESENT;

    _invalidate_page_table_entry(v);
}

void _page_fault_handler(regs_t* reg) {

    uint32_t *addr = (void*)get_cr3();

    dbglogf("!! PAGE FAULT !!\n");

    if(is_pf_access_violation(reg->err_code)) {
        dbglogf("Access Violation!\n");
        hlt();
    } else {
        dbglogf("Page is absent: %p!\n", addr);
        _map_single_frame(addr, NULL, VMEM_FLG_WRITABLE);
        dbglogf("Page has been mapped: Page is present.\n");
    }

}

