## Copyright (c) 2017 "Ian Laird"
## Research Project Operating System (rpos) - https://github.com/en0/rpos
## 
## This file is part of rpos
## 
## rpos is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <asm/proto.h>

.extern _start   # The kernel entry point at the ~0xC0100000 address.

.equ vBase, 0xC0000000          # 3G memory address
.equ ldr, _ldr - vBase          # A 1M refrence to _ldr for boot loader
.equ pgNumber, (vBase >> 22)    # The index of the 3G PDE
.equ vPageCount, 1              # The number of higher half pages to map

.section .data
.align 0x1000

GLOBAL(LDRPDT)
    .set paddr, 0               # Start mapping at address zero

    .rept pgNumber              # Identity map all the page below 3G
        .4byte paddr | 0x83     # PRESENT | READ/WRITE | PAGE_SIZE 4MB
        .set paddr, paddr + 0x400000
    .endr

    .set paddr, 0               # Reset PADDR: Start mapping at address zero

    .rept vPageCount            # Map the higher half pages above 3G
        .4byte paddr | 0x83     # PRESENT | READ/WRITE | PAGE_SIZE 4M
        .set paddr, paddr + 0x400000
    .endr

    .rept 1024 - pgNumber - vPageCount
        .4byte 0x00             # empty
    .endr
END(LDRPDT)


.section .text
.align 4

GLOBAL(ldr)
    mov $(LDRPDT - vBase), %ecx
    mov %ecx, %cr3

    # cr4 |= PSE ; Enable physical size extention.
    mov %cr4, %ecx
    or $0x00000010, %ecx
    mov %ecx, %cr4

    # cr0 |= (PE | PG) ; set protected mode and enable paging.
    mov %cr0, %ecx
    or $0x80000001, %ecx 
    mov %ecx, %cr0

    # jump (absolute) to _start, which is linked around 0xC0100000
    lea _start, %ecx
    jmp *%ecx
ENDPROC(ldr)
