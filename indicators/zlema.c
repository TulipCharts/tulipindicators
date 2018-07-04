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



int ti_zlema_start(TI_REAL const *options) {
    return ((int)options[0] - 1) / 2 - 1;
}


int ti_zlema(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];

    const int period = (int)options[0];
    const int lag = (period - 1) / 2;

    TI_REAL *output = outputs[0];

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_zlema_start(options)) return TI_OKAY;

    const TI_REAL per = 2 / ((TI_REAL)period + 1);

    TI_REAL val = input[lag-1];
    *output++ = val;

    int i;
    for (i = lag; i < size; ++i) {
        TI_REAL c = input[i];
        TI_REAL l = input[i-lag];

        val = ((c + (c-l))-val) * per + val;
        *output++ = val;
    }


    assert(output - outputs[0] == size - ti_zlema_start(options));
    return TI_OKAY;
}
