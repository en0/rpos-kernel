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
#include <rpos/mm.h>

static void *_init_basic_virtual_frame_allocator(void*);

static void _kmap(void* phys, void* virt) {
}

const VirtFrameManager_t vfm_basic = {
    .kmap = _kmap,
    .vfm_init = _init_basic_virtual_frame_allocator
};

static void *_init_basic_virtual_frame_allocator(void* p) {
    return NULL;
}
