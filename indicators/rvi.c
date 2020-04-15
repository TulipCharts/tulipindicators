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
#include <stdio.h>

int ti_rvi_start(TI_REAL const *options) {
    const TI_REAL sma_period = options[0];
    const TI_REAL stddev_period = options[1];
    return stddev_period-1;
}


int ti_rvi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    TI_REAL const *real = inputs[0];
    const TI_REAL sma_period = options[0];
    const TI_REAL stddev_period = options[1];
    TI_REAL *rvi = outputs[0];

    if (sma_period < 1) { return TI_INVALID_OPTION; }
    if (stddev_period < 1) { return TI_INVALID_OPTION; }
    if (size <= ti_rvi_start(options)) { return TI_OKAY; }

    TI_REAL y_sum = 0.;
    TI_REAL xy_sum = 0.;

    const TI_REAL x_sum = stddev_period * (stddev_period + 1) / 2.;
    const TI_REAL xsq_sum = stddev_period * (stddev_period + 1) * (2*stddev_period + 1) / 6.;

    TI_REAL gains_ema = 0.;
    TI_REAL losses_ema = 0.;

    int i = 0;
    for (; i < stddev_period; ++i) {
        xy_sum += real[i] * (i + 1);
        y_sum += real[i];
    }
    {
        --i;

        // y = a + bx
        TI_REAL b = (xy_sum / stddev_period - x_sum / stddev_period * y_sum / stddev_period) / (xsq_sum / stddev_period - (x_sum / stddev_period)*(x_sum / stddev_period));
        TI_REAL a = y_sum / stddev_period - b * x_sum / stddev_period;

        TI_REAL higher = real[i] - (a + b * stddev_period);
        if (higher > 0) {
            gains_ema = higher * higher / stddev_period;
        } else {
            losses_ema = higher * higher / stddev_period;
        }

        if (gains_ema + losses_ema == 0) {
            *rvi++ = 50.;
        } else {
            *rvi++ = gains_ema / (gains_ema + losses_ema) * 100.;
        }

        ++i;
    }
    for (; i < size; ++i) {
        xy_sum += -y_sum + real[i]*stddev_period;
        y_sum += -real[i-(int)stddev_period] + real[i];

        // y = a + bx
        TI_REAL b = (xy_sum / stddev_period - x_sum / stddev_period * y_sum / stddev_period) / (xsq_sum / stddev_period - (x_sum / stddev_period)*(x_sum / stddev_period));
        TI_REAL a = y_sum / stddev_period - b * x_sum / stddev_period;

        TI_REAL higher = real[i] - (a + b * stddev_period);
        if (higher > 0) {
            gains_ema = (higher * higher / stddev_period - gains_ema) * 2. / (sma_period + 1) + gains_ema;
        } else {
            losses_ema = (higher * higher / stddev_period - losses_ema) * 2. / (sma_period + 1) + losses_ema;
        }

        if (gains_ema + losses_ema == 0) {
            *rvi++ = 50.;
        } else {
            *rvi++ = gains_ema / (gains_ema + losses_ema) * 100.;
        }
    }

    return TI_OKAY;
}

struct ti_stream {
    int index;
    int progress;

    struct {
        TI_REAL sma_period;
        TI_REAL stddev_period;
    } options;

    struct {
        TI_REAL y_sum;
        TI_REAL xy_sum;
        TI_REAL gains_ema;
        TI_REAL losses_ema;
    } state;

    struct {
        TI_REAL x_sum;
        TI_REAL xsq_sum;
    } constants;

    BUFFERS(
        BUFFER(price)
    )
};

