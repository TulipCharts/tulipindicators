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

int ti_rmta_start(TI_REAL const *options) {
    return (int)options[0]-1;
}

/* Name: Recursive Moving Trend Average
 * Source: Dennis Meyers. The Japanese Yen, Recursed, 1998. Original description
/* URL: https://meyersanalytics.com/publications2/jyrecursed.pdf
 */

int ti_rmta(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    const TI_REAL beta = options[1];
    TI_REAL *output = outputs[0];

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_rmta_start(options)) return TI_OKAY;

    const TI_REAL alpha = 1. - beta;
    TI_REAL b = (1. - alpha) * input[0] + input[0];
    TI_REAL rmta = (1. - alpha) * input[0] + alpha * (input[0] + b);

    for (int i = 1; i < period-1; ++i) {
        TI_REAL next_b = (1. - alpha) * b + input[i];
        rmta = (1. - alpha) * rmta + alpha * (input[i] + next_b - b);
        b = next_b;
    }

    for (int i = period-1; i < size; ++i) {
        TI_REAL next_b = (1. - alpha) * b + input[i];
        rmta = (1. - alpha) * rmta + alpha * (input[i] + next_b - b);
        b = next_b;
        *output++ = rmta;
    }

    assert(output - outputs[0] == size - ti_rmta_start(options));
    return TI_OKAY;
}