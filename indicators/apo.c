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



int ti_apo_start(TI_REAL const *options) {
    (void)options;
    return 1;
}


int ti_apo(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];

    TI_REAL *apo = outputs[0];

    const int short_period = (int)options[0];
    const int long_period = (int)options[1];

    if (short_period < 1) return TI_INVALID_OPTION;
    if (long_period < 2) return TI_INVALID_OPTION;
    if (long_period < short_period) return TI_INVALID_OPTION;

    if (size <= ti_apo_start(options)) return TI_OKAY;

    TI_REAL short_per = 2 / ((TI_REAL)short_period + 1);
    TI_REAL long_per = 2 / ((TI_REAL)long_period + 1);

    TI_REAL short_ema = input[0];
    TI_REAL long_ema = input[0];

    int i;
    for (i = 1; i < size; ++i) {
        short_ema = (input[i]-short_ema) * short_per + short_ema;
        long_ema = (input[i]-long_ema) * long_per + long_ema;
        const TI_REAL out = short_ema - long_ema;

        *apo++ = out;
    }

    assert(apo - outputs[0] == size - ti_apo_start(options));
    return TI_OKAY;
}
