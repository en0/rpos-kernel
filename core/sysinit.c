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

#include <stdbool.h>

#include <kconfig.h>
#include <klib/dbglog.h>
#include <core/utils.h>

#include "memory/init.h"
#include "boot/multiboot.h"

extern int main(int,char**);

bool sysinit_is_mutliboot_data_present(multiboot_info_t* mbi) {

    /** MBOOT Sanity Check **/
    if( is_flag_set(mbi->flags, MULTIBOOT_INFO_AOUT_SYMS) && 
        is_flag_set(mbi->flags, MULTIBOOT_INFO_ELF_SHDR)) {
        dbglogf("ERROR: Booloader provided inconsistent information..\n");
        return false;
    }

    /** Need memory info **/
    if( !is_flag_set(mbi->flags, MULTIBOOT_INFO_MEM_MAP) ||
        !is_flag_set(mbi->flags, MULTIBOOT_INFO_MEMORY)) {
        dbglogf("ERROR: Missing memory information from bootloader.\n");
        return false;
    }

    /** Need initfs as module **/
    if( !is_flag_set(mbi->flags, MULTIBOOT_INFO_MODS) ) {
        dbglogf("ERROR: Missing initfs information from bootloader.!\n");
        return false;
    }

    return true;
}

bool sysinit_initialize_memory_manager(multiboot_info_t* mbi) {

    multiboot_memory_map_t *mmap;

    gdt_install();

    // Free the memory that the bootloader identified as available.
    for(mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
        (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
        mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {

        if(mmap->type == 1) {
            pmem_free_region((void*)((uint32_t)mmap->addr), (uint32_t)mmap->len);
        }
    }

    // Lock kernel memory, the memory map, and the stack region
    pmem_lock_region(PHYS_ADDR_KSTART, KERNEL_SIZE);

    // Lock the first page. it just messes up NULL checking
    pmem_lock_region(0x00, 1);

    // Lock video memory
    pmem_lock_region(PHYS_ADDR_VGA3, 1);

    // Lock the modules info structure and ramdisk module.
    multiboot_module_t *ramdisk_info = (multiboot_module_t *)mbi->mods_addr;
    pmem_lock_region((void*)mbi->mods_addr, sizeof(uint32_t));
    pmem_lock_region((void*)ramdisk_info->mod_start, ramdisk_info->mod_end - ramdisk_info->mod_start);

    //Virtual memory next.
    return true;
}

void sysinit(multiboot_info_t* mbi) {

    dbglog_init();

    if(!sysinit_is_mutliboot_data_present(mbi))
        abort("Failed to boot while validating multiboot information.");

    else if(!sysinit_initialize_memory_manager(mbi))
        abort("Failed to boot while initializing memory manager.");

    dbglogf("mbi: %s \n", mbi->cmdline);

    // Setup memory management
    // Copy the current stack frame

    // Move the stack to the virtual memory space.
    //asm volatile ( "mov %0, %%esp;" : : "a"(0xFFBFFFFF)); 

    // Setup ISR
    // Setup basic Perif
    // get the boot command line

    //sti();
    main(1, (char**)&"Hello, World");

    // The stack is dead: Cannot return.
    halt();
}

