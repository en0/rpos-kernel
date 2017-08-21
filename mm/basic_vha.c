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

#include <rpos/mm.h>
#include <rpos/log.h>
#include <algo/stack.h>

/*** 
 * All on means no partitions available.
 * 
 *   [ 1       0       ] = 32
 *   [ 1   0   1   1   ] = 16
 *   [ 1 0 1 1 1 1 1 1 ] = 8
 *
 *   * Request for valloc(5)
 *   [ 1       0       ] = 32
 *   [ 1   0   1   1   ] = 16
 *   [ 1 0 1 1 1 1 1 1 ] = 8
 *
 *   [ 1       0       ] = 32
 *   [ 1   0   1   1   ] = 16
 *   [ 1 1 1 1 1 1 1 1 ] = 8
 *
 *   * Request for valloc(14)
 *   [ 1       0       ] = 32
 *   [ 1   0   1   1   ] = 16
 *   [ 1 1 1 1 1 1 1 1 ] = 8
 *
 *   [ 1       0       ] = 32
 *   [ 1   1   1   1   ] = 16
 *   [ 1 1 1 1 1 1 1 1 ] = 8
 *
 *   * Request for valloc(7)
 *   [ 1       0       ] = 32
 *   [ 1   1   1   1   ] = 16
 *   [ 1 1 1 1 1 1 1 1 ] = 8
 *
 *   [ 1       1       ] = 32
 *   [ 1   1   0   0   ] = 16
 *   [ 1 1 1 1 1 1 1 1 ] = 8
 *
 *   [ 1       1       ] = 32
 *   [ 1   1   1   0   ] = 16
 *   [ 1 1 1 1 0 0 1 1 ] = 8
 *
 *   [ 1       1       ] = 32
 *   [ 1   1   1   0   ] = 16
 *   [ 1 1 1 1 1 0 1 1 ] = 8
 *
 *   * Request for vfree(32) :: which is 5th bit in order 0 :: imposible to
 *   * know, we need a avl tree to keep track of the given allocations.
 *   [ 1       1       ] = 32
 *   [ 1   1   1   0   ] = 16
 *   [ 1 1 1 1 1 0 1 1 ] = 8
 *
 *   [ 1       1       ] = 32
 *   [ 1   1   1   0   ] = 16
 *   [ 1 1 1 1 0 0 1 1 ] = 8
 *
 *   [ 1       1       ] = 32
 *   [ 1   1   0   0   ] = 16
 *   [ 1 1 1 1 1 1 1 1 ] = 8
 *
 *   [ 1       0       ] = 32
 *   [ 1   1   1   1   ] = 16
 *   [ 1 1 1 1 1 1 1 1 ] = 8
 ***/

static uint32_t *free_lists[3];

static void* _vmalloc(size_t bytes) {
    return NULL;
}

static void _vfree(void *virt) {
}

static void _init_virtual_heap_allocator(void *heap_start, size_t heap_size) {
    PageFrameAllocatorInfo_t _frame_info;
    pfa_info(&_frame_info);
    log.printf("Initilize vha with frame size: %i",_frame_info.frame_size);
}

VirtualHeapAllocator_t basic_vha = {
    .vmalloc = _vmalloc,
    .vfree = _vfree,
    .vha_init = _init_virtual_heap_allocator
};
