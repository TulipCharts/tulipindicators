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


int ti_ao_start(TI_REAL const *options) {
    (void)options;
    return 33;
}


int ti_ao(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];

    const int period = 34;

    TI_REAL *output = outputs[0];

    if (size <= ti_ao_start(options)) return TI_OKAY;

    TI_REAL sum34 = 0;
    TI_REAL sum5 = 0;
    const TI_REAL per34 = 1.0 / 34.0;
    const TI_REAL per5 = 1.0 / 5.0;

    int i;
    for (i = 0; i < 34; ++i) {
        TI_REAL hl = 0.5 * (high[i] + low[i]);
        sum34 += hl;
        if (i >= 29) sum5 += hl;
    }

    *output++ = (per5 * sum5 - per34 * sum34);

    for (i = period; i < size; ++i) {
        TI_REAL hl = 0.5 * (high[i] + low[i]);
        sum34 += hl;
        sum5 += hl;

        sum34 -= 0.5 * (high[i-34] + low[i-34]);
        sum5  -= 0.5 * (high[i-5] + low[i-5]);

        *output++ = (per5 * sum5 - per34 * sum34);
    }

    assert(output - outputs[0] == size - ti_ao_start(options));
    return TI_OKAY;

}
