/*
 * Tulip Indicators
 * https://tulipindicators.org/
 * Copyright (c) 2010-2019 Tulip Charts LLC
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

#define MAX4(var, a, b, c, d) { \
    var = a; \
    if (var < b) { var = b; } \
    if (var < c) { var = c; } \
    if (var < d) { var = d; } \
}

/* Name: Know Sure Thing
 * Source: Robert W. Colby. The Encyclopedia Of Technical Market Indicators, 2003 (2nd ed)
 * ISBN: 978-0-07-171162-3
 */

int ti_kst_start(TI_REAL const *options) {
    const int roc4 = (int)options[3];
    return roc4;
}

int ti_kst(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    TI_REAL const *real = inputs[0];
    const int roc1 = (int)options[0];
    const int roc2 = (int)options[1];
    const int roc3 = (int)options[2];
    const int roc4 = (int)options[3];
    const int ma1 = (int)options[4];
    const int ma2 = (int)options[5];
    const int ma3 = (int)options[6];
    const int ma4 = (int)options[7];
    TI_REAL *kst = outputs[0];
    TI_REAL *kst_signal = outputs[1];

    if(!(roc1 < roc2 && roc2 < roc3 && roc3 < roc4)) {
        return TI_INVALID_OPTION;
    }
    for (int i = 0; i < 8; ++i) {
        if (options[i] < 1) {
            return TI_INVALID_OPTION;
        }
    }

    TI_REAL max_ma;
    MAX4(max_ma, ma1, ma2, ma3, ma4);

    TI_REAL per1 = 2. / (ma1 + 1);
    TI_REAL per2 = 2. / (ma2 + 1);
    TI_REAL per3 = 2. / (ma3 + 1);
    TI_REAL per4 = 2. / (ma4 + 1);
    TI_REAL per_signal = 2. / (9. + 1.);

    #define ROC(idx, period) ((real[idx] - real[idx-period]) / real[idx-period])

    TI_REAL _1 = ROC(roc1, roc1);
    TI_REAL _2 = ROC(roc2, roc2);
    TI_REAL _3 = ROC(roc3, roc3);
    TI_REAL _4 = ROC(roc4, roc4);

    for (int i = roc1 + 1; i < roc4 + 1 && i < size; ++i) {
        _1 = (ROC(i, roc1) - _1) * per1 + _1;
    }
    for (int i = roc2 + 1; i < roc4 + 1 && i < size; ++i) {
        _2 = (ROC(i, roc2) - _2) * per2 + _2;
    }
    for (int i = roc3 + 1; i < roc4 + 1 && i < size; ++i) {
        _3 = (ROC(i, roc3) - _3) * per3 + _3;
    }
    for (int i = roc4 + 1; i < roc4 + 1 && i < size; ++i) {
        _4 = (ROC(i, roc4) - _4) * per4 + _4;
    }

    TI_REAL val = (_1 * 1. + _2 * 2. + _3 * 3. + _4 * 4.) / 10.;
    *kst_signal++ = val;

    TI_REAL _signal = val;
    *kst++ = _signal;

    for (int i = roc4 + 1; i < size; ++i) {
        _1 = (ROC(i, roc1) - _1) * per1 + _1;
        _2 = (ROC(i, roc2) - _2) * per2 + _2;
        _3 = (ROC(i, roc3) - _3) * per3 + _3;
        _4 = (ROC(i, roc4) - _4) * per4 + _4;
        val = (_1 * 1. + _2 * 2. + _3 * 3. + _4 * 4.) / 10.;
        *kst++ = val;
        _signal = (val - _signal) * per_signal + _signal;
        *kst_signal++ = _signal;
    }

    assert(kst - outputs[0] == size - ti_kst_start(options));
    return TI_OKAY;
}

int ti_kst_ref(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    TI_REAL const *real = inputs[0];
    const TI_REAL *roc = options;
    const TI_REAL *ma = options + 4;
    TI_REAL *kst = outputs[0];
    TI_REAL *kst_signal = outputs[1];


    if(!(roc[0] < roc[1] && roc[1] < roc[2] && roc[2] < roc[3])) {
        return TI_INVALID_OPTION;
    }
    for (int i = 0; i < 8; ++i) {
        if (options[i] < 1) {
            return TI_INVALID_OPTION;
        }
    }

    int size_roc[4];
    int size_ema[4];
    TI_REAL *roc_mem[4];
    TI_REAL *ema_mem[4];

    for (int i = 0; i < 4; ++i) {
        size_roc[i] = size - ti_roc_start(roc + i);
        size_ema[i] = size_roc[i] - ti_ema_start(ma + i);

        roc_mem[i] = malloc(sizeof(TI_REAL[size_roc[i]]));
        ti_roc(size, &real, roc + i, roc_mem + i);

        ema_mem[i] = malloc(sizeof(TI_REAL[size_ema[i]]));

        const TI_REAL *ti_ema_inputs[] = {roc_mem[i]};
        ti_ema(size_roc[i], ti_ema_inputs, ma + i, ema_mem + i);
    }

    int min_len = size_ema[0];
    for (int i = 1; i < 4; ++i) { if (min_len > size_ema[i]) { min_len = size_ema[i]; } }

    int adj[4];
    for (int i = 0; i < 4; ++i) {
        adj[i] = size_ema[i] - min_len;
    }

    for (int i = 0; i < min_len; ++i) {
        *kst++ = (ema_mem[0][i+adj[0]] * 1. + ema_mem[1][i+adj[1]] * 2. + ema_mem[2][i+adj[2]] * 3. + ema_mem[3][i+adj[3]] * 4.) / 10.;
    }

    TI_REAL signal_period = 9.;
    const TI_REAL *ti_ema_inputs[] = {outputs[0]};
    ti_ema(min_len, ti_ema_inputs, &signal_period, &kst_signal);

    for (int i = 0; i < 4; ++i) {
        free(roc_mem[i]);
        free(ema_mem[i]);
    }

    assert(kst - outputs[0] == size - ti_kst_start(options));
    return TI_OKAY;
}
