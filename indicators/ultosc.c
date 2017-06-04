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
#include "../utils/minmax.h"



int ti_ultosc_start(TI_REAL const *options) {
    return (int)options[2];
}


int ti_ultosc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];

    const int short_period = (int)options[0];
    const int medium_period = (int)options[1];
    const int long_period = (int)options[2];

    TI_REAL *output = outputs[0];

    if (short_period < 1) return TI_INVALID_OPTION;
    if (medium_period < short_period) return TI_INVALID_OPTION;
    if (long_period < medium_period) return TI_INVALID_OPTION;
    if (size <= ti_ultosc_start(options)) return TI_OKAY;


    ti_buffer *bp_buf = ti_buffer_new(long_period);
    ti_buffer *r_buf = ti_buffer_new(long_period);

    TI_REAL bp_short_sum = 0, bp_medium_sum = 0;
    TI_REAL r_short_sum = 0, r_medium_sum = 0;

    int i;
    for (i = 1; i < size; ++i) {

        const TI_REAL true_low = MIN(low[i], close[i-1]);
        const TI_REAL true_high = MAX(high[i], close[i-1]);
        const TI_REAL bp = close[i] - true_low;
        const TI_REAL r = true_high - true_low;

        bp_short_sum += bp;
        bp_medium_sum += bp;
        r_short_sum += r;
        r_medium_sum += r;

        ti_buffer_push(bp_buf, bp);
        ti_buffer_push(r_buf, r);

        /* The long sum takes care of itself, but we're piggy-backing
         * the medium and short sums off the same buffers. */
        if (i > short_period) {
            int short_index = bp_buf->index - short_period - 1;
            if (short_index < 0) short_index += long_period;
            bp_short_sum -= bp_buf->vals[short_index];
            r_short_sum -= r_buf->vals[short_index];

            if (i > medium_period) {
                int medium_index = bp_buf->index - medium_period - 1;
                if (medium_index < 0) medium_index += long_period;
                bp_medium_sum -= bp_buf->vals[medium_index];
                r_medium_sum -= r_buf->vals[medium_index];
            }
        }

        if (i >= long_period) {
            const TI_REAL first = 4 * bp_short_sum / r_short_sum;
            const TI_REAL second = 2 * bp_medium_sum / r_medium_sum;
            const TI_REAL third = 1 * bp_buf->sum / r_buf->sum;
            const TI_REAL ult = (first + second + third) * 100.0 / 7.0;
            *output++ = ult;
        }

    }

    ti_buffer_free(bp_buf);
    ti_buffer_free(r_buf);

    assert(output - outputs[0] == size - ti_ultosc_start(options));

    return TI_OKAY;

}
