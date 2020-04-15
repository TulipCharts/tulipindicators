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


int ti_pc_start(TI_REAL const *options) {
    const int period = (int)options[0];
    return period-1;
}


int ti_pc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    TI_REAL const *high = inputs[0];
    TI_REAL const *low = inputs[1];
    const TI_REAL period = options[0];
    TI_REAL *pc_low = outputs[0];
    TI_REAL *pc_high = outputs[1];

    if (period < 1) { return TI_INVALID_OPTION; }
    if (size <= ti_pc_start(options)) { return TI_OKAY; }

    ti_min(size, &low, &period, &pc_low);
    ti_max(size, &high, &period, &pc_high);

    return TI_OKAY;
}

typedef struct ti_stream_pc {
    int index;
    int progress;

    struct {
        TI_REAL period;
    } options;

    struct {
        TI_REAL max;
        int max_idx;
        TI_REAL min;
        int min_idx;
    } state;

    BUFFERS(
        BUFFER(high)
        BUFFER(low)
    )
} ti_stream_pc;

int ti_pc_stream_new(TI_REAL const *options, ti_stream **stream_in) {
    ti_stream_pc **stream = (ti_stream_pc**)stream_in;

    const int period = (int)options[0];

    if (period < 1) { return TI_INVALID_OPTION; }

    *stream = calloc(1, sizeof(**stream));
    if (!*stream) { return TI_OUT_OF_MEMORY; }
    BUFFER_INIT(*stream, high, period);
    BUFFER_INIT(*stream, low, period);
    *stream = realloc(*stream, sizeof(**stream) + sizeof(TI_REAL[BUFFERS_SIZE(*stream)]));
    if (!*stream) { return TI_OUT_OF_MEMORY; }

    (*stream)->index = TI_INDICATOR_PC_INDEX;
    (*stream)->progress = -ti_pc_start(options);
    (*stream)->options.period = period;

    return TI_OKAY;
}

void ti_pc_stream_free(ti_stream *stream) {
    free(stream);
}

int ti_pc_stream_run(ti_stream *stream_in, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    ti_stream_pc *stream = (ti_stream_pc*)stream_in;

    int progress = stream->progress;

    TI_REAL const *high = inputs[0];
    TI_REAL const *low = inputs[1];
    TI_REAL *pc_low = outputs[0];
    TI_REAL *pc_high = outputs[1];

    TI_REAL period = stream->options.period;

    TI_REAL max = stream->state.max;
    int max_idx = stream->state.max_idx;
    TI_REAL min = stream->state.min;
    int min_idx = stream->state.min_idx;

    int i = 0;
    for (; i < size && progress == -period+1; ++i, ++progress) {
        BUFFER_PUSH(stream, high, high[i]);
        BUFFER_PUSH(stream, low, low[i]);
        max = high[i];
        max_idx = progress;
        min = low[i];
        min_idx = progress;
    }
    for (; i < size && progress < 0; ++i, ++progress) {
        BUFFER_PUSH(stream, high, high[i]);
        BUFFER_PUSH(stream, low, low[i]);
        if (max <= high[i]) {
            max = high[i];
            max_idx = progress;
        }
        if (min >= low[i]) {
            min = low[i];
            min_idx = progress;
        }
    }
    for (; i < size; ++i, ++progress) {
        BUFFER_PUSH(stream, high, high[i]);
        BUFFER_PUSH(stream, low, low[i]);
        if (max_idx == progress - period) {
            max_idx = progress;
            BUFFER_AT(max, stream, high, 0);
            for (int j = 1; j < period; ++j) {
                TI_REAL var1; BUFFER_AT(var1, stream, high, -j);
                if (var1 >= max) {
                    max = var1;
                    max_idx = progress-j;
                }
            }
        } else if (high[i] >= max) {
            max = high[i];
            max_idx = progress;
        }
        if (min_idx == progress - period) {
            min_idx = progress;
            BUFFER_AT(min, stream, low, 0);
            for (int j = 1; j < period; ++j) {
                TI_REAL var1; BUFFER_AT(var1, stream, low, -j);
                if (var1 <= min) {
                    min = var1;
                    min_idx = progress-j;
                }
            }
        } else if (low[i] <= min) {
            min = low[i];
            min_idx = progress;
        }

        *pc_low++ = min;
        *pc_high++ = max;
    }

    stream->progress = progress;

    stream->state.max = max;
    stream->state.max_idx = max_idx;
    stream->state.min = min;
    stream->state.min_idx = min_idx;

    return TI_OKAY;
}
