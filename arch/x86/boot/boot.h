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

#include "multiboot.h"
#include <rpos/mm.h>

#ifndef _x86_BOOT_BOOT_H
#define _x86_BOOT_BOOT_H

#ifdef MODULE_EXP_BOOTPARAMS
#define BOOT_PARAMETER_MAX_LENGTH 256
typedef struct BootParameters {
    // put command-line options here.
    char mm[BOOT_PARAMETER_MAX_LENGTH];
} BootParameters_t;
extern BootParameters_t *g_boot_parameters;
void parse_cmdline_params(char*);
#endif /* MODULE_EXP_BOOTPARAMS */

extern multiboot_info_t *g_mboot_info;
extern PageFrameAllocator_t pfa_bitmap;

void activate_kernel_selector();
void i386_start_kernel();

#endif
