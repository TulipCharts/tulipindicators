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



int ti_kama_start(TI_REAL const *options) {
    return (int)options[0]-1;
}


int ti_kama(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_kama_start(options)) return TI_OKAY;

    /* The caller selects the period used in the efficiency ratio.
     * The fast and slow periods are hard set by the algorithm. */
    const TI_REAL short_per = 2 / (2.0 + 1);
    const TI_REAL long_per = 2 / (30.0 + 1);

    TI_REAL sum = 0;

    int i;
    for (i = 1; i < period; ++i) {
        sum += fabs(input[i] - input[i-1]);
    }

    TI_REAL kama = input[period-1];
    *output++ = kama;
    TI_REAL er, sc;

    for (i = period; i < size; ++i) {
        sum += fabs(input[i] - input[i-1]);

        if (i > period) {
            sum -= fabs(input[i-period] - input[i-period-1]);
        }

        if (sum != 0.0) {
            er = fabs(input[i] - input[i-period]) / sum;
        } else {
            er = 1.0;
        }
        sc = pow(er * (short_per - long_per) + long_per, 2);

        kama = kama + sc * (input[i] - kama);
        *output++ = kama;
    }

    assert(output - outputs[0] == size - ti_kama_start(options));
    return TI_OKAY;
}
