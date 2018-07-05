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




#ifndef POSTPROC
#define POSTPROC(x) (x)
#endif


#ifndef INIT

#define INIT() const TI_REAL p = (1.0 / (period))

#define FINAL(forecast) do { \
        const TI_REAL a = (y - b * x) * p; \
        *output++ = POSTPROC(a + b * (forecast)); \
} while (0)

#endif





#define LINEAR_REGRESSION(size, input, period, output, forecast) \
    do { \
    TI_REAL x = 0; /* Sum of Xs. */ \
    TI_REAL x2 = 0; /* Sum of square of Xs. */ \
 \
    TI_REAL y = 0; /* Flat sum of previous numbers. */ \
    TI_REAL xy = 0; /* Weighted sum of previous numbers. */ \
 \
    INIT(); \
 \
    int i; \
    for (i = 0; i < (period)-1; ++i) { \
        x += i+1; \
        x2 += (i+1)*(i+1); \
        xy += (input)[i] * (i+1); \
        y += (input)[i]; \
    } \
 \
    x += (period); \
    x2 += (period) * (period); \
 \
    const TI_REAL bd = 1.0 / ((period) * x2 - x * x); \
 \
    for (i = (period)-1; i < (size); ++i) { \
        xy += (input)[i] * (period); \
        y += (input)[i]; \
 \
        const TI_REAL b = ((period) * xy - x * y) * bd; \
        FINAL((forecast)); \
 \
        xy -= y; \
        y -= (input)[i-(period)+1]; \
    } \
} while (0)


