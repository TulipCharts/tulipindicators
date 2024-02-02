/*
* Tulip Indicators
* https://tulipindicators.org/
* Copyright (c) 2010-2016 Tulip Charts LLC
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


int ti_corr_start(TI_REAL const *options) {
    return (int)options[0] - 1;
}

int ti_corr(int size, TI_REAL const * const *inputs, TI_REAL const *options, TI_REAL * const *outputs) {
    const TI_REAL *x = inputs[0];
    const TI_REAL *y = inputs[1];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];

    if (period < 1 || size <= ti_corr_start(options)) return TI_INVALID_OPTION;

    for (int i = 0; i <= size - period; ++i)
    {
        double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0, sumY2 = 0;
        for (int j = 0; j < period; ++j) {
            const TI_REAL xij = x[i + j];
            const TI_REAL yij = y[i + j];
            sumX += xij;
            sumY += yij;
            sumXY += xij * yij;
            sumX2 += xij * xij;
            sumY2 += yij * yij;
        }

        *output++ = (period * sumXY - sumX * sumY) /
            sqrt((period * sumX2 - sumX * sumX) * (period * sumY2 - sumY * sumY));
    }

    // Ensure the correct number of output values is calculated.
    assert(output - outputs[0] == size - ti_corr_start(options));

    return TI_OKAY;
}
