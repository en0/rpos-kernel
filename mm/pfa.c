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

static PageFrameAllocator_t *active;
size_t frame_size;

void attach_frame_allocator(PageFrameAllocator_t* p) {
    active = p;
}

void pfa_info(PageFrameAllocatorInfo_t* info) {
    active->pfa_info(active, info);
}

void* alloc_frame() {
    return active->alloc_frame(active); 
}

void* alloc_frames(size_t bytes) {
    return active->alloc_frames(active, bytes);
}

void lock_frame(void *addr) {
    active->lock_frame(active, addr);
}

void lock_frames(void *addr, size_t bytes) {
    active->lock_frames(active, addr, bytes);
}

void free_frame(void *addr) {
    active->free_frame(active, addr);
}

void free_frames(void *addr, size_t bytes) {
    active->free_frames(active, addr, bytes);
}

