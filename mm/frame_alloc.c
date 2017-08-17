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
static PageFrameAllocator_t *pfa;

void attach_frame_allocator(PageFrameAllocator_t* p) {
    static PageFrameAllocator_t _pfa;
    _pfa.alloc_frame = p->alloc_frame;
    _pfa.alloc_frames = p->alloc_frames;
    _pfa.free_frame = p->free_frame;
    _pfa.free_frames = p->free_frames;
    _pfa.lock_frame = p->lock_frame;
    _pfa.lock_frames = p->lock_frames;
    pfa = &_pfa;
}

void* alloc_frame() {
    return pfa->alloc_frame(); 
}

void* alloc_frames(size_t bytes) {
    return pfa->alloc_frames(bytes);
}

void lock_frame(void *addr) {
    pfa->lock_frame(addr);
}

void lock_frames(void *addr, size_t bytes) {
    pfa->lock_frames(addr, bytes);
}

void free_frame(void *addr) {
    pfa->free_frame(addr);
}

void free_frames(void *addr, size_t bytes) {
    pfa->free_frames(addr, bytes);
}

