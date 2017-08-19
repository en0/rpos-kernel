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

#include <stddef.h>
#include <rpos/kernel-common.h>

#ifndef _RPOS_KERNEL_H
#define _RPOS_KERNEL_H 1

/* Provided by linker */
extern void* _begin;
extern void* _end;

/*
 * Modules are configured as a prebuild step. Do not edit these directly unless
 * you are modifing the available modules.
 */

#define MODULE

#ifndef MODULE_MM_BITMAP
#define MODULE_MM_BITMAP 1
#endif

#ifndef MODULE
#define MODULE_MM_STACK 1
#endif

#ifndef MODULE
#define MODULE_EXP_BOOTPARAMS 1
#endif

#define PROFILE_NAME "DEBUG"
#define PROFILE_DEBUG 1
#define BUILD_TARGET "i686-rpos"

#endif /* _RPOS_KERNEL_H */
