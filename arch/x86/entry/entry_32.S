## Copyright (c) 2017 "Ian Laird"
## Research Project Operating System (rpos) - https://github.com/en0/rpos
## 
## This file is part of rpos
## 
## rpos is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <asm/proto.h>

## Something like:
##
## /// With an error code example
## ENTRY(general_protection)
##   pushl $do_general_protection
##   jmp common_exception
## END(general_protection)

## /// With no error code example
## ENTRY(divide_error)
##   pushl	$0				# no error code
##   pushl	$do_divide_error
##   jmp	common_exception
## END(divide_error)

## /// int80 example
## ENTRY(entry_INT80_32)
##   pushl  $do_int80_syscall_32
##   jmp	common_syscall
## END(Entry_INT80_32)
##
## 
