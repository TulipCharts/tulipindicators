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


int ti_dc_start(TI_REAL const *options) {
    const int period = (int)options[0];
    return period-1;
}


/* at first, I implemented this by hand, but it turned out to be no better */
int ti_dc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *real = inputs[0];
    const int period = (int)options[0];
    TI_REAL *dc_lower = outputs[0];
    TI_REAL *dc_upper = outputs[1];

    if (period < 1) { return TI_INVALID_OPTION; }
    if (size <= ti_dc_start(options)) { return TI_OKAY; }

    /* TODO unacceptable */
    ti_min(size, &real, options, &dc_lower);
    ti_max(size, &real, options, &dc_upper);

    return TI_OKAY;
}


typedef struct ti_stream_de {
    int index;
    int progress;

    struct {
        int period;
    } options;

    struct {
        TI_REAL max;
        int max_idx;
        TI_REAL min;
        int min_idx;
    } state;

    BUFFERS(
        BUFFER(price)
    )
} ti_stream_de;

int ti_dc_stream_new(TI_REAL const *options, ti_stream **stream_in) {
    ti_stream_de **stream = (ti_stream_de**)stream_in;

    const int period = (int)options[0];

    if (period < 1) { return TI_INVALID_OPTION; }

    *stream = calloc(1, sizeof(**stream));
    if (!*stream) { return TI_OUT_OF_MEMORY; }
    BUFFER_INIT(*stream, price, period);
    *stream = realloc(*stream, sizeof(**stream) + sizeof(TI_REAL[BUFFERS_SIZE(*stream)]));
    if (!*stream) { return TI_OUT_OF_MEMORY; }

    (*stream)->index = TI_INDICATOR_DC_INDEX;
    (*stream)->progress = -ti_dc_start(options);
    (*stream)->options.period = period;

    return TI_OKAY;
}

void ti_dc_stream_free(ti_stream *stream) {
    free(stream);
}

int ti_dc_stream_run(ti_stream *stream_in, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    ti_stream_de *stream = (ti_stream_de*)stream_in;

    int progress = stream->progress;

    const TI_REAL *real = inputs[0];
    TI_REAL *dc_lower = outputs[0];
    TI_REAL *dc_upper = outputs[1];

    const int period = stream->options.period;

    TI_REAL max = stream->state.max;
    int max_idx = stream->state.max_idx;
    TI_REAL min = stream->state.min;
    int min_idx = stream->state.min_idx;

    int i = 0;
    for (; i < size && progress == -period+1; ++i, ++progress) {
        BUFFER_PUSH(stream, price, real[i]);
        max = real[i];
        max_idx = progress;
        min = real[i];
        min_idx = progress;
    }
    for (; i < size && progress < 0; ++i, ++progress) {
        BUFFER_PUSH(stream, price, real[i]);
        if (max <= real[i]) {
            max = real[i];
            max_idx = progress;
        }
        if (min >= real[i]) {
            min = real[i];
            min_idx = progress;
        }
    }
    for (; i < size; ++i, ++progress) {
        BUFFER_PUSH(stream, price, real[i]);
        if (max_idx == progress - period) {
            max_idx = progress;
            BUFFER_AT(max, stream, price, 0);
            for (int j = 1; j < period; ++j) {
                TI_REAL var1; BUFFER_AT(var1, stream, price, -j);
                if (var1 >= max) {
                    max = var1;
                    max_idx = progress-j;
                }
            }
        } else if (real[i] >= max) {
            max = real[i];
            max_idx = progress;
        }
        if (min_idx == progress - period) {
            min_idx = progress;
            BUFFER_AT(min, stream, price, 0);
            for (int j = 1; j < period; ++j) {
                TI_REAL var1; BUFFER_AT(var1, stream, price, -j);
                if (var1 <= min) {
                    min = var1;
                    min_idx = progress-j;
                }
            }
        } else if (real[i] <= min) {
            min = real[i];
            min_idx = progress;
        }

        *dc_lower++ = min;
        *dc_upper++ = max;
    }

    stream->progress = progress;

    stream->state.max = max;
    stream->state.max_idx = max_idx;
    stream->state.min = min;
    stream->state.min_idx = min_idx;

    return TI_OKAY;
}
