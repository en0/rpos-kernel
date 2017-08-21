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

typedef struct PageFrameAllocatorCtx {
    PageFrameAllocator_t pfa;
    BitmapInfo_t bitmap;
    size_t total_memory, free_memory;
    void* manager_memory_start, *manager_memory_end;
} __attribute__((__packed__)) PageFrameAllocatorCtx_t;

// A couple of shortcut macros
#define CTX PageFrameAllocatorCtx_t
#define PFA PageFrameAllocator_t

static void _free_frame(PFA *p, void *addr) {
    CTX *ctx = (CTX*)p;
    // Refuse to free address 0x00.
    if((uint32_t)addr > 0) {
        ctx->free_memory += FRAME_SIZE;
        bitmap_rem(&ctx->bitmap, addr_to_frame(addr));
    }
}

static void _free_frames(PFA *p, void *addr, size_t bytes) {
    uint32_t start = align4(addr);
    uint32_t end = align4(addr + bytes - 1);
    for(; start <= end; start += FRAME_SIZE)
        _free_frame(p, (void*)start);
}

static void _lock_frame(PFA *p, void *addr) {
    CTX *ctx = (CTX*)p;
    bitmap_add(&ctx->bitmap, addr_to_frame(addr));
    ctx->free_memory -= FRAME_SIZE;
}

static void _lock_frames(PFA *p, void *addr, size_t bytes) {
    uint32_t start = align4(addr);
    uint32_t end = align4(addr + bytes - 1);
    for(; start <= end; start += FRAME_SIZE)
        _lock_frame(p, (void*)start);
}

static void* _alloc_frame_low(CTX *ctx) {
    bool success;
    uint32_t frame;
    do {
        if(!bitmap_find_first_absent_between(&ctx->bitmap, 1, HIGH_MEM_FRAME, &frame))
            return NULL;
        success = bitmap_add(&ctx->bitmap, frame);
    } while (!success);

    ctx->free_memory -= FRAME_SIZE;
    return (void*)frame_to_addr(frame);
}

static void* _alloc_frame(PFA *p) {
    CTX *ctx = (CTX*)p;
    bool success;
    uint32_t frame;
    do {
        if(!bitmap_find_first_absent(&ctx->bitmap, HIGH_MEM_FRAME, &frame))
            return _alloc_frame_low(ctx);
        success = bitmap_add(&ctx->bitmap, frame);
    } while (!success);

    ctx->free_memory -= FRAME_SIZE;
    return (void*)frame_to_addr(frame);
}

static void* _alloc_frames(PFA *p, size_t bytes) {
    CTX *ctx = (CTX*)p;

    if(bytes < 1)
        return NULL;

    if(align4(bytes) < bytes)
        bytes += FRAME_SIZE;

    uint32_t frame;
    uint32_t frame_cnt = addr_to_frame(bytes);

    { // TODO: This region needs mutex on [bits]
        if(!bitmap_find_absent_set_between(&ctx->bitmap, 0x00, frame_cnt, HIGH_MEM_FRAME, &frame))
            return NULL;
        _lock_frames(p, frame_to_addr(frame), bytes);
    }

    return frame_to_addr(frame);
}

static void _pfa_info(PFA *p, PageFrameAllocatorInfo_t *info) {
    CTX *ctx = (CTX*)p;
    info->total_memory = ctx->total_memory;
    info->free_memory = ctx->free_memory;
    info->used_memory = ctx->total_memory - ctx->free_memory;
    info->frame_size = FRAME_SIZE;
    info->manager_memory_start = ctx->manager_memory_start;
    info->manager_memory_end  = ctx->manager_memory_end;
    info->pfa = p;
}

PFA *initialize_bitmap_pfa(void *heap, size_t memory_size) {

    uint32_t frame_cnt = addr_to_frame(memory_size);
    size_t word_cnt = bitmap_compute_word_cnt(frame_cnt);

    // Setup the pointers using the heap we just got handed.
    CTX *ctx = heap;
    ctx->bitmap.bitmap_arr = heap + sizeof(CTX);
    ctx->bitmap.word_cnt = word_cnt;

    ctx->total_memory = memory_size;
    ctx->free_memory = 0;
    ctx->manager_memory_start = heap;
    ctx->manager_memory_end = &ctx->bitmap.bitmap_arr[word_cnt];

    // Lock all frames
    memset(ctx->bitmap.bitmap_arr, 0xFFFFFFFF, word_cnt + sizeof(uint32_t));

    // Wireup all the function pointers.
    ctx->pfa.alloc_frame = _alloc_frame;
    ctx->pfa.alloc_frames = _alloc_frames;
    ctx->pfa.free_frame = _free_frame;
    ctx->pfa.free_frames = _free_frames;
    ctx->pfa.lock_frame = _lock_frame;
    ctx->pfa.lock_frames = _lock_frames;
    ctx->pfa.pfa_info = _pfa_info;

    return &ctx->pfa;
}

