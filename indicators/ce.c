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
#include "truerange.h"

#include <stdio.h>

int ti_ce_start(TI_REAL const *options) {
    return (int)options[0]-1;
}

/* Name: Chandelier Exit
 * Sources:
 *  - Alexander Elder. Come Into My Trading Room, 2002, pp. 180-181. CE, original description
 *    ISBN: 9780471225348
 *  - J. Welles Wilder. New Concepts in Technical Trading Systems, 1978, pp. 21-23. ATR, original description
 *    ISBN: 9780894590276
 */

/*********** Plain implementation ************/

int ti_ce(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    TI_REAL const *high = inputs[0];
    TI_REAL const *low = inputs[1];
    TI_REAL const *close = inputs[2];
    const int period = options[0];
    const TI_REAL coef = options[1];
    TI_REAL *ce_high = outputs[0];
    TI_REAL *ce_low = outputs[1];

    if (size <= ti_ce_start(options)) return TI_OKAY;
    if (period < 1) return TI_INVALID_OPTION;

    int i;
    TI_REAL atr = high[0] - low[0];

    TI_REAL truerange; /* set within CALC_TRUERANGE macro */
    TI_REAL val;

    TI_REAL HP = high[0];
    int HP_idx = 0;
    TI_REAL LP = low[0];
    int LP_idx = 0;

    for (i = 1; i < period; ++i) {
        CALC_TRUERANGE();
        atr += truerange;

        if (HP <= (val = high[i])) {
            HP = val;
            HP_idx = i;
        }
        if (LP >= (val = low[i])) {
            LP = val;
            LP_idx = i;
        }
    }
    atr /= (double)period;

    /* performance reasons */
    const TI_REAL smth = (period - 1.) / period;
    const TI_REAL per = 1. / period;

    *ce_high++ = HP - coef * atr;
    *ce_low++ = LP + coef * atr;

    for (i = period; i < size; ++i) {
        CALC_TRUERANGE();
        atr = atr * smth + truerange * per;

        if (HP <= (val = high[i])) {
            HP = val;
            HP_idx = i;
        } else if (HP_idx == i - period) {
            HP = high[i-period+1];
            HP_idx = i-period+1;
            int j;
            for (j = i-period+2; j <= i; ++j) {
                if (HP <= (val = high[j])) {
                    HP = val;
                    HP_idx = j;
                }
            }
        }
        if (LP >= (val = low[i])) {
            LP = val;
            LP_idx = i;
        } else if (LP_idx == i - period) {
            LP = low[i-period+1];
            LP_idx = i-period+1;
            int j;
            for (j = i-period+2; j <= i; ++j) {
                if (LP >= (val = low[j])) {
                    LP = val;
                    LP_idx = j;
                }
            }
        }

        *ce_high++ = HP - coef * atr;
        *ce_low++ = LP + coef * atr;
    }

    return TI_OKAY;
}


/*********** Reference implementation ************/

int ti_ce_ref(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    TI_REAL const *high = inputs[0];
    TI_REAL const *low = inputs[1];
    TI_REAL const *close = inputs[2];
    const int period = options[0];
    const TI_REAL coef = options[1];
    TI_REAL *ce_high = outputs[0];
    TI_REAL *ce_low = outputs[1];

    if (size <= ti_ce_start(options)) { return TI_OKAY; }
    if (period < 1) { return TI_INVALID_OPTION; }

    #define MAX(a, b) (a > b ? a : b)
    #define MAX3(a, b, c) (a > b && a > c ? a : b > a && b > c ? b : c)

    TI_REAL *atr = malloc((size - ti_atr_start(options)) * sizeof(TI_REAL));
    ti_atr(size, inputs, options, &atr);
    TI_REAL *max = malloc((size - ti_max_start(options)) * sizeof(TI_REAL));
    ti_max(size, &high, options, &max);
    TI_REAL *min = malloc((size - ti_min_start(options)) * sizeof(TI_REAL));
    ti_min(size, &low, options, &min);


    for (int i = 0; i < size - period + 1; ++i) {
        TI_REAL HP = max[i];
        *ce_high++ = HP - coef * atr[i];

        TI_REAL LP = min[i];
        *ce_low++ = LP + coef * atr[i];
    }

    free(atr);
    free(max);
    free(min);

    return TI_OKAY;
}


/*********** Streaming implementation ************/

struct ti_ringbuffer_minmax {
    int size;
    int end_idx;
    int min_idx;
    int max_idx;
    TI_REAL min;
    TI_REAL max;
    TI_REAL buffer[][2]; // [i] = min, max
};

typedef struct ti_ringbuffer_minmax ti_ringbuffer_minmax;

struct ti_stream {
    int index;
    int progress;

    int period;
    TI_REAL coef;

    TI_REAL atr;

    TI_REAL prev_close;

    ti_ringbuffer_minmax LP_HP;
};

