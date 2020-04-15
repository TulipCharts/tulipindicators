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
#include "../utils/minmax.h"
#include "../utils/localbuffer.h"


int ti_rmi_start(TI_REAL const *options) {
    const TI_REAL period = options[0];
    const TI_REAL lookback_period = options[1];

    return lookback_period;
}


int ti_rmi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    TI_REAL const *real = inputs[0];
    const TI_REAL period = options[0];
    const TI_REAL lookback_period = options[1];
    TI_REAL *rmi = outputs[0];

    if (period < 1) { return TI_INVALID_OPTION; }
    if (lookback_period < 1) { return TI_INVALID_OPTION; }

    TI_REAL gains_ema;
    TI_REAL losses_ema;

    int i = lookback_period;
    {
        gains_ema = MAX(0, real[i] - real[i-(int)lookback_period]);
        losses_ema = MAX(0, real[i-(int)lookback_period] - real[i]);
        ++i;
    }
    {
        *rmi++ = gains_ema / (gains_ema + losses_ema) * 100.;
    }
    for (; i < size; ++i) {
        gains_ema = (MAX(0, real[i] - real[i-(int)lookback_period]) - gains_ema) * 2. / (1 + period) + gains_ema;
        losses_ema = (MAX(0, real[i-(int)lookback_period] - real[i]) - losses_ema) * 2. / (1 + period) + losses_ema;
        *rmi++ = gains_ema / (gains_ema + losses_ema) * 100.;
    }

    return TI_OKAY;
}

int ti_rmi_ref(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    TI_REAL const *real = inputs[0];
    const TI_REAL period = options[0];
    const TI_REAL lookback_period = options[1];
    TI_REAL *rmi = outputs[0];

    if (period < 1) { return TI_INVALID_OPTION; }
    if (lookback_period < 1) { return TI_INVALID_OPTION; }

    int start = ti_rmi_start(options);
    TI_REAL *gains = malloc(sizeof(TI_REAL[size-start]));
    TI_REAL *losses = malloc(sizeof(TI_REAL[size-start]));

    for (int i = lookback_period; i < size; ++i) {
        gains[i-start] = MAX(0, real[i] - real[i-(int)lookback_period]);
        losses[i-start] = MAX(0, real[i-(int)lookback_period] - real[i]);
    }
    ti_ema(size-start, &gains, &period, &gains);
    ti_ema(size-start, &losses, &period, &losses);

    TI_REAL *inputs_[] = {gains, losses};
    ti_add(size-start, inputs_, 0, &losses);
    for (int i = 0; i < size-start; ++i) {
        *rmi++ = gains[i] / losses[i] * 100.;
    }

    free(gains);
    free(losses);

    return TI_OKAY;
}

struct ti_stream {
    int index;
    int progress;

    struct {
        TI_REAL period;
        TI_REAL lookback_period;
    } options;

    struct {
        TI_REAL gains_ema;
        TI_REAL losses_ema;
    } state;

    BUFFERS(
        BUFFER(price)
    )
};

int ti_rmi_stream_new(TI_REAL const *options, ti_stream **stream) {
    const TI_REAL period = options[0];
    const TI_REAL lookback_period = options[1];

    if (period < 1) { return TI_INVALID_OPTION; }
    if (lookback_period < 1) { return TI_INVALID_OPTION; }

    *stream = calloc(1, sizeof(**stream));
    if (!*stream) { return TI_OUT_OF_MEMORY; }

    (*stream)->index = TI_INDICATOR_RMI_INDEX;
    (*stream)->progress = -ti_rmi_start(options);

    (*stream)->options.period = period;
    (*stream)->options.lookback_period = lookback_period;

    BUFFER_INIT(*stream, price, lookback_period + 1);
    *stream = realloc(*stream, sizeof(**stream) + sizeof(TI_REAL[BUFFERS_SIZE(*stream)]));
    if (!*stream) { return TI_OUT_OF_MEMORY; }

    return TI_OKAY;
}


void ti_rmi_stream_free(ti_stream *stream) {
    free(stream);
}

int ti_rmi_stream_run(ti_stream *stream, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    TI_REAL const *real = inputs[0];
    TI_REAL *rmi = outputs[0];

    int progress = stream->progress;

    TI_REAL period = stream->options.period;
    TI_REAL lookback_period = stream->options.lookback_period;

    TI_REAL gains_ema = stream->state.gains_ema;
    TI_REAL losses_ema = stream->state.losses_ema;

    int i = 0;
    TI_REAL var1;
    for (; i < size && progress < 0; ++i, ++progress) {
        BUFFER_PUSH(stream, price, real[i]);
    }
    if (i < size && progress == 0) {
        BUFFER_PUSH(stream, price, real[i]);
        BUFFER_AT(var1, stream, price, -lookback_period);
        gains_ema = MAX(0, real[i] - var1);
        losses_ema = MAX(0, var1 - real[i]);

        *rmi++ = gains_ema / (gains_ema + losses_ema) * 100.;
        ++i, ++progress;
    }
    for (; i < size; ++i, ++progress) {
        BUFFER_PUSH(stream, price, real[i]);
        BUFFER_AT(var1, stream, price, -lookback_period);
        gains_ema = (MAX(0, real[i] - var1) - gains_ema) * 2. / (period + 1) + gains_ema;
        losses_ema = (MAX(0, var1 - real[i]) - losses_ema) * 2. / (period + 1) + losses_ema;

        *rmi++ = gains_ema / (gains_ema + losses_ema) * 100.;
    }

    stream->progress = progress;

    stream->options.period = period;
    stream->options.lookback_period = lookback_period;

    stream->state.gains_ema = gains_ema;
    stream->state.losses_ema = losses_ema;

    return TI_OKAY;
}