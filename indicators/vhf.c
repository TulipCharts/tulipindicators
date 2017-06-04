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


int ti_vhf_start(TI_REAL const *options) {
    return (int)options[0];
}


int ti_vhf(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *in = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_vhf_start(options)) return TI_OKAY;

    int trail = 1, maxi = -1, mini = -1;
    TI_REAL max = in[0], min = in[0];
    TI_REAL bar;

    TI_REAL sum = 0;

    int i, j;

    TI_REAL yc = in[0];
    TI_REAL c;

    for (i = 1; i < period; ++i) {
        c = in[i];
        sum += fabs(c - yc);
        yc = c;
    }

    for (i = period; i < size; ++i, ++trail) {
        c = in[i];
        sum += fabs(c - yc);
        yc = c;

        if (i > period) {
            sum -= fabs(in[i-period] - in[i-period-1]);
        }

        /* Maintain highest. */
        bar = c;
        if (maxi < trail) {
            maxi = trail;
            max = in[maxi];
            j = trail;
            while(++j <= i) {
                bar = in[j];
                if (bar >= max) {
                    max = bar;
                    maxi = j;
                }
            }
        } else if (bar >= max) {
            maxi = i;
            max = bar;
        }

        /* Maintain lowest. */
        bar = c;
        if (mini < trail) {
            mini = trail;
            min = in[mini];
            j = trail;
            while(++j <= i) {
                bar = in[j];
                if (bar <= min) {
                    min = bar;
                    mini = j;
                }
            }
        } else if (bar <= min) {
            mini = i;
            min = bar;
        }

        /* Calculate it. */
        *output++ = fabs(max - min) / sum;
    }

    assert(output - outputs[0] == size - ti_vhf_start(options));
    return TI_OKAY;
}
