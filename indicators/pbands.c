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


int ti_pbands_start(TI_REAL const *options) {
    const TI_REAL period = options[0];
    return period-1;
}


int ti_pbands(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    TI_REAL const *high = inputs[0];
    TI_REAL const *low = inputs[1];
    TI_REAL const *close = inputs[2];
    const TI_REAL period = options[0];
    TI_REAL *pbands_lower = outputs[0];
    TI_REAL *pbands_upper = outputs[1];

    if (period < 1) { return TI_INVALID_OPTION; }
    if (size <= ti_pbands_start(options)) { return TI_OKAY; }

    TI_REAL y_sum = 0.;
    TI_REAL xy_sum = 0.;

    const TI_REAL x_sum = period * (period + 1) / 2.;
    const TI_REAL xsq_sum = period * (period + 1) * (2*period + 1) / 6.;

    int i = 0;

    for (; i < period; ++i) {
        xy_sum += close[i] * (i + 1);
        y_sum += close[i];
    }
    {
        --i;
        // y = a + bx
        TI_REAL b = (xy_sum / period - x_sum / period * y_sum / period) / (xsq_sum / period - (x_sum / period)*(x_sum / period));

        TI_REAL the_max = high[i];
        for (int j = 1; j < period; ++j) {
            if (the_max < high[i-j] + j * b) {
                the_max = high[i-j] + j * b;
            }
        }
        TI_REAL the_min = low[i];
        for (int j = 1; j < period; ++j) {
            if (the_min > low[i-j] + j * b) {
                the_min = low[i-j] + j * b;
            }
        }
        *pbands_upper++ = the_max;
        *pbands_lower++ = the_min;

        ++i;
    }
    for (; i < size; ++i) {
        xy_sum += -y_sum + close[i]*period;
        y_sum += -close[i-(int)period] + close[i];

        // y = a + bx
        TI_REAL b = (xy_sum / period - x_sum / period * y_sum / period) / (xsq_sum / period - (x_sum / period)*(x_sum / period));

        TI_REAL the_max = high[i];
        for (int j = 1; j < period; ++j) {
            if (the_max < high[i-j] + j * b) {
                the_max = high[i-j] + j * b;
            }
        }
        TI_REAL the_min = low[i];
        for (int j = 1; j < period; ++j) {
            if (the_min > low[i-j] + j * b) {
                the_min = low[i-j] + j * b;
            }
        }
        *pbands_upper++ = the_max;
        *pbands_lower++ = the_min;
    }

    return TI_OKAY;
}

int ti_pbands_ref(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    TI_REAL const *high = inputs[0];
    TI_REAL const *low = inputs[1];
    TI_REAL const *close = inputs[2];
    const TI_REAL period = options[0];
    TI_REAL *pbands_lower = outputs[0];
    TI_REAL *pbands_upper = outputs[1];

    int start = ti_linregslope_start(options);
    TI_REAL *b = malloc(sizeof(TI_REAL[size - start]));

    ti_linregslope(size, &close, &period, &b);

    for (int i = start; i < size; ++i) {
        TI_REAL the_max = high[i];
        for (int j = 1; j < period; ++j) {
            if (the_max < high[i-j] + j * b[i-start]) {
                the_max = high[i-j] + j * b[i-start];
            }
        }
        TI_REAL the_min = low[i];
        for (int j = 1; j < period; ++j) {
            if (the_min > low[i-j] + j * b[i-start]) {
                the_min = low[i-j] + j * b[i-start];
            }
        }

        *pbands_upper++ = the_max;
        *pbands_lower++ = the_min;
    }

    free(b);

    return TI_OKAY;
}


struct ti_stream {
    int index;
    int progress;

    struct {
        TI_REAL period;
    } options;

    struct {
        TI_REAL y_sum;
        TI_REAL xy_sum;
    } state;

    struct {
        TI_REAL x_sum;
        TI_REAL xsq_sum;
    } constants;

    BUFFERS(
        BUFFER(high)
        BUFFER(low)
        BUFFER(close)
    )
};

