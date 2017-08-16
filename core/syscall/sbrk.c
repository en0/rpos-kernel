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

#include <rpos/cpu.h>
#include <rpos/task.h>
#include <rpos/utils.h>
#include <rpos/dbglog.h>
#include <errno.h>

#include "../memory/vmem.h"

void* syscall_sbrk(int incr) {

    dbglogf("SBRK: %i\n", incr);
    void* target_addr = (void*)task_active->brk + incr;
    void* current_addr = (void*)task_active->brk;

    if(task_active->brk_limit < (uint32_t)target_addr)
        return (void*)-ENOMEM;

    vmem_lock_region(current_addr, incr);
    task_active->brk = (uint32_t)target_addr;

    dbglogf(":: syscall_sbrk :: %s :: %p\n", task_active->name, task_active->brk);

    return current_addr;
}
