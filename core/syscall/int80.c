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

#include <core/cpu.h>
#include <klib/dbglog.h>

void syscall_int80_write(regs_t* regs) {
    dbglogf("************    You called an isr!\n"); 
    dbglogs((char*)regs->ecx);
}

void syscall_int80_exit(regs_t* regs) { dbglogf("ISR %i NOT IMPLEMENTED\n", regs->eax); }
void syscall_int80_fork(regs_t* regs) { dbglogf("ISR %i NOT IMPLEMENTED\n", regs->eax); }
void syscall_int80_read(regs_t* regs) { dbglogf("ISR %i NOT IMPLEMENTED\n", regs->eax); }
//void syscall_int80_write(regs_t* regs) { dbglogf("ISR %i NOT IMPLEMENTED\n", regs->eax); }
void syscall_int80_open(regs_t* regs) { dbglogf("ISR %i NOT IMPLEMENTED\n", regs->eax); }
void syscall_int80_close(regs_t* regs) { dbglogf("ISR %i NOT IMPLEMENTED\n", regs->eax); }
void syscall_int80_wait(regs_t* regs) { dbglogf("ISR %i NOT IMPLEMENTED\n", regs->eax); }
void syscall_int80_link(regs_t* regs) { dbglogf("ISR %i NOT IMPLEMENTED\n", regs->eax); }
void syscall_int80_unlink(regs_t* regs) { dbglogf("ISR %i NOT IMPLEMENTED\n", regs->eax); }
void syscall_int80_execve(regs_t* regs) { dbglogf("ISR %i NOT IMPLEMENTED\n", regs->eax); }
void syscall_int80_gettimeofday(regs_t* regs) { dbglogf("ISR %i NOT IMPLEMENTED\n", regs->eax); }
void syscall_int80_stat(regs_t* regs) { dbglogf("ISR %i NOT IMPLEMENTED\n", regs->eax); }
void syscall_int80_lseek(regs_t* regs) { dbglogf("ISR %i NOT IMPLEMENTED\n", regs->eax); }
void syscall_int80_getpid(regs_t* regs) { dbglogf("ISR %i NOT IMPLEMENTED\n", regs->eax); }
void syscall_int80_fstat(regs_t* regs) { dbglogf("ISR %i NOT IMPLEMENTED\n", regs->eax); }
void syscall_int80_kill(regs_t* regs) { dbglogf("ISR %i NOT IMPLEMENTED\n", regs->eax); }
void syscall_int80_times(regs_t* regs) { dbglogf("ISR %i NOT IMPLEMENTED\n", regs->eax); }
void syscall_int80_sbrk(regs_t* regs) { dbglogf("ISR %i NOT IMPLEMENTED\n", regs->eax); }
void syscall_int80_isatty(regs_t* regs) { dbglogf("ISR %i NOT IMPLEMENTED\n", regs->eax); }