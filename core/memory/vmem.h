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
#include <stddef.h>

#ifndef _MEMORY_VMEM_H
#define _MEMORY_VMEM_H

#define VMEM_FLG_PRESENT        0b00000000001
#define VMEM_FLG_WRITABLE       0b00000000010
#define VMEM_FLG_USER           0b00000000100
#define VMEM_FLG_WRITETHROUGH   0b00000001000
#define VMEM_FLG_NOCACHE        0b00000010000
#define VMEM_FLG_ACCESSED       0b00000100000
#define VMEM_FLG_DIRTY          0b00001000000
#define VMEM_FLG_LARGE          0b00010000000
#define VMEM_FLG_GLOBAL         0b00100000000

typedef struct vmem_map_info {
    void *phys_addr; 
    void *virt_addr; 
    size_t byte_len; 
    uint16_t flags;
} vmem_map_info_t;

/*
 * initialize_paging(regions, region_cnt)
 * setup paging and map the given regions into the new page tables.
 *
 * NOTE: This function requires identity mapping for physical addresses. After
 * this method returns, those identity mappings will no longer function and the
 * new page table will be installed.
 *
 * Arguments:
 * - regions: An array of vmem_map_info_t structures that reference what
 *   regions of virtual memory to map to what physical blocks.
 * - region_cnt: The number of structures in the array.
 *
 * Regions:
 * - None
 */

void vmem_initialize_paging(vmem_map_info_t* regions, size_t map_cnt);

/*
 * allocate_kernel_heap_page()
 * Reserve a virtual address frame and return it's address.
 *
 * NOTE: This function does not map the address into the page table.
 *
 * Arguments:
 * - None
 *
 * Regions:
 * - A pointer to the virtual address that was reserved.
 */

void* vmem_allocate_kernel_heap_page();

/*
 * free_page(addr, size)
 * Unlock a virtual address frame.
 *
 * NOTE: This function does not remove the frame from the page table.
 *
 * Arguments:
 * - addr: The address of the first page to be freed. Must be 4k aligned.
 * - size: The size, in bytes, of the region to be freed.
 *
 * Regions:
 * - None
 */
void vmem_free_page(void* addr, size_t size);

#endif
