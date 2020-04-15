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
#include "truerange.h"


int ti_kc_start(TI_REAL const *options) {
    (void)options;
    return 0;
}

int ti_kc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    const TI_REAL period = options[0];
    const TI_REAL multiple = options[1];
    TI_REAL *kc_lower = outputs[0];
    TI_REAL *kc_middle = outputs[1];
    TI_REAL *kc_upper = outputs[2];

    // moving average + multiple * atr
    // moving average
    // moving average - multiple * atr
    // where atr is ema of trueranges

    if (period < 1) {
        return TI_INVALID_OPTION;
    }
    if (multiple < 0 || multiple > 1) {
        return TI_INVALID_OPTION;
    }

    const TI_REAL per = 2 / ((TI_REAL)period + 1);

    TI_REAL price_ema = close[0];
    TI_REAL tr_ema = high[0] - low[0];

    *kc_lower++ = price_ema - multiple * tr_ema;
    *kc_middle++ = price_ema;
    *kc_upper++ = price_ema + multiple * tr_ema;

    TI_REAL truerange;
    for (int i = 1; i < size; ++i) {
        price_ema = (close[i] - price_ema) * per + price_ema;

        CALC_TRUERANGE();
        tr_ema = (truerange - tr_ema) * per + tr_ema;

        *kc_lower++ = price_ema - multiple * tr_ema;
        *kc_middle++ = price_ema;
        *kc_upper++ = price_ema + multiple * tr_ema;
    }

    return TI_OKAY;
}

/// 1. close used instead of a generic input
/// 2. no ref since atr's ema and plain ema do not agree in the start amount, the plain ema way is chosen

typedef struct ti_stream_kc {
    int index;
    int progress;

    struct {
        TI_REAL period;
        TI_REAL multiple;
    } options;

    struct {
        TI_REAL price_ema;
        TI_REAL tr_ema;
    } state;

    BUFFERS(
        BUFFER(close)
    )
} ti_stream_kc;

int ti_kc_stream_new(TI_REAL const *options, ti_stream **stream_in) {
    ti_stream_kc **stream = (ti_stream_kc**)stream_in;

    const TI_REAL period = options[0];
    const TI_REAL multiple = options[1];

    if (period < 0) {
        return TI_INVALID_OPTION;
    }
    if (multiple < 0 || multiple > 1) {
        return TI_INVALID_OPTION;
    }

    *stream = calloc(1, sizeof(ti_stream_kc));
    if (!*stream) {
        return TI_OUT_OF_MEMORY;
    }

    (*stream)->index = TI_INDICATOR_KC_INDEX;
    (*stream)->progress = 0;
    (*stream)->options.period = period;
    (*stream)->options.multiple = multiple;

    BUFFER_INIT(*stream, close, 2);

    *stream = realloc(*stream, sizeof(ti_stream_kc) + sizeof(TI_REAL[BUFFERS_SIZE(*stream)]));
    if (!*stream) {
        return TI_OUT_OF_MEMORY;
    }

    return TI_OKAY;
}

void ti_kc_stream_free(ti_stream *stream) {
    free(stream);
}

int ti_kc_stream_run(ti_stream *stream_in, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    ti_stream_kc *stream = (ti_stream_kc*)stream_in;

    int progress = stream->progress;
    const TI_REAL period = stream->options.period;
    const TI_REAL multiple = stream->options.multiple;

    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    TI_REAL *kc_lower = outputs[0];
    TI_REAL *kc_middle = outputs[1];
    TI_REAL *kc_upper = outputs[2];

    TI_REAL price_ema = stream->state.price_ema;
    TI_REAL tr_ema = stream->state.tr_ema;

    TI_REAL var1;

    const TI_REAL per = 2 / ((TI_REAL)period + 1);

    int i;
    for (i = 0; i < size && progress == 0; ++i, ++progress) {
        BUFFER_PUSH(stream, close, close[i]);

        price_ema = close[i];
        tr_ema = high[i] - low[i];

        *kc_lower++ = price_ema - multiple * tr_ema;
        *kc_middle++ = price_ema;
        *kc_upper++ = price_ema + multiple * tr_ema;
    }
    #define KC_CALC_TRUERANGE(var, h, l, c) do { \
        const TI_REAL ych = fabs((h) - (c)); \
        const TI_REAL ycl = fabs((l) - (c)); \
        TI_REAL v = (h) - (l); \
        if (ych > v) v = ych; \
        if (ycl > v) v = ycl; \
        var = v; \
    } while (0)
    for (; i < size; ++i, ++progress) {
        BUFFER_PUSH(stream, close, close[i]);

        price_ema = (close[i] - price_ema) * per + price_ema;

        BUFFER_AT(var1, stream, close, -1);
        TI_REAL truerange; KC_CALC_TRUERANGE(truerange, high[i], low[i], var1);
        tr_ema = (truerange - tr_ema) * per + tr_ema;

        *kc_lower++ = price_ema - multiple * tr_ema;
        *kc_middle++ = price_ema;
        *kc_upper++ = price_ema + multiple * tr_ema;
    }

    stream->progress = progress;
    stream->state.price_ema = price_ema;
    stream->state.tr_ema = tr_ema;

    return TI_OKAY;
}
