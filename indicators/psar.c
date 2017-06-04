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


int ti_psar_start(TI_REAL const *options) {
    (void)options;
    return 1;
}


int ti_psar(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];

    const TI_REAL accel_step = options[0];
    const TI_REAL accel_max = options[1];

    TI_REAL *output = outputs[0];

    if (accel_step <= 0) return TI_INVALID_OPTION;
    if (accel_max <= accel_step) return TI_INVALID_OPTION;
    if (size < 2) return TI_OKAY;


    /* Try to choose if we start as short or long.
     * There is really no right answer here. */
    int lng;
    if (high[0] + low[0] <= high[1] + low[1])
        lng = 1;
    else
        lng = 0;

    TI_REAL sar, extreme;

    if (lng) {
        extreme = high[0];
        sar = low[0];
    } else {
        extreme = low[0];
        sar = high[0];
    }

    TI_REAL accel = accel_step;

    int i;
    for (i = 1; i < size; ++i) {

        sar = (extreme - sar) * accel + sar;

        if (lng) {

            if (i >= 2 && (sar > low[i-2])) sar = low[i-2];

            if ((sar > low[i-1])) sar = low[i-1];

            if (accel < accel_max && high[i] > extreme) {
                accel += accel_step;
                if (accel > accel_max) accel = accel_max;
            }

            if (high[i] > extreme) extreme = high[i];

        } else {

            if (i >= 2 && (sar < high[i-2])) sar = high[i-2];

            if ((sar < high[i-1])) sar = high[i-1];

            if (accel < accel_max && low[i] < extreme) {
                accel += accel_step;
                if (accel > accel_max) accel = accel_max;
            }

            if (low[i] < extreme) extreme = low[i];
        }



        if ((lng && low[i] < sar) || (!lng && high[i] > sar)) {
            accel = accel_step;
            sar = extreme;

            lng = !lng;

            if (!lng) extreme = low[i];
            else extreme = high[i];

        }


        *output++ = sar;

        /*
        printf("%s%2d %.4f %.4f %.4f %.4f %.4f %s\n", i == 1 ? "\n" : "", i, high[i], low[i], accel, extreme, sar, reverse ? (!lng ? "short" : "long") : "");
        */

    }


    assert(output - outputs[0] == size - ti_psar_start(options));
    return TI_OKAY;
}
