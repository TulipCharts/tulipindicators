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
#include "../utils/localbuffer.h"

int ti_smi_start(TI_REAL const *options) {
    const int q_period = (int)options[0];
    return q_period-1;
}

int ti_smi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    TI_REAL const *high = inputs[0];
    TI_REAL const *low = inputs[1];
    TI_REAL const *close = inputs[2];
    const int q_period = (int)options[0];
    const int r_period = (int)options[1];
    const int s_period = (int)options[2];
    TI_REAL *smi = outputs[0];

    for (int i = 0; i < 3; ++i) { if (options[i] < 1) { return TI_INVALID_OPTION; } }

    int progress = -q_period + 1;

    TI_REAL ema_r_num = NAN;
    TI_REAL ema_s_num = NAN;
    TI_REAL ema_r_den = NAN;
    TI_REAL ema_s_den = NAN;
    TI_REAL ll;
    TI_REAL hh;
    int hh_idx;
    int ll_idx;

    TI_REAL var1;

    int i = 0;

    for (; i < size && progress == -q_period + 1; ++i, ++progress) {
        hh = high[i];
        hh_idx = progress;
        ll = low[i];
        ll_idx = progress;
    }
    for (; i < size && progress < 0; ++i, ++progress) {
        if (hh <= high[i]) {
            hh = high[i];
            hh_idx = progress;
        }
        if (ll >= low[i]) {
            ll = low[i];
            ll_idx = progress;
        }
    }
    for (; i < size && progress == 0; ++i, ++progress) {
        if (hh <= high[i]) {
            hh = high[i];
            hh_idx = progress;
        }
        if (ll >= low[i]) {
            ll = low[i];
            ll_idx = progress;
        }

        ema_r_num = ema_s_num = close[i] - 0.5 * (hh + ll);
        ema_r_den = ema_s_den = hh - ll;

        *smi++ = 100 * ema_s_num / (0.5 * ema_s_den);
    }
    for (; i < size; ++i, ++progress) {
        if (hh_idx == progress - q_period) {
            hh = high[i];
            hh_idx = progress;
            for (int j = 1; j < q_period; ++j) {
                var1 = high[i-j];
                if (var1 > hh) {
                    hh = var1;
                    hh_idx = progress-j;
                }
            }
        } else if (hh <= high[i]) {
            hh = high[i];
            hh_idx = progress;
        }
        if (ll_idx == progress - q_period) {
            ll = low[i];
            ll_idx = progress;
            for (int j = 1; j < q_period; ++j) {
                var1 = low[i-j];
                if (var1 < ll) {
                    ll = var1;
                    ll_idx = progress-j;
                }
            }
        } else if (ll >= low[i]) {
            ll = low[i];
            ll_idx = progress;
        }

        ema_r_num = ((close[i] - 0.5 * (hh + ll)) - ema_r_num) * (2. / (1. + r_period)) + ema_r_num;
        ema_s_num = (ema_r_num - ema_s_num) * (2. / (1. + s_period)) + ema_s_num;

        ema_r_den = ((hh - ll) - ema_r_den) * (2. / (1. + r_period)) + ema_r_den;
        ema_s_den = (ema_r_den - ema_s_den) * (2. / (1. + s_period)) + ema_s_den;

        *smi++ = 100. * ema_s_num / (0.5 * ema_s_den);
    }

    return TI_OKAY;
}

int ti_smi_ref(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    TI_REAL const *high = inputs[0];
    TI_REAL const *low = inputs[1];
    TI_REAL const *close = inputs[2];
    TI_REAL q_period = options[0];
    TI_REAL r_period = options[1];
    TI_REAL s_period = options[2];
    TI_REAL *smi = outputs[0];

    for (int i = 0; i < 3; ++i) { if (options[i] < 1) { return TI_INVALID_OPTION; } }

    const int outsize = size - ti_max_start(options);

    TI_REAL *max = malloc(sizeof(TI_REAL) * (unsigned int)outsize);
    TI_REAL *min = malloc(sizeof(TI_REAL) * (unsigned int)outsize);

    const TI_REAL *ti_max_inputs[] = {high};
    ti_max(size, ti_max_inputs, &q_period, &max);

    const TI_REAL *ti_min_inputs[] = {low};
    ti_min(size, ti_min_inputs, &q_period, &min);

    TI_REAL *num = malloc(sizeof(TI_REAL) * (unsigned int)outsize);
    TI_REAL *den = malloc(sizeof(TI_REAL) * (unsigned int)outsize);

    int i;
    for (i = 0; i < outsize; ++i) {
        num[i] = close[size-outsize + i] - 0.5 * (max[i] + min[i]);
        den[i] = max[i] - min[i];
    }

    const TI_REAL *num_input[] = {num};
    ti_ema(outsize, num_input, &r_period, &num);
    ti_ema(outsize, num_input, &s_period, &num);

    const TI_REAL *den_input[] = {den};
    ti_ema(outsize, den_input, &r_period, &den);
    ti_ema(outsize, den_input, &s_period, &den);

    for (i = 0; i < outsize; ++i) {
        smi[i] = 100. * num[i] / (0.5 * den[i]);
    }

    free(max);
    free(min);
    free(num);
    free(den);

    return TI_OKAY;
}

typedef struct ti_stream_smi {
    int index;
    int progress;

    struct {
        TI_REAL q_period;
        TI_REAL r_period;
        TI_REAL s_period;
    } options;

    struct {
        TI_REAL ema_r_num;
        TI_REAL ema_s_num;
        TI_REAL ema_r_den;
        TI_REAL ema_s_den;
        TI_REAL ll;
        TI_REAL hh;
        int ll_idx;
        int hh_idx;
    } state;

    BUFFERS(
        BUFFER(low)
        BUFFER(high)
    )
} ti_stream_smi;

