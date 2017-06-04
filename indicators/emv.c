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



int ti_emv_start(TI_REAL const *options) {
    (void)options;
    return 1;
}


int ti_emv(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *volume = inputs[2];

    (void)options;

    TI_REAL *output = outputs[0];

    if (size <= ti_emv_start(options)) return TI_OKAY;

    TI_REAL last = (high[0] + low[0]) * 0.5;

    int i;
    for (i = 1; i < size; ++i) {
        TI_REAL hl = (high[i] + low[i]) * 0.5;
        TI_REAL br = volume[i] / 10000.0 / (high[i] - low[i]);

        *output++ = (hl - last) / br;
        last = hl;
    }

    assert(output - outputs[0] == size - ti_emv_start(options));
    return TI_OKAY;
}
