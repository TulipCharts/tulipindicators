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



int ti_rsi_start(TI_REAL const *options) {
    return (int)options[0];
}


int ti_rsi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    const TI_REAL per = 1.0 / ((TI_REAL)period);

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_rsi_start(options)) return TI_OKAY;

    TI_REAL smooth_up = 0, smooth_down = 0;

    int i;
    for (i = 1; i <= period; ++i) {
        const TI_REAL upward = input[i] > input[i-1] ? input[i] - input[i-1] : 0;
        const TI_REAL downward = input[i] < input[i-1] ? input[i-1] - input[i] : 0;
        smooth_up += upward;
        smooth_down += downward;
    }

    smooth_up /= period;
    smooth_down /= period;
    *output++ = 100.0 * (smooth_up / (smooth_up + smooth_down));

    for (i = period+1; i < size; ++i) {
        const TI_REAL upward = input[i] > input[i-1] ? input[i] - input[i-1] : 0;
        const TI_REAL downward = input[i] < input[i-1] ? input[i-1] - input[i] : 0;

        smooth_up = (upward-smooth_up) * per + smooth_up;
        smooth_down = (downward-smooth_down) * per + smooth_down;

        *output++ = 100.0 * (smooth_up / (smooth_up + smooth_down));
    }

    assert(output - outputs[0] == size - ti_rsi_start(options));
    return TI_OKAY;
}
