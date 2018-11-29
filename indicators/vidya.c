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


int ti_vidya_start(TI_REAL const *options) {
    return ((int)(options[1])) - 2;
}


int ti_vidya(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];

    const int short_period = (int)options[0];
    const int long_period = (int)options[1];
    const TI_REAL alpha = options[2];

    TI_REAL *output = outputs[0];

    const TI_REAL short_div = 1.0 / short_period;
    const TI_REAL long_div = 1.0 / long_period;

    if (short_period < 1) return TI_INVALID_OPTION;
    if (long_period < short_period) return TI_INVALID_OPTION;
    if (long_period < 2) return TI_INVALID_OPTION;
    if (alpha < 0.0 || alpha > 1.0) return TI_INVALID_OPTION;
    if (size <= ti_vidya_start(options)) return TI_OKAY;

    TI_REAL short_sum = 0;
    TI_REAL short_sum2 = 0;

    TI_REAL long_sum = 0;
    TI_REAL long_sum2 = 0;

    int i;
    for (i = 0; i < long_period; ++i) {
        long_sum += input[i];
        long_sum2 += input[i] * input[i];

        if (i >= long_period - short_period) {
            short_sum += input[i];
            short_sum2 += input[i] * input[i];
        }
    }

    TI_REAL val = input[long_period-2];
    *output++ = val;

    if (long_period - 1 < size) {
        TI_REAL short_stddev = sqrt(short_sum2 * short_div - (short_sum * short_div) * (short_sum * short_div));
        TI_REAL long_stddev = sqrt(long_sum2 * long_div - (long_sum * long_div) * (long_sum * long_div));
        TI_REAL k = short_stddev / long_stddev;
        if (k != k) k = 0; /* In some conditions it works out that we take the sqrt(-0.0), which gives NaN.
                              That implies that k should be zero. */
        k *= alpha;
        val = (input[long_period-1]-val) * k + val;
        *output++ = val;
    }

    for (i = long_period; i < size; ++i) {
        long_sum += input[i];
        long_sum2 += input[i] * input[i];

        short_sum += input[i];
        short_sum2 += input[i] * input[i];

        long_sum -= input[i-long_period];
        long_sum2 -= input[i-long_period] * input[i-long_period];

        short_sum -= input[i-short_period];
        short_sum2 -= input[i-short_period] * input[i-short_period];

        {
            TI_REAL short_stddev = sqrt(short_sum2 * short_div - (short_sum * short_div) * (short_sum * short_div));
            TI_REAL long_stddev = sqrt(long_sum2 * long_div - (long_sum * long_div) * (long_sum * long_div));
            TI_REAL k = short_stddev / long_stddev;
            if (k != k) k = 0;
            k *= alpha;
            val = (input[i]-val) * k + val;

            *output++ = val;
        }
    }

    assert(output - outputs[0] == size - ti_vidya_start(options));
    return TI_OKAY;
}
