/*
 * TULIP INDICATORS AMALGAMATION
 * This is all of Tulip Indicators in one file.
 * To get the original sources, go to https://tulipindicators.org
 */


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




#define TI_VERSION "0.9.2"
#define TI_BUILD 1645649572




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



/* Vector Absolute Value */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: abs */
#define TI_INDICATOR_ABS_INDEX 0
int ti_abs_start(TI_REAL const *options);
int ti_abs(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Arccosine */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: acos */
#define TI_INDICATOR_ACOS_INDEX 1
int ti_acos_start(TI_REAL const *options);
int ti_acos(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Accumulation/Distribution Line */
/* Type: indicator */
/* Input arrays: 4    Options: 0    Output arrays: 1 */
/* Inputs: high, low, close, volume */
/* Options: none */
/* Outputs: ad */
#define TI_INDICATOR_AD_INDEX 2
int ti_ad_start(TI_REAL const *options);
int ti_ad(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Addition */
/* Type: simple */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: real, real */
/* Options: none */
/* Outputs: add */
#define TI_INDICATOR_ADD_INDEX 3
int ti_add_start(TI_REAL const *options);
int ti_add(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Accumulation/Distribution Oscillator */
/* Type: indicator */
/* Input arrays: 4    Options: 2    Output arrays: 1 */
/* Inputs: high, low, close, volume */
/* Options: short_period, long_period */
/* Outputs: adosc */
#define TI_INDICATOR_ADOSC_INDEX 4
int ti_adosc_start(TI_REAL const *options);
int ti_adosc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Average Directional Movement Index */
/* Type: indicator */
/* Input arrays: 2    Options: 1    Output arrays: 1 */
/* Inputs: high, low */
/* Options: period */
/* Outputs: adx */
#define TI_INDICATOR_ADX_INDEX 5
int ti_adx_start(TI_REAL const *options);
int ti_adx(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Average Directional Movement Rating */
/* Type: indicator */
/* Input arrays: 2    Options: 1    Output arrays: 1 */
/* Inputs: high, low */
/* Options: period */
/* Outputs: adxr */
#define TI_INDICATOR_ADXR_INDEX 6
int ti_adxr_start(TI_REAL const *options);
int ti_adxr(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Awesome Oscillator */
/* Type: indicator */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: high, low */
/* Options: none */
/* Outputs: ao */
#define TI_INDICATOR_AO_INDEX 7
int ti_ao_start(TI_REAL const *options);
int ti_ao(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Absolute Price Oscillator */
/* Type: indicator */
/* Input arrays: 1    Options: 2    Output arrays: 1 */
/* Inputs: real */
/* Options: short_period, long_period */
/* Outputs: apo */
#define TI_INDICATOR_APO_INDEX 8
int ti_apo_start(TI_REAL const *options);
int ti_apo(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Aroon */
/* Type: indicator */
/* Input arrays: 2    Options: 1    Output arrays: 2 */
/* Inputs: high, low */
/* Options: period */
/* Outputs: aroon_down, aroon_up */
#define TI_INDICATOR_AROON_INDEX 9
int ti_aroon_start(TI_REAL const *options);
int ti_aroon(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Aroon Oscillator */
/* Type: indicator */
/* Input arrays: 2    Options: 1    Output arrays: 1 */
/* Inputs: high, low */
/* Options: period */
/* Outputs: aroonosc */
#define TI_INDICATOR_AROONOSC_INDEX 10
int ti_aroonosc_start(TI_REAL const *options);
int ti_aroonosc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Arcsine */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: asin */
#define TI_INDICATOR_ASIN_INDEX 11
int ti_asin_start(TI_REAL const *options);
int ti_asin(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Arctangent */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: atan */
#define TI_INDICATOR_ATAN_INDEX 12
int ti_atan_start(TI_REAL const *options);
int ti_atan(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Average True Range */
/* Type: indicator */
/* Input arrays: 3    Options: 1    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: period */
/* Outputs: atr */
#define TI_INDICATOR_ATR_INDEX 13
int ti_atr_start(TI_REAL const *options);
int ti_atr(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
int ti_atr_ref(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
int ti_atr_stream_new(TI_REAL const *options, ti_stream **stream);
int ti_atr_stream_run(ti_stream *stream, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs);
void ti_atr_stream_free(ti_stream *stream);
/* Average Price */
/* Type: overlay */
/* Input arrays: 4    Options: 0    Output arrays: 1 */
/* Inputs: open, high, low, close */
/* Options: none */
/* Outputs: avgprice */
#define TI_INDICATOR_AVGPRICE_INDEX 14
int ti_avgprice_start(TI_REAL const *options);
int ti_avgprice(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Bollinger Bands */
/* Type: overlay */
/* Input arrays: 1    Options: 2    Output arrays: 3 */
/* Inputs: real */
/* Options: period, stddev */
/* Outputs: bbands_lower, bbands_middle, bbands_upper */
#define TI_INDICATOR_BBANDS_INDEX 15
int ti_bbands_start(TI_REAL const *options);
int ti_bbands(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Balance of Power */
/* Type: indicator */
/* Input arrays: 4    Options: 0    Output arrays: 1 */
/* Inputs: open, high, low, close */
/* Options: none */
/* Outputs: bop */
#define TI_INDICATOR_BOP_INDEX 16
int ti_bop_start(TI_REAL const *options);
int ti_bop(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Commodity Channel Index */
/* Type: indicator */
/* Input arrays: 3    Options: 1    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: period */
/* Outputs: cci */
#define TI_INDICATOR_CCI_INDEX 17
int ti_cci_start(TI_REAL const *options);
int ti_cci(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Ceiling */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: ceil */
#define TI_INDICATOR_CEIL_INDEX 18
int ti_ceil_start(TI_REAL const *options);
int ti_ceil(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Chande Momentum Oscillator */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: cmo */
#define TI_INDICATOR_CMO_INDEX 19
int ti_cmo_start(TI_REAL const *options);
int ti_cmo(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Cosine */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: cos */
#define TI_INDICATOR_COS_INDEX 20
int ti_cos_start(TI_REAL const *options);
int ti_cos(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Hyperbolic Cosine */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: cosh */
#define TI_INDICATOR_COSH_INDEX 21
int ti_cosh_start(TI_REAL const *options);
int ti_cosh(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Crossany */
/* Type: math */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: real, real */
/* Options: none */
/* Outputs: crossany */
#define TI_INDICATOR_CROSSANY_INDEX 22
int ti_crossany_start(TI_REAL const *options);
int ti_crossany(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Crossover */
/* Type: math */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: real, real */
/* Options: none */
/* Outputs: crossover */
#define TI_INDICATOR_CROSSOVER_INDEX 23
int ti_crossover_start(TI_REAL const *options);
int ti_crossover(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Chaikins Volatility */
/* Type: indicator */
/* Input arrays: 2    Options: 1    Output arrays: 1 */
/* Inputs: high, low */
/* Options: period */
/* Outputs: cvi */
#define TI_INDICATOR_CVI_INDEX 24
int ti_cvi_start(TI_REAL const *options);
int ti_cvi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Linear Decay */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: decay */
#define TI_INDICATOR_DECAY_INDEX 25
int ti_decay_start(TI_REAL const *options);
int ti_decay(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Double Exponential Moving Average */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: dema */
#define TI_INDICATOR_DEMA_INDEX 26
int ti_dema_start(TI_REAL const *options);
int ti_dema(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Directional Indicator */
/* Type: indicator */
/* Input arrays: 3    Options: 1    Output arrays: 2 */
/* Inputs: high, low, close */
/* Options: period */
/* Outputs: plus_di, minus_di */
#define TI_INDICATOR_DI_INDEX 27
int ti_di_start(TI_REAL const *options);
int ti_di(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Division */
/* Type: simple */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: real, real */
/* Options: none */
/* Outputs: div */
#define TI_INDICATOR_DIV_INDEX 28
int ti_div_start(TI_REAL const *options);
int ti_div(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Directional Movement */
/* Type: indicator */
/* Input arrays: 2    Options: 1    Output arrays: 2 */
/* Inputs: high, low */
/* Options: period */
/* Outputs: plus_dm, minus_dm */
#define TI_INDICATOR_DM_INDEX 29
int ti_dm_start(TI_REAL const *options);
int ti_dm(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Detrended Price Oscillator */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: dpo */
#define TI_INDICATOR_DPO_INDEX 30
int ti_dpo_start(TI_REAL const *options);
int ti_dpo(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Directional Movement Index */
/* Type: indicator */
/* Input arrays: 2    Options: 1    Output arrays: 1 */
/* Inputs: high, low */
/* Options: period */
/* Outputs: dx */
#define TI_INDICATOR_DX_INDEX 31
int ti_dx_start(TI_REAL const *options);
int ti_dx(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Exponential Decay */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: edecay */
#define TI_INDICATOR_EDECAY_INDEX 32
int ti_edecay_start(TI_REAL const *options);
int ti_edecay(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Exponential Moving Average */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: ema */
#define TI_INDICATOR_EMA_INDEX 33
int ti_ema_start(TI_REAL const *options);
int ti_ema(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Ease of Movement */
/* Type: indicator */
/* Input arrays: 3    Options: 0    Output arrays: 1 */
/* Inputs: high, low, volume */
/* Options: none */
/* Outputs: emv */
#define TI_INDICATOR_EMV_INDEX 34
int ti_emv_start(TI_REAL const *options);
int ti_emv(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Exponential */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: exp */
#define TI_INDICATOR_EXP_INDEX 35
int ti_exp_start(TI_REAL const *options);
int ti_exp(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Fisher Transform */
/* Type: indicator */
/* Input arrays: 2    Options: 1    Output arrays: 2 */
/* Inputs: high, low */
/* Options: period */
/* Outputs: fisher, fisher_signal */
#define TI_INDICATOR_FISHER_INDEX 36
int ti_fisher_start(TI_REAL const *options);
int ti_fisher(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Floor */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: floor */
#define TI_INDICATOR_FLOOR_INDEX 37
int ti_floor_start(TI_REAL const *options);
int ti_floor(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Forecast Oscillator */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: fosc */
#define TI_INDICATOR_FOSC_INDEX 38
int ti_fosc_start(TI_REAL const *options);
int ti_fosc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Hull Moving Average */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: hma */
#define TI_INDICATOR_HMA_INDEX 39
int ti_hma_start(TI_REAL const *options);
int ti_hma(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Kaufman Adaptive Moving Average */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: kama */
#define TI_INDICATOR_KAMA_INDEX 40
int ti_kama_start(TI_REAL const *options);
int ti_kama(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Klinger Volume Oscillator */
/* Type: indicator */
/* Input arrays: 4    Options: 2    Output arrays: 1 */
/* Inputs: high, low, close, volume */
/* Options: short_period, long_period */
/* Outputs: kvo */
#define TI_INDICATOR_KVO_INDEX 41
int ti_kvo_start(TI_REAL const *options);
int ti_kvo(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Lag */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: lag */
#define TI_INDICATOR_LAG_INDEX 42
int ti_lag_start(TI_REAL const *options);
int ti_lag(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Linear Regression */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: linreg */
#define TI_INDICATOR_LINREG_INDEX 43
int ti_linreg_start(TI_REAL const *options);
int ti_linreg(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Linear Regression Intercept */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: linregintercept */
#define TI_INDICATOR_LINREGINTERCEPT_INDEX 44
int ti_linregintercept_start(TI_REAL const *options);
int ti_linregintercept(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Linear Regression Slope */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: linregslope */
#define TI_INDICATOR_LINREGSLOPE_INDEX 45
int ti_linregslope_start(TI_REAL const *options);
int ti_linregslope(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Natural Log */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: ln */
#define TI_INDICATOR_LN_INDEX 46
int ti_ln_start(TI_REAL const *options);
int ti_ln(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Base-10 Log */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: log10 */
#define TI_INDICATOR_LOG10_INDEX 47
int ti_log10_start(TI_REAL const *options);
int ti_log10(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Moving Average Convergence/Divergence */
/* Type: indicator */
/* Input arrays: 1    Options: 3    Output arrays: 3 */
/* Inputs: real */
/* Options: short_period, long_period, signal_period */
/* Outputs: macd, macd_signal, macd_histogram */
#define TI_INDICATOR_MACD_INDEX 48
int ti_macd_start(TI_REAL const *options);
int ti_macd(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Market Facilitation Index */
/* Type: indicator */
/* Input arrays: 3    Options: 0    Output arrays: 1 */
/* Inputs: high, low, volume */
/* Options: none */
/* Outputs: marketfi */
#define TI_INDICATOR_MARKETFI_INDEX 49
int ti_marketfi_start(TI_REAL const *options);
int ti_marketfi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Mass Index */
/* Type: indicator */
/* Input arrays: 2    Options: 1    Output arrays: 1 */
/* Inputs: high, low */
/* Options: period */
/* Outputs: mass */
#define TI_INDICATOR_MASS_INDEX 50
int ti_mass_start(TI_REAL const *options);
int ti_mass(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Maximum In Period */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: max */
#define TI_INDICATOR_MAX_INDEX 51
int ti_max_start(TI_REAL const *options);
int ti_max(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
int ti_max_ref(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Mean Deviation Over Period */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: md */
#define TI_INDICATOR_MD_INDEX 52
int ti_md_start(TI_REAL const *options);
int ti_md(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Median Price */
/* Type: overlay */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: high, low */
/* Options: none */
/* Outputs: medprice */
#define TI_INDICATOR_MEDPRICE_INDEX 53
int ti_medprice_start(TI_REAL const *options);
int ti_medprice(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Money Flow Index */
/* Type: indicator */
/* Input arrays: 4    Options: 1    Output arrays: 1 */
/* Inputs: high, low, close, volume */
/* Options: period */
/* Outputs: mfi */
#define TI_INDICATOR_MFI_INDEX 54
int ti_mfi_start(TI_REAL const *options);
int ti_mfi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Minimum In Period */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: min */
#define TI_INDICATOR_MIN_INDEX 55
int ti_min_start(TI_REAL const *options);
int ti_min(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
int ti_min_ref(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Momentum */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: mom */
#define TI_INDICATOR_MOM_INDEX 56
int ti_mom_start(TI_REAL const *options);
int ti_mom(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Mesa Sine Wave */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 2 */
/* Inputs: real */
/* Options: period */
/* Outputs: msw_sine, msw_lead */
#define TI_INDICATOR_MSW_INDEX 57
int ti_msw_start(TI_REAL const *options);
int ti_msw(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Multiplication */
/* Type: simple */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: real, real */
/* Options: none */
/* Outputs: mul */
#define TI_INDICATOR_MUL_INDEX 58
int ti_mul_start(TI_REAL const *options);
int ti_mul(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Normalized Average True Range */
/* Type: indicator */
/* Input arrays: 3    Options: 1    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: period */
/* Outputs: natr */
#define TI_INDICATOR_NATR_INDEX 59
int ti_natr_start(TI_REAL const *options);
int ti_natr(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Negative Volume Index */
/* Type: indicator */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: close, volume */
/* Options: none */
/* Outputs: nvi */
#define TI_INDICATOR_NVI_INDEX 60
int ti_nvi_start(TI_REAL const *options);
int ti_nvi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* On Balance Volume */
/* Type: indicator */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: close, volume */
/* Options: none */
/* Outputs: obv */
#define TI_INDICATOR_OBV_INDEX 61
int ti_obv_start(TI_REAL const *options);
int ti_obv(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Percentage Price Oscillator */
/* Type: indicator */
/* Input arrays: 1    Options: 2    Output arrays: 1 */
/* Inputs: real */
/* Options: short_period, long_period */
/* Outputs: ppo */
#define TI_INDICATOR_PPO_INDEX 62
int ti_ppo_start(TI_REAL const *options);
int ti_ppo(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Parabolic SAR */
/* Type: overlay */
/* Input arrays: 2    Options: 2    Output arrays: 1 */
/* Inputs: high, low */
/* Options: acceleration_factor_step, acceleration_factor_maximum */
/* Outputs: psar */
#define TI_INDICATOR_PSAR_INDEX 63
int ti_psar_start(TI_REAL const *options);
int ti_psar(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Positive Volume Index */
/* Type: indicator */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: close, volume */
/* Options: none */
/* Outputs: pvi */
#define TI_INDICATOR_PVI_INDEX 64
int ti_pvi_start(TI_REAL const *options);
int ti_pvi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Qstick */
/* Type: indicator */
/* Input arrays: 2    Options: 1    Output arrays: 1 */
/* Inputs: open, close */
/* Options: period */
/* Outputs: qstick */
#define TI_INDICATOR_QSTICK_INDEX 65
int ti_qstick_start(TI_REAL const *options);
int ti_qstick(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Rate of Change */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: roc */
#define TI_INDICATOR_ROC_INDEX 66
int ti_roc_start(TI_REAL const *options);
int ti_roc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Rate of Change Ratio */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: rocr */
#define TI_INDICATOR_ROCR_INDEX 67
int ti_rocr_start(TI_REAL const *options);
int ti_rocr(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Round */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: round */
#define TI_INDICATOR_ROUND_INDEX 68
int ti_round_start(TI_REAL const *options);
int ti_round(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Relative Strength Index */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: rsi */
#define TI_INDICATOR_RSI_INDEX 69
int ti_rsi_start(TI_REAL const *options);
int ti_rsi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Sine */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: sin */
#define TI_INDICATOR_SIN_INDEX 70
int ti_sin_start(TI_REAL const *options);
int ti_sin(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Hyperbolic Sine */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: sinh */
#define TI_INDICATOR_SINH_INDEX 71
int ti_sinh_start(TI_REAL const *options);
int ti_sinh(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Simple Moving Average */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: sma */
#define TI_INDICATOR_SMA_INDEX 72
int ti_sma_start(TI_REAL const *options);
int ti_sma(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
int ti_sma_stream_new(TI_REAL const *options, ti_stream **stream);
int ti_sma_stream_run(ti_stream *stream, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs);
void ti_sma_stream_free(ti_stream *stream);
/* Vector Square Root */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: sqrt */
#define TI_INDICATOR_SQRT_INDEX 73
int ti_sqrt_start(TI_REAL const *options);
int ti_sqrt(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Standard Deviation Over Period */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: stddev */
#define TI_INDICATOR_STDDEV_INDEX 74
int ti_stddev_start(TI_REAL const *options);
int ti_stddev(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Standard Error Over Period */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: stderr */
#define TI_INDICATOR_STDERR_INDEX 75
int ti_stderr_start(TI_REAL const *options);
int ti_stderr(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Stochastic Oscillator */
/* Type: indicator */
/* Input arrays: 3    Options: 3    Output arrays: 2 */
/* Inputs: high, low, close */
/* Options: k_period, k_slowing_period, d_period */
/* Outputs: stoch_k, stoch_d */
#define TI_INDICATOR_STOCH_INDEX 76
int ti_stoch_start(TI_REAL const *options);
int ti_stoch(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Stochastic RSI */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: stochrsi */
#define TI_INDICATOR_STOCHRSI_INDEX 77
int ti_stochrsi_start(TI_REAL const *options);
int ti_stochrsi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Subtraction */
/* Type: simple */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: real, real */
/* Options: none */
/* Outputs: sub */
#define TI_INDICATOR_SUB_INDEX 78
int ti_sub_start(TI_REAL const *options);
int ti_sub(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Sum Over Period */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: sum */
#define TI_INDICATOR_SUM_INDEX 79
int ti_sum_start(TI_REAL const *options);
int ti_sum(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Tangent */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: tan */
#define TI_INDICATOR_TAN_INDEX 80
int ti_tan_start(TI_REAL const *options);
int ti_tan(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Hyperbolic Tangent */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: tanh */
#define TI_INDICATOR_TANH_INDEX 81
int ti_tanh_start(TI_REAL const *options);
int ti_tanh(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Triple Exponential Moving Average */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: tema */
#define TI_INDICATOR_TEMA_INDEX 82
int ti_tema_start(TI_REAL const *options);
int ti_tema(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Degree Conversion */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: degrees */
#define TI_INDICATOR_TODEG_INDEX 83
int ti_todeg_start(TI_REAL const *options);
int ti_todeg(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Radian Conversion */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: radians */
#define TI_INDICATOR_TORAD_INDEX 84
int ti_torad_start(TI_REAL const *options);
int ti_torad(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* True Range */
/* Type: indicator */
/* Input arrays: 3    Options: 0    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: none */
/* Outputs: tr */
#define TI_INDICATOR_TR_INDEX 85
int ti_tr_start(TI_REAL const *options);
int ti_tr(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Triangular Moving Average */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: trima */
#define TI_INDICATOR_TRIMA_INDEX 86
int ti_trima_start(TI_REAL const *options);
int ti_trima(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Trix */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: trix */
#define TI_INDICATOR_TRIX_INDEX 87
int ti_trix_start(TI_REAL const *options);
int ti_trix(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vector Truncate */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: trunc */
#define TI_INDICATOR_TRUNC_INDEX 88
int ti_trunc_start(TI_REAL const *options);
int ti_trunc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Time Series Forecast */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: tsf */
#define TI_INDICATOR_TSF_INDEX 89
int ti_tsf_start(TI_REAL const *options);
int ti_tsf(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Typical Price */
/* Type: overlay */
/* Input arrays: 3    Options: 0    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: none */
/* Outputs: typprice */
#define TI_INDICATOR_TYPPRICE_INDEX 90
int ti_typprice_start(TI_REAL const *options);
int ti_typprice(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Ultimate Oscillator */
/* Type: indicator */
/* Input arrays: 3    Options: 3    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: short_period, medium_period, long_period */
/* Outputs: ultosc */
#define TI_INDICATOR_ULTOSC_INDEX 91
int ti_ultosc_start(TI_REAL const *options);
int ti_ultosc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Variance Over Period */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: var */
#define TI_INDICATOR_VAR_INDEX 92
int ti_var_start(TI_REAL const *options);
int ti_var(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Vertical Horizontal Filter */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: vhf */
#define TI_INDICATOR_VHF_INDEX 93
int ti_vhf_start(TI_REAL const *options);
int ti_vhf(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Variable Index Dynamic Average */
/* Type: overlay */
/* Input arrays: 1    Options: 3    Output arrays: 1 */
/* Inputs: real */
/* Options: short_period, long_period, alpha */
/* Outputs: vidya */
#define TI_INDICATOR_VIDYA_INDEX 94
int ti_vidya_start(TI_REAL const *options);
int ti_vidya(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Annualized Historical Volatility */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: volatility */
#define TI_INDICATOR_VOLATILITY_INDEX 95
int ti_volatility_start(TI_REAL const *options);
int ti_volatility(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Volume Oscillator */
/* Type: indicator */
/* Input arrays: 1    Options: 2    Output arrays: 1 */
/* Inputs: volume */
/* Options: short_period, long_period */
/* Outputs: vosc */
#define TI_INDICATOR_VOSC_INDEX 96
int ti_vosc_start(TI_REAL const *options);
int ti_vosc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Volume Weighted Moving Average */
/* Type: overlay */
/* Input arrays: 2    Options: 1    Output arrays: 1 */
/* Inputs: close, volume */
/* Options: period */
/* Outputs: vwma */
#define TI_INDICATOR_VWMA_INDEX 97
int ti_vwma_start(TI_REAL const *options);
int ti_vwma(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Williams Accumulation/Distribution */
/* Type: indicator */
/* Input arrays: 3    Options: 0    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: none */
/* Outputs: wad */
#define TI_INDICATOR_WAD_INDEX 98
int ti_wad_start(TI_REAL const *options);
int ti_wad(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Weighted Close Price */
/* Type: overlay */
/* Input arrays: 3    Options: 0    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: none */
/* Outputs: wcprice */
#define TI_INDICATOR_WCPRICE_INDEX 99
int ti_wcprice_start(TI_REAL const *options);
int ti_wcprice(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Wilders Smoothing */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: wilders */
#define TI_INDICATOR_WILDERS_INDEX 100
int ti_wilders_start(TI_REAL const *options);
int ti_wilders(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Williams %R */
/* Type: indicator */
/* Input arrays: 3    Options: 1    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: period */
/* Outputs: willr */
#define TI_INDICATOR_WILLR_INDEX 101
int ti_willr_start(TI_REAL const *options);
int ti_willr(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Weighted Moving Average */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: wma */
#define TI_INDICATOR_WMA_INDEX 102
int ti_wma_start(TI_REAL const *options);
int ti_wma(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);
/* Zero-Lag Exponential Moving Average */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: zlema */
#define TI_INDICATOR_ZLEMA_INDEX 103
int ti_zlema_start(TI_REAL const *options);
int ti_zlema(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);








#ifdef __cplusplus
}
#endif

#endif /*__TI_INDICATORS_H__*/

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

#ifndef __BUFFER_H__
#define __BUFFER_H__


typedef struct {
    int size, pushes, index;
    TI_REAL sum;
    TI_REAL vals[1];
} ti_buffer;


ti_buffer *ti_buffer_new(int size);
void ti_buffer_free(ti_buffer *buffer);

/* Pushes a new value, plus updates sum. */
#define ti_buffer_push(BUFFER, VAL) \
do { \
    if ((BUFFER)->pushes >= (BUFFER)->size) { \
        (BUFFER)->sum -= (BUFFER)->vals[(BUFFER)->index]; \
    } \
\
    (BUFFER)->sum += (VAL); \
    (BUFFER)->vals[(BUFFER)->index] = (VAL); \
    (BUFFER)->pushes += 1; \
    (BUFFER)->index = ((BUFFER)->index + 1); \
    if ((BUFFER)->index >= (BUFFER)->size) (BUFFER)->index = 0; \
} while (0)


/* Pushes a new value, skips updating sum. */
#define ti_buffer_qpush(BUFFER, VAL) \
do { \
    (BUFFER)->vals[(BUFFER)->index] = (VAL); \
    (BUFFER)->index = ((BUFFER)->index + 1); \
    if ((BUFFER)->index >= (BUFFER)->size) (BUFFER)->index = 0; \
} while (0)


/* With get, 0 = last value pushed, -1 = value before last, etc. */
#define ti_buffer_get(BUFFER, VAL) ((BUFFER)->vals[((BUFFER)->index + (BUFFER)->size - 1 + (VAL)) % (BUFFER)->size])


#endif /*__BUFFER_H__*/
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


#ifndef __LOCALBUFFER_H__
#define __LOCALBUFFER_H__


/* IMPORTANT USAGE NOTICE:
 *  1. BUFFERS must go in the very end of the struct
 *  2. BUFFER_INIT must go in the order of declaration
 *  3. the zero[3] array must be initialized to zero, e.g. with help of calloc
 *  4. it is ASSUMED that sizeof(double) = 2*sizeof(int)
 */

/* This set of macros implements ring buffer functionality.
 * It was designed with data locality in mind: as opposed to
 * its predecessor buffer.h, it allows the user to manage
 * multiple buffers along with other data as a single chunk
 * of memory, which comes particularly handy when implementing
 * streaming versions of indicators.
 */

/* Example usage:
 * > struct ti_stream {
 * >     int index;
 * >     int progress;
 * >
 * >     BUFFERS(
 * >         BUFFER(a)
 * >         BUFFER(b)
 * >         BUFFER(c)
 * >     )
 * > };
 * >
 * > int ti_xxx_stream_new(TI_REAL const *options, ti_stream **stream) {
 * >     *stream = calloc(1, sizeof(ti_stream));
 * >
 * >     BUFFER_INIT(*stream, a, 5);
 * >     BUFFER_INIT(*stream, b, 8);
 * >     BUFFER_INIT(*stream, c, 13);
 * >
 * >     int size = BUFFERS_SIZE(*stream);
 * >     *stream = realloc(*stream, sizeof(ti_stream) + sizeof(TI_REAL[size]));
 * > }
 */

#define BUFFERS(buffers) struct { double padding; int zero[4]; buffers } buf_info;
#define BUFFER(name) int offset_##name; int size_##name; int index_##name; int padding_##name;
#define BUFFER_INIT(ptr, name, size) do { \
    ((ptr)->buf_info).offset_##name = *(&((ptr)->buf_info).offset_##name-3) + *(&((ptr)->buf_info).offset_##name-4); \
    ((ptr)->buf_info).size_##name = size; \
    ((ptr)->buf_info).index_##name = -1; \
} while (0)
#define BUFFERS_SIZE(ptr) (*((int*)(&((ptr)->buf_info)+1)-3) + *((int*)(&((ptr)->buf_info)+1)-4))
#define BUFFER_AT(result, ptr, name, delta) do { \
    int idx = ((ptr)->buf_info).index_##name + delta; \
    while (idx >= ((ptr)->buf_info).size_##name) { idx -= ((ptr)->buf_info).size_##name; } \
    while (idx < 0) { idx += ((ptr)->buf_info).size_##name; } \
    result = *((TI_REAL*)(&((ptr)->buf_info) + 1) + ((ptr)->buf_info).offset_##name + idx); \
} while (0)
#define BUFFER_PUSH(ptr, name, value) do { \
    int idx = ((ptr)->buf_info).index_##name + 1; \
    if (idx == ((ptr)->buf_info).size_##name) { idx = 0; } \
    *((TI_REAL*)(&((ptr)->buf_info) + 1) + ((ptr)->buf_info).offset_##name + idx) = value; \
    ((ptr)->buf_info).index_##name = idx; \
} while (0)


#endif /*__LOCALBUFFER_H__*/
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


#ifndef __MINMAX_H__
#define __MINMAX_H__


#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))


#endif /*__MINMAX_H__*/
int ti_abs_start(TI_REAL const *options) { (void)options; return 0; } int ti_abs(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (fabs(in1[i])); } return TI_OKAY; }
int ti_acos_start(TI_REAL const *options) { (void)options; return 0; } int ti_acos(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (acos(in1[i])); } return TI_OKAY; }
int ti_ad_start(TI_REAL const *options) {
    (void)options;
    return 0;
}
int ti_ad(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    const TI_REAL *volume = inputs[3];
    (void)options;
    TI_REAL *output = outputs[0];
    TI_REAL sum = 0;
    int i;
    for (i = 0; i < size; ++i) {
        const TI_REAL hl = (high[i] - low[i]);
        if (hl != 0.0) {
            sum += (close[i] - low[i] - high[i] + close[i]) / hl * volume[i];
        }
        output[i] = sum;
    }
    return TI_OKAY;
}
int ti_add_start(TI_REAL const *options) { (void)options; return 0; } int ti_add(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; const TI_REAL *in2 = inputs[1]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (in1[i] + in2[i]); } return TI_OKAY; }
int ti_adosc_start(TI_REAL const *options) {
    return (int)(options[1])-1;
}
int ti_adosc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    const TI_REAL *volume = inputs[3];
    const int short_period = (int)options[0];
    const int long_period = (int)options[1];
    const int start = long_period - 1;
    if (short_period < 1) return TI_INVALID_OPTION;
    if (long_period < short_period) return TI_INVALID_OPTION;
    if (size <= ti_adosc_start(options)) return TI_OKAY;
    const TI_REAL short_per = 2 / ((TI_REAL)short_period + 1);
    const TI_REAL long_per = 2 / ((TI_REAL)long_period + 1);
    TI_REAL *output = outputs[0];
    TI_REAL sum = 0, short_ema = 0, long_ema = 0;
    int i;
    for (i = 0; i < size; ++i) {
        const TI_REAL hl = (high[i] - low[i]);
        if (hl != 0.0) {
            sum += (close[i] - low[i] - high[i] + close[i]) / hl * volume[i];
        }
        if (i == 0) {
            short_ema = sum;
            long_ema = sum;
        } else {
            short_ema = (sum-short_ema) * short_per + short_ema;
            long_ema = (sum-long_ema) * long_per + long_ema;
        }
        if (i >= start) {
            *output++ = short_ema - long_ema;
        }
    }
    assert(output - outputs[0] == size - ti_adosc_start(options));
    return TI_OKAY;
}
int ti_adx_start(TI_REAL const *options) {
    return ((int)options[0]-1) * 2;
}
int ti_adx(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 2) return TI_INVALID_OPTION;
    if (size <= ti_adx_start(options)) return TI_OKAY;
    const TI_REAL per = ((TI_REAL)period-1) / ((TI_REAL)period);
    const TI_REAL invper = 1.0 / ((TI_REAL)period);
    TI_REAL dmup = 0;
    TI_REAL dmdown = 0;
    int i;
    for (i = 1; i < period; ++i) {
        TI_REAL dp, dm;
        do { dp = high[i] - high[i-1]; dm = low[i-1] - low[i]; if (dp < 0) dp = 0; else if (dp > dm) dm = 0; if (dm < 0) dm = 0; else if (dm > dp) dp = 0;} while (0);
        dmup += dp;
        dmdown += dm;
    }
    TI_REAL adx = 0.0;
    {
        TI_REAL di_up = dmup;
        TI_REAL di_down = dmdown;
        TI_REAL dm_diff = fabs(di_up - di_down);
        TI_REAL dm_sum = di_up + di_down;
        TI_REAL dx = dm_diff / dm_sum * 100;
        adx += dx;
    }
    for (i = period; i < size; ++i) {
        TI_REAL dp, dm;
        do { dp = high[i] - high[i-1]; dm = low[i-1] - low[i]; if (dp < 0) dp = 0; else if (dp > dm) dm = 0; if (dm < 0) dm = 0; else if (dm > dp) dp = 0;} while (0);
        dmup = dmup * per + dp;
        dmdown = dmdown * per + dm;
        TI_REAL di_up = dmup;
        TI_REAL di_down = dmdown;
        TI_REAL dm_diff = fabs(di_up - di_down);
        TI_REAL dm_sum = di_up + di_down;
        TI_REAL dx = dm_diff / dm_sum * 100;
        if (i-period < period-2) {
            adx += dx;
        } else if (i-period == period-2) {
            adx += dx;
            *output++ = adx * invper;
        } else {
            adx = adx * per + dx;
            *output++ = adx * invper;
        }
    }
    assert(output - outputs[0] == size - ti_adx_start(options));
    return TI_OKAY;
}
int ti_adxr_start(TI_REAL const *options) {
    return ((int)options[0]-1) * 3;
}
int ti_adxr(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 2) return TI_INVALID_OPTION;
    if (size <= ti_adxr_start(options)) return TI_OKAY;
    const TI_REAL per = ((TI_REAL)period-1) / ((TI_REAL)period);
    const TI_REAL invper = 1.0 / ((TI_REAL)period);
    TI_REAL dmup = 0;
    TI_REAL dmdown = 0;
    int i;
    for (i = 1; i < period; ++i) {
        TI_REAL dp, dm;
        do { dp = high[i] - high[i-1]; dm = low[i-1] - low[i]; if (dp < 0) dp = 0; else if (dp > dm) dm = 0; if (dm < 0) dm = 0; else if (dm > dp) dp = 0;} while (0);
        dmup += dp;
        dmdown += dm;
    }
    TI_REAL adx = 0.0;
    {
        TI_REAL di_up = dmup;
        TI_REAL di_down = dmdown;
        TI_REAL dm_diff = fabs(di_up - di_down);
        TI_REAL dm_sum = di_up + di_down;
        TI_REAL dx = dm_diff / dm_sum * 100;
        adx += dx;
    }
    ti_buffer *adxr = ti_buffer_new(period-1);
    const int first_adxr = ti_adxr_start(options);
    for (i = period; i < size; ++i) {
        TI_REAL dp, dm;
        do { dp = high[i] - high[i-1]; dm = low[i-1] - low[i]; if (dp < 0) dp = 0; else if (dp > dm) dm = 0; if (dm < 0) dm = 0; else if (dm > dp) dp = 0;} while (0);
        dmup = dmup * per + dp;
        dmdown = dmdown * per + dm;
        TI_REAL di_up = dmup;
        TI_REAL di_down = dmdown;
        TI_REAL dm_diff = fabs(di_up - di_down);
        TI_REAL dm_sum = di_up + di_down;
        TI_REAL dx = dm_diff / dm_sum * 100;
        if (i-period < period-2) {
            adx += dx;
        } else if (i-period == period-2) {
            adx += dx;
            ti_buffer_qpush(adxr, adx * invper);
        } else {
            adx = adx * per + dx;
            if (i >= first_adxr) {
                *output++ = 0.5 * (adx * invper + ti_buffer_get(adxr, 1));
            }
            ti_buffer_qpush(adxr, adx * invper);
        }
    }
    ti_buffer_free(adxr);
    assert(output - outputs[0] == size - ti_adxr_start(options));
    return TI_OKAY;
}
int ti_ao_start(TI_REAL const *options) {
    (void)options;
    return 33;
}
int ti_ao(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const int period = 34;
    TI_REAL *output = outputs[0];
    if (size <= ti_ao_start(options)) return TI_OKAY;
    TI_REAL sum34 = 0;
    TI_REAL sum5 = 0;
    const TI_REAL per34 = 1.0 / 34.0;
    const TI_REAL per5 = 1.0 / 5.0;
    int i;
    for (i = 0; i < 34; ++i) {
        TI_REAL hl = 0.5 * (high[i] + low[i]);
        sum34 += hl;
        if (i >= 29) sum5 += hl;
    }
    *output++ = (per5 * sum5 - per34 * sum34);
    for (i = period; i < size; ++i) {
        TI_REAL hl = 0.5 * (high[i] + low[i]);
        sum34 += hl;
        sum5 += hl;
        sum34 -= 0.5 * (high[i-34] + low[i-34]);
        sum5 -= 0.5 * (high[i-5] + low[i-5]);
        *output++ = (per5 * sum5 - per34 * sum34);
    }
    assert(output - outputs[0] == size - ti_ao_start(options));
    return TI_OKAY;
}
int ti_apo_start(TI_REAL const *options) {
    (void)options;
    return 1;
}
int ti_apo(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    TI_REAL *apo = outputs[0];
    const int short_period = (int)options[0];
    const int long_period = (int)options[1];
    if (short_period < 1) return TI_INVALID_OPTION;
    if (long_period < 2) return TI_INVALID_OPTION;
    if (long_period < short_period) return TI_INVALID_OPTION;
    if (size <= ti_apo_start(options)) return TI_OKAY;
    TI_REAL short_per = 2 / ((TI_REAL)short_period + 1);
    TI_REAL long_per = 2 / ((TI_REAL)long_period + 1);
    TI_REAL short_ema = input[0];
    TI_REAL long_ema = input[0];
    int i;
    for (i = 1; i < size; ++i) {
        short_ema = (input[i]-short_ema) * short_per + short_ema;
        long_ema = (input[i]-long_ema) * long_per + long_ema;
        const TI_REAL out = short_ema - long_ema;
        *apo++ = out;
    }
    assert(apo - outputs[0] == size - ti_apo_start(options));
    return TI_OKAY;
}
int ti_aroon_start(TI_REAL const *options) {
    return (int)options[0];
}
int ti_aroon(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    TI_REAL *adown = outputs[0];
    TI_REAL *aup = outputs[1];
    const int period = (int)options[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_aroon_start(options)) return TI_OKAY;
    const TI_REAL scale = 100.0 / period;
    int trail = 0, maxi = -1, mini = -1;
    TI_REAL max = high[0];
    TI_REAL min = low[0];
    TI_REAL bar;
    int i, j;
    for (i = period; i < size; ++i, ++trail) {
        bar = high[i];
        if (maxi < trail) {
            maxi = trail;
            max = high[maxi];
            j = trail;
            while(++j <= i) {
                bar = high[j];
                if (bar >= max) {
                    max = bar;
                    maxi = j;
                }
            }
        } else if (bar >= max) {
            maxi = i;
            max = bar;
        }
        bar = low[i];
        if (mini < trail) {
            mini = trail;
            min = low[mini];
            j = trail;
            while(++j <= i) {
                bar = low[j];
                if (bar <= min) {
                    min = bar;
                    mini = j;
                }
            }
        } else if (bar <= min) {
            mini = i;
            min = bar;
        }
        *adown++ = ((TI_REAL)period - (i-mini)) * scale;
        *aup++ = ((TI_REAL)period - (i-maxi)) * scale;
    }
    assert(adown - outputs[0] == size - ti_aroon_start(options));
    assert(aup - outputs[1] == size - ti_aroon_start(options));
    return TI_OKAY;
}
int ti_aroonosc_start(TI_REAL const *options) {
    return (int)options[0];
}
int ti_aroonosc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    TI_REAL *output = outputs[0];
    const int period = (int)options[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_aroon_start(options)) return TI_OKAY;
    const TI_REAL scale = 100.0 / period;
    int trail = 0, maxi = -1, mini = -1;
    TI_REAL max = high[0];
    TI_REAL min = low[0];
    int i, j;
    for (i = period; i < size; ++i, ++trail) {
        TI_REAL bar = high[i];
        if (maxi < trail) {
            maxi = trail;
            max = high[maxi];
            j = trail;
            while(++j <= i) {
                bar = high[j];
                if (bar >= max) {
                    max = bar;
                    maxi = j;
                }
            }
        } else if (bar >= max) {
            maxi = i;
            max = bar;
        }
        bar = low[i];
        if (mini < trail) {
            mini = trail;
            min = low[mini];
            j = trail;
            while(++j <= i) {
                bar = low[j];
                if (bar <= min) {
                    min = bar;
                    mini = j;
                }
            }
        } else if (bar <= min) {
            mini = i;
            min = bar;
        }
        *output++ = (maxi-mini) * scale;
    }
    assert(output - outputs[0] == size - ti_aroonosc_start(options));
    return TI_OKAY;
}
int ti_asin_start(TI_REAL const *options) { (void)options; return 0; } int ti_asin(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (asin(in1[i])); } return TI_OKAY; }
int ti_atan_start(TI_REAL const *options) { (void)options; return 0; } int ti_atan(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (atan(in1[i])); } return TI_OKAY; }
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
        do{ const TI_REAL l = low[i]; const TI_REAL h = high[i]; const TI_REAL c = close[i-1]; const TI_REAL ych = fabs(h - c); const TI_REAL ycl = fabs(l - c); TI_REAL v = h - l; if (ych > v) v = ych; if (ycl > v) v = ycl; truerange = v;}while(0);
        sum += truerange;
    }
    TI_REAL val = sum / period;
    *output++ = val;
    for (i = period; i < size; ++i) {
        do{ const TI_REAL l = low[i]; const TI_REAL h = high[i]; const TI_REAL c = close[i-1]; const TI_REAL ych = fabs(h - c); const TI_REAL ycl = fabs(l - c); TI_REAL v = h - l; if (ych > v) v = ych; if (ycl > v) v = ycl; truerange = v;}while(0);
        val = (truerange-val) * per + val;
        *output++ = val;
    }
    assert(output - outputs[0] == size - ti_atr_start(options));
    return TI_OKAY;
}
int ti_atr_ref(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const int period = (int)options[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_atr_start(options)) return TI_OKAY;
    const int tr_start = ti_tr_start(options);
    const int tr_size = size - tr_start;
    TI_REAL *truerange = malloc((unsigned int)tr_size * sizeof(TI_REAL));
    if (!truerange) {return TI_OUT_OF_MEMORY;}
    TI_REAL *tr_outputs[1] = {truerange};
    const int tr_ret = ti_tr(size, inputs, 0, tr_outputs);
    if (tr_ret != TI_OKAY) {
        free(truerange);
        return tr_ret;
    }
    const TI_REAL *wilders_inputs[1] = {truerange};
    const int wilders_ret = ti_wilders(tr_size, wilders_inputs, options, outputs);
    free(truerange);
    assert(size - ti_atr_start(options) == size - ti_wilders_start(options));
    return wilders_ret;
}
typedef struct ti_stream_atr {
    int index;
    int progress;
    int period;
    TI_REAL sum;
    TI_REAL last;
    TI_REAL last_close;
} ti_stream_atr;
int ti_atr_stream_new(TI_REAL const *options, ti_stream **stream_in) {
    ti_stream_atr **stream = (ti_stream_atr**) stream_in;
    const int period = (int)options[0];
    if (period < 1) return TI_INVALID_OPTION;
    *stream = malloc(sizeof(ti_stream_atr));
    if (!*stream) {
        return TI_OUT_OF_MEMORY;
    }
    (*stream)->index = TI_INDICATOR_ATR_INDEX;
    (*stream)->progress = -ti_atr_start(options);
    (*stream)->period = period;
    (*stream)->sum = 0.0;
    return TI_OKAY;
}
int ti_atr_stream_run(ti_stream *stream_in, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    ti_stream_atr *stream = (ti_stream_atr*)stream_in;
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    TI_REAL *output = outputs[0];
    const TI_REAL per = 1.0 / ((TI_REAL)stream->period);
    const int start = -(stream->period-1);
    int i = 0;
    if (stream->progress < 1) {
        if (stream->progress == start) {
            stream->sum = high[0] - low[0];
            stream->last_close = close[0];
            ++stream->progress; ++i;
        }
        while (stream->progress <= 0 && i < size) {
            TI_REAL truerange; do { const TI_REAL ych = fabs((high[i]) - (stream->last_close)); const TI_REAL ycl = fabs((low[i]) - (stream->last_close)); TI_REAL v = (high[i]) - (low[i]); if (ych > v) v = ych; if (ycl > v) v = ycl; truerange = v; } while (0);
            stream->sum += truerange;
            stream->last_close = close[i];
            ++stream->progress; ++i;
        }
        if (stream->progress == 1) {
            const TI_REAL val = stream->sum * per;
            stream->last = val;
            *output++ = val;
        }
    }
    if (stream->progress >= 1) {
        TI_REAL val = stream->last;
        while (i < size) {
            TI_REAL truerange; do { const TI_REAL ych = fabs((high[i]) - (stream->last_close)); const TI_REAL ycl = fabs((low[i]) - (stream->last_close)); TI_REAL v = (high[i]) - (low[i]); if (ych > v) v = ych; if (ycl > v) v = ycl; truerange = v; } while (0);
            val = (truerange-val) * per + val;
            *output++ = val;
            stream->last_close = close[i];
            ++stream->progress; ++i;
        }
        stream->last = val;
    }
    return TI_OKAY;
}
void ti_atr_stream_free(ti_stream *stream) {
    free(stream);
}
int ti_avgprice_start(TI_REAL const *options) {
    (void)options;
    return 0;
}
int ti_avgprice(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *open = inputs[0];
    const TI_REAL *high = inputs[1];
    const TI_REAL *low = inputs[2];
    const TI_REAL *close = inputs[3];
    (void)options;
    TI_REAL *output = outputs[0];
    int i;
    for (i = 0; i < size; ++i) {
        output[i] = (open[i] + high[i] + low[i] + close[i]) * 0.25;
    }
    return TI_OKAY;
}
int ti_bbands_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_bbands(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    TI_REAL *lower = outputs[0];
    TI_REAL *middle = outputs[1];
    TI_REAL *upper = outputs[2];
    const int period = (int)options[0];
    const TI_REAL stddev = options[1];
    const TI_REAL scale = 1.0 / period;
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_bbands_start(options)) return TI_OKAY;
    TI_REAL sum = 0;
    TI_REAL sum2 = 0;
    int i;
    for (i = 0; i < period; ++i) {
        sum += input[i];
        sum2 += input[i] * input[i];
    }
    TI_REAL sd = sqrt(sum2 * scale - (sum * scale) * (sum * scale));
    *middle = sum * scale;
    *lower++ = *middle - stddev * sd;
    *upper++ = *middle + stddev * sd;
    ++middle;
    for (i = period; i < size; ++i) {
        sum += input[i];
        sum2 += input[i] * input[i];
        sum -= input[i-period];
        sum2 -= input[i-period] * input[i-period];
        sd = sqrt(sum2 * scale - (sum * scale) * (sum * scale));
        *middle = sum * scale;
        *upper++ = *middle + stddev * sd;
        *lower++ = *middle - stddev * sd;
        ++middle;
    }
    assert(lower - outputs[0] == size - ti_bbands_start(options));
    assert(middle - outputs[1] == size - ti_bbands_start(options));
    assert(upper - outputs[2] == size - ti_bbands_start(options));
    return TI_OKAY;
}
int ti_bop_start(TI_REAL const *options) {
    (void)options;
    return 0;
}
int ti_bop(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *open = inputs[0];
    const TI_REAL *high = inputs[1];
    const TI_REAL *low = inputs[2];
    const TI_REAL *close = inputs[3];
    (void)options;
    TI_REAL *output = outputs[0];
    int i;
    for (i = 0; i < size; ++i) {
        TI_REAL hl = high[i] - low[i];
        if (hl <= 0.0) {
            output[i] = 0;
        } else {
            output[i] = (close[i] - open[i]) / hl;
        }
    }
    return TI_OKAY;
}
int ti_cci_start(TI_REAL const *options) {
    const int period = (int)options[0];
    return (period-1) * 2;
}
int ti_cci(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    const int period = (int)options[0];
    const TI_REAL scale = 1.0 / period;
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_cci_start(options)) return TI_OKAY;
    TI_REAL *output = outputs[0];
    ti_buffer *sum = ti_buffer_new(period);
    int i, j;
    for (i = 0; i < size; ++i) {
        const TI_REAL today = ((high[(i)] + low[(i)] + close[(i)]) * (1.0/3.0));
        ti_buffer_push(sum, today);
        const TI_REAL avg = sum->sum * scale;
        if (i >= period * 2 - 2) {
            TI_REAL acc = 0;
            for (j = 0; j < period; ++j) {
                acc += fabs(avg - sum->vals[j]);
            }
            TI_REAL cci = acc * scale;
            cci *= .015;
            cci = (today-avg)/cci;
            *output++ = cci;
        }
    }
    ti_buffer_free(sum);
    assert(output - outputs[0] == size - ti_cci_start(options));
    return TI_OKAY;
}
int ti_ceil_start(TI_REAL const *options) { (void)options; return 0; } int ti_ceil(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (ceil(in1[i])); } return TI_OKAY; }
int ti_cmo_start(TI_REAL const *options) {
    return (int)options[0];
}
int ti_cmo(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    TI_REAL *output = outputs[0];
    const int period = (int)options[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_cmo_start(options)) return TI_OKAY;
    TI_REAL up_sum = 0, down_sum = 0;
    int i;
    for (i = 1; i <= period; ++i) {
        up_sum += (input[(i)] > input[(i)-1] ? input[(i)] - input[(i)-1] : 0);
        down_sum += (input[(i)] < input[(i)-1] ? input[(i)-1] - input[(i)] : 0);
    }
    *output++ = 100 * (up_sum - down_sum) / (up_sum + down_sum);
    for (i = period+1; i < size; ++i) {
        up_sum -= (input[(i-period)] > input[(i-period)-1] ? input[(i-period)] - input[(i-period)-1] : 0);
        down_sum -= (input[(i-period)] < input[(i-period)-1] ? input[(i-period)-1] - input[(i-period)] : 0);
        up_sum += (input[(i)] > input[(i)-1] ? input[(i)] - input[(i)-1] : 0);
        down_sum += (input[(i)] < input[(i)-1] ? input[(i)-1] - input[(i)] : 0);
        *output++ = 100 * (up_sum - down_sum) / (up_sum + down_sum);
    }
    assert(output - outputs[0] == size - ti_cmo_start(options));
    return TI_OKAY;
}
int ti_cos_start(TI_REAL const *options) { (void)options; return 0; } int ti_cos(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (cos(in1[i])); } return TI_OKAY; }
int ti_cosh_start(TI_REAL const *options) { (void)options; return 0; } int ti_cosh(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (cosh(in1[i])); } return TI_OKAY; }
int ti_crossany_start(TI_REAL const *options) {
    (void)options;
    return 1;
}
int ti_crossany(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *a = inputs[0];
    const TI_REAL *b = inputs[1];
    (void)options;
    TI_REAL *output = outputs[0];
    int i;
    for (i = 1; i < size; ++i) {
        *output++ = (a[i] > b[i] && a[i-1] <= b[i-1])
                 || (a[i] < b[i] && a[i-1] >= b[i-1]);
    }
    return TI_OKAY;
}
int ti_crossover_start(TI_REAL const *options) {
    (void)options;
    return 1;
}
int ti_crossover(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *a = inputs[0];
    const TI_REAL *b = inputs[1];
    (void)options;
    TI_REAL *output = outputs[0];
    int i;
    for (i = 1; i < size; ++i) {
        *output++ = a[i] > b[i] && a[i-1] <= b[i-1];
    }
    return TI_OKAY;
}
int ti_cvi_start(TI_REAL const *options) {
    const int n = (int)options[0];
    return n*2-1;
}
int ti_cvi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_cvi_start(options)) return TI_OKAY;
    const TI_REAL per = 2 / ((TI_REAL)period + 1);
    ti_buffer *lag = ti_buffer_new(period);
    TI_REAL val = high[0]-low[0];
    int i;
    for (i = 1; i < period*2-1; ++i) {
        val = ((high[i]-low[i])-val) * per + val;
        ti_buffer_qpush(lag, val);
    }
    for (i = period*2-1; i < size; ++i) {
        val = ((high[i]-low[i])-val) * per + val;
        const TI_REAL old = lag->vals[lag->index];
        *output++ = 100.0 * (val - old) / old;
        ti_buffer_qpush(lag, val);
    }
    ti_buffer_free(lag);
    assert(output - outputs[0] == size - ti_cvi_start(options));
    return TI_OKAY;
}
int ti_decay_start(TI_REAL const *options) {
    (void)options;
    return 0;
}
int ti_decay(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    TI_REAL *output = outputs[0];
    const int period = (int)options[0];
    const TI_REAL scale = 1.0 / period;
    *output++ = input[0];
    int i;
    for (i = 1; i < size; ++i) {
        TI_REAL d = output[-1] - scale;
        *output++ = input[i] > d ? input[i] : d;
    }
    return TI_OKAY;
}
int ti_dema_start(TI_REAL const *options) {
    const int period = (int)options[0];
    return (period-1) * 2;
}
int ti_dema(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_dema_start(options)) return TI_OKAY;
    const TI_REAL per = 2 / ((TI_REAL)period + 1);
    const TI_REAL per1 = 1.0 - per;
    TI_REAL ema = input[0];
    TI_REAL ema2 = ema;
    int i;
    for (i = 0; i < size; ++i) {
        ema = ema * per1 + input[i] * per;
        if (i == period-1) {
            ema2 = ema;
        }
        if (i >= period-1) {
            ema2 = ema2 * per1 + ema * per;
            if (i >= (period-1) * 2) {
                *output = ema * 2 - ema2;
                ++output;
            }
        }
    }
    assert(output - outputs[0] == size - ti_dema_start(options));
    return TI_OKAY;
}
int ti_di_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_di(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    const int period = (int)options[0];
    TI_REAL *plus_di = outputs[0];
    TI_REAL *minus_di = outputs[1];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_di_start(options)) return TI_OKAY;
    const TI_REAL per = ((TI_REAL)period-1) / ((TI_REAL)period);
    TI_REAL atr = 0;
    TI_REAL dmup = 0;
    TI_REAL dmdown = 0;
    int i;
    for (i = 1; i < period; ++i) {
        TI_REAL truerange;
        do{ const TI_REAL l = low[i]; const TI_REAL h = high[i]; const TI_REAL c = close[i-1]; const TI_REAL ych = fabs(h - c); const TI_REAL ycl = fabs(l - c); TI_REAL v = h - l; if (ych > v) v = ych; if (ycl > v) v = ycl; truerange = v;}while(0);
        atr += truerange;
        TI_REAL dp, dm;
        do { dp = high[i] - high[i-1]; dm = low[i-1] - low[i]; if (dp < 0) dp = 0; else if (dp > dm) dm = 0; if (dm < 0) dm = 0; else if (dm > dp) dp = 0;} while (0);
        dmup += dp;
        dmdown += dm;
    }
    *plus_di++ = 100.0 * dmup / atr;
    *minus_di++ = 100.0 * dmdown / atr;
    for (i = period; i < size; ++i) {
        TI_REAL truerange;
        do{ const TI_REAL l = low[i]; const TI_REAL h = high[i]; const TI_REAL c = close[i-1]; const TI_REAL ych = fabs(h - c); const TI_REAL ycl = fabs(l - c); TI_REAL v = h - l; if (ych > v) v = ych; if (ycl > v) v = ycl; truerange = v;}while(0);
        atr = atr * per + truerange;
        TI_REAL dp, dm;
        do { dp = high[i] - high[i-1]; dm = low[i-1] - low[i]; if (dp < 0) dp = 0; else if (dp > dm) dm = 0; if (dm < 0) dm = 0; else if (dm > dp) dp = 0;} while (0);
        dmup = dmup * per + dp;
        dmdown = dmdown * per + dm;
        *plus_di++ = 100.0 * dmup / atr;
        *minus_di++ = 100.0 * dmdown / atr;
    }
    assert(plus_di - outputs[0] == size - ti_di_start(options));
    assert(minus_di - outputs[1] == size - ti_di_start(options));
    return TI_OKAY;
}
int ti_div_start(TI_REAL const *options) { (void)options; return 0; } int ti_div(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; const TI_REAL *in2 = inputs[1]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (in1[i] / in2[i]); } return TI_OKAY; }
int ti_dm_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_dm(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const int period = (int)options[0];
    TI_REAL *plus_dm = outputs[0];
    TI_REAL *minus_dm = outputs[1];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_dm_start(options)) return TI_OKAY;
    const TI_REAL per = ((TI_REAL)period-1) / ((TI_REAL)period);
    TI_REAL dmup = 0;
    TI_REAL dmdown = 0;
    int i;
    for (i = 1; i < period; ++i) {
        TI_REAL dp, dm;
        do { dp = high[i] - high[i-1]; dm = low[i-1] - low[i]; if (dp < 0) dp = 0; else if (dp > dm) dm = 0; if (dm < 0) dm = 0; else if (dm > dp) dp = 0;} while (0);
        dmup += dp;
        dmdown += dm;
    }
    *plus_dm++ = dmup;
    *minus_dm++ = dmdown;
    for (i = period; i < size; ++i) {
        TI_REAL dp, dm;
        do { dp = high[i] - high[i-1]; dm = low[i-1] - low[i]; if (dp < 0) dp = 0; else if (dp > dm) dm = 0; if (dm < 0) dm = 0; else if (dm > dp) dp = 0;} while (0);
        dmup = dmup * per + dp;
        dmdown = dmdown * per + dm;
        *plus_dm++ = dmup;
        *minus_dm++ = dmdown;
    }
    assert(plus_dm - outputs[0] == size - ti_dm_start(options));
    assert(minus_dm - outputs[1] == size - ti_dm_start(options));
    return TI_OKAY;
}
int ti_dpo_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_dpo(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    const int back = period / 2 + 1;
    TI_REAL *output = outputs[0];
    const TI_REAL scale = 1.0 / period;
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_dpo_start(options)) return TI_OKAY;
    TI_REAL sum = 0;
    int i;
    for (i = 0; i < period; ++i) {
        sum += input[i];
    }
    *output++ = input[period-1-back] - (sum * scale);
    for (i = period; i < size; ++i) {
        sum += input[i];
        sum -= input[i-period];
        *output++ = input[i-back] - (sum * scale);
    }
    assert(output - outputs[0] == size - ti_dpo_start(options));
    return TI_OKAY;
}
int ti_dx_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_dx(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_dx_start(options)) return TI_OKAY;
    const TI_REAL per = ((TI_REAL)period-1) / ((TI_REAL)period);
    TI_REAL dmup = 0;
    TI_REAL dmdown = 0;
    int i;
    for (i = 1; i < period; ++i) {
        TI_REAL dp, dm;
        do { dp = high[i] - high[i-1]; dm = low[i-1] - low[i]; if (dp < 0) dp = 0; else if (dp > dm) dm = 0; if (dm < 0) dm = 0; else if (dm > dp) dp = 0;} while (0);
        dmup += dp;
        dmdown += dm;
    }
    {
        TI_REAL di_up = dmup;
        TI_REAL di_down = dmdown;
        TI_REAL dm_diff = fabs(di_up - di_down);
        TI_REAL dm_sum = di_up + di_down;
        TI_REAL dx = dm_diff / dm_sum * 100;
        *output++ = dx;
    }
    for (i = period; i < size; ++i) {
        TI_REAL dp, dm;
        do { dp = high[i] - high[i-1]; dm = low[i-1] - low[i]; if (dp < 0) dp = 0; else if (dp > dm) dm = 0; if (dm < 0) dm = 0; else if (dm > dp) dp = 0;} while (0);
        dmup = dmup * per + dp;
        dmdown = dmdown * per + dm;
        TI_REAL di_up = dmup;
        TI_REAL di_down = dmdown;
        TI_REAL dm_diff = fabs(di_up - di_down);
        TI_REAL dm_sum = di_up + di_down;
        TI_REAL dx = dm_diff / dm_sum * 100;
        *output++ = dx;
    }
    assert(output - outputs[0] == size - ti_dx_start(options));
    return TI_OKAY;
}
int ti_edecay_start(TI_REAL const *options) {
    (void)options;
    return 0;
}
int ti_edecay(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    const TI_REAL scale = 1.0 - 1.0 / period;
    *output++ = input[0];
    int i;
    for (i = 1; i < size; ++i) {
        TI_REAL d = output[-1] * scale;
        *output++ = input[i] > d ? input[i] : d;
    }
    return TI_OKAY;
}
int ti_ema_start(TI_REAL const *options) {
    (void)options;
    return 0;
}
int ti_ema(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_ema_start(options)) return TI_OKAY;
    const TI_REAL per = 2 / ((TI_REAL)period + 1);
    TI_REAL val = input[0];
    *output++ = val;
    int i;
    for (i = 1; i < size; ++i) {
        val = (input[i]-val) * per + val;
        *output++ = val;
    }
    assert(output - outputs[0] == size - ti_ema_start(options));
    return TI_OKAY;
}
int ti_emv_start(TI_REAL const *options) {
    (void)options;
    return 1;
}
int ti_emv(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *volume = inputs[2];
    (void)options;
    TI_REAL *output = outputs[0];
    if (size <= ti_emv_start(options)) return TI_OKAY;
    TI_REAL last = (high[0] + low[0]) * 0.5;
    int i;
    for (i = 1; i < size; ++i) {
        TI_REAL hl = (high[i] + low[i]) * 0.5;
        TI_REAL br = volume[i] / 10000.0 / (high[i] - low[i]);
        *output++ = (hl - last) / br;
        last = hl;
    }
    assert(output - outputs[0] == size - ti_emv_start(options));
    return TI_OKAY;
}
int ti_exp_start(TI_REAL const *options) { (void)options; return 0; } int ti_exp(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (exp(in1[i])); } return TI_OKAY; }
int ti_fisher_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_fisher(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    TI_REAL *fisher = outputs[0];
    TI_REAL *signal = outputs[1];
    const int period = (int)options[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_fisher_start(options)) return TI_OKAY;
    int trail = 0, maxi = -1, mini = -1;
    TI_REAL max = (0.5 * (high[(0)] + low[(0)]));
    TI_REAL min = (0.5 * (high[(0)] + low[(0)]));
    TI_REAL val1 = 0.0;
    TI_REAL bar;
    TI_REAL fish = 0.0;
    int i, j;
    for (i = period-1; i < size; ++i, ++trail) {
        bar = (0.5 * (high[(i)] + low[(i)]));
        if (maxi < trail) {
            maxi = trail;
            max = (0.5 * (high[(maxi)] + low[(maxi)]));
            j = trail;
            while(++j <= i) {
                bar = (0.5 * (high[(j)] + low[(j)]));
                if (bar >= max) {
                    max = bar;
                    maxi = j;
                }
            }
        } else if (bar >= max) {
            maxi = i;
            max = bar;
        }
        bar = (0.5 * (high[(i)] + low[(i)]));
        if (mini < trail) {
            mini = trail;
            min = (0.5 * (high[(mini)] + low[(mini)]));
            j = trail;
            while(++j <= i) {
                bar = (0.5 * (high[(j)] + low[(j)]));
                if (bar <= min) {
                    min = bar;
                    mini = j;
                }
            }
        } else if (bar <= min) {
            mini = i;
            min = bar;
        }
        TI_REAL mm = max - min;
        if (mm == 0.0) mm = 0.001;
        val1 = 0.33 * 2.0 * ( ((0.5 * (high[(i)] + low[(i)]))-min) / (mm) - 0.5) + 0.67 * val1;
        if (val1 > 0.99) val1 = .999;
        if (val1 < -0.99) val1 = -.999;
        *signal++ = fish;
        fish = 0.5 * log((1.0+val1)/(1.0-val1)) + 0.5 * fish;
        *fisher++ = fish;
    }
    assert(fisher - outputs[0] == size - ti_fisher_start(options));
    assert(signal - outputs[1] == size - ti_fisher_start(options));
    return TI_OKAY;
}
int ti_floor_start(TI_REAL const *options) { (void)options; return 0; } int ti_floor(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (floor(in1[i])); } return TI_OKAY; }
int ti_fosc_start(TI_REAL const *options) {
    return (int)options[0];
}
int ti_fosc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_fosc_start(options)) return TI_OKAY;
    do { TI_REAL x = 0; TI_REAL x2 = 0; TI_REAL y = 0; TI_REAL xy = 0; const TI_REAL p = (1.0 / (period)); TI_REAL tsf = 0;; int i; for (i = 0; i < (period)-1; ++i) { x += i+1; x2 += (i+1)*(i+1); xy += (input)[i] * (i+1); y += (input)[i]; } x += (period); x2 += (period) * (period); const TI_REAL bd = 1.0 / ((period) * x2 - x * x); for (i = (period)-1; i < (size); ++i) { xy += (input)[i] * (period); y += (input)[i]; const TI_REAL b = ((period) * xy - x * y) * bd; do { const TI_REAL a = (y - b * x) * p; if (i >= (period)) {*output++ = 100 * (input[i] - tsf) / input[i];} tsf = (a + b * ((period+1))); } while (0); xy -= y; y -= (input)[i-(period)+1]; } } while (0);
    assert(output - outputs[0] == size - ti_fosc_start(options));
    return TI_OKAY;
}
int ti_hma_start(TI_REAL const *options) {
    const int period = (int)options[0];
    const int periodsqrt = (int)(sqrt(period));
    return period + periodsqrt - 2;
}
int ti_hma(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_hma_start(options)) return TI_OKAY;
    const int period2 = (int)(period / 2);
    const int periodsqrt = (int)(sqrt(period));
    const TI_REAL weights = period * (period+1) / 2;
    const TI_REAL weights2 = period2 * (period2+1) / 2;
    const TI_REAL weightssqrt = periodsqrt * (periodsqrt+1) / 2;
    TI_REAL sum = 0;
    TI_REAL weight_sum = 0;
    TI_REAL sum2 = 0;
    TI_REAL weight_sum2 = 0;
    TI_REAL sumsqrt = 0;
    TI_REAL weight_sumsqrt = 0;
    int i;
    for (i = 0; i < period-1; ++i) {
        weight_sum += input[i] * (i+1);
        sum += input[i];
        if (i >= period - period2) {
            weight_sum2 += input[i] * (i+1-(period-period2));
            sum2 += input[i];
        }
    }
    ti_buffer *buff = ti_buffer_new(periodsqrt);
    for (i = period-1; i < size; ++i) {
        weight_sum += input[i] * period;
        sum += input[i];
        weight_sum2 += input[i] * period2;
        sum2 += input[i];
        const TI_REAL wma = weight_sum / weights;
        const TI_REAL wma2 = weight_sum2 / weights2;
        const TI_REAL diff = 2 * wma2 - wma;
        weight_sumsqrt += diff * periodsqrt;
        sumsqrt += diff;
        ti_buffer_qpush(buff, diff);
        if (i >= (period-1) + (periodsqrt-1)) {
            *output++ = weight_sumsqrt / weightssqrt;
            weight_sumsqrt -= sumsqrt;
            sumsqrt -= ti_buffer_get(buff, 1);
        } else {
            weight_sumsqrt -= sumsqrt;
        }
        weight_sum -= sum;
        sum -= input[i-period+1];
        weight_sum2 -= sum2;
        sum2 -= input[i-period2+1];
    }
    ti_buffer_free(buff);
    assert(output - outputs[0] == size - ti_hma_start(options));
    return TI_OKAY;
}
int ti_kama_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_kama(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_kama_start(options)) return TI_OKAY;
    const TI_REAL short_per = 2 / (2.0 + 1);
    const TI_REAL long_per = 2 / (30.0 + 1);
    TI_REAL sum = 0;
    int i;
    for (i = 1; i < period; ++i) {
        sum += fabs(input[i] - input[i-1]);
    }
    TI_REAL kama = input[period-1];
    *output++ = kama;
    TI_REAL er, sc;
    for (i = period; i < size; ++i) {
        sum += fabs(input[i] - input[i-1]);
        if (i > period) {
            sum -= fabs(input[i-period] - input[i-period-1]);
        }
        if (sum != 0.0) {
            er = fabs(input[i] - input[i-period]) / sum;
        } else {
            er = 1.0;
        }
        sc = pow(er * (short_per - long_per) + long_per, 2);
        kama = kama + sc * (input[i] - kama);
        *output++ = kama;
    }
    assert(output - outputs[0] == size - ti_kama_start(options));
    return TI_OKAY;
}
int ti_kvo_start(TI_REAL const *options) {
    (void)options;
    return 1;
}
int ti_kvo(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    const TI_REAL *volume = inputs[3];
    const int short_period = (int)options[0];
    const int long_period = (int)options[1];
    if (short_period < 1) return TI_INVALID_OPTION;
    if (long_period < short_period) return TI_INVALID_OPTION;
    if (size <= ti_kvo_start(options)) return TI_OKAY;
    const TI_REAL short_per = 2 / ((TI_REAL)short_period + 1);
    const TI_REAL long_per = 2 / ((TI_REAL)long_period + 1);
    TI_REAL *output = outputs[0];
    TI_REAL cm = 0;
    TI_REAL prev_hlc = high[0] + low[0] + close[0];
    int trend = -1;
    TI_REAL short_ema = 0, long_ema = 0;
    int i;
    for (i = 1; i < size; ++i) {
        const TI_REAL hlc = high[i] + low[i] + close[i];
        const TI_REAL dm = high[i] - low[i];
        if (hlc > prev_hlc && trend != 1) {
            trend = 1;
            cm = high[i-1] - low[i-1];
        } else if (hlc < prev_hlc && trend != 0) {
            trend = 0;
            cm = high[i-1] - low[i-1];
        }
        cm += dm;
        const TI_REAL vf = volume[i] * fabs(dm / cm * 2 - 1) * 100 * (trend ? 1.0 : -1.0);
        if (i == 1) {
            short_ema = vf;
            long_ema = vf;
        } else {
            short_ema = (vf-short_ema) * short_per + short_ema;
            long_ema = (vf-long_ema) * long_per + long_ema;
        }
        *output++ = short_ema - long_ema;
        prev_hlc = hlc;
    }
    assert(output - outputs[0] == size - ti_kvo_start(options));
    return TI_OKAY;
}
int ti_lag_start(TI_REAL const *options) {
    return (int)options[0];
}
int ti_lag(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 0) return TI_INVALID_OPTION;
    if (size <= ti_lag_start(options)) return TI_OKAY;
    int i;
    for (i = period; i < size; ++i) {
        *output++ = input[i-period];
    }
    assert(output - outputs[0] == size - ti_lag_start(options));
    return TI_OKAY;
}
int ti_linreg_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_linreg(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_linreg_start(options)) return TI_OKAY;
    do { TI_REAL x = 0; TI_REAL x2 = 0; TI_REAL y = 0; TI_REAL xy = 0; const TI_REAL p = (1.0 / (period)); int i; for (i = 0; i < (period)-1; ++i) { x += i+1; x2 += (i+1)*(i+1); xy += (input)[i] * (i+1); y += (input)[i]; } x += (period); x2 += (period) * (period); const TI_REAL bd = 1.0 / ((period) * x2 - x * x); for (i = (period)-1; i < (size); ++i) { xy += (input)[i] * (period); y += (input)[i]; const TI_REAL b = ((period) * xy - x * y) * bd; do { const TI_REAL a = (y - b * x) * p; *output++ = (a + b * ((period))); } while (0); xy -= y; y -= (input)[i-(period)+1]; } } while (0);
    assert(output - outputs[0] == size - ti_linreg_start(options));
    return TI_OKAY;
}
int ti_linregintercept_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_linregintercept(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_linregintercept_start(options)) return TI_OKAY;
    do { TI_REAL x = 0; TI_REAL x2 = 0; TI_REAL y = 0; TI_REAL xy = 0; const TI_REAL p = (1.0 / (period)); int i; for (i = 0; i < (period)-1; ++i) { x += i+1; x2 += (i+1)*(i+1); xy += (input)[i] * (i+1); y += (input)[i]; } x += (period); x2 += (period) * (period); const TI_REAL bd = 1.0 / ((period) * x2 - x * x); for (i = (period)-1; i < (size); ++i) { xy += (input)[i] * (period); y += (input)[i]; const TI_REAL b = ((period) * xy - x * y) * bd; do { const TI_REAL a = (y - b * x) * p; *output++ = (a + b * ((1))); } while (0); xy -= y; y -= (input)[i-(period)+1]; } } while (0);
    assert(output - outputs[0] == size - ti_linregintercept_start(options));
    return TI_OKAY;
}
int ti_linregslope_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_linregslope(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_linregslope_start(options)) return TI_OKAY;
    do { TI_REAL x = 0; TI_REAL x2 = 0; TI_REAL y = 0; TI_REAL xy = 0; do{}while(0); int i; for (i = 0; i < (period)-1; ++i) { x += i+1; x2 += (i+1)*(i+1); xy += (input)[i] * (i+1); y += (input)[i]; } x += (period); x2 += (period) * (period); const TI_REAL bd = 1.0 / ((period) * x2 - x * x); for (i = (period)-1; i < (size); ++i) { xy += (input)[i] * (period); y += (input)[i]; const TI_REAL b = ((period) * xy - x * y) * bd; do { *output++ = b; } while (0); xy -= y; y -= (input)[i-(period)+1]; } } while (0);
    assert(output - outputs[0] == size - ti_linregslope_start(options));
    return TI_OKAY;
}
int ti_ln_start(TI_REAL const *options) { (void)options; return 0; } int ti_ln(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (log(in1[i])); } return TI_OKAY; }
int ti_log10_start(TI_REAL const *options) { (void)options; return 0; } int ti_log10(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (log10(in1[i])); } return TI_OKAY; }
int ti_macd_start(TI_REAL const *options) {
    const int long_period = (int)options[1];
    return (long_period-1);
}
int ti_macd(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    TI_REAL *macd = outputs[0];
    TI_REAL *signal = outputs[1];
    TI_REAL *hist = outputs[2];
    const int short_period = (int)options[0];
    const int long_period = (int)options[1];
    const int signal_period = (int)options[2];
    if (short_period < 1) return TI_INVALID_OPTION;
    if (long_period < 2) return TI_INVALID_OPTION;
    if (long_period < short_period) return TI_INVALID_OPTION;
    if (signal_period < 1) return TI_INVALID_OPTION;
    if (size <= ti_macd_start(options)) return TI_OKAY;
    TI_REAL short_per = 2 / ((TI_REAL)short_period + 1);
    TI_REAL long_per = 2 / ((TI_REAL)long_period + 1);
    TI_REAL signal_per = 2 / ((TI_REAL)signal_period + 1);
    if (short_period == 12 && long_period == 26) {
        short_per = 0.15;
        long_per = 0.075;
    }
    TI_REAL short_ema = input[0];
    TI_REAL long_ema = input[0];
    TI_REAL signal_ema = 0;
    int i;
    for (i = 1; i < size; ++i) {
        short_ema = (input[i]-short_ema) * short_per + short_ema;
        long_ema = (input[i]-long_ema) * long_per + long_ema;
        const TI_REAL out = short_ema - long_ema;
        if (i == long_period-1) {
            signal_ema = out;
        }
        if (i >= long_period-1) {
            signal_ema = (out-signal_ema) * signal_per + signal_ema;
            *macd++ = out;
            *signal++ = signal_ema;
            *hist++ = out - signal_ema;
        }
    }
    assert(macd - outputs[0] == size - ti_macd_start(options));
    assert(signal - outputs[1] == size - ti_macd_start(options));
    assert(hist - outputs[2] == size - ti_macd_start(options));
    return TI_OKAY;
}
int ti_marketfi_start(TI_REAL const *options) {
    (void)options;
    return 0;
}
int ti_marketfi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *volume = inputs[2];
    (void)options;
    TI_REAL *output = outputs[0];
    if (size <= ti_marketfi_start(options)) return TI_OKAY;
    int i;
    for (i = 0; i < size; ++i) {
        *output++ = (high[i] - low[i]) / volume[i];
    }
    assert(output - outputs[0] == size - ti_marketfi_start(options));
    return TI_OKAY;
}
int ti_mass_start(TI_REAL const *options) {
    int sum_p = (int)options[0]-1;
    return 16 + sum_p;
}
int ti_mass(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_mass_start(options)) return TI_OKAY;
    const TI_REAL per = 2 / (9.0 + 1);
    const TI_REAL per1 = 1.0 - per;
    TI_REAL ema = high[0] - low[0];
    TI_REAL ema2 = ema;
    ti_buffer *sum = ti_buffer_new(period);
    int i;
    for (i = 0; i < size; ++i) {
        TI_REAL hl = high[i] - low[i];
        ema = ema * per1 + hl * per;
        if (i == 8) {
            ema2 = ema;
        }
        if (i >= 8) {
            ema2 = ema2 * per1 + ema * per;
            if (i >= 16) {
                ti_buffer_push(sum, ema/ema2);
                if (i >= 16 + period - 1) {
                    *output++ = sum->sum;
                }
            }
        }
    }
    ti_buffer_free(sum);
    assert(output - outputs[0] == size - ti_mass_start(options));
    return TI_OKAY;
}
int ti_max_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_max(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_max_start(options)) return TI_OKAY;
    int trail = 0, maxi = -1;
    TI_REAL max = input[0];
    int i, j;
    for (i = period-1; i < size; ++i, ++trail) {
        TI_REAL bar = input[i];
        if (maxi < trail) {
            maxi = trail;
            max = input[maxi];
            j = trail;
            while(++j <= i) {
                bar = input[j];
                if (bar >= max) {
                    max = bar;
                    maxi = j;
                }
            }
        } else if (bar >= max) {
            maxi = i;
            max = bar;
        }
        *output++ = max;
    }
    assert(output - outputs[0] == size - ti_max_start(options));
    return TI_OKAY;
}
int ti_max_ref(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_max_start(options)) return TI_OKAY;
    for (int i = period-1; i < size; ++i) {
        TI_REAL max = input[i-period+1];
        for (int j = i-period+2; j <= i; ++j) {
            max = ((max) > (input[j]) ? (max) : (input[j]));
        }
        *output++ = max;
    }
    return TI_OKAY;
}
int ti_md_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_md(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    const TI_REAL scale = 1.0 / period;
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_md_start(options)) return TI_OKAY;
    TI_REAL sum = 0;
    int i, j;
    for (i = 0; i < size; ++i) {
        const TI_REAL today = input[i];
        sum += today;
        if (i >= period) sum -= input[i-period];
        const TI_REAL avg = sum * scale;
        if (i >= period - 1) {
            TI_REAL acc = 0;
            for (j = 0; j < period; ++j) {
                acc += fabs(avg - input[i-j]);
            }
            *output++ = acc * scale;
        }
    }
    assert(output - outputs[0] == size - ti_md_start(options));
    return TI_OKAY;
}
int ti_medprice_start(TI_REAL const *options) {
    (void)options;
    return 0;
}
int ti_medprice(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    (void)options;
    TI_REAL *output = outputs[0];
    int i;
    for (i = 0; i < size; ++i) {
        output[i] = (high[i] + low[i]) * 0.5;
    }
    return TI_OKAY;
}
int ti_mfi_start(TI_REAL const *options) {
    return (int)options[0];
}
int ti_mfi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    const TI_REAL *volume = inputs[3];
    const int period = (int)options[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_mfi_start(options)) return TI_OKAY;
    TI_REAL *output = outputs[0];
    TI_REAL ytyp = ((high[(0)] + low[(0)] + close[(0)]) * (1.0/3.0));
    int i;
    ti_buffer *up = ti_buffer_new(period);
    ti_buffer *down = ti_buffer_new(period);
    for (i = 1; i < size; ++i) {
        const TI_REAL typ = ((high[(i)] + low[(i)] + close[(i)]) * (1.0/3.0));
        const TI_REAL bar = typ * volume[i];
        if (typ > ytyp) {
            ti_buffer_push(up, bar);
            ti_buffer_push(down, 0.0);
        } else if (typ < ytyp) {
            ti_buffer_push(down, bar);
            ti_buffer_push(up, 0.0);
        } else {
            ti_buffer_push(up, 0.0);
            ti_buffer_push(down, 0.0);
        }
        ytyp = typ;
        if (i >= period) {
            *output++ = up->sum / (up->sum + down->sum) * 100.0;
        }
    }
    ti_buffer_free(up);
    ti_buffer_free(down);
    assert(output - outputs[0] == size - ti_mfi_start(options));
    return TI_OKAY;
}
int ti_min_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_min(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_min_start(options)) return TI_OKAY;
    int trail = 0, mini = -1;
    TI_REAL min = input[0];
    int i, j;
    for (i = period-1; i < size; ++i, ++trail) {
        TI_REAL bar = input[i];
        if (mini < trail) {
            mini = trail;
            min = input[mini];
            j = trail;
            while(++j <= i) {
                bar = input[j];
                if (bar <= min) {
                    min = bar;
                    mini = j;
                }
            }
        } else if (bar <= min) {
            mini = i;
            min = bar;
        }
        *output++ = min;
    }
    assert(output - outputs[0] == size - ti_min_start(options));
    return TI_OKAY;
}
int ti_min_ref(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_min_start(options)) return TI_OKAY;
    for (int i = period-1; i < size; ++i) {
        TI_REAL min = input[i-period+1];
        for (int j = i-period+2; j <= i; ++j) {
            min = ((min) < (input[j]) ? (min) : (input[j]));
        }
        *output++ = min;
    }
    assert(output - outputs[0] == size - ti_min_start(options));
    return TI_OKAY;
}
int ti_mom_start(TI_REAL const *options) {
    return (int)options[0];
}
int ti_mom(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_mom_start(options)) return TI_OKAY;
    int i;
    for (i = period; i < size; ++i) {
        *output++ = input[i] - input[i-period];
    }
    assert(output - outputs[0] == size - ti_mom_start(options));
    return TI_OKAY;
}
int ti_msw_start(TI_REAL const *options) {
    return (int)options[0];
}
int ti_msw(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    TI_REAL *sine = outputs[0];
    TI_REAL *lead = outputs[1];
    const int period = (int)options[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_msw_start(options)) return TI_OKAY;
    const TI_REAL pi = 3.1415926;
    const TI_REAL tpi = 2 * pi;
    TI_REAL weight = 0, phase;
    TI_REAL rp, ip;
    int i, j;
    for (i = period; i < size; ++i) {
        rp = 0;
        ip = 0;
        for (j = 0; j < period; ++j) {
            weight = input[i-j];
            rp = rp + cos(tpi * j / period) * weight;
            ip = ip + sin(tpi * j / period) * weight;
        }
        if (fabs(rp) > .001) {
            phase = atan(ip/rp);
        } else {
            phase = tpi / 2.0 * (ip < 0 ? -1.0 : 1.0);
        }
        if (rp < 0.0) phase += pi;
        phase += pi/2.0;
        if (phase < 0.0) phase += tpi;
        if (phase > tpi) phase -= tpi;
        *sine++ = sin(phase);
        *lead++ = sin(phase + pi/4.0);
    }
    assert(sine - outputs[0] == size - ti_msw_start(options));
    assert(lead - outputs[1] == size - ti_msw_start(options));
    return TI_OKAY;
}
int ti_mul_start(TI_REAL const *options) { (void)options; return 0; } int ti_mul(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; const TI_REAL *in2 = inputs[1]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (in1[i] * in2[i]); } return TI_OKAY; }
int ti_natr_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_natr(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_natr_start(options)) return TI_OKAY;
    const TI_REAL per = 1.0 / ((TI_REAL)period);
    TI_REAL sum = 0;
    TI_REAL truerange;
    sum += high[0] - low[0];
    int i;
    for (i = 1; i < period; ++i) {
        do{ const TI_REAL l = low[i]; const TI_REAL h = high[i]; const TI_REAL c = close[i-1]; const TI_REAL ych = fabs(h - c); const TI_REAL ycl = fabs(l - c); TI_REAL v = h - l; if (ych > v) v = ych; if (ycl > v) v = ycl; truerange = v;}while(0);
        sum += truerange;
    }
    TI_REAL val = sum / period;
    *output++ = 100 * (val) / close[period-1];
    for (i = period; i < size; ++i) {
        do{ const TI_REAL l = low[i]; const TI_REAL h = high[i]; const TI_REAL c = close[i-1]; const TI_REAL ych = fabs(h - c); const TI_REAL ycl = fabs(l - c); TI_REAL v = h - l; if (ych > v) v = ych; if (ycl > v) v = ycl; truerange = v;}while(0);
        val = (truerange-val) * per + val;
        *output++ = 100 * (val) / close[i];
    }
    assert(output - outputs[0] == size - ti_natr_start(options));
    return TI_OKAY;
}
int ti_nvi_start(TI_REAL const *options) {
    (void)options;
    return 0;
}
int ti_nvi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *close = inputs[0];
    const TI_REAL *volume = inputs[1];
    (void)options;
    TI_REAL *output = outputs[0];
    if (size <= ti_nvi_start(options)) return TI_OKAY;
    TI_REAL nvi = 1000;
    *output++ = nvi;
    int i;
    for (i = 1; i < size; ++i) {
        if (volume[i] < volume[i-1]) {
            nvi += ((close[i] - close[i-1])/close[i-1]) * nvi;
        }
        *output++ = nvi;
    }
    assert(output - outputs[0] == size - ti_nvi_start(options));
    return TI_OKAY;
}
int ti_obv_start(TI_REAL const *options) {
    (void)options;
    return 0;
}
int ti_obv(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *close = inputs[0];
    const TI_REAL *volume = inputs[1];
    (void)options;
    TI_REAL *output = outputs[0];
    TI_REAL sum = 0;
    *output++ = sum;
    TI_REAL prev = close[0];
    int i;
    for (i = 1; i < size; ++i) {
        if (close[i] > prev) {
            sum += volume[i];
        } else if (close[i] < prev) {
            sum -= volume[i];
        } else {
        }
        prev = close[i];
        *output++ = sum;
    }
    return TI_OKAY;
}
int ti_ppo_start(TI_REAL const *options) {
    (void)options;
    return 1;
}
int ti_ppo(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    TI_REAL *ppo = outputs[0];
    const int short_period = (int)options[0];
    const int long_period = (int)options[1];
    if (short_period < 1) return TI_INVALID_OPTION;
    if (long_period < 2) return TI_INVALID_OPTION;
    if (long_period < short_period) return TI_INVALID_OPTION;
    if (size <= ti_ppo_start(options)) return TI_OKAY;
    TI_REAL short_per = 2 / ((TI_REAL)short_period + 1);
    TI_REAL long_per = 2 / ((TI_REAL)long_period + 1);
    TI_REAL short_ema = input[0];
    TI_REAL long_ema = input[0];
    int i;
    for (i = 1; i < size; ++i) {
        short_ema = (input[i]-short_ema) * short_per + short_ema;
        long_ema = (input[i]-long_ema) * long_per + long_ema;
        const TI_REAL out = 100.0 * (short_ema - long_ema) / long_ema;
        *ppo++ = out;
    }
    assert(ppo - outputs[0] == size - ti_ppo_start(options));
    return TI_OKAY;
}
int ti_psar_start(TI_REAL const *options) {
    (void)options;
    return 1;
}
int ti_psar(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL accel_step = options[0];
    const TI_REAL accel_max = options[1];
    TI_REAL *output = outputs[0];
    if (accel_step <= 0) return TI_INVALID_OPTION;
    if (accel_max <= accel_step) return TI_INVALID_OPTION;
    if (size < 2) return TI_OKAY;
    int lng;
    if (high[0] + low[0] <= high[1] + low[1])
        lng = 1;
    else
        lng = 0;
    TI_REAL sar, extreme;
    if (lng) {
        extreme = high[0];
        sar = low[0];
    } else {
        extreme = low[0];
        sar = high[0];
    }
    TI_REAL accel = accel_step;
    int i;
    for (i = 1; i < size; ++i) {
        sar = (extreme - sar) * accel + sar;
        if (lng) {
            if (i >= 2 && (sar > low[i-2])) sar = low[i-2];
            if ((sar > low[i-1])) sar = low[i-1];
            if (accel < accel_max && high[i] > extreme) {
                accel += accel_step;
                if (accel > accel_max) accel = accel_max;
            }
            if (high[i] > extreme) extreme = high[i];
        } else {
            if (i >= 2 && (sar < high[i-2])) sar = high[i-2];
            if ((sar < high[i-1])) sar = high[i-1];
            if (accel < accel_max && low[i] < extreme) {
                accel += accel_step;
                if (accel > accel_max) accel = accel_max;
            }
            if (low[i] < extreme) extreme = low[i];
        }
        if ((lng && low[i] < sar) || (!lng && high[i] > sar)) {
            accel = accel_step;
            sar = extreme;
            lng = !lng;
            if (!lng) extreme = low[i];
            else extreme = high[i];
        }
        *output++ = sar;
    }
    assert(output - outputs[0] == size - ti_psar_start(options));
    return TI_OKAY;
}
int ti_pvi_start(TI_REAL const *options) {
    (void)options;
    return 0;
}
int ti_pvi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *close = inputs[0];
    const TI_REAL *volume = inputs[1];
    (void)options;
    TI_REAL *output = outputs[0];
    if (size <= ti_pvi_start(options)) return TI_OKAY;
    TI_REAL pvi = 1000;
    *output++ = pvi;
    int i;
    for (i = 1; i < size; ++i) {
        if (volume[i] > volume[i-1]) {
            pvi += ((close[i] - close[i-1])/close[i-1]) * pvi;
        }
        *output++ = pvi;
    }
    assert(output - outputs[0] == size - ti_pvi_start(options));
    return TI_OKAY;
}
int ti_qstick_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_qstick(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *open = inputs[0];
    const TI_REAL *close = inputs[1];
    TI_REAL *output = outputs[0];
    const int period = (int)options[0];
    const TI_REAL scale = 1.0 / period;
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_qstick_start(options)) return TI_OKAY;
    TI_REAL sum = 0;
    int i;
    for (i = 0; i < period; ++i) {
        sum += close[i] - open[i];
    }
    *output++ = sum * scale;
    for (i = period; i < size; ++i) {
        sum += close[i] - open[i];
        sum -= close[i-period] - open[i-period];
        *output++ = sum * scale;
    }
    assert(output - outputs[0] == size - ti_qstick_start(options));
    return TI_OKAY;
}
int ti_roc_start(TI_REAL const *options) {
    return (int)options[0];
}
int ti_roc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_roc_start(options)) return TI_OKAY;
    int i;
    for (i = period; i < size; ++i) {
        *output++ = (input[i] - input[i-period]) / input[i-period];
    }
    assert(output - outputs[0] == size - ti_roc_start(options));
    return TI_OKAY;
}
int ti_rocr_start(TI_REAL const *options) {
    return (int)options[0];
}
int ti_rocr(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_rocr_start(options)) return TI_OKAY;
    int i;
    for (i = period; i < size; ++i) {
        *output++ = input[i] / input[i-period];
    }
    assert(output - outputs[0] == size - ti_rocr_start(options));
    return TI_OKAY;
}
int ti_round_start(TI_REAL const *options) { (void)options; return 0; } int ti_round(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (floor(in1[i] + 0.5)); } return TI_OKAY; }
int ti_rsi_start(TI_REAL const *options) {
    return (int)options[0];
}
int ti_rsi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    const TI_REAL per = 1.0 / ((TI_REAL)period);
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_rsi_start(options)) return TI_OKAY;
    TI_REAL smooth_up = 0, smooth_down = 0;
    int i;
    for (i = 1; i <= period; ++i) {
        const TI_REAL upward = input[i] > input[i-1] ? input[i] - input[i-1] : 0;
        const TI_REAL downward = input[i] < input[i-1] ? input[i-1] - input[i] : 0;
        smooth_up += upward;
        smooth_down += downward;
    }
    smooth_up /= period;
    smooth_down /= period;
    *output++ = 100.0 * (smooth_up / (smooth_up + smooth_down));
    for (i = period+1; i < size; ++i) {
        const TI_REAL upward = input[i] > input[i-1] ? input[i] - input[i-1] : 0;
        const TI_REAL downward = input[i] < input[i-1] ? input[i-1] - input[i] : 0;
        smooth_up = (upward-smooth_up) * per + smooth_up;
        smooth_down = (downward-smooth_down) * per + smooth_down;
        *output++ = 100.0 * (smooth_up / (smooth_up + smooth_down));
    }
    assert(output - outputs[0] == size - ti_rsi_start(options));
    return TI_OKAY;
}
int ti_sin_start(TI_REAL const *options) { (void)options; return 0; } int ti_sin(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (sin(in1[i])); } return TI_OKAY; }
int ti_sinh_start(TI_REAL const *options) { (void)options; return 0; } int ti_sinh(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (sinh(in1[i])); } return TI_OKAY; }
int ti_sma_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_sma(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    const TI_REAL scale = 1.0 / period;
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_sma_start(options)) return TI_OKAY;
    TI_REAL sum = 0;
    int i;
    for (i = 0; i < period; ++i) {
        sum += input[i];
    }
    *output++ = sum * scale;
    for (i = period; i < size; ++i) {
        sum += input[i];
        sum -= input[i-period];
        *output++ = sum * scale;
    }
    assert(output - outputs[0] == size - ti_sma_start(options));
    return TI_OKAY;
}
typedef struct ti_stream_sma {
    int index;
    int progress;
    int period;
    TI_REAL per;
    TI_REAL sum;
    int buffer_idx;
    TI_REAL buffer[];
} ti_stream_sma;
int ti_sma_stream_new(TI_REAL const *options, ti_stream **stream_in) {
    ti_stream_sma **stream = (ti_stream_sma**)stream_in;
    int period = (int)options[0];
    if (period < 1) return TI_INVALID_OPTION;
    *stream = malloc(sizeof(ti_stream_sma) + sizeof(TI_REAL[period]));
    if (!stream) {
        return TI_OUT_OF_MEMORY;
    }
    (*stream)->index = TI_INDICATOR_SMA_INDEX;
    (*stream)->progress = -ti_sma_start(options);
    (*stream)->period = period;
    (*stream)->per = 1. / period;
    (*stream)->sum = 0.;
    (*stream)->buffer_idx = -1;
    return TI_OKAY;
}
int ti_sma_stream_run(ti_stream *stream_in, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    ti_stream_sma *stream = (ti_stream_sma*)stream_in;
    int progress = stream->progress;
    const TI_REAL *real = inputs[0];
    int period = stream->period;
    TI_REAL *sma = outputs[0];
    TI_REAL per = stream->per;
    TI_REAL sum = stream->sum;
    int buffer_idx = stream->buffer_idx;
    TI_REAL *buffer = stream->buffer;
    if (progress == -period + 1) {} else {}
    int i;
    for (i = 0; progress < 1 && i < size; ++i, ++progress) {
        { ++buffer_idx; sum += buffer[buffer_idx] = real[i] * per; };
    }
    if (i > 0 && progress == 1) {
        *sma++ = sum;
    }
    for (; i < size; ++i, ++progress) {
        { ++buffer_idx; if (buffer_idx == period) { buffer_idx = 0; } sum -= buffer[buffer_idx]; sum += buffer[buffer_idx] = real[i] * per; };
        *sma++ = sum;
    }
    stream->progress = progress;
    stream->sum = sum;
    stream->buffer_idx = buffer_idx;
    return TI_OKAY;
}
void ti_sma_stream_free(ti_stream *stream) {
    free(stream);
}
int ti_sqrt_start(TI_REAL const *options) { (void)options; return 0; } int ti_sqrt(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (sqrt(in1[i])); } return TI_OKAY; }
int ti_stddev_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_stddev(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    const TI_REAL scale = 1.0 / period;
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_stddev_start(options)) return TI_OKAY;
    TI_REAL sum = 0;
    TI_REAL sum2 = 0;
    int i;
    for (i = 0; i < period; ++i) {
        sum += input[i];
        sum2 += input[i] * input[i];
    }
    {
        TI_REAL s2s2 = (sum2 * scale - (sum * scale) * (sum * scale));
        if (s2s2 > 0.0) s2s2 = sqrt(s2s2);
        *output++ = s2s2;
    }
    for (i = period; i < size; ++i) {
        sum += input[i];
        sum2 += input[i] * input[i];
        sum -= input[i-period];
        sum2 -= input[i-period] * input[i-period];
        TI_REAL s2s2 = (sum2 * scale - (sum * scale) * (sum * scale));
        if (s2s2 > 0.0) s2s2 = sqrt(s2s2);
        *output++ = s2s2;
    }
    assert(output - outputs[0] == size - ti_stddev_start(options));
    return TI_OKAY;
}
int ti_stderr_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_stderr(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    const TI_REAL scale = 1.0 / period;
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_stderr_start(options)) return TI_OKAY;
    TI_REAL sum = 0;
    TI_REAL sum2 = 0;
    const TI_REAL mul = 1.0 / sqrt(period);
    int i;
    for (i = 0; i < period; ++i) {
        sum += input[i];
        sum2 += input[i] * input[i];
    }
    {
        TI_REAL s2s2 = (sum2 * scale - (sum * scale) * (sum * scale));
        if (s2s2 > 0.0) s2s2 = sqrt(s2s2);
        *output++ = mul * s2s2;
    }
    for (i = period; i < size; ++i) {
        sum += input[i];
        sum2 += input[i] * input[i];
        sum -= input[i-period];
        sum2 -= input[i-period] * input[i-period];
        TI_REAL s2s2 = (sum2 * scale - (sum * scale) * (sum * scale));
        if (s2s2 > 0.0) s2s2 = sqrt(s2s2);
        *output++ = mul * s2s2;
    }
    assert(output - outputs[0] == size - ti_stderr_start(options));
    return TI_OKAY;
}
int ti_stoch_start(TI_REAL const *options) {
    const int kperiod = (int)options[0];
    const int kslow = (int)options[1];
    const int dperiod = (int)options[2];
    return kperiod + kslow + dperiod - 3;
}
int ti_stoch(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    const int kperiod = (int)options[0];
    const int kslow = (int)options[1];
    const int dperiod = (int)options[2];
    const TI_REAL kper = 1.0 / kslow;
    const TI_REAL dper = 1.0 / dperiod;
    TI_REAL *stoch = outputs[0];
    TI_REAL *stoch_ma = outputs[1];
    if (kperiod < 1) return TI_INVALID_OPTION;
    if (kslow < 1) return TI_INVALID_OPTION;
    if (dperiod < 1) return TI_INVALID_OPTION;
    if (size <= ti_stoch_start(options)) return TI_OKAY;
    int trail = 0, maxi = -1, mini = -1;
    TI_REAL max = high[0];
    TI_REAL min = low[0];
    TI_REAL bar;
    ti_buffer *k_sum = ti_buffer_new(kslow);
    ti_buffer *d_sum = ti_buffer_new(dperiod);
    int i, j;
    for (i = 0; i < size; ++i) {
        if (i >= kperiod) ++trail;
        bar = high[i];
        if (maxi < trail) {
            maxi = trail;
            max = high[maxi];
            j = trail;
            while(++j <= i) {
                bar = high[j];
                if (bar >= max) {
                    max = bar;
                    maxi = j;
                }
            }
        } else if (bar >= max) {
            maxi = i;
            max = bar;
        }
        bar = low[i];
        if (mini < trail) {
            mini = trail;
            min = low[mini];
            j = trail;
            while(++j <= i) {
                bar = low[j];
                if (bar <= min) {
                    min = bar;
                    mini = j;
                }
            }
        } else if (bar <= min) {
            mini = i;
            min = bar;
        }
        const TI_REAL kdiff = (max - min);
        const TI_REAL kfast = kdiff == 0.0 ? 0.0 : 100 * ((close[i] - min) / kdiff);
        ti_buffer_push(k_sum, kfast);
        if (i >= kperiod-1 + kslow-1) {
            const TI_REAL k = k_sum->sum * kper;
            ti_buffer_push(d_sum, k);
            if (i >= kperiod-1 + kslow-1 + dperiod-1) {
                *stoch++ = k;
                *stoch_ma++ = d_sum->sum * dper;
            }
        }
    }
    ti_buffer_free(k_sum);
    ti_buffer_free(d_sum);
    assert(stoch - outputs[0] == size - ti_stoch_start(options));
    assert(stoch_ma - outputs[1] == size - ti_stoch_start(options));
    return TI_OKAY;
}
int ti_stochrsi_start(TI_REAL const *options) {
    return ((int)options[0]) * 2 - 1;
}
int ti_stochrsi(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    const TI_REAL per = 1.0 / ((TI_REAL)period);
    if (period < 2) return TI_INVALID_OPTION;
    if (size <= ti_stochrsi_start(options)) return TI_OKAY;
    ti_buffer *rsi = ti_buffer_new(period);
    TI_REAL smooth_up = 0, smooth_down = 0;
    int i;
    for (i = 1; i <= period; ++i) {
        const TI_REAL upward = input[i] > input[i-1] ? input[i] - input[i-1] : 0;
        const TI_REAL downward = input[i] < input[i-1] ? input[i-1] - input[i] : 0;
        smooth_up += upward;
        smooth_down += downward;
    }
    smooth_up /= period;
    smooth_down /= period;
    TI_REAL r = 100.0 * (smooth_up / (smooth_up + smooth_down));
    ti_buffer_push(rsi, r);
    TI_REAL min = r;
    TI_REAL max = r;
    int mini = 0;
    int maxi = 0;
    for (i = period+1; i < size; ++i) {
        const TI_REAL upward = input[i] > input[i-1] ? input[i] - input[i-1] : 0;
        const TI_REAL downward = input[i] < input[i-1] ? input[i-1] - input[i] : 0;
        smooth_up = (upward-smooth_up) * per + smooth_up;
        smooth_down = (downward-smooth_down) * per + smooth_down;
        r = 100.0 * (smooth_up / (smooth_up + smooth_down));
        if (r > max) {
            max = r;
            maxi = rsi->index;
        } else if (maxi == rsi->index) {
            max = r;
            int j;
            for (j = 0; j < rsi->size; ++j) {
                if (j == rsi->index) continue;
                if (rsi->vals[j] > max) {
                    max = rsi->vals[j];
                    maxi = j;
                }
            }
        }
        if (r < min) {
            min = r;
            mini = rsi->index;
        } else if (mini == rsi->index) {
            min = r;
            int j;
            for (j = 0; j < rsi->size; ++j) {
                if (j == rsi->index) continue;
                if (rsi->vals[j] < min) {
                    min = rsi->vals[j];
                    mini = j;
                }
            }
        }
        ti_buffer_qpush(rsi, r);
        if (i > period*2 - 2) {
            const TI_REAL diff = max - min;
            if (diff == 0.0) {
                *output++ = 0.0;
            } else {
                *output++ = (r - min) / (diff);
            }
        }
    }
    ti_buffer_free(rsi);
    assert(output - outputs[0] == size - ti_stochrsi_start(options));
    return TI_OKAY;
}
int ti_sub_start(TI_REAL const *options) { (void)options; return 0; } int ti_sub(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; const TI_REAL *in2 = inputs[1]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (in1[i] - in2[i]); } return TI_OKAY; }
int ti_sum_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_sum(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_sum_start(options)) return TI_OKAY;
    TI_REAL sum = 0;
    int i;
    for (i = 0; i < period; ++i) {
        sum += input[i];
    }
    *output++ = sum;
    for (i = period; i < size; ++i) {
        sum += input[i];
        sum -= input[i-period];
        *output++ = sum;
    }
    assert(output - outputs[0] == size - ti_sum_start(options));
    return TI_OKAY;
}
int ti_tan_start(TI_REAL const *options) { (void)options; return 0; } int ti_tan(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (tan(in1[i])); } return TI_OKAY; }
int ti_tanh_start(TI_REAL const *options) { (void)options; return 0; } int ti_tanh(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = (tanh(in1[i])); } return TI_OKAY; }
int ti_tema_start(TI_REAL const *options) {
    const int period = (int)options[0];
    return (period-1) * 3;
}
int ti_tema(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_tema_start(options)) return TI_OKAY;
    const TI_REAL per = 2 / ((TI_REAL)period + 1);
    const TI_REAL per1 = 1.0 - per;
    TI_REAL ema = input[0];
    TI_REAL ema2 = 0;
    TI_REAL ema3 = 0;
    int i;
    for (i = 0; i < size; ++i) {
        ema = ema * per1 + input[i] * per;
        if (i == period-1) {
            ema2 = ema;
        }
        if (i >= period-1) {
            ema2 = ema2 * per1 + ema * per;
            if (i == (period-1) * 2) {
                ema3 = ema2;
            }
            if (i >= (period-1) * 2) {
                ema3 = ema3 * per1 + ema2 * per;
                if (i >= (period-1) * 3) {
                    *output = 3 * ema - 3 * ema2 + ema3;
                    ++output;
                }
            }
        }
    }
    assert(output - outputs[0] == size - ti_tema_start(options));
    return TI_OKAY;
}
int ti_todeg_start(TI_REAL const *options) { (void)options; return 0; } int ti_todeg(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = ((in1[i] * (180.0 / 3.14159265358979323846))); } return TI_OKAY; }
int ti_torad_start(TI_REAL const *options) { (void)options; return 0; } int ti_torad(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = ((in1[i] * (3.14159265358979323846 / 180.0))); } return TI_OKAY; }
int ti_tr_start(TI_REAL const *options) {
    (void)options;
    return 0;
}
int ti_tr(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    (void)options;
    TI_REAL *output = outputs[0];
    TI_REAL truerange;
    output[0] = high[0] - low[0];
    int i;
    for (i = 1; i < size; ++i) {
        do{ const TI_REAL l = low[i]; const TI_REAL h = high[i]; const TI_REAL c = close[i-1]; const TI_REAL ych = fabs(h - c); const TI_REAL ycl = fabs(l - c); TI_REAL v = h - l; if (ych > v) v = ych; if (ycl > v) v = ycl; truerange = v;}while(0);
        output[i] = truerange;
    }
    return TI_OKAY;
}
int ti_trima_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_trima(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_trima_start(options)) return TI_OKAY;
    if (period <= 2) return ti_sma(size, inputs, options, outputs);
    TI_REAL weights = 1 / (TI_REAL) ((period%2) ?
        ((period/2+1) * (period/2+1)):
        ((period/2+1) * (period/2)));
    TI_REAL weight_sum = 0;
    TI_REAL lead_sum = 0;
    TI_REAL trail_sum = 0;
    const int lead_period = period%2 ? period/2 : period/2-1;
    const int trail_period = lead_period + 1;
    int i, w = 1;
    for (i = 0; i < period-1; ++i) {
        weight_sum += input[i] * w;
        if (i+1 > period-lead_period) lead_sum += input[i];
        if (i+1 <= trail_period) trail_sum += input[i];
        if (i+1 < trail_period) ++w;
        if (i+1 >= period-lead_period) --w;
    }
    int lsi = (period-1)-lead_period+1;
    int tsi1 = (period-1)-period+1+trail_period;
    int tsi2 = (period-1)-period+1;
    for (i = period-1; i < size; ++i) {
        weight_sum += input[i];
        *output++ = weight_sum * weights;
        lead_sum += input[i];
        weight_sum += lead_sum;
        weight_sum -= trail_sum;
        lead_sum -= input[lsi++];
        trail_sum += input[tsi1++];
        trail_sum -= input[tsi2++];
    }
    assert(output - outputs[0] == size - ti_trima_start(options));
    return TI_OKAY;
}
int ti_trix_start(TI_REAL const *options) {
    const int period = (int)options[0];
    return ((period-1)*3)+1;
}
int ti_trix(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_trix_start(options)) return TI_OKAY;
    const int start = (period*3)-2;
    assert(start == ti_trix_start(options));
    const TI_REAL per = 2 / ((TI_REAL)period + 1);
    TI_REAL ema1 = input[0];
    TI_REAL ema2 = 0, ema3 = 0;
    int i;
    for (i = 1; i < start; ++i) {
        ema1 = (input[i]-ema1) * per + ema1;
        if (i == period-1) {
            ema2 = ema1;
        } else if (i > period-1) {
            ema2 = (ema1-ema2) * per + ema2;
            if (i == period * 2 - 2) {
                ema3 = ema2;
            } else if (i > period * 2 - 2) {
                ema3 = (ema2-ema3) * per + ema3;
            }
        }
    }
    for (i = start; i < size; ++i) {
        ema1 = (input[i]-ema1) * per + ema1;
        ema2 = (ema1-ema2) * per + ema2;
        const TI_REAL last = ema3;
        ema3 = (ema2-ema3) * per + ema3;
        *output++ = (ema3-last)/ema3 * 100.0;
    }
    assert(output - outputs[0] == size - ti_trix_start(options));
    return TI_OKAY;
}
int ti_trunc_start(TI_REAL const *options) { (void)options; return 0; } int ti_trunc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) { const TI_REAL *in1 = inputs[0]; (void)options; TI_REAL *output = outputs[0]; int i; for (i = 0; i < size; ++i) { output[i] = ((int)(in1[i])); } return TI_OKAY; }
int ti_tsf_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_tsf(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_tsf_start(options)) return TI_OKAY;
    do { TI_REAL x = 0; TI_REAL x2 = 0; TI_REAL y = 0; TI_REAL xy = 0; const TI_REAL p = (1.0 / (period)); int i; for (i = 0; i < (period)-1; ++i) { x += i+1; x2 += (i+1)*(i+1); xy += (input)[i] * (i+1); y += (input)[i]; } x += (period); x2 += (period) * (period); const TI_REAL bd = 1.0 / ((period) * x2 - x * x); for (i = (period)-1; i < (size); ++i) { xy += (input)[i] * (period); y += (input)[i]; const TI_REAL b = ((period) * xy - x * y) * bd; do { const TI_REAL a = (y - b * x) * p; *output++ = (a + b * ((period+1))); } while (0); xy -= y; y -= (input)[i-(period)+1]; } } while (0);
    assert(output - outputs[0] == size - ti_tsf_start(options));
    return TI_OKAY;
}
int ti_typprice_start(TI_REAL const *options) {
    (void)options;
    return 0;
}
int ti_typprice(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    (void)options;
    TI_REAL *output = outputs[0];
    int i;
    for (i = 0; i < size; ++i) {
        output[i] = (high[i] + low[i] + close[i]) * (1.0/3.0);
    }
    return TI_OKAY;
}
int ti_ultosc_start(TI_REAL const *options) {
    return (int)options[2];
}
int ti_ultosc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    const int short_period = (int)options[0];
    const int medium_period = (int)options[1];
    const int long_period = (int)options[2];
    TI_REAL *output = outputs[0];
    if (short_period < 1) return TI_INVALID_OPTION;
    if (medium_period < short_period) return TI_INVALID_OPTION;
    if (long_period < medium_period) return TI_INVALID_OPTION;
    if (size <= ti_ultosc_start(options)) return TI_OKAY;
    ti_buffer *bp_buf = ti_buffer_new(long_period);
    ti_buffer *r_buf = ti_buffer_new(long_period);
    TI_REAL bp_short_sum = 0, bp_medium_sum = 0;
    TI_REAL r_short_sum = 0, r_medium_sum = 0;
    int i;
    for (i = 1; i < size; ++i) {
        const TI_REAL true_low = MIN(low[i], close[i-1]);
        const TI_REAL true_high = MAX(high[i], close[i-1]);
        const TI_REAL bp = close[i] - true_low;
        const TI_REAL r = true_high - true_low;
        bp_short_sum += bp;
        bp_medium_sum += bp;
        r_short_sum += r;
        r_medium_sum += r;
        ti_buffer_push(bp_buf, bp);
        ti_buffer_push(r_buf, r);
        if (i > short_period) {
            int short_index = bp_buf->index - short_period - 1;
            if (short_index < 0) short_index += long_period;
            bp_short_sum -= bp_buf->vals[short_index];
            r_short_sum -= r_buf->vals[short_index];
            if (i > medium_period) {
                int medium_index = bp_buf->index - medium_period - 1;
                if (medium_index < 0) medium_index += long_period;
                bp_medium_sum -= bp_buf->vals[medium_index];
                r_medium_sum -= r_buf->vals[medium_index];
            }
        }
        if (i >= long_period) {
            const TI_REAL first = 4 * bp_short_sum / r_short_sum;
            const TI_REAL second = 2 * bp_medium_sum / r_medium_sum;
            const TI_REAL third = 1 * bp_buf->sum / r_buf->sum;
            const TI_REAL ult = (first + second + third) * 100.0 / 7.0;
            *output++ = ult;
        }
    }
    ti_buffer_free(bp_buf);
    ti_buffer_free(r_buf);
    assert(output - outputs[0] == size - ti_ultosc_start(options));
    return TI_OKAY;
}
int ti_var_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_var(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    const TI_REAL scale = 1.0 / period;
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_var_start(options)) return TI_OKAY;
    TI_REAL sum = 0;
    TI_REAL sum2 = 0;
    int i;
    for (i = 0; i < period; ++i) {
        sum += input[i];
        sum2 += input[i] * input[i];
    }
    *output++ = sum2 * scale - (sum * scale) * (sum * scale);
    for (i = period; i < size; ++i) {
        sum += input[i];
        sum2 += input[i] * input[i];
        sum -= input[i-period];
        sum2 -= input[i-period] * input[i-period];
        *output++ = sum2 * scale - (sum * scale) * (sum * scale);
    }
    assert(output - outputs[0] == size - ti_var_start(options));
    return TI_OKAY;
}
int ti_vhf_start(TI_REAL const *options) {
    return (int)options[0];
}
int ti_vhf(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *in = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_vhf_start(options)) return TI_OKAY;
    int trail = 1, maxi = -1, mini = -1;
    TI_REAL max = in[0], min = in[0];
    TI_REAL bar;
    TI_REAL sum = 0;
    int i, j;
    TI_REAL yc = in[0];
    TI_REAL c;
    for (i = 1; i < period; ++i) {
        c = in[i];
        sum += fabs(c - yc);
        yc = c;
    }
    for (i = period; i < size; ++i, ++trail) {
        c = in[i];
        sum += fabs(c - yc);
        yc = c;
        if (i > period) {
            sum -= fabs(in[i-period] - in[i-period-1]);
        }
        bar = c;
        if (maxi < trail) {
            maxi = trail;
            max = in[maxi];
            j = trail;
            while(++j <= i) {
                bar = in[j];
                if (bar >= max) {
                    max = bar;
                    maxi = j;
                }
            }
        } else if (bar >= max) {
            maxi = i;
            max = bar;
        }
        bar = c;
        if (mini < trail) {
            mini = trail;
            min = in[mini];
            j = trail;
            while(++j <= i) {
                bar = in[j];
                if (bar <= min) {
                    min = bar;
                    mini = j;
                }
            }
        } else if (bar <= min) {
            mini = i;
            min = bar;
        }
        *output++ = fabs(max - min) / sum;
    }
    assert(output - outputs[0] == size - ti_vhf_start(options));
    return TI_OKAY;
}
int ti_vidya_start(TI_REAL const *options) {
    return ((int)(options[1])) - 2;
}
int ti_vidya(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int short_period = (int)options[0];
    const int long_period = (int)options[1];
    const TI_REAL alpha = options[2];
    TI_REAL *output = outputs[0];
    const TI_REAL short_div = 1.0 / short_period;
    const TI_REAL long_div = 1.0 / long_period;
    if (short_period < 1) return TI_INVALID_OPTION;
    if (long_period < short_period) return TI_INVALID_OPTION;
    if (long_period < 2) return TI_INVALID_OPTION;
    if (alpha < 0.0 || alpha > 1.0) return TI_INVALID_OPTION;
    if (size <= ti_vidya_start(options)) return TI_OKAY;
    TI_REAL short_sum = 0;
    TI_REAL short_sum2 = 0;
    TI_REAL long_sum = 0;
    TI_REAL long_sum2 = 0;
    int i;
    for (i = 0; i < long_period; ++i) {
        long_sum += input[i];
        long_sum2 += input[i] * input[i];
        if (i >= long_period - short_period) {
            short_sum += input[i];
            short_sum2 += input[i] * input[i];
        }
    }
    TI_REAL val = input[long_period-2];
    *output++ = val;
    if (long_period - 1 < size) {
        TI_REAL short_stddev = sqrt(short_sum2 * short_div - (short_sum * short_div) * (short_sum * short_div));
        TI_REAL long_stddev = sqrt(long_sum2 * long_div - (long_sum * long_div) * (long_sum * long_div));
        TI_REAL k = short_stddev / long_stddev;
        if (k != k) k = 0;
        k *= alpha;
        val = (input[long_period-1]-val) * k + val;
        *output++ = val;
    }
    for (i = long_period; i < size; ++i) {
        long_sum += input[i];
        long_sum2 += input[i] * input[i];
        short_sum += input[i];
        short_sum2 += input[i] * input[i];
        long_sum -= input[i-long_period];
        long_sum2 -= input[i-long_period] * input[i-long_period];
        short_sum -= input[i-short_period];
        short_sum2 -= input[i-short_period] * input[i-short_period];
        {
            TI_REAL short_stddev = sqrt(short_sum2 * short_div - (short_sum * short_div) * (short_sum * short_div));
            TI_REAL long_stddev = sqrt(long_sum2 * long_div - (long_sum * long_div) * (long_sum * long_div));
            TI_REAL k = short_stddev / long_stddev;
            if (k != k) k = 0;
            k *= alpha;
            val = (input[i]-val) * k + val;
            *output++ = val;
        }
    }
    assert(output - outputs[0] == size - ti_vidya_start(options));
    return TI_OKAY;
}
int ti_volatility_start(TI_REAL const *options) {
    return (int)options[0];
}
int ti_volatility(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    TI_REAL *output = outputs[0];
    const int period = (int)options[0];
    const TI_REAL scale = 1.0 / period;
    const TI_REAL annual = sqrt(252);
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_volatility_start(options)) return TI_OKAY;
    TI_REAL sum = 0;
    TI_REAL sum2 = 0;
    int i;
    for (i = 1; i <= period; ++i) {
        const TI_REAL c = (input[i]/input[i-1]-1.0);
        sum += c;
        sum2 += c * c;
    }
    *output++ = sqrt(sum2 * scale - (sum * scale) * (sum * scale)) * annual;
    for (i = period+1; i < size; ++i) {
        const TI_REAL c = (input[i]/input[i-1]-1.0);
        sum += c;
        sum2 += c * c;
        const TI_REAL cp = (input[i-period]/input[i-period-1]-1.0);
        sum -= cp;
        sum2 -= cp * cp;
        *output++ = sqrt(sum2 * scale - (sum * scale) * (sum * scale)) * annual;
    }
    assert(output - outputs[0] == size - ti_volatility_start(options));
    return TI_OKAY;
}
int ti_vosc_start(TI_REAL const *options) {
    return (int)options[1]-1;
}
int ti_vosc(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    TI_REAL *output = outputs[0];
    const int short_period = (int)options[0];
    const int long_period = (int)options[1];
    const TI_REAL short_div = 1.0 / short_period;
    const TI_REAL long_div = 1.0 / long_period;
    if (short_period < 1) return TI_INVALID_OPTION;
    if (long_period < short_period) return TI_INVALID_OPTION;
    if (size <= ti_vosc_start(options)) return TI_OKAY;
    TI_REAL short_sum = 0;
    TI_REAL long_sum = 0;
    int i;
    for (i = 0; i < long_period; ++i) {
        if (i >= (long_period - short_period)) {
            short_sum += input[i];
        }
        long_sum += input[i];
    }
    {
        const TI_REAL savg = short_sum * short_div;
        const TI_REAL lavg = long_sum * long_div;
        *output++ = 100.0 * (savg - lavg) / lavg;
    }
    for (i = long_period; i < size; ++i) {
        short_sum += input[i];
        short_sum -= input[i-short_period];
        long_sum += input[i];
        long_sum -= input[i-long_period];
        const TI_REAL savg = short_sum * short_div;
        const TI_REAL lavg = long_sum * long_div;
        *output++ = 100.0 * (savg - lavg) / lavg;
    }
    assert(output - outputs[0] == size - ti_vosc_start(options));
    return TI_OKAY;
}
int ti_vwma_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_vwma(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const TI_REAL *volume = inputs[1];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_vwma_start(options)) return TI_OKAY;
    TI_REAL sum = 0;
    TI_REAL vsum = 0;
    int i;
    for (i = 0; i < period; ++i) {
        sum += input[i] * volume[i];
        vsum += volume[i];
    }
    *output++ = sum / vsum;
    for (i = period; i < size; ++i) {
        sum += input[i] * volume[i];
        sum -= input[i-period] * volume[i-period];
        vsum += volume[i];
        vsum -= volume[i-period];
        *output++ = sum / vsum;
    }
    assert(output - outputs[0] == size - ti_vwma_start(options));
    return TI_OKAY;
}
int ti_wad_start(TI_REAL const *options) {
    (void)options;
    return 1;
}
int ti_wad(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    (void)options;
    if (size <= ti_wad_start(options)) return TI_OKAY;
    TI_REAL *output = outputs[0];
    TI_REAL sum = 0;
    TI_REAL yc = close[0];
    int i;
    for (i = 1; i < size; ++i) {
        const TI_REAL c = close[i];
        if (c > yc) {
            sum += c - MIN(yc, low[i]);
        } else if (c < yc) {
            sum += c - MAX(yc, high[i]);
        } else {
        }
        *output++ = sum;
        yc = close[i];
    }
    assert(output - outputs[0] == size - ti_wad_start(options));
    return TI_OKAY;
}
int ti_wcprice_start(TI_REAL const *options) {
    (void)options;
    return 0;
}
int ti_wcprice(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    (void)options;
    TI_REAL *output = outputs[0];
    int i;
    for (i = 0; i < size; ++i) {
        output[i] = (high[i] + low[i] + close[i] + close[i]) * 0.25;
    }
    return TI_OKAY;
}
int ti_wilders_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_wilders(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_wilders_start(options)) return TI_OKAY;
    const TI_REAL per = 1.0 / ((TI_REAL)period);
    TI_REAL sum = 0;
    int i;
    for (i = 0; i < period; ++i) {
        sum += input[i];
    }
    TI_REAL val = sum / period;
    *output++ = val;
    for (i = period; i < size; ++i) {
        val = (input[i]-val) * per + val;
        *output++ = val;
    }
    assert(output - outputs[0] == size - ti_wilders_start(options));
    return TI_OKAY;
}
int ti_willr_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_willr(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *high = inputs[0];
    const TI_REAL *low = inputs[1];
    const TI_REAL *close = inputs[2];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_willr_start(options)) return TI_OKAY;
    int trail = 0, maxi = -1, mini = -1;
    TI_REAL max = high[0];
    TI_REAL min = low[0];
    TI_REAL bar;
    int i, j;
    for (i = period-1; i < size; ++i, ++trail) {
        bar = high[i];
        if (maxi < trail) {
            maxi = trail;
            max = high[maxi];
            j = trail;
            while(++j <= i) {
                bar = high[j];
                if (bar >= max) {
                    max = bar;
                    maxi = j;
                }
            }
        } else if (bar >= max) {
            maxi = i;
            max = bar;
        }
        bar = low[i];
        if (mini < trail) {
            mini = trail;
            min = low[mini];
            j = trail;
            while(++j <= i) {
                bar = low[j];
                if (bar <= min) {
                    min = bar;
                    mini = j;
                }
            }
        } else if (bar <= min) {
            mini = i;
            min = bar;
        }
        const TI_REAL highlow = (max - min);
        const TI_REAL r = highlow == 0.0 ? 0.0 : -100 * ((max - close[i]) / highlow);
        *output++ = r;
    }
    assert(output - outputs[0] == size - ti_willr_start(options));
    return TI_OKAY;
}
int ti_wma_start(TI_REAL const *options) {
    return (int)options[0]-1;
}
int ti_wma(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_wma_start(options)) return TI_OKAY;
    const TI_REAL weights = period * (period+1) / 2;
    TI_REAL sum = 0;
    TI_REAL weight_sum = 0;
    int i;
    for (i = 0; i < period-1; ++i) {
        weight_sum += input[i] * (i+1);
        sum += input[i];
    }
    for (i = period-1; i < size; ++i) {
        weight_sum += input[i] * period;
        sum += input[i];
        *output++ = weight_sum / weights;
        weight_sum -= sum;
        sum -= input[i-period+1];
    }
    assert(output - outputs[0] == size - ti_wma_start(options));
    return TI_OKAY;
}
int ti_zlema_start(TI_REAL const *options) {
    return ((int)options[0] - 1) / 2 - 1;
}
int ti_zlema(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    const TI_REAL *input = inputs[0];
    const int period = (int)options[0];
    const int lag = (period - 1) / 2;
    TI_REAL *output = outputs[0];
    if (period < 1) return TI_INVALID_OPTION;
    if (size <= ti_zlema_start(options)) return TI_OKAY;
    const TI_REAL per = 2 / ((TI_REAL)period + 1);
    TI_REAL val = input[lag-1];
    *output++ = val;
    int i;
    for (i = lag; i < size; ++i) {
        TI_REAL c = input[i];
        TI_REAL l = input[i-lag];
        val = ((c + (c-l))-val) * per + val;
        *output++ = val;
    }
    assert(output - outputs[0] == size - ti_zlema_start(options));
    return TI_OKAY;
}
ti_buffer *ti_buffer_new(int size) {
    const int s = (int)sizeof(ti_buffer) + (size-1) * (int)sizeof(TI_REAL);
    ti_buffer *ret = (ti_buffer*)malloc((unsigned int)s);
    ret->size = size;
    ret->pushes = 0;
    ret->index = 0;
    ret->sum = 0;
    return ret;
}
void ti_buffer_free(ti_buffer *buffer) {
    free(buffer);
}
const char* ti_version(void) {return TI_VERSION;}
long int ti_build(void) {return TI_BUILD;}
int ti_indicator_count(void) {return TI_INDICATOR_COUNT;}
struct ti_indicator_info ti_indicators[] = { {"abs", "Vector Absolute Value", ti_abs_start, ti_abs, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"abs",0}, 0, 0, 0}, {"acos", "Vector Arccosine", ti_acos_start, ti_acos, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"acos",0}, 0, 0, 0}, {"ad", "Accumulation/Distribution Line", ti_ad_start, ti_ad, 0, TI_TYPE_INDICATOR, 4, 0, 1, {"high","low","close","volume",0}, {"",0}, {"ad",0}, 0, 0, 0}, {"add", "Vector Addition", ti_add_start, ti_add, 0, TI_TYPE_SIMPLE, 2, 0, 1, {"real","real",0}, {"",0}, {"add",0}, 0, 0, 0}, {"adosc", "Accumulation/Distribution Oscillator", ti_adosc_start, ti_adosc, 0, TI_TYPE_INDICATOR, 4, 2, 1, {"high","low","close","volume",0}, {"short_period","long_period",0}, {"adosc",0}, 0, 0, 0}, {"adx", "Average Directional Movement Index", ti_adx_start, ti_adx, 0, TI_TYPE_INDICATOR, 2, 1, 1, {"high","low",0}, {"period",0}, {"adx",0}, 0, 0, 0}, {"adxr", "Average Directional Movement Rating", ti_adxr_start, ti_adxr, 0, TI_TYPE_INDICATOR, 2, 1, 1, {"high","low",0}, {"period",0}, {"adxr",0}, 0, 0, 0}, {"ao", "Awesome Oscillator", ti_ao_start, ti_ao, 0, TI_TYPE_INDICATOR, 2, 0, 1, {"high","low",0}, {"",0}, {"ao",0}, 0, 0, 0}, {"apo", "Absolute Price Oscillator", ti_apo_start, ti_apo, 0, TI_TYPE_INDICATOR, 1, 2, 1, {"real",0}, {"short_period","long_period",0}, {"apo",0}, 0, 0, 0}, {"aroon", "Aroon", ti_aroon_start, ti_aroon, 0, TI_TYPE_INDICATOR, 2, 1, 2, {"high","low",0}, {"period",0}, {"aroon_down","aroon_up",0}, 0, 0, 0}, {"aroonosc", "Aroon Oscillator", ti_aroonosc_start, ti_aroonosc, 0, TI_TYPE_INDICATOR, 2, 1, 1, {"high","low",0}, {"period",0}, {"aroonosc",0}, 0, 0, 0}, {"asin", "Vector Arcsine", ti_asin_start, ti_asin, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"asin",0}, 0, 0, 0}, {"atan", "Vector Arctangent", ti_atan_start, ti_atan, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"atan",0}, 0, 0, 0}, {"atr", "Average True Range", ti_atr_start, ti_atr, ti_atr_ref, TI_TYPE_INDICATOR, 3, 1, 1, {"high","low","close",0}, {"period",0}, {"atr",0}, ti_atr_stream_new, ti_atr_stream_run, ti_atr_stream_free}, {"avgprice", "Average Price", ti_avgprice_start, ti_avgprice, 0, TI_TYPE_OVERLAY, 4, 0, 1, {"open","high","low","close",0}, {"",0}, {"avgprice",0}, 0, 0, 0}, {"bbands", "Bollinger Bands", ti_bbands_start, ti_bbands, 0, TI_TYPE_OVERLAY, 1, 2, 3, {"real",0}, {"period","stddev",0}, {"bbands_lower","bbands_middle","bbands_upper",0}, 0, 0, 0}, {"bop", "Balance of Power", ti_bop_start, ti_bop, 0, TI_TYPE_INDICATOR, 4, 0, 1, {"open","high","low","close",0}, {"",0}, {"bop",0}, 0, 0, 0}, {"cci", "Commodity Channel Index", ti_cci_start, ti_cci, 0, TI_TYPE_INDICATOR, 3, 1, 1, {"high","low","close",0}, {"period",0}, {"cci",0}, 0, 0, 0}, {"ceil", "Vector Ceiling", ti_ceil_start, ti_ceil, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"ceil",0}, 0, 0, 0}, {"cmo", "Chande Momentum Oscillator", ti_cmo_start, ti_cmo, 0, TI_TYPE_INDICATOR, 1, 1, 1, {"real",0}, {"period",0}, {"cmo",0}, 0, 0, 0}, {"cos", "Vector Cosine", ti_cos_start, ti_cos, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"cos",0}, 0, 0, 0}, {"cosh", "Vector Hyperbolic Cosine", ti_cosh_start, ti_cosh, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"cosh",0}, 0, 0, 0}, {"crossany", "Crossany", ti_crossany_start, ti_crossany, 0, TI_TYPE_MATH, 2, 0, 1, {"real","real",0}, {"",0}, {"crossany",0}, 0, 0, 0}, {"crossover", "Crossover", ti_crossover_start, ti_crossover, 0, TI_TYPE_MATH, 2, 0, 1, {"real","real",0}, {"",0}, {"crossover",0}, 0, 0, 0}, {"cvi", "Chaikins Volatility", ti_cvi_start, ti_cvi, 0, TI_TYPE_INDICATOR, 2, 1, 1, {"high","low",0}, {"period",0}, {"cvi",0}, 0, 0, 0}, {"decay", "Linear Decay", ti_decay_start, ti_decay, 0, TI_TYPE_MATH, 1, 1, 1, {"real",0}, {"period",0}, {"decay",0}, 0, 0, 0}, {"dema", "Double Exponential Moving Average", ti_dema_start, ti_dema, 0, TI_TYPE_OVERLAY, 1, 1, 1, {"real",0}, {"period",0}, {"dema",0}, 0, 0, 0}, {"di", "Directional Indicator", ti_di_start, ti_di, 0, TI_TYPE_INDICATOR, 3, 1, 2, {"high","low","close",0}, {"period",0}, {"plus_di","minus_di",0}, 0, 0, 0}, {"div", "Vector Division", ti_div_start, ti_div, 0, TI_TYPE_SIMPLE, 2, 0, 1, {"real","real",0}, {"",0}, {"div",0}, 0, 0, 0}, {"dm", "Directional Movement", ti_dm_start, ti_dm, 0, TI_TYPE_INDICATOR, 2, 1, 2, {"high","low",0}, {"period",0}, {"plus_dm","minus_dm",0}, 0, 0, 0}, {"dpo", "Detrended Price Oscillator", ti_dpo_start, ti_dpo, 0, TI_TYPE_INDICATOR, 1, 1, 1, {"real",0}, {"period",0}, {"dpo",0}, 0, 0, 0}, {"dx", "Directional Movement Index", ti_dx_start, ti_dx, 0, TI_TYPE_INDICATOR, 2, 1, 1, {"high","low",0}, {"period",0}, {"dx",0}, 0, 0, 0}, {"edecay", "Exponential Decay", ti_edecay_start, ti_edecay, 0, TI_TYPE_MATH, 1, 1, 1, {"real",0}, {"period",0}, {"edecay",0}, 0, 0, 0}, {"ema", "Exponential Moving Average", ti_ema_start, ti_ema, 0, TI_TYPE_OVERLAY, 1, 1, 1, {"real",0}, {"period",0}, {"ema",0}, 0, 0, 0}, {"emv", "Ease of Movement", ti_emv_start, ti_emv, 0, TI_TYPE_INDICATOR, 3, 0, 1, {"high","low","volume",0}, {"",0}, {"emv",0}, 0, 0, 0}, {"exp", "Vector Exponential", ti_exp_start, ti_exp, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"exp",0}, 0, 0, 0}, {"fisher", "Fisher Transform", ti_fisher_start, ti_fisher, 0, TI_TYPE_INDICATOR, 2, 1, 2, {"high","low",0}, {"period",0}, {"fisher","fisher_signal",0}, 0, 0, 0}, {"floor", "Vector Floor", ti_floor_start, ti_floor, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"floor",0}, 0, 0, 0}, {"fosc", "Forecast Oscillator", ti_fosc_start, ti_fosc, 0, TI_TYPE_INDICATOR, 1, 1, 1, {"real",0}, {"period",0}, {"fosc",0}, 0, 0, 0}, {"hma", "Hull Moving Average", ti_hma_start, ti_hma, 0, TI_TYPE_OVERLAY, 1, 1, 1, {"real",0}, {"period",0}, {"hma",0}, 0, 0, 0}, {"kama", "Kaufman Adaptive Moving Average", ti_kama_start, ti_kama, 0, TI_TYPE_OVERLAY, 1, 1, 1, {"real",0}, {"period",0}, {"kama",0}, 0, 0, 0}, {"kvo", "Klinger Volume Oscillator", ti_kvo_start, ti_kvo, 0, TI_TYPE_INDICATOR, 4, 2, 1, {"high","low","close","volume",0}, {"short_period","long_period",0}, {"kvo",0}, 0, 0, 0}, {"lag", "Lag", ti_lag_start, ti_lag, 0, TI_TYPE_MATH, 1, 1, 1, {"real",0}, {"period",0}, {"lag",0}, 0, 0, 0}, {"linreg", "Linear Regression", ti_linreg_start, ti_linreg, 0, TI_TYPE_OVERLAY, 1, 1, 1, {"real",0}, {"period",0}, {"linreg",0}, 0, 0, 0}, {"linregintercept", "Linear Regression Intercept", ti_linregintercept_start, ti_linregintercept, 0, TI_TYPE_INDICATOR, 1, 1, 1, {"real",0}, {"period",0}, {"linregintercept",0}, 0, 0, 0}, {"linregslope", "Linear Regression Slope", ti_linregslope_start, ti_linregslope, 0, TI_TYPE_INDICATOR, 1, 1, 1, {"real",0}, {"period",0}, {"linregslope",0}, 0, 0, 0}, {"ln", "Vector Natural Log", ti_ln_start, ti_ln, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"ln",0}, 0, 0, 0}, {"log10", "Vector Base-10 Log", ti_log10_start, ti_log10, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"log10",0}, 0, 0, 0}, {"macd", "Moving Average Convergence/Divergence", ti_macd_start, ti_macd, 0, TI_TYPE_INDICATOR, 1, 3, 3, {"real",0}, {"short_period","long_period","signal_period",0}, {"macd","macd_signal","macd_histogram",0}, 0, 0, 0}, {"marketfi", "Market Facilitation Index", ti_marketfi_start, ti_marketfi, 0, TI_TYPE_INDICATOR, 3, 0, 1, {"high","low","volume",0}, {"",0}, {"marketfi",0}, 0, 0, 0}, {"mass", "Mass Index", ti_mass_start, ti_mass, 0, TI_TYPE_INDICATOR, 2, 1, 1, {"high","low",0}, {"period",0}, {"mass",0}, 0, 0, 0}, {"max", "Maximum In Period", ti_max_start, ti_max, ti_max_ref, TI_TYPE_MATH, 1, 1, 1, {"real",0}, {"period",0}, {"max",0}, 0, 0, 0}, {"md", "Mean Deviation Over Period", ti_md_start, ti_md, 0, TI_TYPE_MATH, 1, 1, 1, {"real",0}, {"period",0}, {"md",0}, 0, 0, 0}, {"medprice", "Median Price", ti_medprice_start, ti_medprice, 0, TI_TYPE_OVERLAY, 2, 0, 1, {"high","low",0}, {"",0}, {"medprice",0}, 0, 0, 0}, {"mfi", "Money Flow Index", ti_mfi_start, ti_mfi, 0, TI_TYPE_INDICATOR, 4, 1, 1, {"high","low","close","volume",0}, {"period",0}, {"mfi",0}, 0, 0, 0}, {"min", "Minimum In Period", ti_min_start, ti_min, ti_min_ref, TI_TYPE_MATH, 1, 1, 1, {"real",0}, {"period",0}, {"min",0}, 0, 0, 0}, {"mom", "Momentum", ti_mom_start, ti_mom, 0, TI_TYPE_INDICATOR, 1, 1, 1, {"real",0}, {"period",0}, {"mom",0}, 0, 0, 0}, {"msw", "Mesa Sine Wave", ti_msw_start, ti_msw, 0, TI_TYPE_INDICATOR, 1, 1, 2, {"real",0}, {"period",0}, {"msw_sine","msw_lead",0}, 0, 0, 0}, {"mul", "Vector Multiplication", ti_mul_start, ti_mul, 0, TI_TYPE_SIMPLE, 2, 0, 1, {"real","real",0}, {"",0}, {"mul",0}, 0, 0, 0}, {"natr", "Normalized Average True Range", ti_natr_start, ti_natr, 0, TI_TYPE_INDICATOR, 3, 1, 1, {"high","low","close",0}, {"period",0}, {"natr",0}, 0, 0, 0}, {"nvi", "Negative Volume Index", ti_nvi_start, ti_nvi, 0, TI_TYPE_INDICATOR, 2, 0, 1, {"close","volume",0}, {"",0}, {"nvi",0}, 0, 0, 0}, {"obv", "On Balance Volume", ti_obv_start, ti_obv, 0, TI_TYPE_INDICATOR, 2, 0, 1, {"close","volume",0}, {"",0}, {"obv",0}, 0, 0, 0}, {"ppo", "Percentage Price Oscillator", ti_ppo_start, ti_ppo, 0, TI_TYPE_INDICATOR, 1, 2, 1, {"real",0}, {"short_period","long_period",0}, {"ppo",0}, 0, 0, 0}, {"psar", "Parabolic SAR", ti_psar_start, ti_psar, 0, TI_TYPE_OVERLAY, 2, 2, 1, {"high","low",0}, {"acceleration_factor_step","acceleration_factor_maximum",0}, {"psar",0}, 0, 0, 0}, {"pvi", "Positive Volume Index", ti_pvi_start, ti_pvi, 0, TI_TYPE_INDICATOR, 2, 0, 1, {"close","volume",0}, {"",0}, {"pvi",0}, 0, 0, 0}, {"qstick", "Qstick", ti_qstick_start, ti_qstick, 0, TI_TYPE_INDICATOR, 2, 1, 1, {"open","close",0}, {"period",0}, {"qstick",0}, 0, 0, 0}, {"roc", "Rate of Change", ti_roc_start, ti_roc, 0, TI_TYPE_INDICATOR, 1, 1, 1, {"real",0}, {"period",0}, {"roc",0}, 0, 0, 0}, {"rocr", "Rate of Change Ratio", ti_rocr_start, ti_rocr, 0, TI_TYPE_INDICATOR, 1, 1, 1, {"real",0}, {"period",0}, {"rocr",0}, 0, 0, 0}, {"round", "Vector Round", ti_round_start, ti_round, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"round",0}, 0, 0, 0}, {"rsi", "Relative Strength Index", ti_rsi_start, ti_rsi, 0, TI_TYPE_INDICATOR, 1, 1, 1, {"real",0}, {"period",0}, {"rsi",0}, 0, 0, 0}, {"sin", "Vector Sine", ti_sin_start, ti_sin, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"sin",0}, 0, 0, 0}, {"sinh", "Vector Hyperbolic Sine", ti_sinh_start, ti_sinh, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"sinh",0}, 0, 0, 0}, {"sma", "Simple Moving Average", ti_sma_start, ti_sma, 0, TI_TYPE_OVERLAY, 1, 1, 1, {"real",0}, {"period",0}, {"sma",0}, ti_sma_stream_new, ti_sma_stream_run, ti_sma_stream_free}, {"sqrt", "Vector Square Root", ti_sqrt_start, ti_sqrt, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"sqrt",0}, 0, 0, 0}, {"stddev", "Standard Deviation Over Period", ti_stddev_start, ti_stddev, 0, TI_TYPE_MATH, 1, 1, 1, {"real",0}, {"period",0}, {"stddev",0}, 0, 0, 0}, {"stderr", "Standard Error Over Period", ti_stderr_start, ti_stderr, 0, TI_TYPE_MATH, 1, 1, 1, {"real",0}, {"period",0}, {"stderr",0}, 0, 0, 0}, {"stoch", "Stochastic Oscillator", ti_stoch_start, ti_stoch, 0, TI_TYPE_INDICATOR, 3, 3, 2, {"high","low","close",0}, {"k_period","k_slowing_period","d_period",0}, {"stoch_k","stoch_d",0}, 0, 0, 0}, {"stochrsi", "Stochastic RSI", ti_stochrsi_start, ti_stochrsi, 0, TI_TYPE_INDICATOR, 1, 1, 1, {"real",0}, {"period",0}, {"stochrsi",0}, 0, 0, 0}, {"sub", "Vector Subtraction", ti_sub_start, ti_sub, 0, TI_TYPE_SIMPLE, 2, 0, 1, {"real","real",0}, {"",0}, {"sub",0}, 0, 0, 0}, {"sum", "Sum Over Period", ti_sum_start, ti_sum, 0, TI_TYPE_MATH, 1, 1, 1, {"real",0}, {"period",0}, {"sum",0}, 0, 0, 0}, {"tan", "Vector Tangent", ti_tan_start, ti_tan, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"tan",0}, 0, 0, 0}, {"tanh", "Vector Hyperbolic Tangent", ti_tanh_start, ti_tanh, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"tanh",0}, 0, 0, 0}, {"tema", "Triple Exponential Moving Average", ti_tema_start, ti_tema, 0, TI_TYPE_OVERLAY, 1, 1, 1, {"real",0}, {"period",0}, {"tema",0}, 0, 0, 0}, {"todeg", "Vector Degree Conversion", ti_todeg_start, ti_todeg, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"degrees",0}, 0, 0, 0}, {"torad", "Vector Radian Conversion", ti_torad_start, ti_torad, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"radians",0}, 0, 0, 0}, {"tr", "True Range", ti_tr_start, ti_tr, 0, TI_TYPE_INDICATOR, 3, 0, 1, {"high","low","close",0}, {"",0}, {"tr",0}, 0, 0, 0}, {"trima", "Triangular Moving Average", ti_trima_start, ti_trima, 0, TI_TYPE_OVERLAY, 1, 1, 1, {"real",0}, {"period",0}, {"trima",0}, 0, 0, 0}, {"trix", "Trix", ti_trix_start, ti_trix, 0, TI_TYPE_INDICATOR, 1, 1, 1, {"real",0}, {"period",0}, {"trix",0}, 0, 0, 0}, {"trunc", "Vector Truncate", ti_trunc_start, ti_trunc, 0, TI_TYPE_SIMPLE, 1, 0, 1, {"real",0}, {"",0}, {"trunc",0}, 0, 0, 0}, {"tsf", "Time Series Forecast", ti_tsf_start, ti_tsf, 0, TI_TYPE_OVERLAY, 1, 1, 1, {"real",0}, {"period",0}, {"tsf",0}, 0, 0, 0}, {"typprice", "Typical Price", ti_typprice_start, ti_typprice, 0, TI_TYPE_OVERLAY, 3, 0, 1, {"high","low","close",0}, {"",0}, {"typprice",0}, 0, 0, 0}, {"ultosc", "Ultimate Oscillator", ti_ultosc_start, ti_ultosc, 0, TI_TYPE_INDICATOR, 3, 3, 1, {"high","low","close",0}, {"short_period","medium_period","long_period",0}, {"ultosc",0}, 0, 0, 0}, {"var", "Variance Over Period", ti_var_start, ti_var, 0, TI_TYPE_MATH, 1, 1, 1, {"real",0}, {"period",0}, {"var",0}, 0, 0, 0}, {"vhf", "Vertical Horizontal Filter", ti_vhf_start, ti_vhf, 0, TI_TYPE_INDICATOR, 1, 1, 1, {"real",0}, {"period",0}, {"vhf",0}, 0, 0, 0}, {"vidya", "Variable Index Dynamic Average", ti_vidya_start, ti_vidya, 0, TI_TYPE_OVERLAY, 1, 3, 1, {"real",0}, {"short_period","long_period","alpha",0}, {"vidya",0}, 0, 0, 0}, {"volatility", "Annualized Historical Volatility", ti_volatility_start, ti_volatility, 0, TI_TYPE_INDICATOR, 1, 1, 1, {"real",0}, {"period",0}, {"volatility",0}, 0, 0, 0}, {"vosc", "Volume Oscillator", ti_vosc_start, ti_vosc, 0, TI_TYPE_INDICATOR, 1, 2, 1, {"volume",0}, {"short_period","long_period",0}, {"vosc",0}, 0, 0, 0}, {"vwma", "Volume Weighted Moving Average", ti_vwma_start, ti_vwma, 0, TI_TYPE_OVERLAY, 2, 1, 1, {"close","volume",0}, {"period",0}, {"vwma",0}, 0, 0, 0}, {"wad", "Williams Accumulation/Distribution", ti_wad_start, ti_wad, 0, TI_TYPE_INDICATOR, 3, 0, 1, {"high","low","close",0}, {"",0}, {"wad",0}, 0, 0, 0}, {"wcprice", "Weighted Close Price", ti_wcprice_start, ti_wcprice, 0, TI_TYPE_OVERLAY, 3, 0, 1, {"high","low","close",0}, {"",0}, {"wcprice",0}, 0, 0, 0}, {"wilders", "Wilders Smoothing", ti_wilders_start, ti_wilders, 0, TI_TYPE_OVERLAY, 1, 1, 1, {"real",0}, {"period",0}, {"wilders",0}, 0, 0, 0}, {"willr", "Williams %R", ti_willr_start, ti_willr, 0, TI_TYPE_INDICATOR, 3, 1, 1, {"high","low","close",0}, {"period",0}, {"willr",0}, 0, 0, 0}, {"wma", "Weighted Moving Average", ti_wma_start, ti_wma, 0, TI_TYPE_OVERLAY, 1, 1, 1, {"real",0}, {"period",0}, {"wma",0}, 0, 0, 0}, {"zlema", "Zero-Lag Exponential Moving Average", ti_zlema_start, ti_zlema, 0, TI_TYPE_OVERLAY, 1, 1, 1, {"real",0}, {"period",0}, {"zlema",0}, 0, 0, 0}, {0,0,0,0,0,0,0,0,0,{0,0},{0,0},{0,0},0,0,0}};
struct ti_stream {
    int index;
    int progress;
};
int ti_stream_run(ti_stream *stream, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    return ti_indicators[stream->index].stream_run(stream, size, inputs, outputs);
}
ti_indicator_info *ti_stream_get_info(ti_stream *stream) {
    return ti_indicators + stream->index;
}
int ti_stream_get_progress(ti_stream *stream) {
    return stream->progress;
}
void ti_stream_free(ti_stream *stream) {
    ti_indicators[stream->index].stream_free(stream);
}
const ti_indicator_info *ti_find_indicator(const char *name) {
    int imin = 0;
    int imax = sizeof(ti_indicators) / sizeof(ti_indicator_info) - 2;
    while (imax >= imin) {
        const int i = (imin + ((imax-imin)/2));
        const int c = strcmp(name, ti_indicators[i].name);
        if (c == 0) {
            return ti_indicators + i;
        } else if (c > 0) {
            imin = i + 1;
        } else {
            imax = i - 1;
        }
    }
    return 0;
}
const char* tc_version(void) {return TC_VERSION;}
long int tc_build(void) {return TC_BUILD;}
int tc_candle_count(void) {return TC_CANDLE_COUNT;}
struct tc_result {
    int progress;
    int space;
    int count;
    int pattern_count;
    tc_hit *hits;
};
tc_config const *tc_config_default() {
    static tc_config default_config = {
        .period = 10,
        .body_none = 0.05,
        .body_short = 0.5,
        .body_long = 1.4,
        .wick_none = 0.05,
        .wick_long = 0.6,
        .near = 0.3
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
    if (!pattern) return 0;
    tc_set k = pattern & ~(pattern-1);
    int imin = 0;
    int imax = sizeof(tc_candles) / sizeof(tc_candle_info) - 2;
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
int tc_run(tc_set patterns, int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
    tc_result_reset(output);
    {
        if ((patterns & (patterns-1)) == 0) {
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
    const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
    while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if ((TC_ABANDONED_BABY_BEAR & patterns) && i>=2 && (open[i-2]<close[i-2]) && ((fabs(open[(i-2)] - close[(i-2)])) > (options->body_long * avg_body)) && (low[i-1] >= high[(i-1)-1]) && ((fabs(open[(i-1)] - close[(i-1)])) < (options->body_none * avg_total)) && (open[i]>close[i]) && (high[i] <= low[(i)-1]) && (close[i] <= close[i-2])) { do { const tc_hit hit = {i, TC_ABANDONED_BABY_BEAR}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_ABANDONED_BABY_BULL & patterns) && i>=2 && (open[i-2]>close[i-2]) && ((fabs(open[(i-2)] - close[(i-2)])) > (options->body_long * avg_body)) && (high[i-1] <= low[(i-1)-1]) && ((fabs(open[(i-1)] - close[(i-1)])) < (options->body_none * avg_total)) && (open[i]<close[i]) && (low[i] >= high[(i)-1]) && (close[i] >= close[i-2])) { do { const tc_hit hit = {i, TC_ABANDONED_BABY_BULL}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_BIG_BLACK_CANDLE & patterns) && (open[i]>close[i]) && ((fabs(open[(i)] - close[(i)])) > (options->body_long * avg_body))) { do { const tc_hit hit = {i, TC_BIG_BLACK_CANDLE}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_BIG_WHITE_CANDLE & patterns) && (open[i]<close[i]) && ((fabs(open[(i)] - close[(i)])) > (options->body_long * avg_body))) { do { const tc_hit hit = {i, TC_BIG_WHITE_CANDLE}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_BLACK_MARUBOZU & patterns) && ((fabs(open[(i)] - close[(i)])) > (options->body_long * avg_body)) && ((high[(i)] - top) < (options->wick_none * avg_total)) && ((bottom - low[(i)]) < (options->wick_none * avg_total)) && (open[i]>close[i])) { do { const tc_hit hit = {i, TC_BLACK_MARUBOZU}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_DOJI & patterns) && ((fabs(open[(i)] - close[(i)])) < (options->body_none * avg_total))) { do { const tc_hit hit = {i, TC_DOJI}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_DRAGONFLY_DOJI & patterns) && ((fabs(open[(i)] - close[(i)])) < (options->body_none * avg_total)) && ((high[(i)] - top) < (options->wick_none * avg_total)) && ((bottom - low[(i)]) > (options->wick_long * avg_body))) { do { const tc_hit hit = {i, TC_DRAGONFLY_DOJI}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_ENGULFING_BEAR & patterns) && i>=1 && (open[i]>close[i]) && (open[i-1]<close[i-1]) && close[i] < open[i-1] && open[i] > close[i-1]) { do { const tc_hit hit = {i, TC_ENGULFING_BEAR}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_ENGULFING_BULL & patterns) && i>=1 && (open[i]<close[i]) && (open[i-1]>close[i-1]) && close[i] > open[i-1] && open[i] < close[i-1]) { do { const tc_hit hit = {i, TC_ENGULFING_BULL}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_EVENING_DOJI_STAR & patterns) && i>=2 && (open[i-2]<close[i-2]) && ((fabs(open[(i-2)] - close[(i-2)])) > (options->body_long * avg_body)) && ((open[(i-1)] < close[(i-1)] ? open[(i-1)] : close[(i-1)]) >= (open[((i-1)-1)] > close[((i-1)-1)] ? open[((i-1)-1)] : close[((i-1)-1)])) && ((fabs(open[(i-1)] - close[(i-1)])) < (options->body_none * avg_total)) && (open[i]>close[i]) && ((open[(i)] > close[(i)] ? open[(i)] : close[(i)]) <= (open[((i)-1)] < close[((i)-1)] ? open[((i)-1)] : close[((i)-1)])) && (close[i] <= close[i-2])) { do { const tc_hit hit = {i, TC_EVENING_DOJI_STAR}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_EVENING_STAR & patterns) && i>=2 && (open[i-2]<close[i-2]) && ((fabs(open[(i-2)] - close[(i-2)])) > (options->body_long * avg_body)) && ((open[(i-1)] < close[(i-1)] ? open[(i-1)] : close[(i-1)]) >= (open[((i-1)-1)] > close[((i-1)-1)] ? open[((i-1)-1)] : close[((i-1)-1)])) && ((fabs(open[(i-1)] - close[(i-1)])) < (options->body_short * avg_body)) && (open[i]>close[i]) && ((open[(i)] > close[(i)] ? open[(i)] : close[(i)]) <= (open[((i)-1)] < close[((i)-1)] ? open[((i)-1)] : close[((i)-1)])) && (close[i] <= close[i-2])) { do { const tc_hit hit = {i, TC_EVENING_STAR}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_FOUR_PRICE_DOJI & patterns) && ((fabs(open[(i)] - close[(i)])) < (options->body_none * avg_total)) && ((high[(i)] - top) < (options->wick_none * avg_total)) && ((bottom - low[(i)]) < (options->wick_none * avg_total))) { do { const tc_hit hit = {i, TC_FOUR_PRICE_DOJI}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_GRAVESTONE_DOJI & patterns) && ((fabs(open[(i)] - close[(i)])) < (options->body_none * avg_total)) && ((high[(i)] - top) > (options->wick_long * avg_body)) && ((bottom - low[(i)]) < (options->wick_none * avg_total))) { do { const tc_hit hit = {i, TC_GRAVESTONE_DOJI}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_HAMMER & patterns) && i>=1 && ((fabs(open[(i)] - close[(i)])) < (options->body_short * avg_body)) && ((high[(i)] - top) < (options->wick_none * avg_total)) && ((bottom - low[(i)]) > (options->wick_long * avg_body)) && (open[(i)] < close[(i)] ? open[(i)] : close[(i)]) <= low[i-1] + (options->near * avg_total)) { do { const tc_hit hit = {i, TC_HAMMER}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_HANGING_MAN & patterns) && i>=1 && ((fabs(open[(i)] - close[(i)])) < (options->body_short * avg_body)) && ((high[(i)] - top) < (options->wick_none * avg_total)) && ((bottom - low[(i)]) > (options->wick_long * avg_body)) && (open[(i)] < close[(i)] ? open[(i)] : close[(i)]) >= high[i-1] - (options->near * avg_total)) { do { const tc_hit hit = {i, TC_HANGING_MAN}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_INVERTED_HAMMER & patterns) && i>=1 && ((fabs(open[(i)] - close[(i)])) < (options->body_short * avg_body)) && ((high[(i)] - top) > (options->wick_long * avg_body)) && ((bottom - low[(i)]) < (options->wick_none * avg_total)) && ((open[(i)] > close[(i)] ? open[(i)] : close[(i)]) <= (open[((i)-1)] < close[((i)-1)] ? open[((i)-1)] : close[((i)-1)]))) { do { const tc_hit hit = {i, TC_INVERTED_HAMMER}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_LONG_LEGGED_DOJI & patterns) && ((fabs(open[(i)] - close[(i)])) < (options->body_none * avg_total)) && ((high[(i)] - top) > (options->wick_long * avg_body)) && ((bottom - low[(i)]) > (options->wick_long * avg_body))) { do { const tc_hit hit = {i, TC_LONG_LEGGED_DOJI}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_MARUBOZU & patterns) && ((fabs(open[(i)] - close[(i)])) > (options->body_long * avg_body)) && ((high[(i)] - top) < (options->wick_none * avg_total)) && ((bottom - low[(i)]) < (options->wick_none * avg_total))) { do { const tc_hit hit = {i, TC_MARUBOZU}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_MORNING_DOJI_STAR & patterns) && i>=2 && (open[i-2]>close[i-2]) && ((fabs(open[(i-2)] - close[(i-2)])) > (options->body_long * avg_body)) && ((open[(i-1)] > close[(i-1)] ? open[(i-1)] : close[(i-1)]) <= (open[((i-1)-1)] < close[((i-1)-1)] ? open[((i-1)-1)] : close[((i-1)-1)])) && ((fabs(open[(i-1)] - close[(i-1)])) < (options->body_none * avg_total)) && (open[i]<close[i]) && ((open[(i)] < close[(i)] ? open[(i)] : close[(i)]) >= (open[((i)-1)] > close[((i)-1)] ? open[((i)-1)] : close[((i)-1)])) && (close[i] >= close[i-2])) { do { const tc_hit hit = {i, TC_MORNING_DOJI_STAR}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_MORNING_STAR & patterns) && i>=2 && (open[i-2]>close[i-2]) && ((fabs(open[(i-2)] - close[(i-2)])) > (options->body_long * avg_body)) && ((open[(i-1)] > close[(i-1)] ? open[(i-1)] : close[(i-1)]) <= (open[((i-1)-1)] < close[((i-1)-1)] ? open[((i-1)-1)] : close[((i-1)-1)])) && ((fabs(open[(i-1)] - close[(i-1)])) < (options->body_short * avg_body)) && (open[i]<close[i]) && ((open[(i)] < close[(i)] ? open[(i)] : close[(i)]) >= (open[((i)-1)] > close[((i)-1)] ? open[((i)-1)] : close[((i)-1)])) && (close[i] >= close[i-2])) { do { const tc_hit hit = {i, TC_MORNING_STAR}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_SHOOTING_STAR & patterns) && i>=1 && ((fabs(open[(i)] - close[(i)])) < (options->body_short * avg_body)) && ((high[(i)] - top) > (options->wick_long * avg_body)) && ((bottom - low[(i)]) < (options->wick_none * avg_total)) && ((open[(i)] < close[(i)] ? open[(i)] : close[(i)]) >= (open[((i)-1)] > close[((i)-1)] ? open[((i)-1)] : close[((i)-1)]))) { do { const tc_hit hit = {i, TC_SHOOTING_STAR}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_SPINNING_TOP & patterns) && ((fabs(open[(i)] - close[(i)])) < (options->body_short * avg_body)) && ((high[(i)] - top) > (fabs(open[(i)] - close[(i)]))) && ((bottom - low[(i)]) > (fabs(open[(i)] - close[(i)])))) { do { const tc_hit hit = {i, TC_SPINNING_TOP}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_STAR & patterns) && i>=1 && ((low[i] >= high[(i)-1]) || (high[i] <= low[(i)-1]))) { do { const tc_hit hit = {i, TC_STAR}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_THREE_BLACK_CROWS & patterns) && i>=2 && (open[i-2]>close[i-2]) && (open[i-1]>close[i-1]) && (open[i]>close[i]) && (open[i-1] < (open[((i-1)-1)] > close[((i-1)-1)] ? open[((i-1)-1)] : close[((i-1)-1)]) && open[i-1] > (open[((i-1)-1)] < close[((i-1)-1)] ? open[((i-1)-1)] : close[((i-1)-1)])) && (open[i] < (open[((i)-1)] > close[((i)-1)] ? open[((i)-1)] : close[((i)-1)]) && open[i] > (open[((i)-1)] < close[((i)-1)] ? open[((i)-1)] : close[((i)-1)]))) { do { const tc_hit hit = {i, TC_THREE_BLACK_CROWS}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_THREE_WHITE_SOLDIERS & patterns) && i>=2 && (open[i-2]<close[i-2]) && (open[i-1]<close[i-1]) && (open[i]<close[i]) && (open[i-1] < (open[((i-1)-1)] > close[((i-1)-1)] ? open[((i-1)-1)] : close[((i-1)-1)]) && open[i-1] > (open[((i-1)-1)] < close[((i-1)-1)] ? open[((i-1)-1)] : close[((i-1)-1)])) && (open[i] < (open[((i)-1)] > close[((i)-1)] ? open[((i)-1)] : close[((i)-1)]) && open[i] > (open[((i)-1)] < close[((i)-1)] ? open[((i)-1)] : close[((i)-1)]))) { do { const tc_hit hit = {i, TC_THREE_WHITE_SOLDIERS}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } if ((TC_WHITE_MARUBOZU & patterns) && ((fabs(open[(i)] - close[(i)])) > (options->body_long * avg_body)) && ((high[(i)] - top) < (options->wick_none * avg_total)) && ((bottom - low[(i)]) < (options->wick_none * avg_total)) && (open[i]<close[i])) { do { const tc_hit hit = {i, TC_WHITE_MARUBOZU}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
    return TC_OKAY;
}
int tc_abandoned_baby_bear(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (i>=2 && (open[i-2]<close[i-2]) && ((fabs(open[(i-2)] - close[(i-2)])) > (options->body_long * avg_body)) && (low[i-1] >= high[(i-1)-1]) && ((fabs(open[(i-1)] - close[(i-1)])) < (options->body_none * avg_total)) && (open[i]>close[i]) && (high[i] <= low[(i)-1]) && (close[i] <= close[i-2])) { do { const tc_hit hit = {i, TC_ABANDONED_BABY_BEAR}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_abandoned_baby_bull(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (i>=2 && (open[i-2]>close[i-2]) && ((fabs(open[(i-2)] - close[(i-2)])) > (options->body_long * avg_body)) && (high[i-1] <= low[(i-1)-1]) && ((fabs(open[(i-1)] - close[(i-1)])) < (options->body_none * avg_total)) && (open[i]<close[i]) && (low[i] >= high[(i)-1]) && (close[i] >= close[i-2])) { do { const tc_hit hit = {i, TC_ABANDONED_BABY_BULL}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_big_black_candle(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if ((open[i]>close[i]) && ((fabs(open[(i)] - close[(i)])) > (options->body_long * avg_body))) { do { const tc_hit hit = {i, TC_BIG_BLACK_CANDLE}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_big_white_candle(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if ((open[i]<close[i]) && ((fabs(open[(i)] - close[(i)])) > (options->body_long * avg_body))) { do { const tc_hit hit = {i, TC_BIG_WHITE_CANDLE}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_black_marubozu(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (((fabs(open[(i)] - close[(i)])) > (options->body_long * avg_body)) && ((high[(i)] - top) < (options->wick_none * avg_total)) && ((bottom - low[(i)]) < (options->wick_none * avg_total)) && (open[i]>close[i])) { do { const tc_hit hit = {i, TC_BLACK_MARUBOZU}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_doji(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (((fabs(open[(i)] - close[(i)])) < (options->body_none * avg_total))) { do { const tc_hit hit = {i, TC_DOJI}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_dragonfly_doji(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (((fabs(open[(i)] - close[(i)])) < (options->body_none * avg_total)) && ((high[(i)] - top) < (options->wick_none * avg_total)) && ((bottom - low[(i)]) > (options->wick_long * avg_body))) { do { const tc_hit hit = {i, TC_DRAGONFLY_DOJI}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_engulfing_bear(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (i>=1 && (open[i]>close[i]) && (open[i-1]<close[i-1]) && close[i] < open[i-1] && open[i] > close[i-1]) { do { const tc_hit hit = {i, TC_ENGULFING_BEAR}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_engulfing_bull(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (i>=1 && (open[i]<close[i]) && (open[i-1]>close[i-1]) && close[i] > open[i-1] && open[i] < close[i-1]) { do { const tc_hit hit = {i, TC_ENGULFING_BULL}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_evening_doji_star(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (i>=2 && (open[i-2]<close[i-2]) && ((fabs(open[(i-2)] - close[(i-2)])) > (options->body_long * avg_body)) && ((open[(i-1)] < close[(i-1)] ? open[(i-1)] : close[(i-1)]) >= (open[((i-1)-1)] > close[((i-1)-1)] ? open[((i-1)-1)] : close[((i-1)-1)])) && ((fabs(open[(i-1)] - close[(i-1)])) < (options->body_none * avg_total)) && (open[i]>close[i]) && ((open[(i)] > close[(i)] ? open[(i)] : close[(i)]) <= (open[((i)-1)] < close[((i)-1)] ? open[((i)-1)] : close[((i)-1)])) && (close[i] <= close[i-2])) { do { const tc_hit hit = {i, TC_EVENING_DOJI_STAR}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_evening_star(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (i>=2 && (open[i-2]<close[i-2]) && ((fabs(open[(i-2)] - close[(i-2)])) > (options->body_long * avg_body)) && ((open[(i-1)] < close[(i-1)] ? open[(i-1)] : close[(i-1)]) >= (open[((i-1)-1)] > close[((i-1)-1)] ? open[((i-1)-1)] : close[((i-1)-1)])) && ((fabs(open[(i-1)] - close[(i-1)])) < (options->body_short * avg_body)) && (open[i]>close[i]) && ((open[(i)] > close[(i)] ? open[(i)] : close[(i)]) <= (open[((i)-1)] < close[((i)-1)] ? open[((i)-1)] : close[((i)-1)])) && (close[i] <= close[i-2])) { do { const tc_hit hit = {i, TC_EVENING_STAR}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_four_price_doji(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (((fabs(open[(i)] - close[(i)])) < (options->body_none * avg_total)) && ((high[(i)] - top) < (options->wick_none * avg_total)) && ((bottom - low[(i)]) < (options->wick_none * avg_total))) { do { const tc_hit hit = {i, TC_FOUR_PRICE_DOJI}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_gravestone_doji(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (((fabs(open[(i)] - close[(i)])) < (options->body_none * avg_total)) && ((high[(i)] - top) > (options->wick_long * avg_body)) && ((bottom - low[(i)]) < (options->wick_none * avg_total))) { do { const tc_hit hit = {i, TC_GRAVESTONE_DOJI}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_hammer(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (i>=1 && ((fabs(open[(i)] - close[(i)])) < (options->body_short * avg_body)) && ((high[(i)] - top) < (options->wick_none * avg_total)) && ((bottom - low[(i)]) > (options->wick_long * avg_body)) && (open[(i)] < close[(i)] ? open[(i)] : close[(i)]) <= low[i-1] + (options->near * avg_total)) { do { const tc_hit hit = {i, TC_HAMMER}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_hanging_man(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (i>=1 && ((fabs(open[(i)] - close[(i)])) < (options->body_short * avg_body)) && ((high[(i)] - top) < (options->wick_none * avg_total)) && ((bottom - low[(i)]) > (options->wick_long * avg_body)) && (open[(i)] < close[(i)] ? open[(i)] : close[(i)]) >= high[i-1] - (options->near * avg_total)) { do { const tc_hit hit = {i, TC_HANGING_MAN}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_inverted_hammer(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (i>=1 && ((fabs(open[(i)] - close[(i)])) < (options->body_short * avg_body)) && ((high[(i)] - top) > (options->wick_long * avg_body)) && ((bottom - low[(i)]) < (options->wick_none * avg_total)) && ((open[(i)] > close[(i)] ? open[(i)] : close[(i)]) <= (open[((i)-1)] < close[((i)-1)] ? open[((i)-1)] : close[((i)-1)]))) { do { const tc_hit hit = {i, TC_INVERTED_HAMMER}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_long_legged_doji(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (((fabs(open[(i)] - close[(i)])) < (options->body_none * avg_total)) && ((high[(i)] - top) > (options->wick_long * avg_body)) && ((bottom - low[(i)]) > (options->wick_long * avg_body))) { do { const tc_hit hit = {i, TC_LONG_LEGGED_DOJI}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_marubozu(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (((fabs(open[(i)] - close[(i)])) > (options->body_long * avg_body)) && ((high[(i)] - top) < (options->wick_none * avg_total)) && ((bottom - low[(i)]) < (options->wick_none * avg_total))) { do { const tc_hit hit = {i, TC_MARUBOZU}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_morning_doji_star(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (i>=2 && (open[i-2]>close[i-2]) && ((fabs(open[(i-2)] - close[(i-2)])) > (options->body_long * avg_body)) && ((open[(i-1)] > close[(i-1)] ? open[(i-1)] : close[(i-1)]) <= (open[((i-1)-1)] < close[((i-1)-1)] ? open[((i-1)-1)] : close[((i-1)-1)])) && ((fabs(open[(i-1)] - close[(i-1)])) < (options->body_none * avg_total)) && (open[i]<close[i]) && ((open[(i)] < close[(i)] ? open[(i)] : close[(i)]) >= (open[((i)-1)] > close[((i)-1)] ? open[((i)-1)] : close[((i)-1)])) && (close[i] >= close[i-2])) { do { const tc_hit hit = {i, TC_MORNING_DOJI_STAR}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_morning_star(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (i>=2 && (open[i-2]>close[i-2]) && ((fabs(open[(i-2)] - close[(i-2)])) > (options->body_long * avg_body)) && ((open[(i-1)] > close[(i-1)] ? open[(i-1)] : close[(i-1)]) <= (open[((i-1)-1)] < close[((i-1)-1)] ? open[((i-1)-1)] : close[((i-1)-1)])) && ((fabs(open[(i-1)] - close[(i-1)])) < (options->body_short * avg_body)) && (open[i]<close[i]) && ((open[(i)] < close[(i)] ? open[(i)] : close[(i)]) >= (open[((i)-1)] > close[((i)-1)] ? open[((i)-1)] : close[((i)-1)])) && (close[i] >= close[i-2])) { do { const tc_hit hit = {i, TC_MORNING_STAR}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_shooting_star(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (i>=1 && ((fabs(open[(i)] - close[(i)])) < (options->body_short * avg_body)) && ((high[(i)] - top) > (options->wick_long * avg_body)) && ((bottom - low[(i)]) < (options->wick_none * avg_total)) && ((open[(i)] < close[(i)] ? open[(i)] : close[(i)]) >= (open[((i)-1)] > close[((i)-1)] ? open[((i)-1)] : close[((i)-1)]))) { do { const tc_hit hit = {i, TC_SHOOTING_STAR}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_spinning_top(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (((fabs(open[(i)] - close[(i)])) < (options->body_short * avg_body)) && ((high[(i)] - top) > (fabs(open[(i)] - close[(i)]))) && ((bottom - low[(i)]) > (fabs(open[(i)] - close[(i)])))) { do { const tc_hit hit = {i, TC_SPINNING_TOP}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_star(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (i>=1 && ((low[i] >= high[(i)-1]) || (high[i] <= low[(i)-1]))) { do { const tc_hit hit = {i, TC_STAR}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_three_black_crows(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (i>=2 && (open[i-2]>close[i-2]) && (open[i-1]>close[i-1]) && (open[i]>close[i]) && (open[i-1] < (open[((i-1)-1)] > close[((i-1)-1)] ? open[((i-1)-1)] : close[((i-1)-1)]) && open[i-1] > (open[((i-1)-1)] < close[((i-1)-1)] ? open[((i-1)-1)] : close[((i-1)-1)])) && (open[i] < (open[((i)-1)] > close[((i)-1)] ? open[((i)-1)] : close[((i)-1)]) && open[i] > (open[((i)-1)] < close[((i)-1)] ? open[((i)-1)] : close[((i)-1)]))) { do { const tc_hit hit = {i, TC_THREE_BLACK_CROWS}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_three_white_soldiers(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (i>=2 && (open[i-2]<close[i-2]) && (open[i-1]<close[i-1]) && (open[i]<close[i]) && (open[i-1] < (open[((i-1)-1)] > close[((i-1)-1)] ? open[((i-1)-1)] : close[((i-1)-1)]) && open[i-1] > (open[((i-1)-1)] < close[((i-1)-1)] ? open[((i-1)-1)] : close[((i-1)-1)])) && (open[i] < (open[((i)-1)] > close[((i)-1)] ? open[((i)-1)] : close[((i)-1)]) && open[i] > (open[((i)-1)] < close[((i)-1)] ? open[((i)-1)] : close[((i)-1)]))) { do { const tc_hit hit = {i, TC_THREE_WHITE_SOLDIERS}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
int tc_white_marubozu(int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        const TC_REAL *open = inputs[0]; const TC_REAL *high = inputs[1]; const TC_REAL *low = inputs[2]; const TC_REAL *close = inputs[3]; const int period = options->period; const TC_REAL div = 1.0 / period; TC_REAL avg_body_sum = 0; TC_REAL avg_total_sum = 0; if (period < 1) return TC_INVALID_OPTION; if (size < period) return TC_OKAY; int i; for (i = 0; i < period; ++i) { avg_body_sum += (fabs(open[(i)] - close[(i)])); avg_total_sum += (high[(i)] - low[(i)]); }
        while(i < size) { const TC_REAL top = (open[(i)] > close[(i)] ? open[(i)] : close[(i)]); const TC_REAL bottom = (open[(i)] < close[(i)] ? open[(i)] : close[(i)]); const TC_REAL body = (fabs(open[(i)] - close[(i)])); const TC_REAL total = (high[(i)] - low[(i)]); const TC_REAL upper = (high[(i)] - top); const TC_REAL lower = (bottom - low[(i)]); const TC_REAL avg_body = avg_body_sum * div; const TC_REAL avg_total = avg_total_sum * div; (void)top;(void)bottom;(void)body;(void)total; (void)upper;(void)lower;(void)avg_body;(void)avg_total; if (((fabs(open[(i)] - close[(i)])) > (options->body_long * avg_body)) && ((high[(i)] - top) < (options->wick_none * avg_total)) && ((bottom - low[(i)]) < (options->wick_none * avg_total)) && (open[i]<close[i])) { do { const tc_hit hit = {i, TC_WHITE_MARUBOZU}; const int r = tc_result_add(output, hit); if (r != TC_OKAY) return r; } while (0); } avg_body_sum += body; avg_body_sum -= (fabs(open[(i-period)] - close[(i-period)])); avg_total_sum += total; avg_total_sum -= (high[(i-period)] - low[(i-period)]); ++i; };
        return TC_OKAY;
    }
