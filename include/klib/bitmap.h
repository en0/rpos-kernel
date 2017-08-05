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
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.  **/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef _KLIB_BITMAP_H
#define _KLIB_BITMAP_H

typedef uint32_t bitmap_population_t;
typedef uint32_t bitmap_index_t;
typedef uint32_t bitmap_word_t;

typedef struct bitmap_info {
    size_t word_cnt;
    bitmap_word_t* bitmap_arr;
} bitmap_info_t;

#define BITMAP_BYTES_PER_WORD (sizeof(bitmap_word_t))
#define BITMAP_BITS_PER_WORD (BITMAP_BYTES_PER_WORD * 8)
#define COMPUTE_BITMAP_WORD_CNT(s) (s / BITMAP_BITS_PER_WORD)

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
 * - Returns a bitmap_info_t structure that represents the new population.
 */

bitmap_info_t* bitmap_create(size_t size);

/*
 * add(member)
 * Add a new member to the population.
 *
 * Arguments:
 * - map: a pointer to the bitmap_info_t structure that represents a population.
 * - member: an integer value that represents a unique member.
 *
 * Return:
 * - Returns true if the given member was not present in the population before 
 *   addition. Else, false.
 */

bool bitmap_add(bitmap_info_t* map, uint32_t member);

/*
 * rem(member)
 * Remove a member from the population.
 *
 * Arguments:
 * - map: a pointer to the bitmap_info_t structure that represents a population.
 * - member: an integer value that represents a unique member.
 *
 * Return:
 * - Returns true if the given member was present in the population before
 *   removal. Else, false.
 */

bool bitmap_rem(bitmap_info_t* map, uint32_t member);

/*
 * is_absent(member)
 * Checks if a member is absent from the population.
 *
 * Arguments:
 * - map: a pointer to the bitmap_info_t structure that represents a population.
 * - member: an integer value that represents a unique member.
 *
 * Return:
 * - Returns true if the given member is absent from the population. Else,
 *   false.
 */

bool bitmap_is_absent(bitmap_info_t* map, uint32_t member);

/*
 * is_present(member)
 * Check if a member is present in the population.
 *
 * Arguments:
 * - map: a pointer to the bitmap_info_t structure that represents a population.
 * - member: an integer value that represents a unique member.
 *
 * Return:
 * - Returns true if the given member is present in the population. Else,
 *   false.
 */

bool bitmap_is_present(bitmap_info_t* map, uint32_t member);

/*
 * find_first_absent(start)
 * Find the next absent member in the population starting from the given index.
 *
 * Arguments:
 * - map: a pointer to the bitmap_info_t structure that represents a population.
 * - start: an integer that represents the unique member value to begin search.
 * - member: an integer pointer that will be populated with the member
 *   identified as absent.
 *
 * Return:
 * - Returns true if a member is found to be absent. Else, returns false.
 */

bool bitmap_find_first_absent(bitmap_info_t* map, uint32_t start, uint32_t* member);

/*
 * find_absent_set(start, cnt)
 * Find a set of a given size of contiguous members that are absent from the 
 * population starting at the given index.
 *
 * Arguments:
 * - map: a pointer to the bitmap_info_t structure that represents a population.
 * - start: an integer that represents the unique member value to begin search.
 * - member: an integer pointer that will be populated with the first member of
 *   the absent set.
 *
 * Return:
 * - Returns true if a member set of sufficient size is found to be absent.
 *   Else, returns false.
 */

bool bitmap_find_absent_set(bitmap_info_t* map, uint32_t start, size_t cnt, uint32_t* first_member);

#endif /* _KLIB_BITMAP_H */
