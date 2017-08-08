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

#include <string.h>
#include <stddef.h>
#include <core/cpu.h>
#include <klib/dbglog.h>

#include "isr.h"
#include "idt.h"
#include "../syscall/int80.h"

#define ISR_VECTOR_MAX 19
void(*g_isr_vectors[ISR_VECTOR_MAX])(regs_t*);

void isr_initialize_api() {

    // This installs all the system calls via the int80 interface.
    //memset((void*)g_isr_vectors, 0x00, 16);
    //g_isr_vectors[1] = syscall_int80_exit;
    //g_isr_vectors[2] = syscall_int80_fork;
    //g_isr_vectors[3] = syscall_int80_read;
    g_isr_vectors[4] = syscall_int80_write;
    //g_isr_vectors[5] = syscall_int80_open;
    //g_isr_vectors[6] = syscall_int80_close;
    //g_isr_vectors[7] = syscall_int80_wait;
    //g_isr_vectors[8] = syscall_int80_link;
    //g_isr_vectors[9] = syscall_int80_unlink;
    //g_isr_vectors[10] = syscall_int80_execve;
    //g_isr_vectors[11] = syscall_int80_lseek;
    //g_isr_vectors[12] = syscall_int80_getpid;
    //g_isr_vectors[13] = syscall_int80_kill;
    //g_isr_vectors[14] = syscall_int80_times;
    g_isr_vectors[15] = syscall_int80_sbrk;
    //g_isr_vectors[16] = syscall_int80_gettimeofday;
    //g_isr_vectors[17] = syscall_int80_stat;
    g_isr_vectors[18] = syscall_int80_fstat;
    //g_isr_vectors[19] = syscall_int80_isatty;

    // Install the int $0x80 idt gate.
    idt_setGate(0x80, &isr_stub128, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
}

void dispatch_isr_handler(regs_t *r) {

    if(r->eax > 0 && r->eax <= ISR_VECTOR_MAX && g_isr_vectors[r->eax] != NULL)
        g_isr_vectors[r->eax](r);
    else {
        dbglogf("WARNING: Unhandled ISR: %i", r->eax);
        for(;;) hlt();
    }
}

//void install_isr_handler(int num, void(*fn)(regs_t*)) {
    //g_isr_vectors[num] = fn;
//}

