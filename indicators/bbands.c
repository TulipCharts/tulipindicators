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



int ti_bbands_start(TI_REAL const *options) {
    return (int)options[0]-1;
}


int ti_bbands(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    TI_REAL *lower = outputs[0];
    TI_REAL *middle = outputs[1];
    TI_REAL *upper = outputs[2];

    const int period = (int)options[0];

    const TI_REAL stddev = options[1];
    const TI_REAL scale = 1.0 / period;

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_bbands_start(options)) return TI_OKAY;

    TI_REAL sum = 0;
    TI_REAL sum2 = 0;

    int i;
    for (i = 0; i < period; ++i) {
        sum += input[i];
        sum2 += input[i] * input[i];
    }

    TI_REAL sd = sqrt(sum2 * scale - (sum * scale) * (sum * scale));
    *middle = sum * scale;
    *lower++ = *middle - stddev * sd;
    *upper++ = *middle + stddev * sd;
    ++middle;

    for (i = period; i < size; ++i) {
        sum += input[i];
        sum2 += input[i] * input[i];

        sum -= input[i-period];
        sum2 -= input[i-period] * input[i-period];

        sd = sqrt(sum2 * scale - (sum * scale) * (sum * scale));
        *middle = sum * scale;
        *upper++ = *middle + stddev * sd;
        *lower++ = *middle - stddev * sd;
        ++middle;
    }

    assert(lower - outputs[0] == size - ti_bbands_start(options));
    assert(middle - outputs[1] == size - ti_bbands_start(options));
    assert(upper - outputs[2] == size - ti_bbands_start(options));
    return TI_OKAY;
}
