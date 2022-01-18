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

#define START (roc_longer_period + wma_period - 1.0)

int ti_copp_start(TI_REAL const *options) {
    const int roc_longer_period = (int)options[1];
    const int wma_period = (int)options[2];
    return (int)START;
}


int ti_copp(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *real = inputs[0];
    const int roc_shorter_period = (int)options[0];
    const int roc_longer_period = (int)options[1];
    const int wma_period = (int)options[2];
    TI_REAL *copp = outputs[0];

    for (int i = 0; i < 3; ++i) {
        if (options[i] < 1) { return TI_INVALID_OPTION; }
    }
    if (roc_longer_period < roc_shorter_period) { return TI_INVALID_OPTION; }
    if (size <= ti_copp_start(options)) { return TI_OKAY; }

    struct {
        BUFFERS(
            BUFFER(price)
            BUFFER(rocs)
        )
    } *buffers;
    buffers = calloc(1, sizeof(*buffers));
    BUFFER_INIT(buffers, price, roc_longer_period+1);
    BUFFER_INIT(buffers, rocs, wma_period+1);
    buffers = realloc(buffers, sizeof(*buffers) + sizeof(TI_REAL[BUFFERS_SIZE(buffers)]));

    TI_REAL denominator = 1. / (wma_period * (wma_period + 1.) / 2.);
    TI_REAL flat_rocs_sum = 0.;
    TI_REAL weighted_rocs_sum = 0.;

    TI_REAL var1, var2, var3;
    int i = 0;
    for (; i < roc_longer_period; ++i) {
        BUFFER_PUSH(buffers, price, real[i]);
    }
    for (; i <= roc_longer_period + wma_period-1; ++i) {
        BUFFER_PUSH(buffers, price, real[i]);

        BUFFER_AT(var1, buffers, price, -roc_shorter_period);
        BUFFER_AT(var2, buffers, price, -roc_longer_period);
        BUFFER_AT(var3, buffers, price, 0);
        BUFFER_PUSH(buffers, rocs, ((var3 / var1 - 1) + (var3 / var2 - 1)) / 2. * 100.);

        BUFFER_AT(var1, buffers, rocs, 0);
        flat_rocs_sum += var1;
        weighted_rocs_sum += var1 * (i + 1 - roc_longer_period);
    }
    if (i == roc_longer_period + wma_period) {
        *copp++ = weighted_rocs_sum * denominator;
    }
    for (; i < size; ++i) {
        BUFFER_PUSH(buffers, price, real[i]);

        BUFFER_AT(var1, buffers, price, -roc_shorter_period);
        BUFFER_AT(var2, buffers, price, -roc_longer_period);
        BUFFER_AT(var3, buffers, price, 0);
        BUFFER_PUSH(buffers, rocs, ((var3 / var1 - 1) + (var3 / var2 - 1)) / 2. * 100.);

        BUFFER_AT(var1, buffers, rocs, 0);
        BUFFER_AT(var2, buffers, rocs, -wma_period);
        weighted_rocs_sum += -flat_rocs_sum + var1 * wma_period;
        flat_rocs_sum += -var2 + var1;
        *copp++ = weighted_rocs_sum * denominator;
    }

    free(buffers);

    return TI_OKAY;
}

int ti_copp_ref(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *real = inputs[0];
    const TI_REAL roc_shorter_period = options[0];
    const TI_REAL roc_longer_period = options[1];
    const TI_REAL wma_period = options[2];
    TI_REAL *copp = outputs[0];

    for (int i = 0; i < 3; ++i) {
        if (options[i] < 1) { return TI_INVALID_OPTION; }
    }
    if (roc_longer_period < roc_shorter_period) { return TI_INVALID_OPTION; }
    if (size < ti_copp_start(options)) { return TI_OKAY; }

    // wma of (roc1 + roc2) / 2 * 100

    int roc_short_len = size-ti_roc_start(&roc_shorter_period);
    TI_REAL *roc_short = malloc(sizeof(TI_REAL[roc_short_len]));
    int roc_long_len = size-ti_roc_start(&roc_longer_period);
    TI_REAL *roc_long = malloc(sizeof(TI_REAL[roc_long_len]));

    TI_REAL *interm = malloc(sizeof(TI_REAL[roc_long_len]));

    // int wma_len = roc_long_len-ti_wma_start(&wma_period);
    // TI_REAL *wma = malloc(sizeof(TI_REAL[wma_len]));

    ti_roc(size, &real, &roc_shorter_period, &roc_short);
    ti_roc(size, &real, &roc_longer_period, &roc_long);

    for (int i = 0; i < roc_long_len; ++i) {
        interm[i] = (roc_long[i] + roc_short[i+(roc_short_len-roc_long_len)]) * 100. / 2.;
    }

    const TI_REAL *ti_wma_inputs[] = {interm};
    ti_wma(roc_long_len, ti_wma_inputs, &wma_period, &copp);

    free(roc_short);
    free(roc_long);
    free(interm);

    return TI_OKAY;
}



