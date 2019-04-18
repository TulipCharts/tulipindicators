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
#include "../utils/minmax.h"

int ti_mama_start(TI_REAL const *options) {
    return 6;
}


int ti_mama(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    TI_REAL const *real = inputs[0];
    const TI_REAL fastlimit = options[0];
    const TI_REAL slowlimit = options[1];

    for (int i = 0; i < 2; ++i) {
        if (options[i] < 0. || options[i] > 1.) {
            return TI_INVALID_OPTION;
        }
    }

    TI_REAL *mama = outputs[0];
    TI_REAL *fama = outputs[1];

    TI_REAL const *price = real;

    struct data {
        BUFFERS(
            BUFFER(smooth)
            BUFFER(detrender)
            BUFFER(I1)
            BUFFER(Q1)
            BUFFER(jI)
            BUFFER(jQ)
            BUFFER(I2)
            BUFFER(Q2)
            BUFFER(Re)
            BUFFER(Im)
            BUFFER(period)
            BUFFER(smoothperiod)
            BUFFER(phase)
            BUFFER(deltaphase)
            BUFFER(alpha)
            BUFFER(mama)
            BUFFER(fama)
        )
    } *data;
    data = calloc(1, sizeof(struct data));
    BUFFER_INIT(data, smooth, 7)
    BUFFER_INIT(data, detrender, 7)
    BUFFER_INIT(data, I1, 7)
    BUFFER_INIT(data, Q1, 7)
    BUFFER_INIT(data, jI, 1)
    BUFFER_INIT(data, jQ, 1)
    BUFFER_INIT(data, I2, 2)
    BUFFER_INIT(data, Q2, 2)
    BUFFER_INIT(data, Re, 2)
    BUFFER_INIT(data, Im, 2)
    BUFFER_INIT(data, period, 2)
    BUFFER_INIT(data, smoothperiod, 2)
    BUFFER_INIT(data, phase, 2)
    BUFFER_INIT(data, deltaphase, 1)
    BUFFER_INIT(data, alpha, 1)
    BUFFER_INIT(data, mama, 1)
    BUFFER_INIT(data, fama, 1)
    data = realloc(data, sizeof(struct data) + sizeof(TI_REAL[BUFFERS_SIZE(data)]));

    for (int i = 0; i < 7; ++i) {
        BUFFER_PUSH(data, smooth, 0);
        BUFFER_PUSH(data, detrender, 0);
        BUFFER_PUSH(data, I1, 0);
        BUFFER_PUSH(data, Q1, 0);
        BUFFER_PUSH(data, jI, 0);
        BUFFER_PUSH(data, jQ, 0);
        BUFFER_PUSH(data, I2, 0);
        BUFFER_PUSH(data, Q2, 0);
        BUFFER_PUSH(data, Re, 0);
        BUFFER_PUSH(data, Im, 0);
        BUFFER_PUSH(data, period, 0);
        BUFFER_PUSH(data, smoothperiod, 0);
        BUFFER_PUSH(data, phase, 0);
        BUFFER_PUSH(data, deltaphase, 0);
        BUFFER_PUSH(data, alpha, 0);
        BUFFER_PUSH(data, mama, 0);
        BUFFER_PUSH(data, fama, 0);
    }

    TI_REAL var1, var2, var3, var4, var5, var6;
    for (int i = 6; i < size; ++i) {
        BUFFER_PUSH(data, smooth, (4 * price[i] + 3 * price[i-1] + 2 * price[i-2] + price[i-3]) / 10.);

        BUFFER_AT(var1, data, smooth, 0);
        BUFFER_AT(var2, data, smooth, -2);
        BUFFER_AT(var3, data, smooth, -4);
        BUFFER_AT(var4, data, smooth, -6);
        BUFFER_AT(var5, data, period, 0);
        BUFFER_PUSH(data, detrender, (.0962*var1 + .5769*var2 - .5769*var3 - .0962*var4) * (.075*var5 + .54));

        BUFFER_AT(var1, data, detrender, 0);
        BUFFER_AT(var2, data, detrender, -2);
        BUFFER_AT(var3, data, detrender, -4);
        BUFFER_AT(var4, data, detrender, -6);
        BUFFER_AT(var5, data, period, 0);
        BUFFER_PUSH(data, Q1, (.0962*var1 + .5769*var2 - .5769*var3 - .0962*var4) * (.075*var5 + .54));

        BUFFER_AT(var1, data, detrender, -3);
        BUFFER_PUSH(data, I1, var1);

        BUFFER_AT(var1, data, I1, 0);
        BUFFER_AT(var2, data, I1, -2);
        BUFFER_AT(var3, data, I1, -4);
        BUFFER_AT(var4, data, I1, -6);
        BUFFER_AT(var5, data, period, 0);
        BUFFER_PUSH(data, jI, (.0962*var1 + .5769*var2 - .5769*var3 - .0962*var4) * (.075 * var5 + 0.54));

        BUFFER_AT(var1, data, Q1, 0);
        BUFFER_AT(var2, data, Q1, -2);
        BUFFER_AT(var3, data, Q1, -4);
        BUFFER_AT(var4, data, Q1, -6);
        BUFFER_AT(var5, data, period, 0);
        BUFFER_PUSH(data, jQ, (.0962*var1 + .5769*var2 - .5769*var3 - .0962*var4) * (.075 * var5 + 0.54));

        BUFFER_AT(var1, data, I1, 0);
        BUFFER_AT(var2, data, jQ, 0);
        BUFFER_AT(var3, data, I2, 0);
        BUFFER_PUSH(data, I2, .2*(var1 - var2) + .8*var3);

        BUFFER_AT(var1, data, Q1, 0);
        BUFFER_AT(var2, data, jI, 0);
        BUFFER_AT(var3, data, Q2, 0);
        BUFFER_PUSH(data, Q2, .2*(var1 + var2) + .8*var3);

        BUFFER_AT(var1, data, I2, 0);
        BUFFER_AT(var2, data, I2, -1);
        BUFFER_AT(var3, data, Q2, 0);
        BUFFER_AT(var4, data, Q2, -1);
        BUFFER_AT(var5, data, Re, 0);
        BUFFER_PUSH(data, Re, .2*(var1*var2 + var3*var4) + .8*var5);

        BUFFER_AT(var1, data, I2, 0);
        BUFFER_AT(var2, data, Q2, -1);
        BUFFER_AT(var3, data, Q2, 0);
        BUFFER_AT(var4, data, I2, -1);
        BUFFER_AT(var5, data, Im, 0);
        BUFFER_PUSH(data, Im, .2*(var1*var2 - var3*var4) + .8*var5);

        TI_REAL period_ = 0.;
        BUFFER_AT(var1, data, Im, 0);
        BUFFER_AT(var2, data, Re, 0);
        BUFFER_AT(var3, data, period, 0);
        if (var1 != 0. && var2 != 0.) {
            period_ = 360. / atan(var1 / var2);
        }
        if (period_ > 1.5 * var3) {
            period_ = 1.5 * var3;
        }
        if (period_ < 0.67 * var3) {
            period_ = 0.67 * var3;
        }
        if (period_ < 6.) {
            period_ = 6.;
        }
        if (period_ > 50.) {
            period_ = 50.;
        }
        BUFFER_PUSH(data, period, .2*period_ + .8*var3);

        BUFFER_AT(var1, data, period, 0);
        BUFFER_AT(var2, data, smoothperiod, 0);
        BUFFER_PUSH(data, smoothperiod, .33*var1 + .67*var2);

        BUFFER_AT(var1, data, I1, 0);
        if (var1 != 0.) {
            BUFFER_AT(var2, data, Q1, 0);
            BUFFER_PUSH(data, phase, atan(var2 / var1));
        } else {
            BUFFER_PUSH(data, phase, 0);
        }

        BUFFER_AT(var1, data, phase, -1);
        BUFFER_AT(var2, data, phase, 0);
        BUFFER_PUSH(data, deltaphase, MAX(1, var1 - var2));

        BUFFER_AT(var1, data, deltaphase, 0);
        BUFFER_PUSH(data, alpha, MAX(slowlimit, fastlimit / var1));

        BUFFER_AT(var1, data, alpha, 0);
        BUFFER_AT(var2, data, mama, 0);
        BUFFER_PUSH(data, mama, var1 * price[i] + (1. - var1) * var2);

        BUFFER_AT(var1, data, alpha, 0);
        BUFFER_AT(var2, data, mama, 0);
        BUFFER_AT(var3, data, fama, 0);
        BUFFER_PUSH(data, fama, .5 * var1 * var2 + (1. - .5 * var1) * var3);

        BUFFER_AT(var1, data, mama, 0);
        *mama++ = var1;
        BUFFER_AT(var1, data, fama, 0);
        *fama++ = var1;
    }

    free(data);

    return TI_OKAY;
}

