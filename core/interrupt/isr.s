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

## Some of this code was addapted from 
## http://www.osdever.net/tutorials/view/brans-kernel-development-tutorial 

                .extern dispatch_isr_handler
                .equ handler, dispatch_isr_handler

                .section .text
                .align 4

                .macro isr_stub num
                .global isr_stub\num
isr_stub\num:   cli                     # Disable interrupts.
                push $0x0               # Push a empty errorCode
                push $\num              # Push the ISR number onto the stack.
                jmp isr_common          # goto handler stub
                .endm

isr_common:     pusha                   # Push all GP Registes
                push %ds                # Push all the segment registers
                push %es
                push %fs
                push %gs
                mov $0x10, %ax          # Load the Kernel Data Segment.
                mov %ax, %ds
                mov %ax, %es
                mov %ax, %fs
                mov %ax, %gs
                mov %esp, %eax          # Push up the stack pointer
                push %eax
                mov $handler, %eax
                call *%eax              # Call isr_dispatch, preserves the 'eip' register
                pop %eax
                pop %gs
                pop %fs
                pop %es
                pop %ds
                popa                    # Restore GP registers
                add $8, %esp            # Clean up the stack from isr_stubN

                # NOTE ------------------------------------------
                # iret will pop EFLAGS which contains the
                # original state of the interrupt enable flag. 
                # This will effectivly turn interrupts back on.

                iret                    # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

                isr_stub 128
