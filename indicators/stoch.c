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


int ti_stoch_start(TI_REAL const *options) {
    const int kperiod = (int)options[0];
    const int kslow = (int)options[1];
    const int dperiod = (int)options[2];
    return kperiod + kslow + dperiod - 3;
}


int ti_stoch(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    const int kperiod = (int)options[0];
    const int kslow = (int)options[1];
    const int dperiod = (int)options[2];

    const TI_REAL kper = 1.0 / kslow;
    const TI_REAL dper = 1.0 / dperiod;

    TI_REAL *stoch = outputs[0];
    TI_REAL *stoch_ma = outputs[1];

    if (kperiod < 1) return TI_INVALID_OPTION;
    if (kslow < 1) return TI_INVALID_OPTION;
    if (dperiod < 1) return TI_INVALID_OPTION;
    if (size <= ti_stoch_start(options)) return TI_OKAY;

    int trail = 0, maxi = -1, mini = -1;
    TI_REAL max = high[0];
    TI_REAL min = low[0];
    TI_REAL bar;

    ti_buffer *k_sum = ti_buffer_new(kslow);
    ti_buffer *d_sum = ti_buffer_new(dperiod);

    int i, j;
    for (i = 0; i < size; ++i) {
        if (i >= kperiod) ++trail;

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


        /* Calculate it. */
        const TI_REAL kdiff = (max - min);
        const TI_REAL kfast = kdiff == 0.0 ? 0.0 : 100 * ((close[i] - min) / kdiff);
        ti_buffer_push(k_sum, kfast);


        if (i >= kperiod-1 + kslow-1) {
            const TI_REAL k = k_sum->sum * kper;
            ti_buffer_push(d_sum, k);

            if (i >= kperiod-1 + kslow-1 + dperiod-1) {
                *stoch++ = k;
                *stoch_ma++ = d_sum->sum * dper;
            }
        }
    }

    ti_buffer_free(k_sum);
    ti_buffer_free(d_sum);

    assert(stoch - outputs[0] == size - ti_stoch_start(options));
    assert(stoch_ma - outputs[1] == size - ti_stoch_start(options));
    return TI_OKAY;

}