int ti_mama_ref(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    TI_REAL const *real = inputs[0];
    const TI_REAL fastlimit = options[0];
    const TI_REAL slowlimit = options[1];
    TI_REAL *mama = outputs[0];
    TI_REAL *fama = outputs[1];

    for (int i = 0; i < 2; ++i) {
        if (options[i] < 0. || options[i] > 1.) {
            return TI_INVALID_OPTION;
        }
    }

    // Straightforward translation of the original definition in EasyLanguage
    TI_REAL const *price = real;

    TI_REAL *smooth = calloc(size, sizeof(TI_REAL));
    TI_REAL *detrender = calloc(size, sizeof(TI_REAL));
    TI_REAL *I1 = calloc(size, sizeof(TI_REAL));
    TI_REAL *Q1 = calloc(size, sizeof(TI_REAL));
    TI_REAL *jI = calloc(size, sizeof(TI_REAL));
    TI_REAL *jQ = calloc(size, sizeof(TI_REAL));
    TI_REAL *I2 = calloc(size, sizeof(TI_REAL));
    TI_REAL *Q2 = calloc(size, sizeof(TI_REAL));
    TI_REAL *Re = calloc(size, sizeof(TI_REAL));
    TI_REAL *Im = calloc(size, sizeof(TI_REAL));
    TI_REAL *period = calloc(size, sizeof(TI_REAL));
    TI_REAL *smoothperiod = calloc(size, sizeof(TI_REAL));
    TI_REAL *phase = calloc(size, sizeof(TI_REAL));
    TI_REAL *deltaphase = calloc(size, sizeof(TI_REAL));
    TI_REAL *alpha = calloc(size, sizeof(TI_REAL));

    for (int i = 6; i < size; ++i) {
        smooth[i] = (4 * price[i] + 3 * price[i-1] + 2 * price[i-2] + price[i-3]) / 10.;
        detrender[i] = (.0962*smooth[i] + .5769*smooth[i-2] - .5769*smooth[i-4] - .0962*smooth[i-6]) * (.075*period[i-1] + .54);

        Q1[i] = (.0962*detrender[i] + .5769*detrender[i-2] - .5769*detrender[i-4] - .0962*detrender[i-6]) * (.075*period[i-1] + .54);
        I1[i] = detrender[i-3];

        jI[i] = (.0962*I1[i] + .5769*I1[i-2] - .5769*I1[i-4] - .0962*I1[i-6]) * (.075 * period[i-1] + .54);
        jQ[i] = (.0962*Q1[i] + .5769*Q1[i-2] - .5769*Q1[i-4] - .0962*Q1[i-6]) * (.075 * period[i-1] + .54);

        I2[i] = I1[i] - jQ[i];
        Q2[i] = Q1[i] + jI[i];

        I2[i] = .2*I2[i] + .8*I2[i-1];
        Q2[i] = .2*Q2[i] + .8*Q2[i-1];

        Re[i] = I2[i] * I2[i-1] + Q2[i] * Q2[i-1];
        Im[i] = I2[i] * Q2[i-1] - Q2[i] * I2[i-1];
        Re[i] = .2*Re[i] + .8*Re[i-1];
        Im[i] = .2*Im[i] + .8*Im[i-1];

        if (Im[i] != 0. && Re[i] != 0.) {
            period[i] = 360. / atan(Im[i] / Re[i]);
        }
        if (period[i] > 1.5 * period[i-1]) {
            period[i] = 1.5 * period[i-1];
        }
        if (period[i] < .67 * period[i-1]) {
            period[i] = .67 * period[i-1];
        }
        if (period[i] < 6.) {
            period[i] = 6.;
        }
        if (period[i] > 50.) {
            period[i] = 50.;
        }
        period[i] = .2 * period[i] + .8 * period[i-1];
        smoothperiod[i] = .33 * period[i] + .67 * smoothperiod[i-1];
        if (I1[i] != 0.) {
            phase[i] = atan(Q1[i] / I1[i]);
        }
        deltaphase[i] = phase[i-1] - phase[i];
        if (deltaphase[i] < 1.) {
            deltaphase[i] = 1.;
        }
        alpha[i] = fastlimit / deltaphase[i];
        if (alpha[i] < slowlimit) { alpha[i] = slowlimit; }
        *mama++ = alpha[i] * price[i] + (1. - alpha[i]) * (i > 6 ? mama[-1] : 0);
        *fama++ = .5 * alpha[i] * mama[-1] + (1. - .5 * alpha[i]) * (i > 6 ? fama[-1] : 0);
    }

    free(smooth);
    free(detrender);
    free(I1);
    free(Q1);
    free(jI);
    free(jQ);
    free(I2);
    free(Q2);
    free(Re);
    free(Im);
    free(period);
    free(smoothperiod);
    free(phase);
    free(deltaphase);
    free(alpha);

    return TI_OKAY;
}

