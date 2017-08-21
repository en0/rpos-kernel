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

#include <rpos/const.h>

#ifndef _RPOS_INIT_H
#define _RPOS_INIT_H 1

#define __initcall(fn,lvl) \
    static Initcall_t __initcall_##fn \
    __attribute__((__section__(".initcall" lvl ".init"))) \
    __used = fn

#define early_initcall(fn) __initcall(fn, "0")
#define core_initcall(fn) __initcall(fn, "1")
#define arch_initcall(fn) __initcall(fn, "2")
#define subsys_initcall(fn) __initcall(fn, "3")
#define fs_initcall(fn) __initcall(fn, "4")
#define rootfs_initcall(fn) __initcall(fn, "5")
#define device_initcall(fn) __initcall(fn, "6")
#define late_initcall(fn) __initcall(fn, "7")

typedef int (*Initcall_t)(void);

extern Initcall_t __initcall_start[];
extern Initcall_t __initcall_end;

void start_kernel();
void arch_start_kernel();
void start_kernel();

typedef struct BootOptions {
    char *cmdline;
    void *mods[];
} BootOptions_t;

#endif /* ! _RPOS_INIT_H */
