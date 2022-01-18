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


/*VERSION*/

/*VERSION_DEFINE*/


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




/*DEFINES*/



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



/*FUNCTIONS*/



#ifdef __cplusplus
}
#endif

#endif /*__TC_CANDLES_H__*/
