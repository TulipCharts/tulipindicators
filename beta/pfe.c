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
#include "../utils/buffer.h"

int ti_pfe_start(TI_REAL const *options) {
    const int period = (int)options[0];
    return period;
}

/* Name: Polarized Fractal Effeciency
 * Source: Robert W. Colby. The Encyclopedia of Technical Market Indicators, 2002, p. 520
 */

int ti_pfe(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *real = inputs[0];
    const int period = (int)options[0];
    const int ema_period = (int)options[1];
    TI_REAL *pfe = outputs[0];

    if (period < 1) { return TI_INVALID_OPTION; }
    if (size <= ti_pfe_start(options)) { return TI_OKAY; }

    #define SIGN(expr) ((expr) > 0 ? 1. : -1.)
    #define EMA_NEXT(val) (((val) - ema) * per + ema);
    const TI_REAL per = 2. / (ema_period + 1.);

    ti_buffer *denom = ti_buffer_new(period);

    int i;
    for (i = 1; i < period; ++i) {
        ti_buffer_push(denom, sqrt(pow(real[i] - real[i-1], 2) + 1.));
    }
    ti_buffer_push(denom, sqrt(pow(real[i] - real[i-1], 2) + 1.));
    TI_REAL numer = SIGN(real[i] - real[i-(int)period]) * 100. * sqrt(pow(real[i] - real[i-(int)period], 2) + 100.);
    TI_REAL ema = numer / denom->sum;
    *pfe++ = ema;

    for (i = period+1; i < size; ++i) {
        ti_buffer_push(denom, sqrt(pow(real[i] - real[i-1], 2) + 1.));
        TI_REAL numer2 = SIGN(real[i] - real[i-(int)period]) * 100. * sqrt(pow(real[i] - real[i-(int)period], 2) + 100.);

        ema = EMA_NEXT(numer2 / denom->sum);
        *pfe++ = ema;
    }

    free(denom);
    assert(pfe - outputs[0] == size - ti_pfe_start(options));
    return TI_OKAY;
}

/* Essentially, EMA 5 of
 * sign(c[i] - c[i-period]) * 100 *
 *  sqrt((c[i] - c[i-period])^2 + 100) /
 *  sum(sqrt((c[j] - c[j-1])^2 + 1) for j in range(i-period+1, i+1))
 */

int ti_pfe_ref(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *real = inputs[0];
    const int period = (int)options[0];
    const int ema_period = (int)options[1];
    TI_REAL *pfe = outputs[0];

    if (period < 1) { return TI_INVALID_OPTION; }
    if (size <= ti_pfe_start(options)) { return TI_OKAY; }

    for (int i = period; i < size; ++i) {
        TI_REAL div = 0.;
        for (int j = i - period + 1; j <= i; ++j) {
            div += sqrt(pow(real[j] - real[j-1], 2) + 1);
        }
        *pfe++ = SIGN(real[i] - real[i-(int)period]) * 100. * sqrt(pow(real[i] - real[i-(int)period], 2) + 100.) / div;
    }

    const TI_REAL *ti_ema_inputs[] = {outputs[0]};
    const TI_REAL ti_ema_options[] = {ema_period};
    ti_ema(size - period, ti_ema_inputs, ti_ema_options, outputs);

    assert(pfe - outputs[0] == size - ti_pfe_start(options));
    return TI_OKAY;
}

/* Streaming */

typedef struct ti_stream_pfe {
    /* required */
    int index;
    int progress;

    /* indicator specific */
    int period;
    int ema_period;

    TI_REAL ema;
    TI_REAL numer;
    TI_REAL last_removed;

    int buffers_idx;
    TI_REAL buffer2_sum;
    TI_REAL buffer[][2];
} ti_stream_pfe;

