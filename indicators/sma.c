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



int ti_sma_start(TI_REAL const *options) {
    return (int)options[0]-1;
}


int ti_sma(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    const TI_REAL scale = 1.0 / period;

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_sma_start(options)) return TI_OKAY;

    TI_REAL sum = 0;

    int i;
    for (i = 0; i < period; ++i) {
        sum += input[i];
    }

    *output++ = sum * scale;

    for (i = period; i < size; ++i) {
        sum += input[i];
        sum -= input[i-period];
        *output++ = sum * scale;
    }

    assert(output - outputs[0] == size - ti_sma_start(options));
    return TI_OKAY;
}


typedef struct ti_stream_sma {
    int index;
    int progress;

    int period;

    TI_REAL per;

    TI_REAL sum;
    int buffer_idx;
    TI_REAL buffer[];
} ti_stream_sma;


int ti_sma_stream_new(TI_REAL const *options, ti_stream **stream_in) {
    ti_stream_sma **stream = (ti_stream_sma**)stream_in;

    int period = (int)options[0];
    if (period < 1) return TI_INVALID_OPTION;

    *stream = malloc(sizeof(ti_stream_sma) + sizeof(TI_REAL[period]));
    if (!stream) {
        return TI_OUT_OF_MEMORY;
    }

    (*stream)->index = TI_INDICATOR_SMA_INDEX;
    (*stream)->progress = -ti_sma_start(options);
    (*stream)->period = period;

    (*stream)->per = 1. / period;

    (*stream)->sum = 0.;
    (*stream)->buffer_idx = -1;

    return TI_OKAY;
}

// ~10mfps gain
#define BUFFER_PUSH_NONFULL(val) { \
    ++buffer_idx; \
    sum += buffer[buffer_idx] = val; \
}

#define BUFFER_PUSH_FULL(val) { \
    ++buffer_idx; \
    if (buffer_idx == period) { buffer_idx = 0; } \
    sum -= buffer[buffer_idx]; \
    sum += buffer[buffer_idx] = val; \
}

int ti_sma_stream_run(ti_stream *stream_in, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    ti_stream_sma *stream = (ti_stream_sma*)stream_in;

    int progress = stream->progress;

    const TI_REAL *real = inputs[0];
    int period = stream->period;
    TI_REAL *sma = outputs[0];

    TI_REAL per = stream->per;

    TI_REAL sum = stream->sum;
    int buffer_idx = stream->buffer_idx;
    TI_REAL *buffer = stream->buffer;

    if (progress == -period + 1) {} else {} // initialize
    int i;
    for (i = 0; progress < 1 && i < size; ++i, ++progress) { // warm up
        BUFFER_PUSH_NONFULL(real[i] * per);
    }
    if (i > 0 && progress == 1) { // just warmed
        *sma++ = sum;
    }
    for (; i < size; ++i, ++progress) { // continue in normal mode
        BUFFER_PUSH_FULL(real[i] * per);
        *sma++ = sum;
    }

    stream->progress = progress;

    stream->sum = sum;
    stream->buffer_idx = buffer_idx;

    return TI_OKAY;
}

void ti_sma_stream_free(ti_stream *stream) {
    free(stream);
}
