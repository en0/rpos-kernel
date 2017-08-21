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

#include <rpos/log.h>
#include <rpos/init.h>
#include <rpos/kernel.h>
#include <log/serial.h>

void i386_start_kernel() {

     /*
      * This code should:
      *
      * // Copy initfs to new memory >16mb and free old memory.
      * // Fill in boot spec 
      * //  - cmdline (argv, argc) Dont forget about init=/system
      * //  - void *initfs
      * //  - some *memory_regions struct.
      * // launch i386_start_kernel <= *boot_spec
      *
      * X. Initializse a paging sturcture and activate it.
      * X. Install perminate Page Frame Allocator and protect memory regions.
      * X. Free up the <16mb physical address space.
      *   a. Copy the kernel to a higher phsycial memory address. (use new pager)
      *   b. The initfs is already over 16m but needs mapped into virtual space
      * X. Setup the kernel stack. << 
      * X. Setup the kernel Heap Allocator.
      * X. Fill out the bootspec:
      *   a. The boot command line.
      *   b. The location of the initfs.
      *   c. The available memory regions.
      * X. launch start_kernel <= *boot_spec
      */

    start_kernel();
}
