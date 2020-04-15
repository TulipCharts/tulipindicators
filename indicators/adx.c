/*
 * Tulip Indicators
 * https://tulipindicators.org/
 * Copyright (c) 2010-2018 Tulip Charts LLC
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


int ti_adx_start(TI_REAL const *options) {
    return ((int)options[0]-1) * 2;
}


int ti_adx(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];

    const int period = (int)options[0];

    TI_REAL *output = outputs[0];

    if (period < 2) return TI_INVALID_OPTION;
    if (size <= ti_adx_start(options)) return TI_OKAY;

    const TI_REAL per = ((TI_REAL)period-1) / ((TI_REAL)period);
    const TI_REAL invper = 1.0 / ((TI_REAL)period);


    TI_REAL dmup = 0;
    TI_REAL dmdown = 0;

    int i;
    for (i = 1; i < period; ++i) {
        TI_REAL dp, dm;
        CALC_DIRECTION(dp, dm);

        dmup += dp;
        dmdown += dm;
    }

    TI_REAL adx = 0.0;

    {
        TI_REAL di_up = dmup;
        TI_REAL di_down = dmdown;
        TI_REAL dm_diff = fabs(di_up - di_down);
        TI_REAL dm_sum = di_up + di_down;
        TI_REAL dx = dm_diff / dm_sum * 100;

        adx += dx;
    }


    for (i = period; i < size; ++i) {

        TI_REAL dp, dm;
        CALC_DIRECTION(dp, dm);


        dmup = dmup * per + dp;
        dmdown = dmdown * per + dm;


        TI_REAL di_up = dmup;
        TI_REAL di_down = dmdown;
        TI_REAL dm_diff = fabs(di_up - di_down);
        TI_REAL dm_sum = di_up + di_down;
        TI_REAL dx = dm_diff / dm_sum * 100;


        if (i-period < period-2) {
            adx += dx;
        } else if (i-period == period-2) {
            adx += dx;
            *output++ = adx * invper;
        } else {
            adx = adx * per + dx;
            *output++ = adx * invper;
        }

    }

    assert(output - outputs[0] == size - ti_adx_start(options));
    return TI_OKAY;
}
