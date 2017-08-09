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

#include <sys/types.h>
#include <sys/stat.h>


#ifndef _INCLUDE_RPOS_FS_H
#define _INCLUDE_RPOS_FS_H

struct file_operations;
struct file;

typedef struct file_operations {
    int (*read)(struct file*, void*, size_t);
    int (*write)(struct file*, void*, size_t);
    int (*fstat)(struct file*, struct stat*);
    int (*close)(struct file*);
} file_operations_t;

typedef struct file {
    struct file_operations fops;
    int mode;
    int type;
    int is_dir;
    int fpos;
    int length;
    char* full_name;
} file_t;

#endif
