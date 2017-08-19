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
#include <string.h>
#include <arch/mm.h>
#include <algo/bitmap.h>
#include <rpos/log.h>

static BitmapInfo_t *bits;
static void *_init_frame_allocator(void*, size_t);

static void _free_frame(void *addr) {
    // Refuse to free address 0x00.
    if((uint32_t)addr > 0)
        bitmap_rem(bits, addr_to_frame(addr));
}

static void _free_frames(void *addr, size_t bytes) {
    uint32_t start = align4(addr);
    uint32_t end = align4(addr + bytes - 1);
    for(; start <= end; start += FRAME_SIZE)
        _free_frame((void*)start);
}

static void _lock_frame(void *addr) {
    bitmap_add(bits, addr_to_frame(addr));
}

static void _lock_frames(void *addr, size_t bytes) {
    uint32_t start = align4(addr);
    uint32_t end = align4(addr + bytes - 1);
    for(; start <= end; start += FRAME_SIZE)
        _lock_frame((void*)start);
}

static void* _alloc_frame_low() {
    bool success;
    uint32_t frame;
    do {
        if(!bitmap_find_first_absent_between(bits, 1, HIGH_MEM_FRAME, &frame))
            return NULL;
        success = bitmap_add(bits, frame);
    } while (!success);

    return (void*)frame_to_addr(frame);
}

static void* _alloc_frame() {
    bool success;
    uint32_t frame;
    do {
        if(!bitmap_find_first_absent(bits, HIGH_MEM_FRAME, &frame))
            return _alloc_frame_low();
        success = bitmap_add(bits, frame);
    } while (!success);

    return (void*)frame_to_addr(frame);
}

static void* _alloc_frames(size_t bytes) {

    if(bytes < 1)
        return NULL;

    if(align4(bytes) < bytes)
        bytes += FRAME_SIZE;

    uint32_t frame;
    uint32_t frame_cnt = addr_to_frame(bytes);

    { // TODO: This region needs mutex on [bits]
        if(!bitmap_find_absent_set_between(bits, 0x00, frame_cnt, HIGH_MEM_FRAME, &frame))
            return NULL;
        _lock_frames(frame_to_addr(frame), bytes);
    }

    return frame_to_addr(frame);
}

static void *_init_frame_allocator(void *data, size_t mem_size_bytes) {

    uint32_t *storage = data;

    static BitmapInfo_t _bits;

    // Zero the map out so it will be re-initialized if called twice.
    memset(&_bits, 0x00, sizeof(BitmapInfo_t));

    // Compute the number of frames in the given address range.
    uint32_t frame_cnt = addr_to_frame(mem_size_bytes);

    // Compute the number of 32bit words we need to store the frame count.
    size_t word_cnt = bitmap_compute_word_cnt(frame_cnt);

    // Setup the bitmap structure with the calculated values
    _bits.word_cnt = word_cnt;
    _bits.bitmap_arr = storage;
    bits = &_bits;

    // Lock all the frames
    memset(storage, 0xFFFFFFFF, word_cnt * sizeof(uint32_t));

    // Lock the first frame, it messes with erro checking.
    bitmap_add(bits, 0);

    // Return the end of the region used for the memory map.
    return &storage[word_cnt + 1];
}

PageFrameAllocator_t bitmap_pfa = {
    .alloc_frame = _alloc_frame,
    .alloc_frames = _alloc_frames,
    .free_frame = _free_frame,
    .free_frames = _free_frames,
    .lock_frame = _lock_frame,
    .lock_frames = _lock_frames,
    .pfa_init = _init_frame_allocator
};
