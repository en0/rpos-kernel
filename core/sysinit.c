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
#include <stdbool.h>

#include <kconfig.h>
#include <klib/dbglog.h>
#include <core/utils.h>

#include "boot/multiboot.h"

#include "memory/gdt.h"
#include "memory/pmem.h"
#include "memory/vmem.h"

#include "interrupt/idt.h"
#include "interrupt/irq.h"
#include "interrupt/fault.h"

#include "peripheral/rtc.h"

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

    // Build a struct with the map of all the memory we need into paging.
    vmem_map_info_t initial_regions[] = {
        { PHYS_ADDR_KSTART, VIRT_ADDR_KSTART, KERNEL_SIZE, VMEM_FLG_WRITABLE },
        { PHYS_ADDR_VGA3, VIRT_ADDR_VGA3, 4096, VMEM_FLG_WRITABLE },
        { NULL, VIRT_ADDR_ESTACK,  STACK_SIZE, VMEM_FLG_WRITABLE },
        { (void*)ramdisk_info->mod_start, VIRT_ADDR_RAMDISK,
          ramdisk_info->mod_end - ramdisk_info->mod_start,
          0x00 /* Read only */ },
    };

    vmem_initialize_paging(initial_regions, 4);

    return true;
}

bool sysinit_initialize_interrupt_api(multiboot_info_t* mbi) {

    idt_install();
    irq_initialize_api();
    fault_initialize_api();
    return true;
}

bool sysinit_initialize_peripherals(multiboot_info_t* mbi) {
    rtc_install();
    return true;
}

void sysinit(multiboot_info_t* mbi) {

    char cmdline[256];
    uint32_t esp, ebp, stack_frame_size;

    memcpy(cmdline, (void*)mbi->cmdline, 256);

    dbglog_init();

    /** Start core services. **/
    if(!sysinit_is_mutliboot_data_present(mbi))
        abort("Failed to boot while validating multiboot information.");

    else if(!sysinit_initialize_interrupt_api(mbi))
        abort("Failed to boot while initializing interrupt api.");

    else if(!sysinit_initialize_memory_manager(mbi))
        abort("Failed to boot while initializing memory manager.");

    else if(!sysinit_initialize_peripherals(mbi))
        abort("Failed to boot while initializing peripherals.");

    /** Move the stack to the new 8MB stack location **/
    asm volatile ( 
        "mov %%esp, %0;" 
        "mov %%ebp, %1;" 
        : "=r"(esp), "=r"(ebp));

    stack_frame_size = ebp - esp;
    memcpy((void*)VIRT_ADDR_STACK-stack_frame_size, (void*)esp, stack_frame_size);

    asm volatile (
        "mov %0, %%esp;"
        "mov %1, %%ebp;"
       : : "r"(VIRT_ADDR_STACK-stack_frame_size), "r"(VIRT_ADDR_STACK));

    /** Parse the command line to pass to main. **/
    int arg_cnt;
    char *arg_values[255];
    for(arg_cnt = 0, arg_values[arg_cnt] = strtok(cmdline, " "); 
        arg_cnt < 255 && arg_values[arg_cnt] != NULL;
        arg_cnt++, arg_values[arg_cnt] = strtok(NULL, " "));

    sti();
    main(arg_cnt, arg_values);

    for(;;);

    // !! The stack is dead: Cannot return. !!
    halt();
}

