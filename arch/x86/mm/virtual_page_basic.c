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

#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <arch/mm.h>
#include <rpos/utils.h>
#include <rpos/log.h>

uint32_t* g_vmem_page_dir;
uint32_t* g_vmem_page_tbl = VIRT_ADDR_PGPTE;

static void *_init_basic_virtual_frame_allocator(MemoryRegionInfo_t*, size_t);

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

static uint8_t flags_to_arch_flags(VFMAccessFlags flags) {

    uint8_t ret = 0;

#define isFlagSet(v,f) ((v&f) == f)

    if(isFlagSet(flags, VFM_FLG_WRITE))
        ret |= x86_FLG_PG_WRITE;

    if(!isFlagSet(flags, VFM_FLG_SUPERVISOR))
        ret |= x86_FLG_PG_USER;

    return ret;
}

static void _kmap(void* phys, void* virt, VFMAccessFlags flags) {

    uint32_t tbl_index = addr_to_table_index(virt);
    uint8_t arch_flags = flags_to_arch_flags(flags);

    if(phys == NULL)
        phys = alloc_frame();

    g_vmem_page_tbl[tbl_index] =
        align4(phys) | arch_flags | x86_FLG_PG_PRESENT;

    _invalidate_page_table_entry(virt);
}

static void _initialize_map_region(MemoryRegionInfo_t *region) {

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
            p = (uint32_t)alloc_frame();

        // Lookup the physical table location
        dir_index = addr_to_directory_index(v);
        tbl_index = addr_to_table_index(v);
        tbl = (uint32_t*)(g_vmem_page_dir[dir_index] & 0xFFFFF000);

        // Add the page to the page table.
        tbl[tbl_index & 0x3ff] = 
            align4(p) | flags_to_arch_flags(region->flags) | x86_FLG_PG_PRESENT;

        //log.printf("MAP REGION { %p => %p, %i Bytes }\n", v, p, region->byte_len);
    }
}

const VirtFrameManager_t vfm_basic = {
    .kmap = _kmap,
    .vfm_init = _init_basic_virtual_frame_allocator
};

static void *_init_basic_virtual_frame_allocator(MemoryRegionInfo_t* info, size_t count) {

    uint32_t dir_index = addr_to_directory_index(VIRT_ADDR_PGPDE);

    g_vmem_page_dir = alloc_frame();
    memset(g_vmem_page_dir, 0x00, 1024*sizeof(uint32_t));

    g_vmem_page_dir[dir_index] = 
        (uint32_t)g_vmem_page_dir | x86_FLG_PG_PRESENT | x86_FLG_PG_WRITE | x86_FLG_PG_GLOBAL;

    // Fill all page directory entries with a valid reference to a page table.
    uint32_t i, *tbl;
    for(i = 0; i < dir_index; i++) {
        tbl = alloc_frame();
        g_vmem_page_dir[i] = (uint32_t)tbl | x86_FLG_PG_PRESENT | x86_FLG_PG_WRITE;
        memset(tbl, 0x00, 0x1000);
    }

    for(int i = 0; i < count; i++)
        _initialize_map_region(&info[i]);

    asm volatile("mov %0, %%cr3;" : : "r"(g_vmem_page_dir));
    g_vmem_page_dir = VIRT_ADDR_PGPDE;

    return NULL;
}
