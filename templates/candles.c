/*
 * Tulip Indicators
 * https://tulipindicators.org/
 * Copyright (c) 2010-2020 Tulip Charts LLC
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



/*VERSION*/



#ifndef TI_SKIP_SYSTEM_HEADERS
#include <math.h>
#include <stdlib.h>
#include <string.h>
#endif

#include "candles.h"



const char* tc_version(void) {return TC_VERSION;}
long int tc_build(void) {return TC_BUILD;}
int tc_candle_count(void) {return TC_CANDLE_COUNT;}



struct tc_result {
    int progress; /* Total number of bars read. */
    int space; /* Number of patterns available to store. */
    int count; /* Number of bars with patterns. */
    int pattern_count; /* Number of patterns found. */
    tc_hit *hits; /* Store actual found patterns. */
};


tc_config const *tc_config_default() {
    static tc_config default_config = {
        .period = 10,

        .body_none  = 0.05,
        .body_short = 0.5,
        .body_long  = 1.4,

        .wick_none  = 0.05,
        .wick_long  = 0.6,

        .near       = 0.3
    };

    return &default_config;
}


tc_candle_info tc_candles[] = {
/*INDEX*/
};


const tc_candle_info *tc_find_candle(const char *name) {
    int imin = 0;
    int imax = sizeof(tc_candles) / sizeof(tc_candle_info) - 2;

    /* Binary search. */
    while (imax >= imin) {
        const int i = (imin + ((imax-imin)/2));
        const int c = strcmp(name, tc_candles[i].name);
        if (c == 0) {
            return tc_candles + i;
        } else if (c > 0) {
            imin = i + 1;
        } else {
            imax = i - 1;
        }
    }

    return 0;
}


const tc_candle_info *tc_get_info(tc_set pattern) {
    /* Find lowest set bit. */
    if (!pattern) return 0;

    tc_set k = pattern & ~(pattern-1);

    int imin = 0;
    int imax = sizeof(tc_candles) / sizeof(tc_candle_info) - 2;

    /* Binary search. */
    while (imax >= imin) {
        const int i = (imin + ((imax-imin)/2));
        if (k == tc_candles[i].pattern) {
            return tc_candles + i;
        } else if (k > tc_candles[i].pattern) {
            imin = i + 1;
        } else {
            imax = i - 1;
        }
    }

    return 0;
}


void tc_config_set_to_default(tc_config *config) {
    memcpy(config, tc_config_default(), sizeof(tc_config));
}


tc_result *tc_result_new() {
    tc_result *k = malloc(sizeof(tc_result));
    if (!k) return 0;

    memset(k, 0, sizeof(tc_result));
    return k;
}


static void tc_result_reset(tc_result *result) {
    result->progress = 0;
    result->count = 0;
    result->pattern_count = 0;
}


void tc_result_free(tc_result *result) {
    if (!result) return;
    free(result->hits);
    free(result);
}


int tc_result_count(const tc_result *result) {
    return result->count;
}

int tc_result_pattern_count(const tc_result *result) {
    return result->pattern_count;
}


tc_hit tc_result_get(const tc_result *result, int index) {
    if (index < result->count) {
        return result->hits[index];
    }

    tc_hit hit = {0};
    return hit;
}


tc_set tc_result_at(const tc_result *result, int index) {
    int imin = 0;
    int imax = result->count;
    if (!imax) return 0;

    /* Binary search. */
    while (imax >= imin) {
        const int i = (imin + ((imax-imin)/2));
        if (index == result->hits[i].index) {
            return result->hits[i].patterns;
        } else if (index > result->hits[i].index) {
            imin = i + 1;
        } else {
            imax = i - 1;
        }
    }

    return 0;
}



static int tc_result_add(tc_result *result, tc_hit hit) {

    ++result->pattern_count;

    if (result->count > 0) {
        if (result->hits[result->count-1].index == hit.index) {
            TC_SET_ADD(result->hits[result->count-1].patterns, hit.patterns);
            return TC_OKAY;
        }
    }

    if (result->space == 0) {
        result->space = 2048;
        result->count = 0;
        result->hits = malloc(sizeof(tc_hit) * (unsigned int)result->space);

    } else if (result->space == result->count) {
        result->space *= 2;
        tc_hit *hits = realloc(result->hits, sizeof(tc_hit) * (unsigned int)result->space);
        if (!hits) {
            return TC_OUT_OF_MEMORY;
        }
        result->hits = hits;
    }

    result->hits[result->count] = hit;
    ++result->count;

    return TC_OKAY;
}


