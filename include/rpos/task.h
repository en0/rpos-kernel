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

#ifndef _INCLUDE_RPOS_TASK_H
#define _INCLUDE_RPOS_TASK_H 1

#include <stdint.h>
#include <stdbool.h>
#include <rpos/cpu.h>

typedef struct task_regs {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t eip, cs, eflags, ss;
} task_regs_t;

typedef struct task {
    const char* name;
    task_regs_t regs;
    uint32_t pde, brk, brk_limit;
    bool is_supervisor;
    struct task* prev;
    struct task* next;
} task_t;

extern task_t* g_task_active;
void task_initialize_kernel_task(const char* name, void* heap, void* heap_limit);
#define task_active g_task_active

#endif