struct ti_stream {
    int index;
    int progress;

    TI_REAL options[2];

    BUFFERS(
        BUFFER(price)
        BUFFER(smooth)
        BUFFER(detrender)
        BUFFER(I1)
        BUFFER(Q1)
        BUFFER(jI)
        BUFFER(jQ)
        BUFFER(I2)
        BUFFER(Q2)
        BUFFER(Re)
        BUFFER(Im)
        BUFFER(period)
        BUFFER(smoothperiod)
        BUFFER(phase)
        BUFFER(deltaphase)
        BUFFER(alpha)
        BUFFER(mama)
        BUFFER(fama)
    );
};

int ti_mama_stream_new(TI_REAL const *options, ti_stream **stream) {
    for (int i = 0; i < 2; ++i) {
        if (options[i] < 0. || options[i] > 1.) {
            return TI_INVALID_OPTION;
        }
    }

    *stream = calloc(1, sizeof(ti_stream));
    if (!*stream) {
        return TI_OUT_OF_MEMORY;
    }

    (*stream)->index = TI_INDICATOR_MAMA_INDEX;
    (*stream)->progress = -6;


    memcpy((*stream)->options, options, sizeof(TI_REAL[2]));

    BUFFER_INIT(*stream, price, 4)
    BUFFER_INIT(*stream, smooth, 7)
    BUFFER_INIT(*stream, detrender, 7)
    BUFFER_INIT(*stream, I1, 7)
    BUFFER_INIT(*stream, Q1, 7)
    BUFFER_INIT(*stream, jI, 1)
    BUFFER_INIT(*stream, jQ, 1)
    BUFFER_INIT(*stream, I2, 2)
    BUFFER_INIT(*stream, Q2, 2)
    BUFFER_INIT(*stream, Re, 2)
    BUFFER_INIT(*stream, Im, 2)
    BUFFER_INIT(*stream, period, 2)
    BUFFER_INIT(*stream, smoothperiod, 2)
    BUFFER_INIT(*stream, phase, 2)
    BUFFER_INIT(*stream, deltaphase, 1)
    BUFFER_INIT(*stream, alpha, 1)
    BUFFER_INIT(*stream, mama, 1)
    BUFFER_INIT(*stream, fama, 1)

    *stream = realloc(*stream, sizeof(struct ti_stream) + sizeof(TI_REAL[BUFFERS_SIZE(*stream)]));
    if (!*stream) {
        return TI_OUT_OF_MEMORY;
    }

    return TI_OKAY;
}


