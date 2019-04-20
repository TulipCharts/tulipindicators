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

    TI_REAL max = high[i];
    int max_idx = i;
    TI_REAL min = low[i];
    int min_idx = i;

    for (; i < period; ++i) {
        if (max <= high[i]) {
            max = high[i];
            max_idx = i;
        }
        if (min >= low[i]) {
            min = low[i];
            min_idx = i;
        }

        xy_sum += close[i] * (i + 1);
        y_sum += close[i];

    }
    {
        --i;
        // y = a + bx
        TI_REAL b = (xy_sum / period - x_sum / period * y_sum / period) / (xsq_sum / period - (x_sum / period)*(x_sum / period));
        TI_REAL a = y_sum / period - b * x_sum / period;

        *pbands_upper++ = (a + b*period) + (max - (a + b*(period - (i - max_idx))));
        *pbands_lower++ = (a + b*period) - (a + b*(period - (i - min_idx)) - min);

        // *pbands_upper++ = a + b*period + (max - (a + b*(period-(i-max_idx))));
        // *pbands_lower++ = a + b*period - ((a + b*(period-(i-min_idx))) - min);
        ++i;
    }
    for (; i < size; ++i) {
        if (max_idx == i - period) {
            max_idx = i - period+1;
            max = high[i-(int)period+1];
            for (int j = i-period+2; j <= i; ++j) {
                if (high[j] >= max) {
                    max = high[j];
                    max_idx = j;
                }
            }
        } else if (high[i] >= max) {
            max = high[i];
            max_idx = i;
        }
        if (min_idx == i - period) {
            min_idx = i - period+1;
            min = low[i-(int)period+1];
            for (int j = i-period+2; j <= i; ++j) {
                if (low[j] <= min) {
                    min = low[j];
                    min_idx = j;
                }
            }
        } else if (low[i] <= min) {
            min = low[i];
            min_idx = i;
        }

        xy_sum += -y_sum + close[i]*period;
        y_sum += -close[i-(int)period] + close[i];

        // y = a + bx
        TI_REAL b = (xy_sum / period - x_sum / period * y_sum / period) / (xsq_sum / period - (x_sum / period)*(x_sum / period));
        TI_REAL a = y_sum / period - b * x_sum / period;

        *pbands_upper++ = (a + b*period) + (max - (a + b*(period - (i - max_idx))));
        *pbands_lower++ = (a + b*period) - (a + b*(period - (i - min_idx)) - min);
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
    TI_REAL *a = malloc(sizeof(TI_REAL[size - start]));

    ti_linregslope(size, &close, &period, &b);
    ti_linregintercept(size, &close, &period, &a);

    for (int i = start; i < size; ++i) {
        TI_REAL max;
        TI_REAL min;
        int max_idx;
        int min_idx;

        max = high[i-(int)period+1];
        max_idx = i-(int)period+1;
        for (int j = i-(int)period+2; j <= i; ++j) {
            if (max <= high[j]) {
                max = high[j];
                max_idx = j;
            }
        }
        min = low[i-(int)period+1];
        min_idx = i-(int)period+1;
        for (int j = i-period+2; j <= i; ++j) {
            if (min >= low[j]) {
                min = low[j];
                min_idx = j;
            }
        }

        *pbands_upper++ = a[i-start] + b[i-start]*period + (max - (a[i-start] + b[i-start]*(period-(i-max_idx))));
        *pbands_lower++ = a[i-start] + b[i-start]*period - ((a[i-start] + b[i-start]*(period-(i-min_idx))) - min);
    }

    free(a);
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
        TI_REAL max;
        int max_idx;
        TI_REAL min;
        int min_idx;

        TI_REAL xsq_sum;
        TI_REAL x_sum;
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

    TI_REAL max = stream->state.max;
    int max_idx = stream->state.max_idx;
    TI_REAL min = stream->state.min;
    int min_idx = stream->state.min_idx;

    TI_REAL y_sum = stream->state.y_sum;
    TI_REAL xy_sum = stream->state.xy_sum;

    const TI_REAL xsq_sum = stream->constants.xsq_sum;
    const TI_REAL x_sum = stream->constants.x_sum;

    int i = 0;
    TI_REAL var1;
    if (i < size && progress == -period+1) {
        BUFFER_PUSH(stream, high, high[0]);
        BUFFER_PUSH(stream, low, low[0]);
        BUFFER_PUSH(stream, close, close[0]);

        max = high[0];
        max_idx = progress;
        min = low[0];
        min_idx = progress;
    }
    for (; i < size && progress <= 0; ++i, ++progress) {
        BUFFER_PUSH(stream, high, high[i]);
        BUFFER_PUSH(stream, low, low[i]);
        BUFFER_PUSH(stream, close, close[i]);
        if (max <= high[i]) {
            max = high[i];
            max_idx = progress;
        }
        if (min >= low[i]) {
            min = low[i];
            min_idx = progress;
        }

        xy_sum += close[i] * (progress - (-period+1) + 1);
        y_sum += close[i];
    }
    if (i > 0 && progress == 1) {
        --progress;

        // y = a + bx
        TI_REAL b = (xy_sum / period - x_sum / period * y_sum / period) / (xsq_sum / period - (x_sum / period)*(x_sum / period));
        TI_REAL a = y_sum / period - b * x_sum / period;

        *pbands_upper++ = (a + b*period) + (max - (a + b*(period - (progress - max_idx))));
        *pbands_lower++ = (a + b*period) - (a + b*(period - (progress - min_idx)) - min);

        ++progress;
    }
    for (; i < size; ++i, ++progress) {
        BUFFER_PUSH(stream, high, high[i]);
        BUFFER_PUSH(stream, low, low[i]);
        BUFFER_PUSH(stream, close, close[i]);
        if (max_idx == progress - period) {
            max_idx = progress - period+1;
            BUFFER_AT(max, stream, high, -period+1);
            for (int j = -period+2; j <= 0; ++j) {
                BUFFER_AT(var1, stream, high, j);
                if (var1 >= max) {
                    max = var1;
                    max_idx = progress+j;
                }
            }
        } else if (high[i] >= max) {
            max = high[i];
            max_idx = progress;
        }
        if (min_idx == progress - period) {
            min_idx = progress - period+1;
            BUFFER_AT(min, stream, low, -period+1);
            for (int j = -period+2; j <= 0; ++j) {
                BUFFER_AT(var1, stream, low, j);
                if (var1 <= min) {
                    min = var1;
                    min_idx = progress+j;
                }
            }
        } else if (low[i] <= min) {
            min = low[i];
            min_idx = progress;
        }

        xy_sum += -y_sum + close[i]*period;
        BUFFER_AT(var1, stream, close, -period);
        y_sum += -var1 + close[i];

        // y = a + bx
        TI_REAL b = (xy_sum / period - x_sum / period * y_sum / period) / (xsq_sum / period - (x_sum / period)*(x_sum / period));
        TI_REAL a = y_sum / period - b * x_sum / period;

        *pbands_upper++ = (a + b*period) + (max - (a + b*(period - (progress - max_idx))));
        *pbands_lower++ = (a + b*period) - (a + b*(period - (progress - min_idx)) - min);
    }

    stream->progress = progress;

    stream->options.period = period;

    stream->state.max = max;
    stream->state.max_idx = max_idx;
    stream->state.min = min;
    stream->state.min_idx = min_idx;

    stream->state.y_sum = y_sum;
    stream->state.xy_sum = xy_sum;

    return TI_OKAY;
}