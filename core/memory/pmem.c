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

#include <klib/bitmap.h>

#define LOW_LIMIT 0x100
#define PAGE_BIT_FRAME 12
#define BITMAP_ARR_LEN (COMPUTE_BITMAP_WORD_CNT(0x100000))

#define addr_to_frame(addr) (((uint32_t)addr)>>PAGE_BIT_FRAME)
#define frame_to_addr(page) ((void*)(page<<PAGE_BIT_FRAME))


bitmap_word_t g_pmem_map_data[BITMAP_ARR_LEN];
bitmap_info_t g_pmem_map = { BITMAP_ARR_LEN, g_pmem_map_data };

void pmem_free_region(void* addr, size_t size) {

    uint32_t first_frame = addr_to_frame(addr);
    uint32_t last_frame = addr_to_frame((addr+size-1));

    for(;first_frame <= last_frame; first_frame++)
        bitmap_rem(&g_pmem_map, first_frame);
}

void pmem_lock_region(void* addr, size_t size) {

    uint32_t first_frame = addr_to_frame(addr);
    uint32_t last_frame = addr_to_frame((addr+size-1));

    for(;first_frame <= last_frame; first_frame++)
        bitmap_add(&g_pmem_map, first_frame);
}

void* pmem_allocate_frame() {

    uint32_t frame;

    if(bitmap_find_first_absent(&g_pmem_map, LOW_LIMIT, &frame)) {
        bitmap_add(&g_pmem_map, frame);
        return (void*)frame_to_addr(frame);
    }

    return NULL;
}

void pmem_free_frame(void* addr, uint32_t size) { 
    pmem_free_region(addr, size); 
}