int ti_smi_stream_new(TI_REAL const *options, ti_stream **stream_in) {
    ti_stream_smi **stream = (ti_stream_smi**)stream_in;

    const int q_period = (int)options[0];
    const int r_period = (int)options[1];
    const int s_period = (int)options[2];

    for (int i = 0; i < 3; ++i) { if (options[i] < 1) { return TI_INVALID_OPTION; } }

    *stream = calloc(1, sizeof(**stream));
    if (!*stream) { return TI_OUT_OF_MEMORY; }

    (*stream)->index = TI_INDICATOR_SMI_INDEX;
    (*stream)->progress = -ti_smi_start(options);

    (*stream)->options.q_period = q_period;
    (*stream)->options.r_period = r_period;
    (*stream)->options.s_period = s_period;

    BUFFER_INIT(*stream, low, q_period);
    BUFFER_INIT(*stream, high, q_period);

    *stream = realloc(*stream, sizeof(**stream) + sizeof(TI_REAL) * (unsigned int)BUFFERS_SIZE(*stream));
    if (!*stream) { return TI_OUT_OF_MEMORY; }

    return TI_OKAY;
}


void ti_smi_stream_free(ti_stream *stream) {
    free(stream);
}

int ti_smi_stream_run(ti_stream *stream_in, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    ti_stream_smi *stream = (ti_stream_smi*)stream_in;

    TI_REAL const *high = inputs[0];
    TI_REAL const *low = inputs[1];
    TI_REAL const *close = inputs[2];
    TI_REAL *smi = outputs[0];

    const TI_REAL q_period = stream->options.q_period;
    const TI_REAL r_period = stream->options.r_period;
    const TI_REAL s_period = stream->options.s_period;

    int progress = stream->progress;

    TI_REAL ema_r_num = stream->state.ema_r_num;
    TI_REAL ema_s_num = stream->state.ema_s_num;
    TI_REAL ema_r_den = stream->state.ema_r_den;
    TI_REAL ema_s_den = stream->state.ema_s_den;
    TI_REAL ll = stream->state.ll;
    TI_REAL hh = stream->state.hh;
    int hh_idx = stream->state.hh_idx;
    int ll_idx = stream->state.ll_idx;

    TI_REAL var1;

    int i = 0;
    for (; i < size && progress == -q_period + 1; ++i, ++progress) {
        BUFFER_PUSH(stream, low, low[i]);
        BUFFER_PUSH(stream, high, high[i]);

        hh = high[i];
        hh_idx = progress;
        ll = low[i];
        ll_idx = progress;
    }
    for (; i < size && progress < 0; ++i, ++progress) {
        BUFFER_PUSH(stream, low, low[i]);
        BUFFER_PUSH(stream, high, high[i]);

        if (hh <= high[i]) {
            hh = high[i];
            hh_idx = progress;
        }
        if (ll >= low[i]) {
            ll = low[i];
            ll_idx = progress;
        }
    }
    for (; i < size && progress == 0; ++i, ++progress) {
        BUFFER_PUSH(stream, low, low[i]);
        BUFFER_PUSH(stream, high, high[i]);

        if (hh <= high[i]) {
            hh = high[i];
            hh_idx = progress;
        }
        if (ll >= low[i]) {
            ll = low[i];
            ll_idx = progress;
        }

        ema_r_num = ema_s_num = close[i] - 0.5 * (hh + ll);
        ema_r_den = ema_s_den = hh - ll;

        *smi++ = 100 * ema_s_num / (0.5 * ema_s_den);
    }
    for (; i < size; ++i, ++progress) {
        BUFFER_PUSH(stream, low, low[i]);
        BUFFER_PUSH(stream, high, high[i]);

        if (hh_idx == progress - q_period) {
            hh = high[i];
            hh_idx = progress;
            for (int j = 1; j < q_period; ++j) {
                BUFFER_AT(var1, stream, high, -j);
                if (var1 > hh) {
                    hh = var1;
                    hh_idx = progress-j;
                }
            }
        } else if (hh <= high[i]) {
            hh = high[i];
            hh_idx = progress;
        }
        if (ll_idx == progress - q_period) {
            ll = low[i];
            ll_idx = progress;
            for (int j = 1; j < q_period; ++j) {
                BUFFER_AT(var1, stream, low, -j);
                if (var1 < ll) {
                    ll = var1;
                    ll_idx = progress-j;
                }
            }
        } else if (ll >= low[i]) {
            ll = low[i];
            ll_idx = progress;
        }

        ema_r_num = ((close[i] - 0.5 * (hh + ll)) - ema_r_num) * (2. / (1. + r_period)) + ema_r_num;
        ema_s_num = (ema_r_num - ema_s_num) * (2. / (1. + s_period)) + ema_s_num;

        ema_r_den = ((hh - ll) - ema_r_den) * (2. / (1. + r_period)) + ema_r_den;
        ema_s_den = (ema_r_den - ema_s_den) * (2. / (1. + s_period)) + ema_s_den;

        *smi++ = 100. * ema_s_num / (0.5 * ema_s_den);
    }


    stream->progress = progress;

    stream->state.ema_r_num = ema_r_num;
    stream->state.ema_s_num = ema_s_num;
    stream->state.ema_r_den = ema_r_den;
    stream->state.ema_s_den = ema_s_den;

    stream->state.ll = ll;
    stream->state.hh = hh;
    stream->state.hh_idx = hh_idx;
    stream->state.ll_idx = ll_idx;

    return TI_OKAY;
}
