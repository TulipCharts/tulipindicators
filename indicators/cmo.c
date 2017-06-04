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

#define UPWARD(I) (input[(I)] > input[(I)-1] ? input[(I)] - input[(I)-1] : 0)
#define DOWNWARD(I) (input[(I)] < input[(I)-1] ? input[(I)-1] - input[(I)] : 0)


int ti_cmo_start(TI_REAL const *options) {
    return (int)options[0];
}


int ti_cmo(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    TI_REAL *output = outputs[0];

    const int period = (int)options[0];

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_cmo_start(options)) return TI_OKAY;

    TI_REAL up_sum = 0, down_sum = 0;

    int i;
    for (i = 1; i <= period; ++i) {
        up_sum += UPWARD(i);
        down_sum += DOWNWARD(i);
    }

    *output++ = 100 * (up_sum - down_sum) / (up_sum + down_sum);

    for (i = period+1; i < size; ++i) {
        up_sum -= UPWARD(i-period);
        down_sum -= DOWNWARD(i-period);

        up_sum += UPWARD(i);
        down_sum += DOWNWARD(i);

        *output++ = 100 * (up_sum - down_sum) / (up_sum + down_sum);
    }

    assert(output - outputs[0] == size - ti_cmo_start(options));
    return TI_OKAY;

}
