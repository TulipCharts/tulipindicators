/*
 * Tulip Indicators
 * https://tulipindicators.org/
 * Copyright (c) 2010-2020 Tulip Charts LLC
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


int ti_abands_start(TI_REAL const *options) {
    const TI_REAL period = options[0];
    return (int)period-1;
}


/* Name: Acceleration Bands
 * Source: Price Headley. Big Trends In Trading, 2002, p. 92. Original description
 * ISBN: 0-471-41269-4
*/

int ti_abands(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    const int period = (int)options[0];
    TI_REAL *lower_band = outputs[0];
    TI_REAL *upper_band = outputs[1];
    TI_REAL *middle_point = outputs[2];

    if (period < 1) { return TI_INVALID_OPTION; }
    if (size <= ti_abands_start(options)) return TI_OKAY;

    #define MULT(i) (4. * (high[i] - low[i]) / (high[i] + low[i]))

    TI_REAL per = 1. / period;

    ti_buffer *buffer_high = ti_buffer_new(period);
    ti_buffer *buffer_low = ti_buffer_new(period);
    TI_REAL close_sum = 0;

    for (int i = 0; i < period; ++i) {
        TI_REAL mult = MULT(i);

        TI_REAL high_val = (1. + mult) * high[i];
        ti_buffer_push(buffer_high, high_val);

        TI_REAL low_val = (1. - mult) * low[i];
        ti_buffer_push(buffer_low, low_val);

        close_sum += close[i];
    }

    *upper_band++ = buffer_high->sum * per;
    *lower_band++ = buffer_low->sum * per;
    *middle_point++ = close_sum * per;

    for (int i = period; i < size; ++i) {
        TI_REAL mult = MULT(i);

        TI_REAL high_val = (1. + mult) * high[i];
        ti_buffer_push(buffer_high, high_val);

        TI_REAL low_val = (1. - mult) * low[i];
        ti_buffer_push(buffer_low, low_val);

        close_sum += close[i] - close[i-period];

        *upper_band++ = buffer_high->sum * per;
        *lower_band++ = buffer_low->sum * per;
        *middle_point++ = close_sum * per;
    }

    ti_buffer_free(buffer_high);
    ti_buffer_free(buffer_low);

    assert(lower_band - outputs[0] == size - ti_abands_start(options));
    return TI_OKAY;
}


int ti_abands_ref(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    const int period = (int)options[0];
    const TI_REAL inv_period = 1.0 / period;
    TI_REAL *lower_band = outputs[0];
    TI_REAL *upper_band = outputs[1];
    TI_REAL *middle_point = outputs[2];

    if (period < 1) { return TI_INVALID_OPTION; }
    if (size <= ti_abands_start(options)) return TI_OKAY;

    for (int i = period-1; i < size; ++i) {
        TI_REAL upper = 0;
        TI_REAL mid = 0;
        TI_REAL lower = 0;
        for (int j = i-period+1; j <= i; ++j) {
            upper += (high[j] * (1. + 2. * ((((high[j] - low[j]) / ((high[j] + low[j]) / 2.)) * 1000.) * .001))) * inv_period;
            mid += close[j] * inv_period;
            lower += (low[j] * (1. - 2. * ((((high[j] - low[j]) / ((high[j] + low[j]) / 2.)) * 1000.) * .001))) * inv_period;
        }
        *upper_band++ = upper;
        *middle_point++ = mid;
        *lower_band++ = lower;
    }

    assert(lower_band - outputs[0] == size - ti_abands_start(options));
    return TI_OKAY;
}