int ti_pfe_stream_new(TI_REAL const *options, ti_stream **stream_in) {
    ti_stream_pfe **stream = (ti_stream_pfe**)stream_in;

    const int period = (int)options[0];
    const int ema_period = (int)options[1];

    if (period < 1) { return TI_INVALID_OPTION; }

    *stream = malloc(sizeof(ti_stream_pfe) + sizeof(TI_REAL[(int)period][2]));

    if (!stream) { return TI_OUT_OF_MEMORY; }

    (*stream)->index = TI_INDICATOR_PFE_INDEX;
    (*stream)->progress = -ti_pfe_start(options);
    (*stream)->period = period;
    (*stream)->ema_period = ema_period;
    (*stream)->buffers_idx = -1;
    (*stream)->buffer2_sum = 0.;

    return TI_OKAY;
}

#define PUSH_NONFULL(buffer, value1, value2) do { \
    ++buffers_idx; \
    (*buffer)[buffers_idx][0] = value1; \
    (*buffer)[buffers_idx][1] = value2; \
    buffer2_sum += value2; \
} while (0)

#define PUSH_FULL(buffer, value1, value2) do { \
    if (++buffers_idx == period) { buffers_idx = 0; } \
    last_removed = (*buffer)[buffers_idx][0]; \
    (*buffer)[buffers_idx][0] = value1; \
    buffer2_sum -= (*buffer)[buffers_idx][1]; \
    buffer2_sum += (*buffer)[buffers_idx][1] = value2; \
} while (0)

#define LOOKBACK(var, buffer, amount, which) do { \
    int idx = buffers_idx - (amount); \
    if (idx < 0) { idx += period; } \
    else if (idx >= period) { idx -= period; } \
    var = (*buffer)[idx][which]; \
} while (0)

void ti_pfe_stream_free(ti_stream *stream) {
    free(stream);
}

int ti_pfe_stream_run(ti_stream *stream_in, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    ti_stream_pfe *stream = (ti_stream_pfe*)stream_in;

    const TI_REAL *real = inputs[0];
    const int period = stream->period;
    const TI_REAL ema_period = stream->ema_period;
    TI_REAL *pfe = outputs[0];

    TI_REAL ema = stream->ema;
    TI_REAL numer = stream->numer;
    TI_REAL last_removed = stream->last_removed;

    int buffers_idx = stream->buffers_idx;
    TI_REAL buffer2_sum = stream->buffer2_sum;
    TI_REAL (*buffer)[][2] = &stream->buffer;

    #define SIGN(expr) ((expr) > 0 ? 1. : -1.)
    #define EMA_NEXT(val) (((val) - ema) * per + ema);
    const TI_REAL per = 2. / (ema_period + 1.);

    int progress = stream->progress;

    int i = 0;
    if (progress == -period) {
        PUSH_NONFULL(buffer, real[i], 0);
        ++i, ++progress;
    } else {}
    for (; progress < 0 && i < size; ++i, ++progress) { // warm up
        TI_REAL prev; LOOKBACK(prev, buffer, 0, 0);
        PUSH_NONFULL(buffer, real[i], sqrt(pow(real[i] - prev, 2) + 1.));
    }
    if (progress == 0 && i < size) { // last step of warming
        TI_REAL prev; LOOKBACK(prev, buffer, 0, 0);
        PUSH_FULL(buffer, real[i], sqrt(pow(real[i] - prev, 2) + 1.));
        numer = SIGN(real[i] - last_removed) * 100. * sqrt(pow(real[i] - last_removed, 2) + 100.);
        ema = numer / buffer2_sum;
        *pfe++ = ema;

        ++i, ++progress;
    }
    for (; i < size; ++i, ++progress) { // continue in normal mode
        TI_REAL prev; LOOKBACK(prev, buffer, 0, 0);
        PUSH_FULL(buffer, real[i], sqrt(pow(real[i] - prev, 2) + 1.));
        numer = SIGN(real[i] - last_removed) * 100. * sqrt(pow(real[i] - last_removed, 2) + 100.);
        ema = EMA_NEXT(numer / buffer2_sum);
        *pfe++ = ema;
    }

    stream->progress = progress;
    stream->ema = ema;
    stream->numer = numer;
    stream->last_removed = last_removed;
    stream->buffers_idx = buffers_idx;
    stream->buffer2_sum = buffer2_sum;

    return TI_OKAY;
}
