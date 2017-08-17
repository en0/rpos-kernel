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

#include <stddef.h>
#include <rpos/kernel-common.h>

#ifndef _RPOS_KERNEL_H
#define _RPOS_KERNEL_H 1

/* Provided by linker */
extern void* _begin;
extern void* _end;

/* physical address loayout */
#define PHYS_ADDR_KSTART    ((void*)&_begin - 0xc0000000)
#define PHYS_ADDR_KEND      ((void*)&_end - 0xc0000000)
#define PHYS_ADDR_VGA3      ((void*)0x000B8000)

/* Virtual address layout */
#define VIRT_ADDR_PGPDE     ((void*)0xFFFFF000)
#define VIRT_ADDR_PGPTE     ((void*)0xFFC00000)
#define VIRT_ADDR_STACK     ((void*)0xFFBFFFFF)
#define VIRT_ADDR_ESTACK    ((void*)0xFF400000)
#define VIRT_ADDR_EHEAP     ((void*)0xFF3FFFFF)
#define VIRT_ADDR_HEAP      ((void*)0xC0400000)
#define VIRT_ADDR_RAMDISK   ((void*)((uint32_t)(&_end + 0x1000) & 0xFFFFF000))
#define VIRT_ADDR_KEND      ((void*)&_end)
#define VIRT_ADDR_KSTART    ((void*)&_begin)
#define VIRT_ADDR_VGA3      ((void*)0xc00B8000)

/* Virtual address layout - Userspace */
#define VIRT_ADDR_USR_STACK ((void*)0xBFFFFFFF)
#define VIRT_ADDR_USR_MAPS  ((void*)0x40000000)
#define VIRT_ADDR_USR_TEXT  ((void*)0x08048000)

/* Size information */
#define KERNEL_SIZE         ((size_t)(PHYS_ADDR_KEND - PHYS_ADDR_KSTART))
#define KERNEL_HEAP         ((heap_info_t *)VIRT_ADDR_HEAP)
#define STACK_SIZE          ((uint32_t)(VIRT_ADDR_STACK - VIRT_ADDR_ESTACK))

/*
 * Modules are configured as a prebuild step. Do not edit these directly unless
 * you are modifing the available modules.
 */

#define MODULE

#ifndef MODULE_MM_BITMAP
#define MODULE_MM_BITMAP 1
#endif

#ifndef MODULE
#define MODULE_MM_STACK 1
#endif

#ifndef MODULE
#define MODULE_EXP_BOOTPARAMS 1
#endif

#define PROFILE_NAME ""
#define PROFILE_ 1
#define BUILD_TARGET ""

#endif /* _RPOS_KERNEL_H */