#define PUSH(RINGBUFFER, MIN, MAX) do { \
    int end_idx = ((RINGBUFFER).end_idx + 1) % (RINGBUFFER).size; \
    (RINGBUFFER).end_idx = end_idx; \
 \
    (RINGBUFFER).buffer[end_idx][1] = (MAX); \
    (RINGBUFFER).buffer[end_idx][0] = (MIN); \
    TI_REAL val; \
    if ((RINGBUFFER).max_idx == end_idx) { \
        (RINGBUFFER).max = (RINGBUFFER).buffer[0][1]; \
        (RINGBUFFER).max_idx = 0; \
        int j; \
        for (j = 1; j < (RINGBUFFER).size; ++j) { \
            if ((RINGBUFFER).max <= (val = (RINGBUFFER).buffer[j][1])) { \
                (RINGBUFFER).max = val; \
                (RINGBUFFER).max_idx = j; \
            } \
        } \
    } else if ((RINGBUFFER).max <= (MAX)) { \
        (RINGBUFFER).max = (MAX); \
        (RINGBUFFER).max_idx = end_idx; \
    } \
    if ((RINGBUFFER).min_idx == end_idx) { \
        (RINGBUFFER).min = (RINGBUFFER).buffer[0][0]; \
        (RINGBUFFER).min_idx = 0; \
        int j; \
        for (j = 1; j < (RINGBUFFER).size; ++j) { \
            if ((RINGBUFFER).min >= (val = (RINGBUFFER).buffer[j][0])) { \
                (RINGBUFFER).min = val; \
                (RINGBUFFER).min_idx = j; \
            } \
        } \
    } else if ((RINGBUFFER).min >= (MIN)) { \
        (RINGBUFFER).min = (MIN); \
        (RINGBUFFER).min_idx = end_idx; \
    } \
} while (0) \

#define GET(RINGBUFFER, MIN, MAX) do { \
    (MIN) = (RINGBUFFER).buffer[(RINGBUFFER).min_idx][0]; \
    (MAX) = (RINGBUFFER).buffer[(RINGBUFFER).max_idx][1]; \
} while (0) \

#define INIT(RINGBUFFER, PERIOD) do { \
    (RINGBUFFER).size = (PERIOD); \
    (RINGBUFFER).end_idx = -1; \
    (RINGBUFFER).min_idx = 0; \
    (RINGBUFFER).max_idx = 0; \
    for (int i = 0; i < (PERIOD); ++i) { \
        (RINGBUFFER).buffer[i][0] = 0; \
        (RINGBUFFER).buffer[i][1] = 0; \
    } \
} while (0) \

int ti_ce_stream_new(TI_REAL const *options, ti_stream **stream) {
    const int period = options[0];
    const TI_REAL coef = options[1];
    *stream = malloc(sizeof(ti_stream) + sizeof(TI_REAL[period][2]));
    if (!*stream) {
        return TI_OUT_OF_MEMORY;
    }

    (*stream)->index = TI_INDICATOR_CE_INDEX;
    (*stream)->progress = -ti_atr_start(options);

    (*stream)->period = period;
    (*stream)->coef = coef;

    INIT((*stream)->LP_HP, period);

    return TI_OKAY;
}

void ti_ce_stream_free(ti_stream *stream) {
    free(stream);
}

#undef CALC_TRUERANGE
#define CALC_TRUERANGE() do {\
        const TI_REAL l = low[i];\
        const TI_REAL h = high[i];\
        const TI_REAL c = prev_close;\
        const TI_REAL ych = fabs(h - c);\
        const TI_REAL ycl = fabs(l - c);\
        TI_REAL v = h - l;\
        if (ych > v) v = ych;\
        if (ycl > v) v = ycl;\
        truerange = v;\
} while (0)


int ti_ce_stream_run(ti_stream *stream, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    int progress = stream->progress;

    TI_REAL const *high = inputs[0];
    TI_REAL const *low = inputs[1];
    TI_REAL const *close = inputs[2];
    const int period = stream->period;
    const TI_REAL coef = stream->coef;
    TI_REAL *ce_high = outputs[0];
    TI_REAL *ce_low = outputs[1];

    TI_REAL atr;

    TI_REAL prev_close;

    ti_ringbuffer_minmax *LP_HP = &stream->LP_HP;

    const TI_REAL smth = (period - 1.) / period;
    const TI_REAL per = 1. / period;
    TI_REAL LP;
    TI_REAL HP;

    int i = 0;
    if (progress == -period+1) { // initialize
        atr = high[0] - low[0];

        PUSH(*LP_HP, low[0], high[0]);

        prev_close = close[0];

        ++i, ++progress;
    } else { // otherwise, load
        atr = stream->atr;

        prev_close = stream->prev_close;
    }
    for (i; progress < 1 && i < size; ++i, ++progress) { // warm up
        TI_REAL truerange; CALC_TRUERANGE();
        atr += truerange;

        PUSH(*LP_HP, low[i], high[i]);

        prev_close = close[i];
    }
    if (i > 0 && progress == 1) { // just warmed
        atr /= (double)period;
        GET(*LP_HP, LP, HP);
        *ce_high++ = HP - coef * atr;
        *ce_low++ = LP + coef * atr;
    }
    for (i; i < size; ++i, ++progress) { // continue in normal mode
        TI_REAL truerange; CALC_TRUERANGE();
        atr = atr * smth + truerange * per;

        PUSH(*LP_HP, low[i], high[i]);
        GET(*LP_HP, LP, HP);

        *ce_high++ = HP - coef * atr;
        *ce_low++ = LP + coef * atr;

        prev_close = close[i];
    }

    stream->progress = progress;
    stream->atr = atr;
    stream->prev_close = prev_close;


    return TI_OKAY;
}