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




/*
 *
 * Version 0.9.2
 * Header Build 1645649572
 *
 */

/*
 *
 * This file is generated. Do not modify directly.
 *
 */




#ifndef TI_SKIP_SYSTEM_HEADERS
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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
{"abandoned_baby_bear", "Abandoned Baby Bear", TC_ABANDONED_BABY_BEAR, tc_abandoned_baby_bear},
{"abandoned_baby_bull", "Abandoned Baby Bull", TC_ABANDONED_BABY_BULL, tc_abandoned_baby_bull},
{"big_black_candle", "Big Black Candle", TC_BIG_BLACK_CANDLE, tc_big_black_candle},
{"big_white_candle", "Big White Candle", TC_BIG_WHITE_CANDLE, tc_big_white_candle},
{"black_marubozu", "Black Marubozu", TC_BLACK_MARUBOZU, tc_black_marubozu},
{"doji", "Doji", TC_DOJI, tc_doji},
{"dragonfly_doji", "Dragonfly Doji", TC_DRAGONFLY_DOJI, tc_dragonfly_doji},
{"engulfing_bear", "Engulfing Bear", TC_ENGULFING_BEAR, tc_engulfing_bear},
{"engulfing_bull", "Engulfing Bull", TC_ENGULFING_BULL, tc_engulfing_bull},
{"evening_doji_star", "Evening Doji Star", TC_EVENING_DOJI_STAR, tc_evening_doji_star},
{"evening_star", "Evening Star", TC_EVENING_STAR, tc_evening_star},
{"four_price_doji", "Four Price Doji", TC_FOUR_PRICE_DOJI, tc_four_price_doji},
{"gravestone_doji", "Gravestone Doji", TC_GRAVESTONE_DOJI, tc_gravestone_doji},
{"hammer", "Hammer", TC_HAMMER, tc_hammer},
{"hanging_man", "Hanging Man", TC_HANGING_MAN, tc_hanging_man},
{"inverted_hammer", "Inverted Hammer", TC_INVERTED_HAMMER, tc_inverted_hammer},
{"long_legged_doji", "Long Legged Doji", TC_LONG_LEGGED_DOJI, tc_long_legged_doji},
{"marubozu", "Marubozu", TC_MARUBOZU, tc_marubozu},
{"morning_doji_star", "Morning Doji Star", TC_MORNING_DOJI_STAR, tc_morning_doji_star},
{"morning_star", "Morning Star", TC_MORNING_STAR, tc_morning_star},
{"shooting_star", "Shooting Star", TC_SHOOTING_STAR, tc_shooting_star},
{"spinning_top", "Spinning Top", TC_SPINNING_TOP, tc_spinning_top},
{"star", "Star", TC_STAR, tc_star},
{"three_black_crows", "Three Black Crows", TC_THREE_BLACK_CROWS, tc_three_black_crows},
{"three_white_soldiers", "Three White Soldiers", TC_THREE_WHITE_SOLDIERS, tc_three_white_soldiers},
{"white_marubozu", "White Marubozu", TC_WHITE_MARUBOZU, tc_white_marubozu},
{0,0,0,0}
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
    int imax = result->count - 1;
    if (!imax) return 0;

    /* Binary search. */
    while (imax >= imin) {
        const int i = (imin + ((imax-imin)/2));
        assert(i >= 0);
        assert(i < result->count);
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
if (patterns == TC_ABANDONED_BABY_BEAR) return tc_abandoned_baby_bear(size, inputs, options, output);
if (patterns == TC_ABANDONED_BABY_BULL) return tc_abandoned_baby_bull(size, inputs, options, output);
if (patterns == TC_BIG_BLACK_CANDLE) return tc_big_black_candle(size, inputs, options, output);
if (patterns == TC_BIG_WHITE_CANDLE) return tc_big_white_candle(size, inputs, options, output);
if (patterns == TC_BLACK_MARUBOZU) return tc_black_marubozu(size, inputs, options, output);
if (patterns == TC_DOJI) return tc_doji(size, inputs, options, output);
if (patterns == TC_DRAGONFLY_DOJI) return tc_dragonfly_doji(size, inputs, options, output);
if (patterns == TC_ENGULFING_BEAR) return tc_engulfing_bear(size, inputs, options, output);
if (patterns == TC_ENGULFING_BULL) return tc_engulfing_bull(size, inputs, options, output);
if (patterns == TC_EVENING_DOJI_STAR) return tc_evening_doji_star(size, inputs, options, output);
if (patterns == TC_EVENING_STAR) return tc_evening_star(size, inputs, options, output);
if (patterns == TC_FOUR_PRICE_DOJI) return tc_four_price_doji(size, inputs, options, output);
if (patterns == TC_GRAVESTONE_DOJI) return tc_gravestone_doji(size, inputs, options, output);
if (patterns == TC_HAMMER) return tc_hammer(size, inputs, options, output);
if (patterns == TC_HANGING_MAN) return tc_hanging_man(size, inputs, options, output);
if (patterns == TC_INVERTED_HAMMER) return tc_inverted_hammer(size, inputs, options, output);
if (patterns == TC_LONG_LEGGED_DOJI) return tc_long_legged_doji(size, inputs, options, output);
if (patterns == TC_MARUBOZU) return tc_marubozu(size, inputs, options, output);
if (patterns == TC_MORNING_DOJI_STAR) return tc_morning_doji_star(size, inputs, options, output);
if (patterns == TC_MORNING_STAR) return tc_morning_star(size, inputs, options, output);
if (patterns == TC_SHOOTING_STAR) return tc_shooting_star(size, inputs, options, output);
if (patterns == TC_SPINNING_TOP) return tc_spinning_top(size, inputs, options, output);
if (patterns == TC_STAR) return tc_star(size, inputs, options, output);
if (patterns == TC_THREE_BLACK_CROWS) return tc_three_black_crows(size, inputs, options, output);
if (patterns == TC_THREE_WHITE_SOLDIERS) return tc_three_white_soldiers(size, inputs, options, output);
if (patterns == TC_WHITE_MARUBOZU) return tc_white_marubozu(size, inputs, options, output);

        }
    }

    SETUP

    LOOP(
    /* Abandoned Baby Bear */
    if ((TC_ABANDONED_BABY_BEAR & patterns) && i>=2 && WHITE(i-2) && BODY_LONG(i-2) && STAR_UP(i-1) && BODY_NONE(i-1) && BLACK(i) && STAR_DOWN(i) && (close[i] <= close[i-2])) {
        HIT(TC_ABANDONED_BABY_BEAR);
    }

    /* Abandoned Baby Bull */
    if ((TC_ABANDONED_BABY_BULL & patterns) && i>=2 && BLACK(i-2) && BODY_LONG(i-2) && STAR_DOWN(i-1) && BODY_NONE(i-1) && WHITE(i) && STAR_UP(i) && (close[i] >= close[i-2])) {
        HIT(TC_ABANDONED_BABY_BULL);
    }

    /* Big Black Candle */
    if ((TC_BIG_BLACK_CANDLE & patterns) && BLACK(i) && BODY_LONG(i)) {
        HIT(TC_BIG_BLACK_CANDLE);
    }

    /* Big White Candle */
    if ((TC_BIG_WHITE_CANDLE & patterns) && WHITE(i) && BODY_LONG(i)) {
        HIT(TC_BIG_WHITE_CANDLE);
    }

    /* Black Marubozu */
    if ((TC_BLACK_MARUBOZU & patterns) && BODY_LONG(i) && WICK_UPPER_NONE(i) && WICK_LOWER_NONE(i) && BLACK(i)) {
        HIT(TC_BLACK_MARUBOZU);
    }

    /* Doji */
    if ((TC_DOJI & patterns) && BODY_NONE(i)) {
        HIT(TC_DOJI);
    }

    /* Dragonfly Doji */
    if ((TC_DRAGONFLY_DOJI & patterns) && BODY_NONE(i) && WICK_UPPER_NONE(i) && WICK_LOWER_LONG(i)) {
        HIT(TC_DRAGONFLY_DOJI);
    }

    /* Engulfing Bear */
    if ((TC_ENGULFING_BEAR & patterns) && i>=1 && BLACK(i) && WHITE(i-1) && close[i] < open[i-1] && open[i] > close[i-1]) {
        HIT(TC_ENGULFING_BEAR);
    }

    /* Engulfing Bull */
    if ((TC_ENGULFING_BULL & patterns) && i>=1 && WHITE(i) && BLACK(i-1) && close[i] > open[i-1] && open[i] < close[i-1]) {
        HIT(TC_ENGULFING_BULL);
    }

    /* Evening Doji Star */
    if ((TC_EVENING_DOJI_STAR & patterns) && i>=2 && WHITE(i-2) && BODY_LONG(i-2) && GAP_UP(i-1) && BODY_NONE(i-1) && BLACK(i) && GAP_DOWN(i) && (close[i] <= close[i-2])) {
        HIT(TC_EVENING_DOJI_STAR);
    }

    /* Evening Star */
    if ((TC_EVENING_STAR & patterns) && i>=2 && WHITE(i-2) && BODY_LONG(i-2) && GAP_UP(i-1) && BODY_SHORT(i-1) && BLACK(i) && GAP_DOWN(i) && (close[i] <= close[i-2])) {
        HIT(TC_EVENING_STAR);
    }

    /* Four Price Doji */
    if ((TC_FOUR_PRICE_DOJI & patterns) && BODY_NONE(i) && WICK_UPPER_NONE(i) && WICK_LOWER_NONE(i)) {
        HIT(TC_FOUR_PRICE_DOJI);
    }

    /* Gravestone Doji */
    if ((TC_GRAVESTONE_DOJI & patterns) && BODY_NONE(i) && WICK_UPPER_LONG(i) && WICK_LOWER_NONE(i)) {
        HIT(TC_GRAVESTONE_DOJI);
    }

    /* Hammer */
    if ((TC_HAMMER & patterns) && i>=1 && BODY_SHORT(i) && WICK_UPPER_NONE(i) && WICK_LOWER_LONG(i) && BOTTOM(i) <= low[i-1] + OPT_NEAR()) {
        HIT(TC_HAMMER);
    }

    /* Hanging Man */
    if ((TC_HANGING_MAN & patterns) && i>=1 && BODY_SHORT(i) && WICK_UPPER_NONE(i) && WICK_LOWER_LONG(i) && BOTTOM(i) >= high[i-1] - OPT_NEAR()) {
        HIT(TC_HANGING_MAN);
    }

    /* Inverted Hammer */
    if ((TC_INVERTED_HAMMER & patterns) && i>=1 && BODY_SHORT(i) && WICK_UPPER_LONG(i) && WICK_LOWER_NONE(i) && GAP_DOWN(i)) {
        HIT(TC_INVERTED_HAMMER);
    }

    /* Long Legged Doji */
    if ((TC_LONG_LEGGED_DOJI & patterns) && BODY_NONE(i) && WICK_UPPER_LONG(i) && WICK_LOWER_LONG(i)) {
        HIT(TC_LONG_LEGGED_DOJI);
    }

    /* Marubozu */
    if ((TC_MARUBOZU & patterns) && BODY_LONG(i) && WICK_UPPER_NONE(i) && WICK_LOWER_NONE(i)) {
        HIT(TC_MARUBOZU);
    }

    /* Morning Doji Star */
    if ((TC_MORNING_DOJI_STAR & patterns) && i>=2 && BLACK(i-2) && BODY_LONG(i-2) && GAP_DOWN(i-1) && BODY_NONE(i-1) && WHITE(i) && GAP_UP(i) && (close[i] >= close[i-2])) {
        HIT(TC_MORNING_DOJI_STAR);
    }

    /* Morning Star */
    if ((TC_MORNING_STAR & patterns) && i>=2 && BLACK(i-2) && BODY_LONG(i-2) && GAP_DOWN(i-1) && BODY_SHORT(i-1) && WHITE(i) && GAP_UP(i) && (close[i] >= close[i-2])) {
        HIT(TC_MORNING_STAR);
    }

    /* Shooting Star */
    if ((TC_SHOOTING_STAR & patterns) && i>=1 && BODY_SHORT(i) && WICK_UPPER_LONG(i) && WICK_LOWER_NONE(i) && GAP_UP(i)) {
        HIT(TC_SHOOTING_STAR);
    }

    /* Spinning Top */
    if ((TC_SPINNING_TOP & patterns) && BODY_SHORT(i) && WICK_UPPER_LONGER_THAN_BODY(i) && WICK_LOWER_LONGER_THAN_BODY(i)) {
        HIT(TC_SPINNING_TOP);
    }

    /* Star */
    if ((TC_STAR & patterns) && i>=1 && STAR(i)) {
        HIT(TC_STAR);
    }

    /* Three Black Crows */
    if ((TC_THREE_BLACK_CROWS & patterns) && i>=2 && BLACK(i-2) && BLACK(i-1) && BLACK(i) && OPEN_INSIDE(i-1) && OPEN_INSIDE(i)) {
        HIT(TC_THREE_BLACK_CROWS);
    }

    /* Three White Soldiers */
    if ((TC_THREE_WHITE_SOLDIERS & patterns) && i>=2 && WHITE(i-2) && WHITE(i-1) && WHITE(i) && OPEN_INSIDE(i-1) && OPEN_INSIDE(i)) {
        HIT(TC_THREE_WHITE_SOLDIERS);
    }

    /* White Marubozu */
    if ((TC_WHITE_MARUBOZU & patterns) && BODY_LONG(i) && WICK_UPPER_NONE(i) && WICK_LOWER_NONE(i) && WHITE(i)) {
        HIT(TC_WHITE_MARUBOZU);
    }
);

    return TC_OKAY;
}