#define BODY(i) (fabs(open[(i)] - close[(i)]))
#define TOTAL(i) (high[(i)] - low[(i)])
#define TOP(i) (open[(i)] > close[(i)] ? open[(i)] : close[(i)])
#define BOTTOM(i) (open[(i)] < close[(i)] ? open[(i)] : close[(i)])
#define UPPER(i) (high[(i)] - top)
#define LOWER(i) (bottom - low[(i)])

#define OPT_BODY_NONE() (options->body_none * avg_total)
#define OPT_BODY_SHORT() (options->body_short * avg_body)
#define OPT_BODY_LONG() (options->body_long * avg_body)

#define OPT_WICK_NONE() (options->wick_none * avg_total)
#define OPT_WICK_LONG() (options->wick_long * avg_body)

#define OPT_NEAR() (options->near * avg_total)

#define BLACK(i) (open[i]>close[i])
#define WHITE(i) (open[i]<close[i])

#define BODY_NONE(i) (BODY(i) < OPT_BODY_NONE())
#define BODY_SHORT(i) (BODY(i) < OPT_BODY_SHORT())
#define BODY_LONG(i) (BODY(i) > OPT_BODY_LONG())

#define WICK_UPPER_NONE(i) (UPPER(i) < OPT_WICK_NONE())
#define WICK_UPPER_LONG(i) (UPPER(i) > OPT_WICK_LONG())
#define WICK_UPPER_LONGER_THAN_BODY(i) (UPPER(i) > BODY(i))

#define WICK_LOWER_NONE(i) (LOWER(i) < OPT_WICK_NONE())
#define WICK_LOWER_LONG(i) (LOWER(i) > OPT_WICK_LONG())
#define WICK_LOWER_LONGER_THAN_BODY(i) (LOWER(i) > BODY(i))

#define GAP_UP(i) (BOTTOM(i) >= TOP((i)-1))
#define GAP_DOWN(i) (TOP(i) <= BOTTOM((i)-1))
#define GAP(i) (GAP_UP(i) || GAP_DOWN(i))

#define STAR_UP(i) (low[i] >= high[(i)-1])
#define STAR_DOWN(i) (high[i] <= low[(i)-1])
#define STAR(i) (STAR_UP(i) || STAR_DOWN(i))

#define OPEN_INSIDE(i) (open[i] < TOP((i)-1) && open[i] > BOTTOM((i)-1))

#define HIT(what) do { \
const tc_hit hit = {i, what}; \
const int r = tc_result_add(output, hit); \
if (r != TC_OKAY) return r; \
} while (0)


#define SETUP \
    const TC_REAL *open = inputs[0]; \
    const TC_REAL *high = inputs[1]; \
    const TC_REAL *low = inputs[2]; \
    const TC_REAL *close = inputs[3]; \
 \
    const int period = options->period; \
    const TC_REAL div = 1.0 / period; \
 \
    TC_REAL avg_body_sum = 0; \
    TC_REAL avg_total_sum = 0; \
 \
    if (period < 1) return TC_INVALID_OPTION; \
    if (size < period) return TC_OKAY; \
 \
    int i; \
    for (i = 0; i < period; ++i) { \
        avg_body_sum += BODY(i); \
        avg_total_sum += TOTAL(i); \
    }


#define LOOP(x) \
    while(i < size) { \
        const TC_REAL top = TOP(i); \
        const TC_REAL bottom = BOTTOM(i); \
        const TC_REAL body = BODY(i); \
        const TC_REAL total = TOTAL(i); \
        const TC_REAL upper = UPPER(i); \
        const TC_REAL lower = LOWER(i); \
 \
        const TC_REAL avg_body = avg_body_sum * div; \
        const TC_REAL avg_total = avg_total_sum * div; \
 \
        (void)top;(void)bottom;(void)body;(void)total; \
        (void)upper;(void)lower;(void)avg_body;(void)avg_total;\
 \
 x \
 \
        avg_body_sum += body; \
        avg_body_sum -= BODY(i-period); \
 \
        avg_total_sum += total; \
        avg_total_sum -= TOTAL(i-period); \
 \
        ++i; \
    }

int tc_run(tc_set patterns, int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {

    tc_result_reset(output);

    {
        if ((patterns & (patterns-1)) == 0) {
            //Only one pattern set.
/*SHORT*/
        }
    }

    SETUP

    LOOP(/*PATTERNS*/);

    return TC_OKAY;
}


/*FUNCTIONS*/

