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



int ti_trix_start(TI_REAL const *options) {
    const int period = (int)options[0];
    return ((period-1)*3)+1;
}


int ti_trix(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_trix_start(options)) return TI_OKAY;

    const int start = (period*3)-2;
    assert(start == ti_trix_start(options));
    const TI_REAL per = 2 / ((TI_REAL)period + 1);

    TI_REAL ema1 = input[0];
    TI_REAL ema2 = 0, ema3 = 0;

    int i;
    for (i = 1; i < start; ++i) {
        ema1 = (input[i]-ema1) * per + ema1;

        if (i == period-1) {
            ema2 = ema1;
        } else if (i > period-1) {
            ema2 = (ema1-ema2) * per + ema2;

            if (i == period * 2 - 2) {
                ema3 = ema2;
            } else if (i > period * 2 - 2) {
                ema3 = (ema2-ema3) * per + ema3;
            }
        }
    }

    for (i = start; i < size; ++i) {
        ema1 = (input[i]-ema1) * per + ema1;
        ema2 = (ema1-ema2) * per + ema2;
        const TI_REAL last = ema3;
        ema3 = (ema2-ema3) * per + ema3;
        *output++ = (ema3-last)/ema3 * 100.0;
    }


    assert(output - outputs[0] == size - ti_trix_start(options));
    return TI_OKAY;

}
