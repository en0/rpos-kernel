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

#ifndef _MEMORY_PMEM_H
#define _MEMORY_PMEM_H

/*
 * pmem_free_region(address, size)
 * Free (unlock) a set of memory frames so that they can be allocated later.
 *
 * Note: If a frame is only partialy refrenced, the entire frame will be freed.
 *
 * Arguments:
 * - addr - A 4kb aligned pointer to the first frame in the range.
 * - size - the number of bytes in the region.
 *
 * Returns:
 * - none
 */

void pmem_free_region(void* addr, size_t size);

/*
 * pmem_lock_region(address, size)
 * lock a set of memory frames so that they will not be allocated.
 *
 * Note: If a frame is only partialy refrenced, the entire frame will be locked.
 *
 * Arguments:
 * - addr - A 4kb aligned pointer to the first frame in the range.
 * - size - the number of bytes in the region.
 *
 * Returns:
 * - none
 */

void pmem_lock_region(void* addr, size_t size);

/*
 * pmem_allocate_frame()
 * Find and lock a frame of memory.
 *
 * Arguments:
 * - None
 *
 * Returns:
 * - A 4kb aligned pointer to the newly allocated frame.
 */

void* pmem_allocate_frame();

/*
 * pmem_free_frame(addr, size)
 * Free (unlock) a set of memory frames so that they can be allocated later.
 *
 * Note: If a frame is only partialy refrenced, the entire frame will be freed.
 *
 * Arguments:
 * - addr - A 4kb aligned pointer to the first frame in the range.
 * - size - the number of bytes in the region.
 *
 * Returns:
 * - none
 */

void pmem_free_frame(void* addr, uint32_t size);

#endif /* _MEMORY_PMEM_H */