int ti_pbands_stream_new(TI_REAL const *options, ti_stream **stream) {
    const TI_REAL period = options[0];
    if (period < 1) { return TI_INVALID_OPTION; }

    *stream = calloc(1, sizeof(**stream));
    if (!*stream) { return TI_OUT_OF_MEMORY; }

    (*stream)->index = TI_INDICATOR_PBANDS_INDEX;
    (*stream)->progress = -ti_pbands_start(options);

    (*stream)->options.period = period;

    (*stream)->state.y_sum = 0.;
    (*stream)->state.xy_sum = 0.;

    (*stream)->constants.x_sum = period * (period + 1) / 2.;
    (*stream)->constants.xsq_sum = period * (period + 1) * (2*period + 1) / 6.;

    BUFFER_INIT(*stream, high, period+1);
    BUFFER_INIT(*stream, low, period+1);
    BUFFER_INIT(*stream, close, period+1);

    *stream = realloc(*stream, sizeof(**stream) + sizeof(TI_REAL[BUFFERS_SIZE(*stream)]));
    if (!*stream) { return TI_OUT_OF_MEMORY; }

    return TI_OKAY;
}

void ti_pbands_stream_free(ti_stream *stream) {
    free(stream);
}

int ti_pbands_stream_run(ti_stream *stream, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    TI_REAL const *high = inputs[0];
    TI_REAL const *low = inputs[1];
    TI_REAL const *close = inputs[2];
    TI_REAL *pbands_lower = outputs[0];
    TI_REAL *pbands_upper = outputs[1];

    int progress = stream->progress;

    const TI_REAL period = stream->options.period;

    TI_REAL y_sum = stream->state.y_sum;
    TI_REAL xy_sum = stream->state.xy_sum;

    const TI_REAL xsq_sum = stream->constants.xsq_sum;
    const TI_REAL x_sum = stream->constants.x_sum;

    int i = 0;
    TI_REAL var1;
    for (; i < size && progress <= 0; ++i, ++progress) {
        BUFFER_PUSH(stream, high, high[i]);
        BUFFER_PUSH(stream, low, low[i]);
        BUFFER_PUSH(stream, close, close[i]);

        xy_sum += close[i] * (progress - (-period+1) + 1);
        y_sum += close[i];
    }

    if (i > 0 && progress == 1) {
        --progress;
        --i;

        // y = a + bx
        TI_REAL b = (xy_sum / period - x_sum / period * y_sum / period) / (xsq_sum / period - (x_sum / period)*(x_sum / period));

        TI_REAL the_max = high[i];
        for (int j = 1; j < period; ++j) {
            BUFFER_AT(var1, stream, high, -j);
            if (the_max < var1 + j * b) {
                the_max = var1 + j * b;
            }
        }
        TI_REAL the_min = low[i];
        for (int j = 1; j < period; ++j) {
            BUFFER_AT(var1, stream, low, -j);
            if (the_min > var1 + j * b) {
                the_min = var1 + j * b;
            }
        }
        *pbands_upper++ = the_max;
        *pbands_lower++ = the_min;

        ++progress;
        ++i;
    }

    for (; i < size; ++i, ++progress) {
        BUFFER_PUSH(stream, high, high[i]);
        BUFFER_PUSH(stream, low, low[i]);
        BUFFER_PUSH(stream, close, close[i]);

        xy_sum += -y_sum + close[i]*period;
        BUFFER_AT(var1, stream, close, -period);
        y_sum += -var1 + close[i];

        // y = a + bx
        TI_REAL b = (xy_sum / period - x_sum / period * y_sum / period) / (xsq_sum / period - (x_sum / period)*(x_sum / period));

        TI_REAL the_max = high[i];
        for (int j = 1; j < period; ++j) {
            BUFFER_AT(var1, stream, high, -j);
            if (the_max < var1 + j * b) {
                the_max = var1 + j * b;
            }
        }
        TI_REAL the_min = low[i];
        for (int j = 1; j < period; ++j) {
            BUFFER_AT(var1, stream, low, -j);
            if (the_min > var1 + j * b) {
                the_min = var1 + j * b;
            }
        }
        *pbands_upper++ = the_max;
        *pbands_lower++ = the_min;
    }

    stream->progress = progress;

    stream->state.y_sum = y_sum;
    stream->state.xy_sum = xy_sum;

    return TI_OKAY;
}