int tc_abandoned_baby_bear(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (i>=2 && WHITE(i-2) && BODY_LONG(i-2) && STAR_UP(i-1) && BODY_NONE(i-1) && BLACK(i) && STAR_DOWN(i) && (close[i] <= close[i-2])) {
                HIT(TC_ABANDONED_BABY_BEAR);
            }
        );
        return TC_OKAY;
    }

int tc_abandoned_baby_bull(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (i>=2 && BLACK(i-2) && BODY_LONG(i-2) && STAR_DOWN(i-1) && BODY_NONE(i-1) && WHITE(i) && STAR_UP(i) && (close[i] >= close[i-2])) {
                HIT(TC_ABANDONED_BABY_BULL);
            }
        );
        return TC_OKAY;
    }

int tc_big_black_candle(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (BLACK(i) && BODY_LONG(i)) {
                HIT(TC_BIG_BLACK_CANDLE);
            }
        );
        return TC_OKAY;
    }

int tc_big_white_candle(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (WHITE(i) && BODY_LONG(i)) {
                HIT(TC_BIG_WHITE_CANDLE);
            }
        );
        return TC_OKAY;
    }

int tc_black_marubozu(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (BODY_LONG(i) && WICK_UPPER_NONE(i) && WICK_LOWER_NONE(i) && BLACK(i)) {
                HIT(TC_BLACK_MARUBOZU);
            }
        );
        return TC_OKAY;
    }

