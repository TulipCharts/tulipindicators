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


int ti_vwap_start(TI_REAL const *options) {
    return (int)options[0]-1;
}

int ti_vwap(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    const TI_REAL *volume = inputs[3];
    const int period = (int)options[0];
    TI_REAL *vwap = outputs[0];

    if (period < 1) { return TI_INVALID_OPTION; }

    int progress = -period+1;

    TI_REAL num = 0;
    TI_REAL den = 0;

    int i = 0;
    for (; i < size && progress < 1; ++i, ++progress) {
        num += (high[i] + low[i] + close[i]) / 3. * volume[i];
        den += volume[i];
    }
    if (i > 0 && progress == 1) {
        *vwap++ = num / den;
    }
    for (; i < size; ++i, ++progress) {
        num += (high[i] + low[i] + close[i]) / 3. * volume[i]
            - (high[i-(int)period] + low[i-(int)period] + close[i-(int)period]) / 3. * volume[i-(int)period];
        den += volume[i] - volume[i-(int)period];

        *vwap++ = num / den;
    }

    return TI_OKAY;
}

int ti_vwap_ref(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *volume = inputs[3];
    const TI_REAL period = options[0];
    TI_REAL *vwap = outputs[0];

    if (period < 1) { return TI_INVALID_OPTION; }

    int outsize = size - ti_typprice_start(&period);
    TI_REAL *typprice = malloc(sizeof(TI_REAL) * (unsigned int)outsize);
    ti_typprice(size, inputs, 0, &typprice);

    TI_REAL sum = 0;
    TI_REAL vsum = 0;

    int i;
    for (i = 0; i < size && i < period; ++i) {
        sum += typprice[i] * volume[i];
        vsum += volume[i];
    }

    *vwap++ = sum / vsum;

    for (i = (int)period; i < size; ++i) {
        sum += typprice[i] * volume[i];
        sum -= typprice[i-(int)period] * volume[i-(int)period];
        vsum += volume[i];
        vsum -= volume[i-(int)period];

        *vwap++ = sum / vsum;
    }

    free(typprice);

    return TI_OKAY;
}

typedef struct ti_stream_vwap {
    int index;
    int progress;

    struct {
        int period;
    } options;

    struct {
        TI_REAL num;
        TI_REAL den;
    } state;

    BUFFERS(
        BUFFER(high)
        BUFFER(low)
        BUFFER(close)
        BUFFER(volume)
    )
} ti_stream_vwap;

int ti_vwap_stream_new(TI_REAL const *options, ti_stream **stream_in) {
    ti_stream_vwap **stream = (ti_stream_vwap**)stream_in;

    const int period = (int)options[0];
    if (period < 1) { return TI_INVALID_OPTION; }

    *stream = calloc(1, sizeof(**stream));
    if (!*stream) { return TI_OUT_OF_MEMORY; }

    (*stream)->index = TI_INDICATOR_VWAP_INDEX;
    (*stream)->progress = -ti_vwap_start(options);

    (*stream)->options.period = period;

    (*stream)->state.num = 0;
    (*stream)->state.den = 0;

    BUFFER_INIT(*stream, high, period + 1);
    BUFFER_INIT(*stream, low, period + 1);
    BUFFER_INIT(*stream, close, period + 1);
    BUFFER_INIT(*stream, volume, period + 1);

    *stream = realloc(*stream, sizeof(**stream) + sizeof(TI_REAL) * (unsigned int)BUFFERS_SIZE(*stream));
    if (!*stream) { return TI_OUT_OF_MEMORY; }

    return TI_OKAY;
}

void ti_vwap_stream_free(ti_stream *stream) {
    free(stream);
}

int ti_vwap_stream_run(ti_stream *stream_in, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    ti_stream_vwap *stream = (ti_stream_vwap*)stream_in;

    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    const TI_REAL *volume = inputs[3];
    TI_REAL *vwap = outputs[0];

    int progress = stream->progress;

    const int period = stream->options.period;

    TI_REAL num = stream->state.num;
    TI_REAL den = stream->state.den;

    TI_REAL var1, var2, var3, var4;

    int i = 0;
    for (; i < size && progress < 1; ++i, ++progress) {
        BUFFER_PUSH(stream, high, high[i]);
        BUFFER_PUSH(stream, low, low[i]);
        BUFFER_PUSH(stream, close, close[i]);
        BUFFER_PUSH(stream, volume, volume[i]);

        num += (high[i] + low[i] + close[i]) / 3. * volume[i];
        den += volume[i];
    }
    if (i > 0 && progress == 1) {
        *vwap++ = num / den;
    }
    for (; i < size; ++i, ++progress) {
        BUFFER_PUSH(stream, high, high[i]);
        BUFFER_PUSH(stream, low, low[i]);
        BUFFER_PUSH(stream, close, close[i]);
        BUFFER_PUSH(stream, volume, volume[i]);

        BUFFER_AT(var1, stream, high, -period);
        BUFFER_AT(var2, stream, low, -period);
        BUFFER_AT(var3, stream, close, -period);
        BUFFER_AT(var4, stream, volume, -period);

        num += (high[i] + low[i] + close[i]) / 3. * volume[i] - (var1 + var2 + var3) / 3. * var4;
        den += volume[i] - var4;

        *vwap++ = num / den;
    }

    stream->progress = progress;

    stream->state.num = num;
    stream->state.den = den;

    return TI_OKAY;
}
