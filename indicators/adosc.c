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



int ti_adosc_start(TI_REAL const *options) {
    return (int)(options[1])-1;
}


int ti_adosc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    const TI_REAL *volume = inputs[3];

    const int short_period = (int)options[0];
    const int long_period = (int)options[1];

    const int start = long_period - 1;

    if (short_period < 1) return TI_INVALID_OPTION;
    if (long_period < short_period) return TI_INVALID_OPTION;
    if (size <= ti_adosc_start(options)) return TI_OKAY;

    const TI_REAL short_per = 2 / ((TI_REAL)short_period + 1);
    const TI_REAL long_per = 2 / ((TI_REAL)long_period + 1);

    TI_REAL *output = outputs[0];
    TI_REAL sum = 0, short_ema = 0, long_ema = 0;
    int i;
    for (i = 0; i < size; ++i) {
        const TI_REAL hl = (high[i] - low[i]);
        if (hl != 0.0) {
            sum += (close[i] - low[i] - high[i] + close[i]) / hl * volume[i];
        }

        if (i == 0) {
            short_ema = sum;
            long_ema = sum;
        } else {
            short_ema = (sum-short_ema) * short_per + short_ema;
            long_ema = (sum-long_ema) * long_per + long_ema;
        }

        if (i >= start) {
            *output++ = short_ema - long_ema;
        }
    }

    assert(output - outputs[0] == size - ti_adosc_start(options));

    return TI_OKAY;
}

