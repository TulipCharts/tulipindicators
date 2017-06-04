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


#define CALC_TRUERANGE() do{\
        const TI_REAL l = low[i];\
        const TI_REAL h = high[i];\
        const TI_REAL c = close[i-1];\
        const TI_REAL ych = fabs(h - c);\
        const TI_REAL ycl = fabs(l - c);\
        TI_REAL v = h - l;\
        if (ych > v) v = ych;\
        if (ycl > v) v = ycl;\
        truerange = v;\
}while(0)

