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

#include <stdint.h>
#include <string.h>
#include <rpos/kernel.h>
#include <rpos/const.h>
#include <rpos/log.h>
#include <log/serial.h>
#include <asm/segment.h>

#include "boot.h"

static void setup_boot_gdt() {
    static const uint64_t gdt_entries[] __attribute__((aligned(16))) = {
        [GDT_ENTRY_CS] = GDT_ENTRY(GDT_FLAGS(1,0,1,0,1,1,1), 0, 0xFFFFF),
        [GDT_ENTRY_DS] = GDT_ENTRY(GDT_FLAGS(1,0,0,0,1,1,1), 0, 0xFFFFF),
        [GDT_ENTRY_TSS] = 0
    };

    static struct Gdt gdt = {
        .length = sizeof(gdt_entries),
        .table = (uint32_t)&gdt_entries
    };

    asm volatile("lgdtl %0" : : "m"(gdt));
    log.printf("=> x86_boot :: GDT => [%p], CS(%x), DS(%x)\n", &gdt, __CS, __DS);
}

static void setup_boot_idt() {
    static const struct Gdt idt = {0, 0};
    asm volatile("lidtl %0" : : "m"(idt));
    log.printf("=> x86_boot :: IDT => [%p], (NULL)\n", &idt);
}

static void* find_physical_heap_start() {

    // Default to end of kernel.
    void* heap_start = PHYS_ADDR_KEND;

    // If we have kernel modules loaded, then the end is after the last one.
    multiboot_module_t *mod = (multiboot_module_t*)g_mboot_info->mods_addr;
    if(g_mboot_info->mods_count > 0)
        heap_start = (void*)mod[g_mboot_info->mods_count-1].mod_end;

    // Align on the next page boundry if needed.
    if(!is_align4(heap_start))
        heap_start = (void*)next_frame(heap_start);

    return heap_start;
}

static size_t setup_boot_pfa(uint32_t *mm_storage_start) {

    multiboot_info_t *mbi = g_mboot_info;

    // Memory comes in from multiboot as KB. Convert to bytes
    uint32_t total_memory_bytes = (mbi->mem_lower + mbi->mem_upper)<<10;

    // Initialize the basic bitmap PFA.
    void* mm_storage_end = 
        bitmap_pfa.pfa_init(mm_storage_start, total_memory_bytes);

    // Free the memory that the bootloader identified as available.
    for(multiboot_memory_map_t *mmap = (multiboot_memory_map_t*) mbi->mmap_addr;
        (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
        mmap = (multiboot_memory_map_t*) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {

        if(mmap->type == 1)
            bitmap_pfa.free_frames((void*)((uint32_t)mmap->addr), (uint32_t)mmap->len);
    }

    // Lock the kernel all the way to the end of the memory map.
    // This will lock any modules stored in memory as well.
    bitmap_pfa.lock_frames(PHYS_ADDR_KSTART, mm_storage_end - PHYS_ADDR_KSTART);

    // Lock video memory
    bitmap_pfa.lock_frame(PHYS_ADDR_VGA3);

    // Install bitmap_pfa for the rest of the system to use.
    attach_frame_allocator(&bitmap_pfa);

    log.printf("=> x86_boot :: PFA Ready => [%p - %p] - %i Kb\n", mm_storage_start, mm_storage_end, ((void*)mm_storage_end - (void*)mm_storage_start)>>10);

    return (void*)mm_storage_end - (void*)mm_storage_start;
}

static void setup_boot_vfm(MemoryRegionInfo_t *region_map, size_t region_map_cnt) {

    // Initialize the virtual frame manager.
    basic_vfm.vfm_init(region_map, region_map_cnt);

    // Install the virtual frame manager for global system use.
    attach_virtual_frame_manager(&basic_vfm);

    log.printf("=> x86_boot :: VFM Ready => [%p - %p] - %i Kb\n", VIRT_ADDR_PGPTE, 0xFFFFFFFF, (0xFFFFFFFF - (uint32_t)VIRT_ADDR_PGPTE)>>10);
}

void static setup_boot_mm() {

    void* phys_heap = find_physical_heap_start();

    // Setup a boot-time PFA > returns the number of bytes used from the heap.
    size_t phys_heap_used = setup_boot_pfa(phys_heap);

    // Build a struct with the map of all the memory we need into paging.
    MemoryRegionInfo_t region_map[] = {
        { PHYS_ADDR_KSTART, VIRT_ADDR_KSTART, KERNEL_SIZE, VFM_FLG_WRITE },
        { PHYS_ADDR_VGA3, VIRT_ADDR_VGA3, 4096, VFM_FLG_WRITE },
        { phys_heap, phys_heap, phys_heap_used, VFM_FLG_WRITE },
        { NULL, VIRT_ADDR_ESTACK,  STACK_SIZE, VFM_FLG_WRITE },
    }; size_t region_map_cnt = sizeof(region_map) / sizeof(MemoryRegionInfo_t);

    // Setup a boot-time VFM.
    setup_boot_vfm(region_map, region_map_cnt);
}

static int __used split_cmdline(const char *argv[], size_t arg_max) {
    // TODO: Not sure i am using thie yet.
    int i = 0;
    char *cmdline = (char*)g_mboot_info->cmdline;
    char *cptr = strtok(cmdline, " ");

    for(;cptr != NULL && i < arg_max; cptr = strtok(NULL, " "), i++)
        argv[i] = cptr; 

    return i;
}

void x86_boot_entry(void) {

    log.attach(&log_target_com1);
    log.printf("=> x86_boot :: Start @ x86_boot_entry => [%p]\n", x86_boot_entry);
    log.printf("=> x86_boot :: Total Memory - %i Mb\n", (g_mboot_info->mem_lower + g_mboot_info->mem_upper)>>10);
    log.printf("=> x86_boot :: Kernel Memory => [%p - %p] - %i Kb\n", &_begin, &_end,  KERNEL_SIZE / 1024);

    // Setup and install descriptors
    setup_boot_idt();
    setup_boot_gdt();
    activate_kernel_selector();

    // setup boot-time memory management
    setup_boot_mm();

    // Idea about virtual memory managent
    //
    // -- These are very platform specific
    // pfa: physical frame allocator
    // vpm: virtual page manager
    //
    // -- This should not be platform specific.
    // vha: virtual heap allocator
    //
    // -- Right now this is implemented in /mm but it should be in /arch/*/mm
    // vfm->kmap - Map phys to virt <<-- This is the one right now. YOU ARE HERE
    // vfm->kunmap - Unmap a virtual address.
    // vfm->physat - get the physical at virtual frame
    //
    // kmalloc - allocate fractional memory frames from the kernel heap.
    // kfree - deallocate fractional memory frames to the kernel heap.
    // vmalloc - allocate a virtual frame nomap ?? not sure i want this.

    // Split commandline options into argv
#define ARGC_MAX 32
    //const char *argv[ARGC_MAX];
    //int argc = split_cmdline(argv, ARGC_MAX);

    // Call kernel startup.
    i386_start_kernel();

    log.puts("\n!! Reached the end !!\n");
}
