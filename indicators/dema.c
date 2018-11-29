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



int ti_dema_start(TI_REAL const *options) {
    const int period = (int)options[0];
    return (period-1) * 2;
}


int ti_dema(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_dema_start(options)) return TI_OKAY;

    const TI_REAL per = 2 / ((TI_REAL)period + 1);
    const TI_REAL per1 = 1.0 - per;

    /*Calculate EMA(input)*/
    TI_REAL ema = input[0];

    /*Calculate EMA(EMA(input))*/
    TI_REAL ema2 = ema;

    int i;
    for (i = 0; i < size; ++i) {
        ema = ema * per1 + input[i] * per;
        if (i == period-1) {
            ema2 = ema;
        }
        if (i >= period-1) {
            ema2 = ema2 * per1 + ema * per;
            if (i >= (period-1) * 2) {
                *output = ema * 2 - ema2;
                ++output;
            }
        }
    }

    assert(output - outputs[0] == size - ti_dema_start(options));
    return TI_OKAY;
}