int tc_doji(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (BODY_NONE(i)) {
                HIT(TC_DOJI);
            }
        );
        return TC_OKAY;
    }

int tc_dragonfly_doji(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (BODY_NONE(i) && WICK_UPPER_NONE(i) && WICK_LOWER_LONG(i)) {
                HIT(TC_DRAGONFLY_DOJI);
            }
        );
        return TC_OKAY;
    }

int tc_engulfing_bear(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (i>=1 && BLACK(i) && WHITE(i-1) && close[i] < open[i-1] && open[i] > close[i-1]) {
                HIT(TC_ENGULFING_BEAR);
            }
        );
        return TC_OKAY;
    }

int tc_engulfing_bull(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (i>=1 && WHITE(i) && BLACK(i-1) && close[i] > open[i-1] && open[i] < close[i-1]) {
                HIT(TC_ENGULFING_BULL);
            }
        );
        return TC_OKAY;
    }

int tc_evening_doji_star(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (i>=2 && WHITE(i-2) && BODY_LONG(i-2) && GAP_UP(i-1) && BODY_NONE(i-1) && BLACK(i) && GAP_DOWN(i) && (close[i] <= close[i-2])) {
                HIT(TC_EVENING_DOJI_STAR);
            }
        );
        return TC_OKAY;
    }

