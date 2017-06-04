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



int ti_obv_start(TI_REAL const *options) {
    (void)options;
    return 0;
}


int ti_obv(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *close = inputs[0];
    const TI_REAL *volume = inputs[1];

    (void)options;

    TI_REAL *output = outputs[0];
    TI_REAL sum = 0;
    *output++ = sum;

    TI_REAL prev = close[0];
    int i;
    for (i = 1; i < size; ++i) {
        if (close[i] > prev) {
            sum += volume[i];
        } else if (close[i] < prev) {
            sum -= volume[i];
        } else {
            /* No change. */
        }

        prev = close[i];
        *output++ = sum;
    }

    return TI_OKAY;
}
