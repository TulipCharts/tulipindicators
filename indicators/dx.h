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

#ifndef __DX_H__
#define __DX_H__



/* This is used with the DX family of indicators.
 * e.g. DX ADX ADXR
 */


#include "truerange.h"



#define CALC_DIRECTION(up, down) do {\
        up = high[i] - high[i-1];\
        down = low[i-1] - low[i];\
\
        if (up < 0)\
            up = 0;\
        else if (up > down)\
            down = 0;\
\
        if (down < 0)\
            down = 0;\
        else if (down > up)\
            up = 0;\
} while (0)



#endif /*__DX_H__*/
