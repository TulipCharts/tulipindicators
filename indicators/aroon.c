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



int ti_aroon_start(TI_REAL const *options) {
    return (int)options[0];
}


int ti_aroon(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    TI_REAL *adown = outputs[0];
    TI_REAL *aup = outputs[1];
    const int period = (int)options[0];

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_aroon_start(options)) return TI_OKAY;

    const TI_REAL scale = 100.0 / period;

    int trail = 0, maxi = -1, mini = -1;
    TI_REAL max = high[0];
    TI_REAL min = low[0];
    TI_REAL bar;

    int i, j;
    for (i = period; i < size; ++i, ++trail) {
        /* Maintain highest. */
        bar = high[i];
        if (maxi < trail) {
            maxi = trail;
            max = high[maxi];
            j = trail;
            while(++j <= i) {
                bar = high[j];
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
        bar = low[i];
        if (mini < trail) {
            mini = trail;
            min = low[mini];
            j = trail;
            while(++j <= i) {
                bar = low[j];
                if (bar <= min) {
                    min = bar;
                    mini = j;
                }
            }
        } else if (bar <= min) {
            mini = i;
            min = bar;
        }


        /* Calculate the indicator. */
        *adown++ = ((TI_REAL)period - (i-mini)) * scale;
        *aup++ = ((TI_REAL)period - (i-maxi)) * scale;
    }


    assert(adown - outputs[0] == size - ti_aroon_start(options));
    assert(aup - outputs[1] == size - ti_aroon_start(options));
    return TI_OKAY;
}
