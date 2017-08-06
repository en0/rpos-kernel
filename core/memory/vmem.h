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

void vmem_initialize_paging(vmem_map_info_t* regions, size_t map_cnt);
void vmem_initialize_map_address(void *p, void *v, uint16_t flags);
void* vmem_allocate_kernel_heap_page();
void vmem_free_page(void* addr, size_t size);

#endif
