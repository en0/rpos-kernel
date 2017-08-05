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

#include <klib/dbglog.h>

#ifndef _UTILS_H
#define _UTILS_H 1

#define is_flag_set(a,b) ((a & b) == b)
static inline void hlt() { for(;;) asm("hlt"); }
static inline void sti() { asm("sti"); }
static inline void cli() { asm("cli"); }
static inline void halt() { cli(); hlt(); }
static inline void abort(const char* m) { dbglogf("ABORT: %s\n", m); cli(); hlt(); }

#endif /* _UTILS_H */
