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



int ti_vosc_start(TI_REAL const *options) {
    return (int)options[1]-1;
}


int ti_vosc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    TI_REAL *output = outputs[0];

    const int short_period = (int)options[0];
    const int long_period = (int)options[1];

    const TI_REAL short_div = 1.0 / short_period;
    const TI_REAL long_div = 1.0 / long_period;

    if (short_period < 1) return TI_INVALID_OPTION;
    if (long_period < short_period) return TI_INVALID_OPTION;
    if (size <= ti_vosc_start(options)) return TI_OKAY;

    TI_REAL short_sum = 0;
    TI_REAL long_sum = 0;

    int i;
    for (i = 0; i < long_period; ++i) {
        if (i >= (long_period - short_period)) {
            short_sum += input[i];
        }
        long_sum += input[i];
    }

    {
        const TI_REAL savg = short_sum * short_div;
        const TI_REAL lavg = long_sum * long_div;
        *output++ = 100.0 * (savg - lavg) / lavg;
    }

    for (i = long_period; i < size; ++i) {
        short_sum += input[i];
        short_sum -= input[i-short_period];

        long_sum += input[i];
        long_sum -= input[i-long_period];

        const TI_REAL savg = short_sum * short_div;
        const TI_REAL lavg = long_sum * long_div;
        *output++ = 100.0 * (savg - lavg) / lavg;
    }

    assert(output - outputs[0] == size - ti_vosc_start(options));
    return TI_OKAY;
}
