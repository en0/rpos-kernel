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
#include <stdint.h>

#ifndef _RPOS_MM_H
#define _RPOS_MM_H 1

typedef struct PageFrameAllocator {
    void*(*alloc_frame)();
    void*(*alloc_frames)(size_t);
    void(*free_frame)(void*);
    void(*free_frames)(void*,size_t);
    void(*lock_frame)(void*);
    void(*lock_frames)(void*,size_t);
    void*(*pfa_init)(void*,size_t);
} PageFrameAllocator_t;

/*
 * alloc_frame()
 *
 * Allocates a single frame of physical memory. The allocator attempts to
 * obtain this page from a memory range above 16Mb. If there is no memory
 * available above 16Mb, an allocation will be made from the DMA memory region.
 *
 * Aguments:
 *  None
 *
 * Return:
 *  A pointer to the 4kb alinged physcial memory address of the allocated page.
 *  If no system memory is found, a "Out of memory" interrupt will be raised.
 *  If the allocator cannot return an available page, a NULL reference will be
 *  returned.
 */

void* alloc_frame();

/*
 * alloc_frames(size)
 *
 * Allocates contiguous frames, if availalbe. The allocation will be made ONLY
 * from memory lower then 16Mb. This is inteded for hardware that requires
 * contiguous physical memory for operation.
 *
 * Arguments:
 *  size: The amount of contiguous memory in bytes.
 *
 * Return:
 *  A 4kb alinged pointer to the first page in the allocated range.
 */

void* alloc_frames(size_t);

/*
 * lock_frame(address)
 *
 * Locks a single frame. This is usefull for initial setup.
 *
 * Note, that allocations and locks are made on 4kb frames. If a lock request
 * partially overlaps a frame, the entire frame will be locked.
 *
 * Arguments:
 *  address: The address of the frame to be locked.
 *
 * Returns:
 *  None
 */

void lock_frame(void*);

/*
 * lock_frames(address, size)
 *
 * Locks contiguous frames starting at address. This is usefull for initial
 * setup.
 *
 * Note, that allocations and locks are made on 4kb frames. If a lock request
 * partially overlaps a frame, the entire frame will be locked.
 *
 * Arguments:
 *  address: The first address in the range to be locked
 *  size: The amount of contiguous memory in bytes.
 *
 * Return:
 *  A 4kb alinged pointer to the first page in the allocated range.
 */

void lock_frames(void*,size_t);

/*
 * free_frame(address)
 *
 * Frees a single page back to the system.
 *
 * Arguments:
 *  address: The address of the frame to be freed.
 *
 * Return:
 *  None
 */

void free_frame(void* addr);

/*
 * free_frames(address, size)
 *
 * Frees a range frames back to the system.
 *
 * Note, that allocations and locks are made on 4kb frames. If a free request
 * partially overlaps a frame, the entire frame will be freed.
 *
 * Arguments:
 *  address: The address of the frame to be freed.
 *  size: The amount of contiguous memory in bytes.
 *
 * Return:
 *  None
 */

void free_frames(void *addr, size_t bytes);


/*
 * attach_frame_allocator(pfa)
 *
 * Install the given page frame allocator.
 * 
 * Note: This method makes mo effort to sync allocation inforation when a new 
 * PFA is installed.
 *
 * Arguments:
 *  pfa: A pointer to a PageFrameAllocator
 *
 * Return:
 *  None
 */ 

void attach_frame_allocator(PageFrameAllocator_t* p);

/***
 * Start VFM headers
 ***/

#define VFM_FLG_WRITE       (1<<0)
#define VFM_FLG_SUPERVISOR  (1<<1)

typedef uint8_t VFMAccessFlags;

typedef struct MemoryRegionInfo {
    void *phys_addr; 
    void *virt_addr; 
    size_t byte_len; 
    VFMAccessFlags flags;
} MemoryRegionInfo_t;

typedef struct VirtFrameManager {
    void(*kmap)(void*,void*,VFMAccessFlags);
    void(*kunmap)(void*);
    void*(*physat)(void*);
    void*(*vfm_init)(MemoryRegionInfo_t*, size_t);
} VirtFrameManager_t;

/*
 * kmap(phys, virt, flags)
 *
 * Map a physical frame to a virtual frame.
 * NOTE: Both addresses should be 4kb aligned.
 *
 * Arguments:
 *  phys: A 4k aligned physical address.
 *  virt: A 4k alinged virtual address.
 *  flags: The flags to place on the new page
 *
 * Return:
 *  None.
 */

void kmap(void* phys, void* virt, VFMAccessFlags flags);

/*
 * kunmap(virt)
 *
 * Unmap a frame.
 *
 * Arguments:
 *  virt: A 4k aligned virtual address.
 *
 * Return:
 *  None
 */

void kunmap(void* virt);

/*
 * physat(virt)
 *
 * Retrieve a mapped physical address.
 *
 * Arguments:
 *  virt: A 4k aligned virtual address.
 *
 * Return:
 *  If the virtual address is mapped, the physical address is returned. Else,
 *  NULL.
 */

void* physat(void* virt);

/*
 * attach_virtual_frame_manager(vfm)
 *
 * Install a virtual frame allocator
 *
 * Note: This method makes mo effort to sync mapping inforation when a new VFM
 * is installed.
 *
 * Arguments:
 *  pfa: A pointer to a VirtualFrameManager
 *
 * Return:
 *  None
 */ 

void attach_virtual_frame_manager(VirtFrameManager_t*);

#endif /* !_RPOS_MM_H */
