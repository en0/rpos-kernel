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
#include <rpos/dbglog.h>

int syscall_exit(int c) {
    dbglogf(":: syscall :: exit ::");
    for(;;hlt());
}

int syscall_fork() {
    dbglogf(":: syscall :: fork ::");
    for(;;hlt());
}

int syscall_read(int file, char *ptr, int len) {
    dbglogf(":: syscall :: read ::");
    for(;;hlt());
}

//int syscall_write(int file, char *ptr, int len) {
//    dbglogf(":: syscall- :: write ::");
//    for(;;hlt());
//}

int syscall_open(const char *name, int flags, int mode) {
    dbglogf(":: syscall :: open ::");
    for(;;hlt());
}

int syscall_close(int file) {
    dbglogf(":: syscall- :: close ::");
    for(;;hlt());
}

int syscall_wait(int *status) {
    dbglogf(":: syscall :: wait ::");
    for(;;hlt());
}

int syscall_link(char *old, char *new) {
    dbglogf(":: syscall :: link ::");
    for(;;hlt());
}

int syscall_unlink(char *name) {
    dbglogf(":: syscall-> :: unlink ::");
    for(;;hlt());
}

int syscall_execve(char *name, char **argv, char **env) {
    dbglogf(":: syscall-> :: execve ::");
    for(;;hlt());
}

int syscall_lseek(int file, int ptr, int dir) {
    dbglogf(":: syscall- :: lseek ::");
    for(;;hlt());
}

int syscall_getpid() {
    dbglogf(":: syscall-> :: getpid ::");
    for(;;hlt());
}

int syscall_kill(int pid, int sig) {
    dbglogf(":: syscall :: kill ::");
    for(;;hlt());
}

int syscall_times(struct tms *buf) {
    dbglogf(":: syscall->t syscall :: t syscall_times ::");
    for(;;hlt());
}

//int syscall_sbrk(int incr) {
//    dbglogf(":: syscall :: sbrk ::");
//    for(;;hlt());
//}

int syscall_gettimeofday(struct timeval *p, void *z) {
    dbglogf(":: syscall->gettim :: gettimeofday ::");
    for(;;hlt());
}

int syscall_stat(const char *file, struct stat *st) {
    dbglogf(":: syscall :: stat ::");
    for(;;hlt());
}

//int syscall_fstat(int file, struct stat *st) {
//    dbglogf(":: syscall- :: fstat ::");
//    for(;;hlt());
//}

//int syscall_isatty(int file) {
//    dbglogf(":: syscall-> :: isatty ::");
//    for(;;hlt());
//}

int syscall_unknown(regs_t* regs) {
    dbglogf(":: syscall->u :: unknown ::");
    for(;;hlt());
}
