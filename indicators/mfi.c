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
#include "../utils/buffer.h"

#define TYPPRICE(INDEX) ((high[(INDEX)] + low[(INDEX)] + close[(INDEX)]) * (1.0/3.0))

int ti_mfi_start(TI_REAL const *options) {
    return (int)options[0];
}


int ti_mfi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    const TI_REAL *volume = inputs[3];
    const int period = (int)options[0];

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_mfi_start(options)) return TI_OKAY;

    TI_REAL *output = outputs[0];

    TI_REAL ytyp = TYPPRICE(0);

    int i;

    ti_buffer *up = ti_buffer_new(period);
    ti_buffer *down = ti_buffer_new(period);

    for (i = 1; i < size; ++i) {
        const TI_REAL typ = TYPPRICE(i);
        const TI_REAL bar = typ * volume[i];

        if (typ > ytyp) {
            ti_buffer_push(up, bar);
            ti_buffer_push(down, 0.0);
        } else if (typ < ytyp) {
            ti_buffer_push(down, bar);
            ti_buffer_push(up, 0.0);
        } else {
            ti_buffer_push(up, 0.0);
            ti_buffer_push(down, 0.0);
        }

        ytyp = typ;

        if (i >= period) {
            *output++ = up->sum / (up->sum + down->sum) * 100.0;
        }
    }

    ti_buffer_free(up);
    ti_buffer_free(down);

    assert(output - outputs[0] == size - ti_mfi_start(options));
    return TI_OKAY;
}