int ti_rvi_stream_new(TI_REAL const *options, ti_stream **stream) {
    const TI_REAL sma_period = options[0];
    const TI_REAL stddev_period = options[1];
    if (sma_period < 1) { return TI_INVALID_OPTION; }
    if (stddev_period < 1) { return TI_INVALID_OPTION; }

    *stream = calloc(1, sizeof(**stream));
    if (!*stream) { return TI_OUT_OF_MEMORY; }

    (*stream)->index = TI_INDICATOR_RVI_INDEX;
    (*stream)->progress = -ti_rvi_start(options);

    (*stream)->options.sma_period = sma_period;
    (*stream)->options.stddev_period = stddev_period;

    (*stream)->state.y_sum = 0.;
    (*stream)->state.xy_sum = 0.;
    (*stream)->state.gains_ema = 0.;
    (*stream)->state.losses_ema = 0.;

    (*stream)->constants.x_sum = stddev_period * (stddev_period + 1) / 2.;
    (*stream)->constants.xsq_sum = stddev_period * (stddev_period + 1) * (2*stddev_period + 1) / 6.;

    BUFFER_INIT(*stream, price, stddev_period+1);

    *stream = realloc(*stream, sizeof(**stream) + sizeof(TI_REAL[BUFFERS_SIZE(*stream)]));
    if (!*stream) { return TI_OUT_OF_MEMORY; }

    return TI_OKAY;
}

void ti_rvi_stream_free(ti_stream *stream) {
    free(stream);
}

int ti_rvi_stream_run(ti_stream *stream, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    TI_REAL const *real = inputs[0];
    TI_REAL *rvi = outputs[0];

    int progress = stream->progress;

    TI_REAL sma_period = stream->options.sma_period;
    TI_REAL stddev_period = stream->options.stddev_period;

    TI_REAL y_sum = stream->state.y_sum;
    TI_REAL xy_sum = stream->state.xy_sum;
    TI_REAL gains_ema = stream->state.gains_ema;
    TI_REAL losses_ema = stream->state.losses_ema;

    TI_REAL x_sum = stream->constants.x_sum;
    TI_REAL xsq_sum = stream->constants.xsq_sum;

    int i = 0;
    TI_REAL var1;
    for (; i < size && progress <= 0; ++i, ++progress) {
        BUFFER_PUSH(stream, price, real[i]);

        xy_sum += real[i] * (progress - (-stddev_period+1) + 1);
        y_sum += real[i];
    }
    if (i > 0 && progress == 1) {
        --i;

        // y = a + bx
        TI_REAL b = (xy_sum / stddev_period - x_sum / stddev_period * y_sum / stddev_period) / (xsq_sum / stddev_period - (x_sum / stddev_period)*(x_sum / stddev_period));
        TI_REAL a = y_sum / stddev_period - b * x_sum / stddev_period;

        TI_REAL higher = real[i] - (a + b * stddev_period);
        if (higher > 0) {
            gains_ema = higher * higher / stddev_period;
        } else {
            losses_ema = higher * higher / stddev_period;
        }

        if (gains_ema + losses_ema == 0) {
            *rvi++ = 50.;
        } else {
            *rvi++ = gains_ema / (gains_ema + losses_ema) * 100.;
        }

        ++i;
    }
    for (; i < size; ++i, ++progress) {
        BUFFER_PUSH(stream, price, real[i]);

        xy_sum += -y_sum + real[i]*stddev_period;
        BUFFER_AT(var1, stream, price, -stddev_period);
        y_sum += -var1 + real[i];

        // y = a + bx
        TI_REAL b = (xy_sum / stddev_period - x_sum / stddev_period * y_sum / stddev_period) / (xsq_sum / stddev_period - (x_sum / stddev_period)*(x_sum / stddev_period));
        TI_REAL a = y_sum / stddev_period - b * x_sum / stddev_period;

        TI_REAL higher = real[i] - (a + b * stddev_period);
        if (higher > 0) {
            gains_ema = (higher * higher / stddev_period - gains_ema) * 2. / (sma_period + 1) + gains_ema;
        } else {
            losses_ema = (higher * higher / stddev_period - losses_ema) * 2. / (sma_period + 1) + losses_ema;
        }

        if (gains_ema + losses_ema == 0) {
            *rvi++ = 50.;
        } else {
            *rvi++ = gains_ema / (gains_ema + losses_ema) * 100.;
        }
    }

    stream->progress = progress;

    stream->state.y_sum = y_sum;
    stream->state.xy_sum = xy_sum;
    stream->state.gains_ema = gains_ema;
    stream->state.losses_ema = losses_ema;

    stream->constants.x_sum = x_sum;
    stream->constants.xsq_sum = xsq_sum;

    return TI_OKAY;
}