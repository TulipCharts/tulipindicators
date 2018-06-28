/*
 * Tulip Indicators
 * https://tulipindicators.org/
 * Copyright (c) 2010-2017 Tulip Charts LLC
 * Lewis Van Winkle (LV@tulipcharts.org)
 *
 * This file is part of Tulip Indicators.
 *
 * Tulip Indicators is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * Tulip Indicators is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Tulip Indicators.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __BUFFER_H__
#define __BUFFER_H__


typedef struct {
    int size, pushes, index;
    TI_REAL sum;
    TI_REAL vals[1];
} ti_buffer;


ti_buffer *ti_buffer_new(int size);
void ti_buffer_free(ti_buffer *buffer);

/* Pushes a new value, plus updates sum. */
#define ti_buffer_push(BUFFER, VAL) \
do { \
    if ((BUFFER)->pushes >= (BUFFER)->size) { \
        (BUFFER)->sum -= (BUFFER)->vals[(BUFFER)->index]; \
    } \
\
    (BUFFER)->sum += (VAL); \
    (BUFFER)->vals[(BUFFER)->index] = (VAL); \
    (BUFFER)->pushes += 1; \
    (BUFFER)->index = ((BUFFER)->index + 1); \
    if ((BUFFER)->index >= (BUFFER)->size) (BUFFER)->index = 0; \
} while (0)


/* Pushes a new value, skips updating sum. */
#define ti_buffer_qpush(BUFFER, VAL) \
do { \
    (BUFFER)->vals[(BUFFER)->index] = (VAL); \
    (BUFFER)->index = ((BUFFER)->index + 1); \
    if ((BUFFER)->index >= (BUFFER)->size) (BUFFER)->index = 0; \
} while (0)


/* With get, 0 = last value pushed, -1 = value before last, etc. */
#define ti_buffer_get(BUFFER, VAL) ((BUFFER)->vals[((BUFFER)->index + (BUFFER)->size - 1 + (VAL)) % (BUFFER)->size])


#endif /*__BUFFER_H__*/
