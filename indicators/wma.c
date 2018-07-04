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



int ti_wma_start(TI_REAL const *options) {
    return (int)options[0]-1;
}


int ti_wma(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_wma_start(options)) return TI_OKAY;

    /* Weights for 6 period WMA:
     * 1 2 3 4 5 6
     */

    const TI_REAL weights = period * (period+1) / 2;

    TI_REAL sum = 0; /* Flat sum of previous numbers. */
    TI_REAL weight_sum = 0; /* Weighted sum of previous numbers. */

    int i;
    for (i = 0; i < period-1; ++i) {
        weight_sum += input[i] * (i+1);
        sum += input[i];
    }

    for (i = period-1; i < size; ++i) {
        weight_sum += input[i] * period;
        sum += input[i];

        *output++ = weight_sum / weights;

        weight_sum -= sum;
        sum -= input[i-period+1];
    }


    assert(output - outputs[0] == size - ti_wma_start(options));
    return TI_OKAY;
}
