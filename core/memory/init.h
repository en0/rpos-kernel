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

#ifndef _MEMORY_INIT_H
#define _MEMORY_INIT_H 1

void gdt_install(void);
void pmem_free_region(void* addr, size_t size);
void pmem_lock_region(void* addr, size_t size);
void* pmem_allocate_frame();
void pmem_free_frame(void* addr, uint32_t size);

#endif /* _MEMORY_INIT_H */
