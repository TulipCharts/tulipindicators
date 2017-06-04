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

#ifndef __SIMPLE1_H__
#define __SIMPLE1_H__

/* This is used for the simple functions that
 * take one input vectors and apply a unary
 * operator for a single output.
 * (e.g. sqrt, sin)
 */

/* Fallback */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


#define SIMPLE1(START, FUN, OP) \
int START(TI_REAL const *options) { \
    (void)options; \
    return 0; \
} \
 \
 \
int FUN(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { \
    const TI_REAL *in1 = inputs[0]; \
 \
    (void)options; \
 \
    TI_REAL *output = outputs[0]; \
 \
    int i; \
    for (i = 0; i < size; ++i) { \
        output[i] = (OP); \
    } \
 \
    return TI_OKAY; \
} \

#endif /*__SIMPLE1_H__*/
