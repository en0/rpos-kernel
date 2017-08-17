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
#include <algo/stack.h>

StackInfo_t* stack_create(size_t size) {
    /** NOT IMPLEMENTED **/
    return NULL;
}

void stack_init(StackInfo_t *stk, uint32_t *array, size_t count) {
    stk->limit = &array[count - 1];
    stk->top = (stk->bottom = array) - 1;
    stk->state = STACK_EMPTY;
}

static inline void set_state(StackInfo_t *stk) {
    if(stk->limit == stk->top)
        stk->state = STACK_FULL;
    else if(stk->top == stk->bottom)
        stk->state = STACK_EMPTY;
    else
        stk->state = STACK_HAS_ITEMS;
}

void stack_push(StackInfo_t* stk, uint32_t val) {
    if(stk->top < stk->limit)
        *++stk->top = val;
    set_state(stk);
}

uint32_t stack_pop(StackInfo_t* stk) {
    if(stk->top >= stk->bottom) {
        set_state(stk);
        return *stk->top--;
    } else {
        set_state(stk);
        return 0;
    }
}

void stack_insert(StackInfo_t *stk, uint32_t val, size_t at) {

    uint32_t *idx = &stk->bottom[at];

    if(idx > stk->top) 
        return;

    else if(stk->top < stk->limit) {
        for(uint32_t *s = stk->top, *d = ++stk->top; d != idx; *d-- = *s--);
        *idx = val;
    }

    set_state(stk);
}

uint32_t stack_remove(StackInfo_t* stk, size_t at) {

    uint32_t *a = &stk->bottom[at];
    uint32_t ret = *a;
    size_t len = stk->top - a;

    if(len < 1) return 0;
    memcpy(a, a+1, len*sizeof(uint32_t));
    stk->top--;

    set_state(stk);
    return ret;
}

