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
#include <rpos/mm.h>
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

static void setup_boot_pfa() {

    uint32_t *mm_storage = (uint32_t*)&_end;
    multiboot_info_t *mbi = g_mboot_info;
    multiboot_module_t *mod_info = (multiboot_module_t *)mbi->mods_addr;

    // If there are modules, _end is not available. Find the end of the modules.
    if(mbi->mods_count > 0)
        mm_storage = (uint32_t*)mod_info[mbi->mods_count-1].mod_end;

    // Make sure the bitmap is on a 4k boundry
    if(!is_align4(mm_storage))
        mm_storage = (uint32_t*)next_frame(mm_storage);

    // Memory comes in from multiboot as KB. Convert to bytes
    uint32_t total_memory_bytes = (mbi->mem_lower + mbi->mem_upper)<<10;

    // Initialize the basic bitmap PFA.
    void* mm_storage_end = 
        pfa_bitmap.pfa_init(mm_storage, total_memory_bytes);

    // Free the memory that the bootloader identified as available.
    for(multiboot_memory_map_t *mmap = (multiboot_memory_map_t*) mbi->mmap_addr;
        (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
        mmap = (multiboot_memory_map_t*) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {

        if(mmap->type == 1) {
            log.printf("REGION: [%p - %p]\n", (uint32_t)mmap->addr, (uint32_t)mmap->addr + (uint32_t)mmap->len);
            pfa_bitmap.free_frames((void*)((uint32_t)mmap->addr), (uint32_t)mmap->len);
        }
    }

    // Lock kernel memory regions (physcial space).
    pfa_bitmap.lock_frames(PHYS_ADDR_KSTART, KERNEL_SIZE);

    // Lock the bitmap memory itself
    pfa_bitmap.lock_frames(mm_storage, mm_storage_end - (void*)mm_storage);

    // Lock video memory
    pfa_bitmap.lock_frame(PHYS_ADDR_VGA3);

    if(mbi->mods_count > 0) {
        // Lock the modules info structure and ramdisk module.
        pfa_bitmap.lock_frames((void*)mbi->mods_addr, sizeof(uint32_t));
        pfa_bitmap.lock_frames((void*)mod_info->mod_start, mod_info->mod_end - mod_info->mod_start);
    }

    // Install pfa_bitmap for the rest of the system to use.
    attach_frame_allocator(&pfa_bitmap);

    log.printf("=> x86_boot :: mm_storage => [%p - %p] - %i Kb\n", mm_storage, mm_storage_end, ((void*)mm_storage_end - (void*)mm_storage)>>10);
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

    // Setup a boot-time PFA.
    setup_boot_pfa();

    // Idea about virtual memory managent
    //
    // pfa: physical frame allocator
    // vpm: virtual page manager
    // vha: virtual heap allocator
    //
    // vfm->kmap - Map phys to virt <<-- This is the one right now. YOU ARE HERE
    // vfm->physat - get the physical at virtual frame
    //
    // kmalloc - allocate fractional memory frames from the kernel heap.
    // kfree - deallocate fractional memory frames to the kernel heap.
    // vfalloc - allocate a virtual frame nomap ?? not sure i want this.

    // Split commandline options into argv
#define ARGC_MAX 32
    //const char *argv[ARGC_MAX];
    //int argc = split_cmdline(argv, ARGC_MAX);

    // Call kernel startup.
    i386_start_kernel();

    log.puts("\n!! Reached the end !!\n");
}
