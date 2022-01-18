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


int ti_fi_start(TI_REAL const *options) {
    (void)options;
    return 1;
}


int ti_fi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *close = inputs[0];
    const TI_REAL *volume = inputs[1];
    const TI_REAL period = options[0];
    TI_REAL *fi = outputs[0];

    if (period < 1) { return TI_INVALID_OPTION; }
    if (size <= ti_fi_start(options)) { return TI_OKAY; }

    const TI_REAL per = 2. / (period + 1.);

    TI_REAL ema = volume[1] * (close[1] - close[0]);

    for (int i = 1; i < size; ++i) {
        ema = (volume[i] * (close[i] - close[i-1]) - ema) * per + ema;
        *fi++ = ema;
    }

    return TI_OKAY;
}

int ti_fi_ref(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *close = inputs[0];
    const TI_REAL *volume = inputs[1];
    const TI_REAL period = options[0];
    TI_REAL *fi = outputs[0];

    if (size <= ti_fi_start(options)) { return TI_OKAY; }

    for (int i = 1; i < size; ++i) {
        fi[i-1] = volume[i] * (close[i] - close[i-1]);
    }

    const TI_REAL *ti_ema_inputs[] = {fi};
    ti_ema(size-1, ti_ema_inputs, &period, &fi);

    return TI_OKAY;
}

typedef struct ti_stream_fi {
    int index;
    int progress;

    TI_REAL per;
    TI_REAL ema;
    TI_REAL previous_close;
} ti_stream_fi;

int ti_fi_stream_new(TI_REAL const *options, ti_stream **stream_in) {
    ti_stream_fi **stream = (ti_stream_fi**)stream_in;

    TI_REAL const period = options[0];
    if (period < 1) { return TI_INVALID_OPTION; }
    *stream = malloc(sizeof(**stream));
    if (!*stream) { return TI_OUT_OF_MEMORY; }

    (*stream)->per = 2. / (period + 1.);
    (*stream)->index = TI_INDICATOR_FI_INDEX;
    (*stream)->progress = -1;

    return TI_OKAY;
}

void ti_fi_stream_free(ti_stream *stream) {
    free(stream);
}

int ti_fi_stream_run(ti_stream *stream_in, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    ti_stream_fi *stream = (ti_stream_fi*)stream_in;

    int progress = stream->progress;

    const TI_REAL *close = inputs[0];
    const TI_REAL *volume = inputs[1];
    TI_REAL *fi = outputs[0];

    TI_REAL per = stream->per;
    TI_REAL ema = stream->ema;
    TI_REAL previous_close = stream->previous_close;

    int i = 0;
    for (; i < size && progress == -1; ++i, ++progress) {
        previous_close = close[i];
    }
    for (; i < size && progress == 0; ++i, ++progress) {
        ema = volume[i] * (close[i] - previous_close);
        previous_close = close[i];
        *fi++ = ema;
    }
    for (; i < size; ++i, ++progress) {
        ema = (volume[i] * (close[i] - previous_close) - ema) * per + ema;
        previous_close = close[i];
        *fi++ = ema;
    }

    stream->progress = progress;
    stream->ema = ema;
    stream->previous_close = previous_close;

    return TI_OKAY;
}
