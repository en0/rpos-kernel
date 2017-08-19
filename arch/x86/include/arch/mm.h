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
#include <stdint.h>
#include <rpos/mm.h>
#include <rpos/kernel.h>

#ifndef _ARCH_MM_H
#define _ARCH_MM_H 1

#define _4K 0x1000
#define _16M 0x1000000

#define FRAME_SIZE _4K
#define HIGH_MEM _16M
#define HIGH_MEM_FRAME (HIGH_MEM>>12)

#define x86_FLG_PG_PRESENT        0b00000000001
#define x86_FLG_PG_WRITE          0b00000000010
#define x86_FLG_PG_USER           0b00000000100
#define x86_FLG_PG_WRITETHROUGH   0b00000001000
#define x86_FLG_PG_NOCACHE        0b00000010000
#define x86_FLG_PG_ACCESSED       0b00000100000
#define x86_FLG_PG_DIRTY          0b00001000000
#define x86_FLG_PG_LARGE          0b00010000000
#define x86_FLG_PG_GLOBAL         0b00100000000

#define align4(addr) ((uint32_t)(addr) & 0xFFFFF000)
#define is_align4(addr) (align4(addr) == (uint32_t)addr)
#define next_frame(addr) (align4((uint32_t)addr + 0x1000))
#define addr_to_frame(addr) (((uint32_t)addr)>>12)
#define frame_to_addr(page) ((void*)(page<<12))

#define addr_to_directory_index(addr) (((uint32_t)addr)>>22)
#define directory_index_to_addr(addr) (((uint32_t)addr)<<22)
#define addr_to_table_index addr_to_frame
#define table_index_to_addr frame_to_addr

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

extern PageFrameAllocator_t bitmap_pfa;
extern VirtFrameManager_t basic_vfm;

#endif /* ! _ARCH_MM_H */
