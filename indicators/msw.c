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



int ti_msw_start(TI_REAL const *options) {
    return (int)options[0];
}


int ti_msw(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    TI_REAL *sine = outputs[0];
    TI_REAL *lead = outputs[1];
    const int period = (int)options[0];

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_msw_start(options)) return TI_OKAY;

    const TI_REAL pi = 3.1415926;
    const TI_REAL tpi = 2 * pi;

    TI_REAL weight = 0, phase;
    TI_REAL rp, ip;

    int i, j;
    for (i = period; i < size; ++i) {
        rp = 0;
        ip = 0;
        for (j = 0; j < period; ++j) {
            weight = input[i-j];
            rp = rp + cos(tpi * j / period) * weight;
            ip = ip + sin(tpi * j / period) * weight;
        }
        if (fabs(rp) > .001) {
            phase = atan(ip/rp);
        } else {
            phase = tpi / 2.0 * (ip < 0 ? -1.0 : 1.0);
        }
        if (rp < 0.0) phase += pi;
        phase += pi/2.0;
        if (phase < 0.0) phase += tpi;
        if (phase > tpi) phase -= tpi;
        //phase = 180 * phase / pi;

        *sine++ = sin(phase);
        *lead++ = sin(phase + pi/4.0);
    }


    assert(sine - outputs[0] == size - ti_msw_start(options));
    assert(lead - outputs[1] == size - ti_msw_start(options));
    return TI_OKAY;
}
