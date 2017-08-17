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

#ifndef _x86_ASM_PROTO_H
#define _x86_ASM_PROTO_H

#ifdef __ASSEMBLER__

#define GLOBAL(fn) .globl fn; fn:
#define END(l) .size l, . - l;
#define ENDPROC(fn) .type fn, @function; END(fn);

#endif /* __ASSEMBLER__ */

#endif /* !_x86_ASM_PROTO_H */
