/*
 * Tulip Indicators
 * https://tulipindicators.org/
 * Copyright (c) 2010-2017 Tulip Charts LLC
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
#include "truerange.h"
#include "../utils/minmax.h"



int ti_atr_start(TI_REAL const *options) {
    return (int)options[0]-1;
}


int ti_atr(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];

    const int period = (int)options[0];

    TI_REAL *output = outputs[0];

    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_atr_start(options)) return TI_OKAY;

    const TI_REAL per = 1.0 / ((TI_REAL)period);

    TI_REAL sum = 0;
    TI_REAL truerange;

    sum += high[0] - low[0];
    int i;
    for (i = 1; i < period; ++i) {
        CALC_TRUERANGE();
        sum += truerange;
    }


    TI_REAL val = sum / period;
    *output++ = val;

    for (i = period; i < size; ++i) {
        CALC_TRUERANGE();
        val = (truerange-val) * per + val;
        *output++ = val;
    }


    assert(output - outputs[0] == size - ti_atr_start(options));
    return TI_OKAY;
}


int ti_atr_ref(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {

    //atr = ti_wilders(ti_tr)

    //First calculate true range.
    const int tr_start = ti_tr_start(0);
    const int tr_size = size - tr_start;
    TI_REAL *truerange = malloc((unsigned int)tr_size * sizeof(TI_REAL));
    if (!truerange) {return TI_OUT_OF_MEMORY;}

    TI_REAL *tr_outputs[1] = {truerange};
    const int tr_ret = ti_tr(size, inputs, 0, tr_outputs);
    if (tr_ret != TI_OKAY) {
        free(truerange);
        return tr_ret;
    }


    //Then wilders.
    const TI_REAL *wilders_inputs[1] = {truerange};
    const int wilders_ret = ti_wilders(tr_size, wilders_inputs, options, outputs);

    free(truerange);


    assert(size - ti_atr_start(options) == size - ti_wilders_start(options));

    return wilders_ret;
}


struct ti_stream {
    /* required */
    int index;
    int progress;

    /* indicator specific */
    int period;
    TI_REAL sum;
    TI_REAL last;
};


ti_stream *ti_atr_stream_new(TI_REAL const *options) {
    const int period = (int)options[0];
    if (period < 1) return 0;

    ti_stream *stream = malloc(sizeof(ti_stream));
    if (stream) {

        stream->index = TI_INDICATOR_ATR_INDEX;
        stream->progress = -ti_atr_start(options);

        stream->period = period;

        stream->sum = 0.0;

    }
    return stream;
}


int ti_atr_stream_run(ti_stream *stream, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];

    TI_REAL *output = outputs[0];

    const TI_REAL per = 1.0 / ((TI_REAL)stream->period);

    const int start = -(stream->period-1);
    int i = 0; /* place in input */


    if (stream->progress < 1) {
        if (stream->progress == start) {
            /* first bar of input */
            stream->sum = high[0] - low[0];
            ++stream->progress; ++i;
        }

        /* still calculating first output */
        while (stream->progress <= 0 && i < size) {
            TI_REAL truerange; CALC_TRUERANGE();
            stream->sum += truerange;
            ++stream->progress; ++i;
        }

        if (stream->progress == 1) {
            const TI_REAL val = stream->sum * per;
            stream->last = val;
            *output++ = val;
        }
    }

    if (stream->progress >= 1) {
        /* steady state */
        TI_REAL val = stream->last;
        while (i < size) {
            TI_REAL truerange; CALC_TRUERANGE();
            val = (truerange-val) * per + val;
            *output++ = val;
            ++stream->progress; ++i;
        }

        stream->last = val;
    }

    return TI_OKAY;
}


void ti_atr_stream_free(ti_stream *stream) {
    free(stream);
}
