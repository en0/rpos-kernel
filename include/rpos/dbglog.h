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

#ifndef _INCLUDE_DBGLOG_H
#define _INCLUDE_DBGLOG_H

#include <kconfig.h>

#ifdef PROFILE_DEBUG

/* Initialize COM1 */
void dbglog_init();

/* Log a single char : like putchar(); */
int dbglogc(int character);

/* Log a string : like puts */
int dbglogs(const char* str);

/* Log using a format : like printf
 *
 * Pattern tokens:
 * %c - interpret argument as a single character : typeof char
 * %s - interpret argument as string : typeof char*
 * %d - interpret argument as alias for %i
 * %i - interpret argument as signed integer : typeof int
 * %u - interpret argument as unsigned integer : typeof unsigned int
 * %x - interpret argument as unsinged integer as hex : typeof unsigned int
 * %o - interpret argument as unsigned integer as octal : typeof unsigned int
 * %p - interpret argument as unsigned integer as pointer : typeof unsigned int
 * %b - interpret argument as unsigned integer as binary : typeof unsigned int
 * %n - interpret argument as nothing : Will still pop sizeof int.
 * %% - interpret argument as literal % character
 *
 * Example:
 * dbglogf("Hello, %s", "world");
 * */
int dbglogf(const char *format, ...);

#else

#define dbglog_init() do {} while(0)
#define dbglogc(...) do {} while(0)
#define dbglogs(...) do {} while(0)
#define dbglogf(...) do {} while(0)

#endif /* PROFILE_DEBUG */

#endif /* _INCLUDE_DBGLOG_H */
