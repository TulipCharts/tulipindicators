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


int ti_tsi_start(TI_REAL const *options) {
    (void)options;
    return 1;
}

int ti_tsi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *real = inputs[0];
    const TI_REAL y_period = options[0];
    const TI_REAL z_period = options[1];
    TI_REAL *tsi = outputs[0];

    for (int i = 0; i < 2; ++i) { if (options[i] < 1) { return TI_INVALID_OPTION; } }

    int progress = -1;

    TI_REAL price;
    TI_REAL y_ema_num;
    TI_REAL z_ema_num;
    TI_REAL y_ema_den;
    TI_REAL z_ema_den;

    int i = 0;

    for (; i < size && progress == -1; ++i, ++progress) {
        price = real[i];
    }
    for (; i < size && progress == 0; ++i, ++progress) {
        z_ema_num = y_ema_num = real[i] - price;
        z_ema_den = y_ema_den = fabs(real[i] - price);


        *tsi++ = 100. * (z_ema_den ? z_ema_num / z_ema_den : 0);

        price = real[i];
    }
    for (; i < size; ++i, ++progress) {
        y_ema_num = ((real[i] - price) - y_ema_num) * 2. / (1. + y_period) + y_ema_num;
        y_ema_den = ((fabs(real[i] - price)) - y_ema_den) * 2. / (1. + y_period) + y_ema_den;

        z_ema_num = (y_ema_num - z_ema_num) * 2. / (1. + z_period) + z_ema_num;
        z_ema_den = (y_ema_den - z_ema_den) * 2. / (1. + z_period) + z_ema_den;


        *tsi++ = 100. * (z_ema_den ? z_ema_num / z_ema_den : 0);

        price = real[i];
    }

    return TI_OKAY;
}

int ti_tsi_ref(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *real = inputs[0];
    const TI_REAL y_period = options[0];
    const TI_REAL z_period = options[1];
    TI_REAL *tsi = outputs[0];

    for (int i = 0; i < 2; ++i) { if (options[i] < 1) { return TI_INVALID_OPTION; } }

    TI_REAL _one = 1;
    int outsize = size - ti_mom_start(&_one);
    TI_REAL *momentum = malloc(sizeof(TI_REAL) * (unsigned int)outsize);
    ti_mom(size, &real, &_one, &momentum);

    TI_REAL *absmomentum = malloc(sizeof(TI_REAL) * (unsigned int)outsize);
    const TI_REAL *momentum_input[] = {momentum};

    ti_abs(outsize, momentum_input, 0, &absmomentum);
    ti_ema(outsize, momentum_input, &y_period, &momentum);
    ti_ema(outsize, momentum_input, &z_period, &momentum);

    const TI_REAL *absmomentum_input[] = {absmomentum};
    ti_ema(outsize, absmomentum_input, &y_period, &absmomentum);
    ti_ema(outsize, absmomentum_input, &z_period, &absmomentum);

    for (int i = 0; i < outsize; ++i) {
        *tsi++ = 100. * momentum[i] / absmomentum[i];
    }

    free(momentum);
    free(absmomentum);

    return TI_OKAY;
}

typedef struct ti_stream_tsi {
    int index;
    int progress;

    struct {
        TI_REAL y_period;
        TI_REAL z_period;
    } options;

    struct {
        TI_REAL price;
        TI_REAL y_ema_num;
        TI_REAL z_ema_num;
        TI_REAL y_ema_den;
        TI_REAL z_ema_den;
    } state;
} ti_stream_tsi;

int ti_tsi_stream_new(TI_REAL const *options, ti_stream **stream_in) {
    ti_stream_tsi **stream = (ti_stream_tsi**)stream_in;

    const TI_REAL y_period = options[0];
    const TI_REAL z_period = options[1];
    for (int i = 0; i < 2; ++i) { if (options[i] < 1) { return TI_INVALID_OPTION; } }

    *stream = malloc(sizeof(**stream));
    if (!*stream) { return TI_OUT_OF_MEMORY; }

    (*stream)->index = TI_INDICATOR_TSI_INDEX;
    (*stream)->progress = -ti_tsi_start(options);

    (*stream)->options.y_period = y_period;
    (*stream)->options.z_period = z_period;

    return TI_OKAY;
}

void ti_tsi_stream_free(ti_stream *stream) {
    free(stream);
}

int ti_tsi_stream_run(ti_stream *stream_in, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    ti_stream_tsi *stream = (ti_stream_tsi*)stream_in;

    const TI_REAL *real = inputs[0];
    TI_REAL *tsi = outputs[0];

    const TI_REAL y_period = stream->options.y_period;
    const TI_REAL z_period = stream->options.z_period;

    int progress = stream->progress;

    TI_REAL price = stream->state.price;
    TI_REAL y_ema_num = stream->state.y_ema_num;
    TI_REAL z_ema_num = stream->state.z_ema_num;
    TI_REAL y_ema_den = stream->state.y_ema_den;
    TI_REAL z_ema_den = stream->state.z_ema_den;

    int i = 0;

    for (; i < size && progress == -1; ++i, ++progress) {
        price = real[i];
    }
    for (; i < size && progress == 0; ++i, ++progress) {
        z_ema_num = y_ema_num = real[i] - price;
        z_ema_den = y_ema_den = fabs(real[i] - price);


        *tsi++ = 100. * (z_ema_den ? z_ema_num / z_ema_den : 0);

        price = real[i];
    }
    for (; i < size; ++i, ++progress) {
        y_ema_num = ((real[i] - price) - y_ema_num) * 2. / (1. + y_period) + y_ema_num;
        y_ema_den = ((fabs(real[i] - price)) - y_ema_den) * 2. / (1. + y_period) + y_ema_den;

        z_ema_num = (y_ema_num - z_ema_num) * 2. / (1. + z_period) + z_ema_num;
        z_ema_den = (y_ema_den - z_ema_den) * 2. / (1. + z_period) + z_ema_den;


        *tsi++ = 100. * (z_ema_den ? z_ema_num / z_ema_den : 0);

        price = real[i];
    }

    stream->progress = progress;

    stream->state.price = price;
    stream->state.y_ema_num = y_ema_num;
    stream->state.z_ema_num = z_ema_num;
    stream->state.y_ema_den = y_ema_den;
    stream->state.z_ema_den = z_ema_den;

    return TI_OKAY;
}
