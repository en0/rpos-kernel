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

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef _ALGO_STACK_H
#define _ALGO_STACK_H

#define STACK_EMPTY     1
#define STACK_FULL      2
#define STACK_HAS_ITEMS 4

typedef struct StackInfo {
    uint32_t* limit;
    uint32_t* top;
    uint32_t* bottom;
    uint8_t state;
} StackInfo_t;

/*
 * create(size);
 * Allocate a new stack of the given size.
 *
 * Arguments:
 *  size: the number of 32-bit integers the stack can store.
 *
 * Return:
 *  A pointer to the StackInfo structure.
 */

StackInfo_t* stack_create(size_t size);

/*
 * init(stack, data, size)
 *
 * Initialize StackInfo using the given values. This requires the memory is
 * already allocated for both the StackInfo Structure and the data where the
 * stack values are stored.
 *
 * Arguments:
 *  stack: A pointer to a StackInfo structure.
 *  data: A pointer to the location the stack data will be stored.
 *  size: The maximum number of 32-bit integers that will fit in the stack data.
 *
 * Returns:
 *  None
 */

void stack_init(StackInfo_t*, uint32_t*, size_t);

/*
 * push(stack, val)
 *
 * Push a new value to the top of the stack. If the stack is full, the
 * operation will have no effect.
 *
 * Arguments:
 *  stack: A pointer to a StackInfo structure.
 *  val: A 32-bit value to store on the stack.
 *
 * Return:
 *  None
 */

void stack_push(StackInfo_t*, uint32_t);

/*
 * pop(stack)
 *
 * Pop a new value from the top of the stack. If the stack is empty, the
 * operation will have no effect and zero will be returned.
 *
 * Arguments:
 *  stack: A pointer to a StackInfo structure.
 *
 * Return:
 *  The 32-bit value that was just pulled off the top of the stack.
 */

uint32_t stack_pop(StackInfo_t* stk);

/*
 * insert(stack, val, at)
 *
 * Insert a value into the stack at a specific location. This will move the
 * current data up and increase the stack population by 1. If there is no room
 * in the stack, the operation will have no effect.
 *
 * If the index specified by at is ahead of the top of the stack, the operation
 * will have no effect.
 *
 * Arguments:
 *  stack: A pointer to a StackInfo structure.
 *  val: A 32-bit value to put in the stack
 *  at: the location (zero based index) to insert the item.
 *
 * Return:
 *  None
 */

void stack_insert(StackInfo_t *stk, uint32_t val, size_t at);

/*
 * remove(stack, at)
 *
 * Remove a value from the stack at a specific location. This will move the
 * current data down and decrease the stack population by 1. If the stack is
 * already empty, the operation will have no effect and zero will be returned.
 *
 * If the index specified by at is ahead of the top of the stack, the operation
 * has no effect and zero will be returned.
 *
 * Arguments:
 *  stack: A pointer to a StackInfo structure.
 *  at: the location (zero based index) to remove the item from.
 *
 * Return:
 *  If successfull, the value of the 32-bit integer removed from the stack.
 */

uint32_t stack_remove(StackInfo_t*,size_t);

#endif /* ! _ALGO_STACK_H */
