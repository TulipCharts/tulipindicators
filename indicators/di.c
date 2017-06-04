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

#include "../indicators.h"
#include "dx.h"


int ti_di_start(TI_REAL const *options) {
    return (int)options[0]-1;
}


int ti_di(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];

    const int period = (int)options[0];

    TI_REAL *plus_di = outputs[0];
    TI_REAL *minus_di = outputs[1];

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_di_start(options)) return TI_OKAY;

    const TI_REAL per = ((TI_REAL)period-1) / ((TI_REAL)period);

    TI_REAL atr = 0;
    TI_REAL dmup = 0;
    TI_REAL dmdown = 0;

    int i;
    for (i = 1; i < period; ++i) {
        TI_REAL truerange;
        CALC_TRUERANGE();
        atr += truerange;

        TI_REAL dp, dm;
        CALC_DIRECTION(dp, dm);

        dmup += dp;
        dmdown += dm;
    }


    *plus_di++  = 100.0 * dmup / atr;
    *minus_di++ = 100.0 * dmdown / atr;


    for (i = period; i < size; ++i) {
        TI_REAL truerange;
        CALC_TRUERANGE();
        atr = atr * per + truerange;


        TI_REAL dp, dm;
        CALC_DIRECTION(dp, dm);


        dmup = dmup * per + dp;
        dmdown = dmdown * per + dm;

        *plus_di++  = 100.0 * dmup / atr;
        *minus_di++ = 100.0 * dmdown / atr;
    }


    assert(plus_di - outputs[0] == size - ti_di_start(options));
    assert(minus_di - outputs[1] == size - ti_di_start(options));

    return TI_OKAY;
}
