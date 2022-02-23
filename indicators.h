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

