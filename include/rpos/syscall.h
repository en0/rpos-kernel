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

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/errno.h>
#include <rpos/cpu.h>

#ifndef _RPOS_SYSCALL_H
#define _RPOS_SYSCALL_H 1

int syscall_exit(int c);
int syscall_fork();
int syscall_read(int file, char *ptr, int len);
int syscall_write(int file, char *ptr, int len);
int syscall_open(const char *name, int flags, int mode);
int syscall_close(int file);
int syscall_wait(int *status);
int syscall_link(char *old, char *new);
int syscall_unlink(char *name);
int syscall_execve(char *name, char **argv, char **env);
int syscall_lseek(int file, int ptr, int dir);
int syscall_getpid();
int syscall_kill(int pid, int sig);
int syscall_times(struct tms *buf);
int syscall_sbrk(int incr);
int syscall_gettimeofday(struct timeval *p, void *z);
int syscall_stat(const char *file, struct stat *st);
int syscall_fstat(int file, struct stat *st);
int syscall_isatty(int file);
int syscall_unknown(regs_t* regs);

#endif