int tc_evening_star(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (i>=2 && WHITE(i-2) && BODY_LONG(i-2) && GAP_UP(i-1) && BODY_SHORT(i-1) && BLACK(i) && GAP_DOWN(i) && (close[i] <= close[i-2])) {
                HIT(TC_EVENING_STAR);
            }
        );
        return TC_OKAY;
    }

int tc_four_price_doji(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (BODY_NONE(i) && WICK_UPPER_NONE(i) && WICK_LOWER_NONE(i)) {
                HIT(TC_FOUR_PRICE_DOJI);
            }
        );
        return TC_OKAY;
    }

int tc_gravestone_doji(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (BODY_NONE(i) && WICK_UPPER_LONG(i) && WICK_LOWER_NONE(i)) {
                HIT(TC_GRAVESTONE_DOJI);
            }
        );
        return TC_OKAY;
    }

int tc_hammer(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (i>=1 && BODY_SHORT(i) && WICK_UPPER_NONE(i) && WICK_LOWER_LONG(i) && BOTTOM(i) <= low[i-1] + OPT_NEAR()) {
                HIT(TC_HAMMER);
            }
        );
        return TC_OKAY;
    }

int tc_hanging_man(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (i>=1 && BODY_SHORT(i) && WICK_UPPER_NONE(i) && WICK_LOWER_LONG(i) && BOTTOM(i) >= high[i-1] - OPT_NEAR()) {
                HIT(TC_HANGING_MAN);
            }
        );
        return TC_OKAY;
    }

