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


#ifndef __TI_INDICATORS_H__
#define __TI_INDICATORS_H__


/*VERSION*/

/*VERSION_DEFINE*/


#ifndef TI_SKIP_SYSTEM_HEADERS
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif


const char* ti_version(void);
long int ti_build(void);
int ti_indicator_count(void);





#define TI_REAL double

#define TI_INDICATOR_COUNT 104 /* Total number of indicators. */

#define TI_OKAY                    0
#define TI_INVALID_OPTION          1
#define TI_OUT_OF_MEMORY           2

#define TI_TYPE_OVERLAY            1 /* These have roughly the same range as the input data. */
#define TI_TYPE_INDICATOR          2 /* Everything else (e.g. oscillators). */
#define TI_TYPE_MATH               3 /* These aren't so good for plotting, but are useful with formulas. */
#define TI_TYPE_SIMPLE             4 /* These apply a simple operator (e.g. addition, sin, sqrt). */
#define TI_TYPE_COMPARATIVE        5 /* These are designed to take inputs from different securities. i.e. compare stock A to stock B.*/

#define TI_MAXINDPARAMS 16 /* No indicator will use more than this many inputs, options, or outputs. */


typedef int (*ti_indicator_start_function)(TI_REAL const *options);
typedef int (*ti_indicator_function)(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);


struct ti_stream; typedef struct ti_stream ti_stream;
typedef int (*ti_indicator_stream_new)(TI_REAL const *options, ti_stream **stream);
typedef int (*ti_indicator_stream_run)(ti_stream *stream, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs);
typedef void (*ti_indicator_stream_free)(ti_stream *stream);


typedef struct ti_indicator_info {
    const char *name;
    const char *full_name;
    ti_indicator_start_function start;
    ti_indicator_function indicator;
    ti_indicator_function indicator_ref;
    int type, inputs, options, outputs;
    const char *input_names[TI_MAXINDPARAMS];
    const char *option_names[TI_MAXINDPARAMS];
    const char *output_names[TI_MAXINDPARAMS];
    ti_indicator_stream_new stream_new;
    ti_indicator_stream_run stream_run;
    ti_indicator_stream_free stream_free;
} ti_indicator_info;


/*Complete array of all indicators. Last element is 0,0,0,0...*/
extern ti_indicator_info ti_indicators[];


/*Searches for an indicator by name. Returns 0 if not found.*/
const ti_indicator_info *ti_find_indicator(const char *name);




int ti_stream_run(ti_stream *stream, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs);
ti_indicator_info *ti_stream_get_info(ti_stream *stream);
int ti_stream_get_progress(ti_stream *stream);
void ti_stream_free(ti_stream *stream);





/*
 *
 *
 *
 *
 *
 *        All indicators below, sorted alphabetically.
 *
 *
 *
 *
 *
 */



/*INDICATORS*/







#ifdef __cplusplus
}
#endif

#endif /*__TI_INDICATORS_H__*/
