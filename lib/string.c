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

#include <string.h>

void *memcpy(void *destination, const void *source, size_t num) {
    char* d = (char*)destination;
    char* s = (char*)source;
    for(;num > 0; *d++ = *s++, num--);
    return destination;
}

void *memset(void* ptr, int value, size_t num) {
    char* d = (char*)ptr;
    char v = (char)(value & 0xFF);
    for(;num > 0; *d++ = v, num--);
    return ptr;
}

size_t strlen(const char* str) {
    size_t i;
    for(i = 0; *str != '\0'; str++, i++);
    return i;
}

char* strrev(const char *str) {
    int i, j;
    char c;
    char *s = (char*)str;
    for(i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
    return (char*)str;
}

char* strcpy(char* destination, const char* s) {
    char* d = (char*)destination;
    for(;*s != '\0'; *d++ = *s++);
    *d = '\0';
    return destination;
}

char* strncpy(char* destination, const char* s, size_t num) {
    char* d = (char*)destination;
    for(;*s != '\0' && num > 0; *d++ = *s++, num--);
    *d = '\0';
    return destination;
}

char* strcat(char* destination, const char* s) {
    char* d = (destination + strlen(destination));
    for(;*s != '\0'; *d++ = *s++);
    *d = '\0';
    return destination;
}

char* strncat(char* destination, const char* s, size_t num) {
    char* d = (destination + strlen(destination));
    for(;*s != '\0' && num > 0; *d++ = *s++, num--);
    *d = '\0';
    return destination;
}

char* strstr(const char *haystack, const char *needle) {
    const char *n, *h, *r;
    for(r = h = haystack, n = needle; *h != '\0'; h++) {
        if(*n == *h) {
            if(*++n == '\0') 
                return (char*)r + 1;
        } else {
            r = h;
            n = needle;
        }
    }
    return NULL;
}

char* strtok(char *str, const char *delimiters) {

    char *ret;
    static char *ptr;

    if(str != NULL)
        ptr = str;

    else if(ptr == NULL)
      return NULL;

    else if(*++ptr == '\0')
      return NULL;
    

    for(ret = ptr; *ptr != '\0'; ptr++) {
        for(const char *d = delimiters; *d != '\0'; d++) {
            if(*ptr == *d) {
                *ptr = '\0';
                return ret;
            }
        }
    }

    ptr--;
    return ret;
}

int strcmp(const char* s1, const char* s2) {
    while(*s1 && (*s1 == *s2)) { s1++; s2++; }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
    while(*s1 && (*s1 == *s2) && n > 0) { s1++; s2++; n--; }
    return n == 0 ? 0 : *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
