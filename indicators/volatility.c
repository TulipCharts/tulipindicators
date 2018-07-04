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

#define CHANGE(i) (input[i]/input[i-1]-1.0)



int ti_volatility_start(TI_REAL const *options) {
    return (int)options[0];
}


int ti_volatility(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    TI_REAL *output = outputs[0];
    const int period = (int)options[0];
    const TI_REAL scale = 1.0 / period;
    const TI_REAL annual = sqrt(252); /* Multiplier, number of trading days in year. */

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_volatility_start(options)) return TI_OKAY;

    TI_REAL sum = 0;
    TI_REAL sum2 = 0;

    int i;
    for (i = 1; i <= period; ++i) {
        const TI_REAL c = CHANGE(i);
        sum += c;
        sum2 += c * c;
    }

    *output++ = sqrt(sum2 * scale - (sum * scale) * (sum * scale)) * annual;

    for (i = period+1; i < size; ++i) {
        const TI_REAL c = CHANGE(i);
        sum += c;
        sum2 += c * c;

        const TI_REAL cp = CHANGE(i-period);
        sum -= cp;
        sum2 -= cp * cp;

        *output++ = sqrt(sum2 * scale - (sum * scale) * (sum * scale)) * annual;
    }

    assert(output - outputs[0] == size - ti_volatility_start(options));
    return TI_OKAY;

}
