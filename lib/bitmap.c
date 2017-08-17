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

#include <algo/bitmap.h>

BitmapInfo_t* bitmap_create(size_t size) {
    /** NOT IMPLEMENTED **/
    return NULL;
}

bool bitmap_add(BitmapInfo_t* map, uint32_t member) {

    uint32_t index = member / BITMAP_BITS_PER_WORD;
    uint32_t offset = member % BITMAP_BITS_PER_WORD;

    if(index >= map->word_cnt)
        return false;

    uint32_t before = map->bitmap_arr[index];
    map->bitmap_arr[index] |= (1<<offset);
    return (map->bitmap_arr[index] == before ? false : true);
}

bool bitmap_rem(BitmapInfo_t* map, uint32_t member) {
    uint32_t index = member / BITMAP_BITS_PER_WORD;
    uint32_t offset = member % BITMAP_BITS_PER_WORD;

    if(index >= map->word_cnt)
        return false;

    uint32_t before = map->bitmap_arr[index];
    map->bitmap_arr[index] &= ~(1<<offset);
    return (map->bitmap_arr[index] == before ? false : true);
}

bool bitmap_is_absent(BitmapInfo_t* map, uint32_t member) {
    uint32_t index = member / BITMAP_BITS_PER_WORD;
    uint32_t offset = member % BITMAP_BITS_PER_WORD;

    if(index >= map->word_cnt)
        return false;

    return (map->bitmap_arr[index] & (1<<offset)) == 0;
}

bool bitmap_is_present(BitmapInfo_t* map, uint32_t member) {
    uint32_t index = member / BITMAP_BITS_PER_WORD;
    uint32_t offset = member % BITMAP_BITS_PER_WORD;

    if(index >= map->word_cnt)
        return false;

    return (map->bitmap_arr[index] & (1<<offset)) > 0;
}

bool bitmap_find_first_absent(BitmapInfo_t* map, uint32_t start, uint32_t* member) {
    uint32_t max_member = map->word_cnt * BITMAP_BITS_PER_WORD;
    for(*member = start; *member < max_member; (*member)++)
        if(bitmap_is_absent(map, *member))
            return true;
    return false;
}

bool bitmap_find_first_absent_between(BitmapInfo_t *map, uint32_t start, uint32_t limit, uint32_t *member) {
    for(*member = start; *member < limit; (*member)++)
        if(bitmap_is_absent(map, *member))
            return true;
    return false;
}

bool bitmap_find_absent_set(BitmapInfo_t* map, uint32_t start, size_t cnt, uint32_t* first_member) {

    uint32_t member;
    uint32_t found_members = 0;
    uint32_t max_member = map->word_cnt * BITMAP_BITS_PER_WORD;

    for(member = start; member < max_member; member++) {

        if(bitmap_is_absent(map, member)) {

            if(++found_members == cnt) {
                *first_member = member - found_members + 1;
                return true;
            }

        } else
            found_members = 0;
    }

    return false;
}

bool bitmap_find_absent_set_between(BitmapInfo_t* map, uint32_t start, size_t cnt, uint32_t limit, uint32_t* first_member) {
    uint32_t member;
    uint32_t found_members = 0;

    for(member = start; member < limit; member++) {

        if(bitmap_is_absent(map, member)) {

            if(++found_members == cnt) {
                *first_member = member - found_members + 1;
                return true;
            }

        } else
            found_members = 0;
    }

    return false;
}

