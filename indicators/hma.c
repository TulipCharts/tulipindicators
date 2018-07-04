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
#include "../utils/buffer.h"


int ti_hma_start(TI_REAL const *options) {
    const int period = (int)options[0];
    const int periodsqrt = (int)(sqrt(period));
    return period + periodsqrt - 2;
}


int ti_hma(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_hma_start(options)) return TI_OKAY;

    /* HMA(input, N) = WMA((2 * WMA(input, N/2) - WMA(input, N)), sqrt(N)) */
    /* Need to do three WMAs, with periods N, N/2, and sqrt N.*/

    const int period2 = (int)(period / 2);
    const int periodsqrt = (int)(sqrt(period));

    const TI_REAL weights = period * (period+1) / 2;
    const TI_REAL weights2 = period2 * (period2+1) / 2;
    const TI_REAL weightssqrt = periodsqrt * (periodsqrt+1) / 2;

    TI_REAL sum = 0; /* Flat sum of previous numbers. */
    TI_REAL weight_sum = 0; /* Weighted sum of previous numbers. */

    TI_REAL sum2 = 0;
    TI_REAL weight_sum2 = 0;

    TI_REAL sumsqrt = 0;
    TI_REAL weight_sumsqrt = 0;

    /* Setup up the WMA(period) and WMA(period/2) on the input. */
    int i;
    for (i = 0; i < period-1; ++i) {
        weight_sum += input[i] * (i+1);
        sum += input[i];

        if (i >= period - period2) {
            weight_sum2 += input[i] * (i+1-(period-period2));
            sum2 += input[i];
        }
    }

    ti_buffer *buff = ti_buffer_new(periodsqrt);

    for (i = period-1; i < size; ++i) {
        weight_sum += input[i] * period;
        sum += input[i];

        weight_sum2 += input[i] * period2;
        sum2 += input[i];

        const TI_REAL wma = weight_sum / weights;
        const TI_REAL wma2 = weight_sum2 / weights2;
        const TI_REAL diff = 2 * wma2 - wma;

        weight_sumsqrt += diff * periodsqrt;
        sumsqrt += diff;

        ti_buffer_qpush(buff, diff);

        if (i >= (period-1) + (periodsqrt-1)) {
            *output++ = weight_sumsqrt / weightssqrt;

            weight_sumsqrt -= sumsqrt;
            sumsqrt -= ti_buffer_get(buff, 1);
        } else {
            weight_sumsqrt -= sumsqrt;
        }


        weight_sum -= sum;
        sum -= input[i-period+1];

        weight_sum2 -= sum2;
        sum2 -= input[i-period2+1];
    }

    ti_buffer_free(buff);

    assert(output - outputs[0] == size - ti_hma_start(options));
    return TI_OKAY;
}
