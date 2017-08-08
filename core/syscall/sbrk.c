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
#include <errno.h>

void* syscall_sbrk(int incr);

void syscall_int80_sbrk(regs_t* regs) {
    regs->eax = (uint32_t)syscall_sbrk(regs->ebx);
}

void* syscall_sbrk(int incr) {
    dbglogf("SBRK is asking for %i\n", incr);
    dbglogf("ISR syscall_int80_sbrk NOT IMPLEMENTED\n"); 
    for(;;hlt()); 
}