int ti_mama_stream_run(ti_stream *stream, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    int progress = stream->progress;

    TI_REAL const *real = inputs[0];
    const TI_REAL fastlimit = stream->options[0];
    const TI_REAL slowlimit = stream->options[1];
    TI_REAL *mama = outputs[0];
    TI_REAL *fama = outputs[1];

    TI_REAL const *price = real;

    int i = 0;
    if (progress == -6) {}
    for (i; i < size && progress < 0; ++i, ++progress) {
        BUFFER_PUSH(stream, price, price[i]);
        BUFFER_PUSH(stream, smooth, 0);
        BUFFER_PUSH(stream, detrender, 0);
        BUFFER_PUSH(stream, I1, 0);
        BUFFER_PUSH(stream, Q1, 0);
        BUFFER_PUSH(stream, jI, 0);
        BUFFER_PUSH(stream, jQ, 0);
        BUFFER_PUSH(stream, I2, 0);
        BUFFER_PUSH(stream, Q2, 0);
        BUFFER_PUSH(stream, Re, 0);
        BUFFER_PUSH(stream, Im, 0);
        BUFFER_PUSH(stream, period, 0);
        BUFFER_PUSH(stream, smoothperiod, 0);
        BUFFER_PUSH(stream, phase, 0);
        BUFFER_PUSH(stream, deltaphase, 0);
        BUFFER_PUSH(stream, alpha, 0);
        BUFFER_PUSH(stream, mama, 0);
        BUFFER_PUSH(stream, fama, 0);
    }
    for (i; i < size; ++i, ++progress) {
        TI_REAL var1, var2, var3, var4, var5, var6;

        BUFFER_PUSH(stream, price, price[i]);

        BUFFER_AT(var1, stream, price, 0);
        BUFFER_AT(var2, stream, price, -1);
        BUFFER_AT(var3, stream, price, -2);
        BUFFER_AT(var4, stream, price, -3);
        BUFFER_PUSH(stream, smooth, (4 * var1 + 3 * var2 + 2 * var3 + var4) / 10.);

        BUFFER_AT(var1, stream, smooth, 0);
        BUFFER_AT(var2, stream, smooth, -2);
        BUFFER_AT(var3, stream, smooth, -4);
        BUFFER_AT(var4, stream, smooth, -6);
        BUFFER_AT(var5, stream, period, 0);
        BUFFER_PUSH(stream, detrender, (.0962*var1 + .5769*var2 - .5769*var3 - .0962*var4) * (.075*var5 + .54));

        BUFFER_AT(var1, stream, detrender, 0);
        BUFFER_AT(var2, stream, detrender, -2);
        BUFFER_AT(var3, stream, detrender, -4);
        BUFFER_AT(var4, stream, detrender, -6);
        BUFFER_AT(var5, stream, period, 0);
        BUFFER_PUSH(stream, Q1, (.0962*var1 + .5769*var2 - .5769*var3 - .0962*var4) * (.075*var5 + .54));

        BUFFER_AT(var1, stream, detrender, -3);
        BUFFER_PUSH(stream, I1, var1);

        BUFFER_AT(var1, stream, I1, 0);
        BUFFER_AT(var2, stream, I1, -2);
        BUFFER_AT(var3, stream, I1, -4);
        BUFFER_AT(var4, stream, I1, -6);
        BUFFER_AT(var5, stream, period, 0);
        BUFFER_PUSH(stream, jI, (.0962*var1 + .5769*var2 - .5769*var3 - .0962*var4) * (.075 * var5 + 0.54));

        BUFFER_AT(var1, stream, Q1, 0);
        BUFFER_AT(var2, stream, Q1, -2);
        BUFFER_AT(var3, stream, Q1, -4);
        BUFFER_AT(var4, stream, Q1, -6);
        BUFFER_AT(var5, stream, period, 0);
        BUFFER_PUSH(stream, jQ, (.0962*var1 + .5769*var2 - .5769*var3 - .0962*var4) * (.075 * var5 + 0.54));

        BUFFER_AT(var1, stream, I1, 0);
        BUFFER_AT(var2, stream, jQ, 0);
        BUFFER_AT(var3, stream, I2, 0);
        BUFFER_PUSH(stream, I2, .2*(var1 - var2) + .8*var3);

        BUFFER_AT(var1, stream, Q1, 0);
        BUFFER_AT(var2, stream, jI, 0);
        BUFFER_AT(var3, stream, Q2, 0);
        BUFFER_PUSH(stream, Q2, .2*(var1 + var2) + .8*var3);

        BUFFER_AT(var1, stream, I2, 0);
        BUFFER_AT(var2, stream, I2, -1);
        BUFFER_AT(var3, stream, Q2, 0);
        BUFFER_AT(var4, stream, Q2, -1);
        BUFFER_AT(var5, stream, Re, 0);
        BUFFER_PUSH(stream, Re, .2*(var1*var2 + var3*var4) + .8*var5);

        BUFFER_AT(var1, stream, I2, 0);
        BUFFER_AT(var2, stream, Q2, -1);
        BUFFER_AT(var3, stream, Q2, 0);
        BUFFER_AT(var4, stream, I2, -1);
        BUFFER_AT(var5, stream, Im, 0);
        BUFFER_PUSH(stream, Im, .2*(var1*var2 - var3*var4) + .8*var5);

        TI_REAL period_ = 0.;
        BUFFER_AT(var1, stream, Im, 0);
        BUFFER_AT(var2, stream, Re, 0);
        BUFFER_AT(var3, stream, period, 0);
        if (var1 != 0. && var2 != 0.) {
            period_ = 360. / atan(var1 / var2);
        }
        if (period_ > 1.5 * var3) {
            period_ = 1.5 * var3;
        }
        if (period_ < 0.67 * var3) {
            period_ = 0.67 * var3;
        }
        if (period_ < 6.) {
            period_ = 6.;
        }
        if (period_ > 50.) {
            period_ = 50.;
        }
        BUFFER_PUSH(stream, period, .2*period_ + .8*var3);

        BUFFER_AT(var1, stream, period, 0);
        BUFFER_AT(var2, stream, smoothperiod, 0);
        BUFFER_PUSH(stream, smoothperiod, .33*var1 + .67*var2);

        BUFFER_AT(var1, stream, I1, 0);
        if (var1 != 0.) {
            BUFFER_AT(var2, stream, Q1, 0);
            BUFFER_PUSH(stream, phase, atan(var2 / var1));
        } else {
            BUFFER_PUSH(stream, phase, 0);
        }

        BUFFER_AT(var1, stream, phase, -1);
        BUFFER_AT(var2, stream, phase, 0);
        BUFFER_PUSH(stream, deltaphase, MAX(1, var1 - var2));

        BUFFER_AT(var1, stream, deltaphase, 0);
        BUFFER_PUSH(stream, alpha, MAX(slowlimit, fastlimit / var1));

        BUFFER_AT(var1, stream, alpha, 0);
        BUFFER_AT(var2, stream, mama, 0);
        BUFFER_AT(var3, stream, price, 0);
        BUFFER_PUSH(stream, mama, var1 * var3 + (1. - var1) * var2);

        BUFFER_AT(var1, stream, alpha, 0);
        BUFFER_AT(var2, stream, mama, 0);
        BUFFER_AT(var3, stream, fama, 0);
        BUFFER_PUSH(stream, fama, .5 * var1 * var2 + (1. - .5 * var1) * var3);

        BUFFER_AT(var1, stream, mama, 0);
        *mama++ = var1;
        BUFFER_AT(var1, stream, fama, 0);
        *fama++ = var1;

    }

    stream->progress = progress;

    return TI_OKAY;
}


void ti_mama_stream_free(ti_stream *stream) {
    free(stream);
}