typedef struct ti_stream_copp {
    int index;
    int progress;

    struct {
        int roc_shorter_period;
        int roc_longer_period;
        int wma_period;
    } options;

    struct {
        TI_REAL price_sum;
        TI_REAL denominator;
        TI_REAL rocs_per;
        TI_REAL flat_rocs_sum;
        TI_REAL weighted_rocs_sum;
    } state;

    BUFFERS(
        BUFFER(price)
        BUFFER(rocs)
    )
} ti_stream_copp;

int ti_copp_stream_new(TI_REAL const *options, ti_stream **stream_in) {
    ti_stream_copp **stream = (ti_stream_copp**)stream_in;

    const int roc_shorter_period = (int)options[0];
    const int roc_longer_period = (int)options[1];
    const int wma_period = (int)options[2];

    for (int i = 0; i < 3; ++i) {
        if (options[i] < 1) { return TI_INVALID_OPTION; }
    }
    if (roc_longer_period < roc_shorter_period) { return TI_INVALID_OPTION; }

    *stream = calloc(1, sizeof(**stream));
    if (!stream) { return TI_OUT_OF_MEMORY; }
    BUFFER_INIT(*stream, price, roc_longer_period+1);
    BUFFER_INIT(*stream, rocs, wma_period+1);
    *stream = realloc(*stream, sizeof(**stream) + sizeof(TI_REAL[BUFFERS_SIZE(*stream)]));
    if (!stream) { return TI_OUT_OF_MEMORY; }

    (*stream)->index = TI_INDICATOR_COPP_INDEX;
    (*stream)->progress = -ti_copp_start(options);

    memcpy(&(*stream)->options, options, sizeof(TI_REAL[4]));

    (*stream)->state.denominator = 1. / (wma_period * (wma_period + 1) / 2);
    (*stream)->state.rocs_per = 100. / 2.;
    (*stream)->state.flat_rocs_sum = 0.;
    (*stream)->state.weighted_rocs_sum = 0.;

    return TI_OKAY;
}

void ti_copp_stream_free(ti_stream *stream) {
    free(stream);
}

int ti_copp_stream_run(ti_stream *stream_in, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    ti_stream_copp* stream = (ti_stream_copp*)stream_in;

    int progress = stream->progress;

    const TI_REAL *real = inputs[0];
    TI_REAL *copp = outputs[0];

    const int roc_shorter_period = stream->options.roc_shorter_period;
    const int roc_longer_period = stream->options.roc_longer_period;
    const int wma_period = stream->options.wma_period;

    TI_REAL denominator = stream->state.denominator;
    TI_REAL rocs_per = stream->state.rocs_per;
    TI_REAL flat_rocs_sum = stream->state.flat_rocs_sum;
    TI_REAL weighted_rocs_sum = stream->state.weighted_rocs_sum;

    TI_REAL var1, var2, var3;
    int i = 0;
    for (; i < size && progress < -START + roc_longer_period; ++i, ++progress) {
        BUFFER_PUSH(stream, price, real[i]);
    }
    for (; i < size && progress <= -START + roc_longer_period + wma_period-1; ++i, ++progress) {
        BUFFER_PUSH(stream, price, real[i]);

        BUFFER_AT(var1, stream, price, -roc_shorter_period);
        BUFFER_AT(var2, stream, price, -roc_longer_period);
        BUFFER_AT(var3, stream, price, 0);
        BUFFER_PUSH(stream, rocs, ((var3 / var1 - 1) + (var3 / var2 - 1)) * rocs_per);

        BUFFER_AT(var1, stream, rocs, 0);
        flat_rocs_sum += var1;
        weighted_rocs_sum += var1 * (progress + 1 - (-START + roc_longer_period));
    }
    if (i > 0 && progress == -START + roc_longer_period + wma_period) {
        *copp++ = weighted_rocs_sum * denominator;
    }
    for (; i < size; ++i, ++progress) {
        BUFFER_PUSH(stream, price, real[i]);

        BUFFER_AT(var1, stream, price, -roc_shorter_period);
        BUFFER_AT(var2, stream, price, -roc_longer_period);
        BUFFER_AT(var3, stream, price, 0);
        BUFFER_PUSH(stream, rocs, ((var3 / var1 - 1) + (var3 / var2 - 1)) * rocs_per);

        BUFFER_AT(var1, stream, rocs, 0);
        BUFFER_AT(var2, stream, rocs, -wma_period);
        weighted_rocs_sum += -flat_rocs_sum + var1 * wma_period;
        flat_rocs_sum += -var2 + var1;
        *copp++ = weighted_rocs_sum * denominator;
    }

    stream->progress = progress;
    stream->state.flat_rocs_sum = flat_rocs_sum;
    stream->state.weighted_rocs_sum = weighted_rocs_sum;

    return TI_OKAY;
}


#undef START
