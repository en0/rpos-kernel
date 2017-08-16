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
#include <stdint.h>
#include <stdarg.h>
#include <rpos/dbglog.h>
#include <rpos/cpu.h>

#ifdef PROFILE_DEBUG

#define COM1 0x3f8
#define COM2 0x2f8
#define COM3 0x3e8
#define COM4 0x2e8

void dbglog_init() {
    outb(0x00, COM1+1);
    outb(0x80, COM1+3);
    outb(0x03, COM1+0);
    outb(0x00, COM1+1);
    outb(0x03, COM1+3);
    outb(0xC7, COM1+2);
    outb(0x0B, COM1+4);
}

static inline int is_transmit_empty() {
   return inb(COM1 + 5) & 0x20;
}
 
static inline void write_serial(char a) {
   while (is_transmit_empty() == 0);
   outb(a, COM1);
}

char* _reverse_string(const char *str) {
    int i, j;
    char c;
    char *s = (char*)str;
    for(i = 0, j = strlen((const char*)s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
    return (char*)str;
}

char* _int_to_ascii(int value, char* str, int base) {
    static char* num = "0123456789abcdef";
    char* ret = str;
    int sign = value;

    if (base<2 || base>16) {
        *ret='\0';
        return NULL;
    }

    if(value < 0)
        value = -value;

    do {
        *ret++ = num[value % base];
    } while(value/=base);

    if(sign < 0)
        *ret++ = '-';

    *ret++ = '\0';

    return _reverse_string(str);
}

char* _uint_to_ascii(unsigned value, char* str, int base) {
    static char* num = "0123456789abcdef";
    char* ret = str;

    if (base<2 || base>16) {
        *ret='\0';
        return NULL;
    }

    do {
        *ret++ = num[value % base];
    } while(value/=base);

    *ret++ = '\0';

    return _reverse_string(str);
}

int dbglogc(int character) {

    switch((char)(character & 0xFF)) {

        case '\r':
        case '\b':
            /* skip */
            break;

        default:
            write_serial(character);
            break;
    }

    return character;
}


int dbglogs(const char* str) {
    int i;
    for(i = 1; *str != '\0'; str++, i++)
        dbglogc((int)*str);
    dbglogc('\n');
    return i;
}

int dbglogf(const char *format, ...) {

    const char *p;
    int _ret = 0;

    int va_char;
    char *va_str;
    char buffer[32];

    va_list params;
    va_start(params, format);

    for(p = format; *p != '\0'; p++) {
        if(*p != '%') {
            dbglogc((int)*p);
            _ret++;
            continue;
        }

        switch(*++p) {
            case '%' : /* Literal % */
                dbglogc((int)'%');
                _ret++;
                break;

            case 'c' : /* Charater */
                va_char = va_arg(params, int);
                dbglogc(va_char);
                _ret++;
                break;

            case 's' : /* C String */
                va_str = (char*)va_arg(params, int);
                for(;*va_str != '\0'; va_str++) {
                    dbglogc((int)(*va_str));
                    _ret++;
                }
                break;

            case 'd' : /* Signed Integer */
            case 'i' :
                va_char = (int)va_arg(params, int);
                _int_to_ascii(va_char, buffer, 10);
                dbglogf(buffer);
                break;

            case 'u' : /* Unsigned Integer */
                va_char = (int)va_arg(params, int);
                _uint_to_ascii(va_char, buffer, 10);
                dbglogf(buffer);
                break;

            case 'x' : /* Unsigned Hex (lower case only) */
            case 'X' :
                va_char = (int)va_arg(params, int);
                _uint_to_ascii(va_char, buffer, 16);
                dbglogf(buffer);
                break;

            case 'o' : /* Unsigned Octal */
                va_char = (int)va_arg(params, int);
                _uint_to_ascii(va_char, buffer, 8);
                dbglogf(buffer);
                break;

            case 'p' : /* Pointer */
                va_char = (int)va_arg(params, int);
                _uint_to_ascii(va_char, buffer, 16);
                dbglogf("0x%s", buffer);
                break;

            case 'b' : /* Binary */
                va_char = (int)va_arg(params, int);
                _uint_to_ascii(va_char, buffer, 2);
                dbglogf("%sb", buffer);

            case 'n' : /* nothing (Param must be an int) */
                va_char = (int)va_arg(params, int);
                break;
        }
    }

    va_end(params);
    return 0;
}

#endif /** PROFILE_DEBUG **/
