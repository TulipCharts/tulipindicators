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


int ti_mass_start(TI_REAL const *options) {
    int sum_p = (int)options[0]-1;
    /* The ema uses a hard-coded period of 9.
     * (9-1)*2 = 16 */
    return 16 + sum_p;
}


int ti_mass(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_mass_start(options)) return TI_OKAY;

    /*mass uses a hard-coded 9 period for the ema*/
    const TI_REAL per = 2 / (9.0 + 1);
    const TI_REAL per1 = 1.0 - per;

    /*Calculate EMA(h-l)*/
    TI_REAL ema = high[0] - low[0];

    /*Calculate EMA(EMA(h-l))*/
    TI_REAL ema2 = ema;

    ti_buffer *sum = ti_buffer_new(period);

    int i;
    for (i = 0; i < size; ++i) {
        TI_REAL hl = high[i] - low[i];

        ema = ema * per1 + hl * per;

        if (i == 8) {
            ema2 = ema;
        }
        if (i >= 8) {
            ema2 = ema2 * per1 + ema * per;

            if (i >= 16) {
                ti_buffer_push(sum, ema/ema2);
                if (i >= 16 + period - 1) {
                    *output++ = sum->sum;
                }
            }
        }
    }

    ti_buffer_free(sum);

    assert(output - outputs[0] == size - ti_mass_start(options));
    return TI_OKAY;
}
