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

#define TYPPRICE(INDEX) ((high[(INDEX)] + low[(INDEX)] + close[(INDEX)]) * (1.0/3.0))


int ti_cci_start(TI_REAL const *options) {
    const int period = (int)options[0];
    return (period-1) * 2;
}


int ti_cci(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];

    const int period = (int)options[0];

    const TI_REAL scale = 1.0 / period;

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_cci_start(options)) return TI_OKAY;

    TI_REAL *output = outputs[0];

    ti_buffer *sum = ti_buffer_new(period);

    int i, j;
    for (i = 0; i < size; ++i) {
        const TI_REAL today = TYPPRICE(i);
        ti_buffer_push(sum, today);
        const TI_REAL avg = sum->sum * scale;

        if (i >= period * 2 - 2) {
            TI_REAL acc = 0;
            for (j = 0; j < period; ++j) {
                acc += fabs(avg - sum->vals[j]);
            }

            TI_REAL cci = acc * scale;
            cci *= .015;
            cci = (today-avg)/cci;
            *output++ = cci;
        }
    }

    ti_buffer_free(sum);

    assert(output - outputs[0] == size - ti_cci_start(options));
    return TI_OKAY;
}
