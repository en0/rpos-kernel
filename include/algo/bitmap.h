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

#ifndef _ALGO_BITMAP_H
#define _ALGO_BITMAP_H

typedef struct BitmapInfo {
    size_t word_cnt;
    //uint32_t population;
    uint32_t* bitmap_arr;
} BitmapInfo_t;

#define BITMAP_BYTES_PER_WORD (sizeof(uint32_t))
#define BITMAP_BITS_PER_WORD (BITMAP_BYTES_PER_WORD * 8)
#define bitmap_compute_word_cnt(s) (s / BITMAP_BITS_PER_WORD)

/*
 * create(population_size)
 * Create a new, emtpy population of a given size.
 *
 * Note: This function allocates memory from the heap.
 *
 * Arguemtns:
 * - size: The total number of possible members within the population.
 *
 * Returns:
 * - Returns a BitmapInfo_t structure that represents the new population.
 */

BitmapInfo_t* bitmap_create(size_t size);

/*
 * add(member)
 * Add a new member to the population.
 *
 * Arguments:
 * - map: a pointer to the BitmapInfo_t structure that represents a population.
 * - member: an integer value that represents a unique member.
 *
 * Return:
 * - Returns true if the given member was not present in the population before 
 *   addition. Else, false.
 */

bool bitmap_add(BitmapInfo_t* map, uint32_t member);

/*
 * rem(member)
 * Remove a member from the population.
 *
 * Arguments:
 * - map: a pointer to the BitmapInfo_t structure that represents a population.
 * - member: an integer value that represents a unique member.
 *
 * Return:
 * - Returns true if the given member was present in the population before
 *   removal. Else, false.
 */

bool bitmap_rem(BitmapInfo_t* map, uint32_t member);

/*
 * is_absent(member)
 * Checks if a member is absent from the population.
 *
 * Arguments:
 * - map: a pointer to the BitmapInfo_t structure that represents a population.
 * - member: an integer value that represents a unique member.
 *
 * Return:
 * - Returns true if the given member is absent from the population. Else,
 *   false.
 */

bool bitmap_is_absent(BitmapInfo_t* map, uint32_t member);

/*
 * is_present(member)
 * Check if a member is present in the population.
 *
 * Arguments:
 * - map: a pointer to the BitmapInfo_t structure that represents a population.
 * - member: an integer value that represents a unique member.
 *
 * Return:
 * - Returns true if the given member is present in the population. Else,
 *   false.
 */

bool bitmap_is_present(BitmapInfo_t* map, uint32_t member);

/*
 * find_first_absent(start)
 * Find the next absent member in the population starting from the given index.
 *
 * Arguments:
 * - map: a pointer to the BitmapInfo_t structure that represents a population.
 * - start: an integer that represents the unique member value to begin search.
 * - member: an integer pointer that will be populated with the member
 *   identified as absent.
 *
 * Return:
 * - Returns true if a member is found to be absent. Else, returns false.
 */

bool bitmap_find_first_absent(BitmapInfo_t* map, uint32_t start, uint32_t* member);

/*
 * find_first_absent_between(start, limit)
 *
 * Find the next absent member in the population starting at the given index
 * and stopping at the limit index.
 *
 * Arguments:
 * - map: a pointer to the BitmapInfo_t structure that represents a population.
 * - start: an integer that represents the unique member value to begin search.
 * - limit: an integer that represents the unique member value to stop the search.
 * - member: an integer pointer that will be populated with the member
 *   identified as absent.
 *
 * Return:
 * - Returns true if a member is found to be absent. Else, returns false.
 */

bool bitmap_find_first_absent_between(BitmapInfo_t*, uint32_t, uint32_t, uint32_t*);

/*
 * find_absent_set(start, cnt)
 * Find a set of a given size of contiguous members that are absent from the 
 * population starting at the given index.
 *
 * Arguments:
 * - map: a pointer to the BitmapInfo_t structure that represents a population.
 * - start: an integer that represents the unique member value to begin search.
 * - member: an integer pointer that will be populated with the first member of
 *   the absent set.
 *
 * Return:
 * - Returns true if a member set of sufficient size is found to be absent.
 *   Else, returns false.
 */

bool bitmap_find_absent_set(BitmapInfo_t* map, uint32_t start, size_t cnt, uint32_t* first_member);

/*
 * find_absent_set_between(start, cnt, limit)
 *
 * Find contiguous members of a given size that are absent from the 
 * population between the given indecies.
 *
 * Arguments:
 * - map: a pointer to the BitmapInfo_t structure that represents a population.
 * - start: an integer that represents the unique member value to begin search.
 * - member: an integer pointer that will be populated with the first member of
 *   the absent set.
 * - limit: an integer that represents the unique member value to stop search
 *
 * Return:
 * - Returns true if a member set of sufficient size is found to be absent.
 *   Else, returns false.
 */

bool bitmap_find_absent_set_between(BitmapInfo_t* map, uint32_t start, size_t cnt, uint32_t limit, uint32_t* first_member);

#endif /* !_ALGO_BITMAP_H */