int tc_inverted_hammer(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (i>=1 && BODY_SHORT(i) && WICK_UPPER_LONG(i) && WICK_LOWER_NONE(i) && GAP_DOWN(i)) {
                HIT(TC_INVERTED_HAMMER);
            }
        );
        return TC_OKAY;
    }

int tc_long_legged_doji(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (BODY_NONE(i) && WICK_UPPER_LONG(i) && WICK_LOWER_LONG(i)) {
                HIT(TC_LONG_LEGGED_DOJI);
            }
        );
        return TC_OKAY;
    }

int tc_marubozu(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (BODY_LONG(i) && WICK_UPPER_NONE(i) && WICK_LOWER_NONE(i)) {
                HIT(TC_MARUBOZU);
            }
        );
        return TC_OKAY;
    }

int tc_morning_doji_star(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (i>=2 && BLACK(i-2) && BODY_LONG(i-2) && GAP_DOWN(i-1) && BODY_NONE(i-1) && WHITE(i) && GAP_UP(i) && (close[i] >= close[i-2])) {
                HIT(TC_MORNING_DOJI_STAR);
            }
        );
        return TC_OKAY;
    }

int tc_morning_star(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (i>=2 && BLACK(i-2) && BODY_LONG(i-2) && GAP_DOWN(i-1) && BODY_SHORT(i-1) && WHITE(i) && GAP_UP(i) && (close[i] >= close[i-2])) {
                HIT(TC_MORNING_STAR);
            }
        );
        return TC_OKAY;
    }

int tc_shooting_star(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (i>=1 && BODY_SHORT(i) && WICK_UPPER_LONG(i) && WICK_LOWER_NONE(i) && GAP_UP(i)) {
                HIT(TC_SHOOTING_STAR);
            }
        );
        return TC_OKAY;
    }

int tc_spinning_top(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (BODY_SHORT(i) && WICK_UPPER_LONGER_THAN_BODY(i) && WICK_LOWER_LONGER_THAN_BODY(i)) {
                HIT(TC_SPINNING_TOP);
            }
        );
        return TC_OKAY;
    }

int tc_star(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (i>=1 && STAR(i)) {
                HIT(TC_STAR);
            }
        );
        return TC_OKAY;
    }

int tc_three_black_crows(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (i>=2 && BLACK(i-2) && BLACK(i-1) && BLACK(i) && OPEN_INSIDE(i-1) && OPEN_INSIDE(i)) {
                HIT(TC_THREE_BLACK_CROWS);
            }
        );
        return TC_OKAY;
    }

int tc_three_white_soldiers(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (i>=2 && WHITE(i-2) && WHITE(i-1) && WHITE(i) && OPEN_INSIDE(i-1) && OPEN_INSIDE(i)) {
                HIT(TC_THREE_WHITE_SOLDIERS);
            }
        );
        return TC_OKAY;
    }

int tc_white_marubozu(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if (BODY_LONG(i) && WICK_UPPER_NONE(i) && WICK_LOWER_NONE(i) && WHITE(i)) {
                HIT(TC_WHITE_MARUBOZU);
            }
        );
        return TC_OKAY;
    }




