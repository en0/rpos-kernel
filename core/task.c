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

#include <stdint.h>
#include <stdbool.h>
#include <rpos/task.h>

task_t* g_task_active;
task_t g_task_kernel_task;

void task_initialize_kernel_task(const char* name, void* heap, void* heap_limit) {
    g_task_kernel_task.name = name;
    g_task_kernel_task.brk = (uint32_t)heap;
    g_task_kernel_task.brk_limit = (uint32_t)heap_limit;
    g_task_kernel_task.is_supervisor = true;
    g_task_kernel_task.next = &g_task_kernel_task;
    g_task_kernel_task.prev = &g_task_kernel_task;
    g_task_active = &g_task_kernel_task;
}

void task_set_active(task_t* task) {
}
