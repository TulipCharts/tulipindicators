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


#ifndef __TC_CANDLES_H__
#define __TC_CANDLES_H__



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




#define TC_VERSION "0.9.2"
#define TC_BUILD 1645649572




#ifndef TI_SKIP_SYSTEM_HEADERS
#include <stdint.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif


const char* tc_version(void);
long int tc_build(void);
int tc_candle_count(void);

#define TC_OKAY 0
#define TC_INVALID_OPTION 1
#define TC_OUT_OF_MEMORY 2

typedef double TC_REAL;
typedef struct tc_result tc_result;

typedef uint64_t tc_set; /* Stores a set of candles. */
#define TC_SET_CLEAR(x) do {x = 0;} while (0)
#define TC_SET_ADD(x, c) do {x |= c;} while (0)
#define TC_SET_CHECK(x, c) (x & c)
#define TC_SET_REMOVE(x, c) (x & (~c))
#define TC_SET_NEXT(x) (x &= (x-1))




#define TC_CANDLE_COUNT 26 /* Total number of recognized candle patterns. */

#define TC_NONE (0)
#define TC_ALL ((1L<<26)-1)

#define TC_ABANDONED_BABY_BEAR (1L<<0)
#define TC_ABANDONED_BABY_BULL (1L<<1)
#define TC_BIG_BLACK_CANDLE (1L<<2)
#define TC_BIG_WHITE_CANDLE (1L<<3)
#define TC_BLACK_MARUBOZU (1L<<4)
#define TC_DOJI (1L<<5)
#define TC_DRAGONFLY_DOJI (1L<<6)
#define TC_ENGULFING_BEAR (1L<<7)
#define TC_ENGULFING_BULL (1L<<8)
#define TC_EVENING_DOJI_STAR (1L<<9)
#define TC_EVENING_STAR (1L<<10)
#define TC_FOUR_PRICE_DOJI (1L<<11)
#define TC_GRAVESTONE_DOJI (1L<<12)
#define TC_HAMMER (1L<<13)
#define TC_HANGING_MAN (1L<<14)
#define TC_INVERTED_HAMMER (1L<<15)
#define TC_LONG_LEGGED_DOJI (1L<<16)
#define TC_MARUBOZU (1L<<17)
#define TC_MORNING_DOJI_STAR (1L<<18)
#define TC_MORNING_STAR (1L<<19)
#define TC_SHOOTING_STAR (1L<<20)
#define TC_SPINNING_TOP (1L<<21)
#define TC_STAR (1L<<22)
#define TC_THREE_BLACK_CROWS (1L<<23)
#define TC_THREE_WHITE_SOLDIERS (1L<<24)
#define TC_WHITE_MARUBOZU (1L<<25)




/* tc_config holds configuration settings for candle pattern recognition. */
typedef struct tc_config {
    int period; /* Window length over which all averages are calculated. */

    TC_REAL body_none; /* Bodies <= this * High-Low average are considered to be no body (doji). */
    TC_REAL body_short; /* Bodies <= this * Open-Close average are considered to be short body. */
    TC_REAL body_long; /* Bodies >= this * Open-Close average are considered to be long. */

    TC_REAL wick_none; /* Wicks <= this * High-Low average are considered to be no wick. */
    TC_REAL wick_long; /* Wicks >= this * Open-Close average are considered to be long wick. */

    TC_REAL near; /* Price differences <= this * High-Low range are considered to be near each other. */

} tc_config;


/* tc_hit stores an index and pattern set to represent all patterns found on one input bar. */
typedef struct tc_hit {
    int index; /* Which bar the candle patterns were found on. */
    tc_set patterns; /* Which candle patterns were found. */
} tc_hit;


/* All single-pattern candle functions have this signature. */
typedef int (*tc_candle_function)(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);


/* tc_candle_info stores info about each candle pattern supported by this library. */
typedef struct tc_candle_info {
    const char *name;
    const char *full_name;
    tc_set pattern;
    tc_candle_function candle;
} tc_candle_info;


/* Complete array of all candles. Last element is 0,0,0,0... */
extern tc_candle_info tc_candles[];

/* Find candle pattern info by name. Returns 0 if not found. */
const tc_candle_info *tc_find_candle(const char *name);

/* Find candle pattern info from tc_set. Returns 0 if not found. */
const tc_candle_info *tc_get_info(tc_set pattern);


tc_config const *tc_config_default(); /* Get default configuration. */
void tc_config_set_to_default(tc_config *config); /* Initializes a configuration to the default values. */


tc_result *tc_result_new(); /* Create new result container. */
void tc_result_free(tc_result *result); /* Free result container. */
int tc_result_count(const tc_result *result); /* Return number of bars with one or more patterns. */
int tc_result_pattern_count(const tc_result *result); /* Return total number of patterns found. */

tc_hit tc_result_get(const tc_result *result, int index); /* Return the Nth pattern found. */
tc_set tc_result_at(const tc_result *result, int index); /* Return all found patterns for input index. */



/* Scan input for candle patterns. May call once or repeatedly with new inputs. */
int tc_run(tc_set patterns, int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);



/*
 *
 *
 *
 *
 *
 *        All candle patterns below, sorted alphabetically.
 *
 *
 *
 *
 *
 */



int tc_abandoned_baby_bear(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_abandoned_baby_bull(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_big_black_candle(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_big_white_candle(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_black_marubozu(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_doji(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_dragonfly_doji(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_engulfing_bear(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_engulfing_bull(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_evening_doji_star(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_evening_star(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_four_price_doji(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_gravestone_doji(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_hammer(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_hanging_man(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_inverted_hammer(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_long_legged_doji(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_marubozu(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_morning_doji_star(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_morning_star(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_shooting_star(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_spinning_top(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_star(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_three_black_crows(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_three_white_soldiers(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);
int tc_white_marubozu(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);




#ifdef __cplusplus
}
#endif

#endif /*__TC_CANDLES_H__*/

