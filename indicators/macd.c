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


int ti_macd_start(TI_REAL const *options) {
    /* NB we return data before signal is strictly valid. */
    const int long_period = (int)options[1];
    return (long_period-1);
}


int ti_macd(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];

    TI_REAL *macd = outputs[0];
    TI_REAL *signal = outputs[1];
    TI_REAL *hist = outputs[2];

    const int short_period = (int)options[0];
    const int long_period = (int)options[1];
    const int signal_period = (int)options[2];

    if (short_period < 1) return TI_INVALID_OPTION;
    if (long_period < 2) return TI_INVALID_OPTION;
    if (long_period < short_period) return TI_INVALID_OPTION;
    if (signal_period < 1) return TI_INVALID_OPTION;

    if (size <= ti_macd_start(options)) return TI_OKAY;

    TI_REAL short_per = 2 / ((TI_REAL)short_period + 1);
    TI_REAL long_per = 2 / ((TI_REAL)long_period + 1);
    TI_REAL signal_per = 2 / ((TI_REAL)signal_period + 1);

    if (short_period == 12 && long_period == 26) {
        /* I don't like this, but it's what people expect. */
        short_per = 0.15;
        long_per = 0.075;
    }


    TI_REAL short_ema = input[0];
    TI_REAL long_ema = input[0];
    TI_REAL signal_ema = 0;

    int i;
    for (i = 1; i < size; ++i) {
        short_ema = (input[i]-short_ema) * short_per + short_ema;
        long_ema = (input[i]-long_ema) * long_per + long_ema;
        const TI_REAL out = short_ema - long_ema;

        if (i == long_period-1) {
            signal_ema = out;
        }
        if (i >= long_period-1) {
            signal_ema = (out-signal_ema) * signal_per + signal_ema;

            *macd++ = out;
            *signal++ = signal_ema;
            *hist++ = out - signal_ema;
        }
    }


    assert(macd - outputs[0] == size - ti_macd_start(options));
    assert(signal - outputs[1] == size - ti_macd_start(options));
    assert(hist - outputs[2] == size - ti_macd_start(options));
    return TI_OKAY;
}
