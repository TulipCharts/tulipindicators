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



int ti_pvi_start(TI_REAL const *options) {
    (void)options;
    return 0;
}


int ti_pvi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *close = inputs[0];
    const TI_REAL *volume = inputs[1];

    (void)options;

    TI_REAL *output = outputs[0];

    if (size <= ti_pvi_start(options)) return TI_OKAY;

    TI_REAL pvi = 1000;
    *output++ = pvi;

    int i;
    for (i = 1; i < size; ++i) {

        if (volume[i] > volume[i-1]) {
            pvi += ((close[i] - close[i-1])/close[i-1]) * pvi;
        }
        *output++ = pvi;
    }

    assert(output - outputs[0] == size - ti_pvi_start(options));
    return TI_OKAY;
}
