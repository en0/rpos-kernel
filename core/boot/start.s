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

        .global _start
        .set VALID_MAGIC, 0x2BADB002

        .section .tstack, "aw", @nobits
        .skip 1024              ## 1KB of stack space.
stack:  nop

        .section .text
        .align 4

_start: movl $stack, %esp       ## Setup temp stack
        pushl $0                ## Reset EFLAGS
        popf
        cmp $VALID_MAGIC, %eax  ## Check the multiboot magic number
        jne .halt

        ## Initialize the system
        ## Setups memory, ISRs, and basic perifrials.
.start: push %ebx               ## EBX points to multiboot_info_t
        call sysinit            ## Call sysinit(mboot*)

        ## Either the system is not multiboot complient or sysinit
        ## returned - Either way, we cannot do anything. System is dead.
.halt:  cli                     ## Stop all hardware interupts
        hlt                     ## Halt the CPU
        jmp .                   ## Infinite loop
