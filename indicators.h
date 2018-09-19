/*
 * Tulip Indicators
 * https://tulipindicators.org/
 * Copyright (c) 2010-2018 Tulip Charts LLC
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
 * Version 0.8.4
 * Header Build 1537377628
 *
 */

 /*
  *
  * This file is generated. Do not modify directly.
  *
  */

#ifndef __TI_INDICATORS_H__
#define __TI_INDICATORS_H__


#define TI_VERSION "0.8.4"
#define TI_BUILD 1537377628


#ifndef TI_SKIP_SYSTEM_HEADERS
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


const char* ti_version();
long int ti_build();





#define TI_REAL double

#define TI_INDICATOR_COUNT 104 /* Total number of indicators. */

#define TI_OKAY                    0
#define TI_INVALID_OPTION          1

#define TI_TYPE_OVERLAY            1 /* These have roughly the same range as the input data. */
#define TI_TYPE_INDICATOR          2 /* Everything else (e.g. oscillators). */
#define TI_TYPE_MATH               3 /* These aren't so good for plotting, but are useful with formulas. */
#define TI_TYPE_SIMPLE             4 /* These apply a simple operator (e.g. addition, sin, sqrt). */
#define TI_TYPE_COMPARATIVE        5 /* These are designed to take inputs from different securities. i.e. compare stock A to stock B.*/

#define TI_MAXINDPARAMS 10 /* No indicator will use more than this many inputs, options, or outputs. */


typedef int (*ti_indicator_start_function)(TI_REAL const *options);
typedef int (*ti_indicator_function)(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);

typedef struct ti_indicator_info {
    char *name;
    char *full_name;
    ti_indicator_start_function start;
    ti_indicator_function indicator;
    int type, inputs, options, outputs;
    char *input_names[TI_MAXINDPARAMS];
    char *option_names[TI_MAXINDPARAMS];
    char *output_names[TI_MAXINDPARAMS];
} ti_indicator_info;


/*Complete array of all indicators. Last element is 0,0,0,0...*/
extern ti_indicator_info ti_indicators[];


/*Searches for an indicator by name. Returns 0 if not found.*/
const ti_indicator_info *ti_find_indicator(const char *name);






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
int ti_abs_start(TI_REAL const *options);
int ti_abs(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Arccosine */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: acos */
int ti_acos_start(TI_REAL const *options);
int ti_acos(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Accumulation/Distribution Line */
/* Type: indicator */
/* Input arrays: 4    Options: 0    Output arrays: 1 */
/* Inputs: high, low, close, volume */
/* Options: none */
/* Outputs: ad */
int ti_ad_start(TI_REAL const *options);
int ti_ad(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Addition */
/* Type: simple */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: real, real */
/* Options: none */
/* Outputs: add */
int ti_add_start(TI_REAL const *options);
int ti_add(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Accumulation/Distribution Oscillator */
/* Type: indicator */
/* Input arrays: 4    Options: 2    Output arrays: 1 */
/* Inputs: high, low, close, volume */
/* Options: short period, long period */
/* Outputs: adosc */
int ti_adosc_start(TI_REAL const *options);
int ti_adosc(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Average Directional Movement Index */
/* Type: indicator */
/* Input arrays: 3    Options: 1    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: period */
/* Outputs: dx */
int ti_adx_start(TI_REAL const *options);
int ti_adx(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Average Directional Movement Rating */
/* Type: indicator */
/* Input arrays: 3    Options: 1    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: period */
/* Outputs: dx */
int ti_adxr_start(TI_REAL const *options);
int ti_adxr(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Awesome Oscillator */
/* Type: indicator */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: high, low */
/* Options: none */
/* Outputs: ao */
int ti_ao_start(TI_REAL const *options);
int ti_ao(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Absolute Price Oscillator */
/* Type: indicator */
/* Input arrays: 1    Options: 2    Output arrays: 1 */
/* Inputs: real */
/* Options: short period, long period */
/* Outputs: apo */
int ti_apo_start(TI_REAL const *options);
int ti_apo(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Aroon */
/* Type: indicator */
/* Input arrays: 2    Options: 1    Output arrays: 2 */
/* Inputs: high, low */
/* Options: period */
/* Outputs: aroon_down, aroon_up */
int ti_aroon_start(TI_REAL const *options);
int ti_aroon(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Aroon Oscillator */
/* Type: indicator */
/* Input arrays: 2    Options: 1    Output arrays: 1 */
/* Inputs: high, low */
/* Options: period */
/* Outputs: aroonosc */
int ti_aroonosc_start(TI_REAL const *options);
int ti_aroonosc(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Arcsine */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: asin */
int ti_asin_start(TI_REAL const *options);
int ti_asin(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Arctangent */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: atan */
int ti_atan_start(TI_REAL const *options);
int ti_atan(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Average True Range */
/* Type: indicator */
/* Input arrays: 3    Options: 1    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: period */
/* Outputs: atr */
int ti_atr_start(TI_REAL const *options);
int ti_atr(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Average Price */
/* Type: overlay */
/* Input arrays: 4    Options: 0    Output arrays: 1 */
/* Inputs: open, high, low, close */
/* Options: none */
/* Outputs: avgprice */
int ti_avgprice_start(TI_REAL const *options);
int ti_avgprice(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Bollinger Bands */
/* Type: overlay */
/* Input arrays: 1    Options: 2    Output arrays: 3 */
/* Inputs: real */
/* Options: period, stddev */
/* Outputs: bbands_lower, bbands_middle, bbands_upper */
int ti_bbands_start(TI_REAL const *options);
int ti_bbands(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Balance of Power */
/* Type: indicator */
/* Input arrays: 4    Options: 0    Output arrays: 1 */
/* Inputs: open, high, low, close */
/* Options: none */
/* Outputs: bop */
int ti_bop_start(TI_REAL const *options);
int ti_bop(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Commodity Channel Index */
/* Type: indicator */
/* Input arrays: 3    Options: 1    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: period */
/* Outputs: cci */
int ti_cci_start(TI_REAL const *options);
int ti_cci(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Ceiling */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: ceil */
int ti_ceil_start(TI_REAL const *options);
int ti_ceil(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Chande Momentum Oscillator */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: cmo */
int ti_cmo_start(TI_REAL const *options);
int ti_cmo(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Cosine */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: cos */
int ti_cos_start(TI_REAL const *options);
int ti_cos(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Hyperbolic Cosine */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: cosh */
int ti_cosh_start(TI_REAL const *options);
int ti_cosh(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Crossany */
/* Type: math */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: real, real */
/* Options: none */
/* Outputs: crossany */
int ti_crossany_start(TI_REAL const *options);
int ti_crossany(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Crossover */
/* Type: math */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: real, real */
/* Options: none */
/* Outputs: crossover */
int ti_crossover_start(TI_REAL const *options);
int ti_crossover(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Chaikins Volatility */
/* Type: indicator */
/* Input arrays: 2    Options: 1    Output arrays: 1 */
/* Inputs: high, low */
/* Options: period */
/* Outputs: cvi */
int ti_cvi_start(TI_REAL const *options);
int ti_cvi(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Linear Decay */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: decay */
int ti_decay_start(TI_REAL const *options);
int ti_decay(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Double Exponential Moving Average */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: dema */
int ti_dema_start(TI_REAL const *options);
int ti_dema(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Directional Indicator */
/* Type: indicator */
/* Input arrays: 3    Options: 1    Output arrays: 2 */
/* Inputs: high, low, close */
/* Options: period */
/* Outputs: plus_di, minus_di */
int ti_di_start(TI_REAL const *options);
int ti_di(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Division */
/* Type: simple */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: real, real */
/* Options: none */
/* Outputs: div */
int ti_div_start(TI_REAL const *options);
int ti_div(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Directional Movement */
/* Type: indicator */
/* Input arrays: 2    Options: 1    Output arrays: 2 */
/* Inputs: high, low */
/* Options: period */
/* Outputs: plus_dm, minus_dm */
int ti_dm_start(TI_REAL const *options);
int ti_dm(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Detrended Price Oscillator */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: dpo */
int ti_dpo_start(TI_REAL const *options);
int ti_dpo(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Directional Movement Index */
/* Type: indicator */
/* Input arrays: 3    Options: 1    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: period */
/* Outputs: dx */
int ti_dx_start(TI_REAL const *options);
int ti_dx(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Exponential Decay */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: edecay */
int ti_edecay_start(TI_REAL const *options);
int ti_edecay(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Exponential Moving Average */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: ema */
int ti_ema_start(TI_REAL const *options);
int ti_ema(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Ease of Movement */
/* Type: indicator */
/* Input arrays: 3    Options: 0    Output arrays: 1 */
/* Inputs: high, low, volume */
/* Options: none */
/* Outputs: emv */
int ti_emv_start(TI_REAL const *options);
int ti_emv(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Exponential */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: exp */
int ti_exp_start(TI_REAL const *options);
int ti_exp(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Fisher Transform */
/* Type: indicator */
/* Input arrays: 2    Options: 1    Output arrays: 2 */
/* Inputs: high, low */
/* Options: period */
/* Outputs: fisher, fisher_signal */
int ti_fisher_start(TI_REAL const *options);
int ti_fisher(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Floor */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: floor */
int ti_floor_start(TI_REAL const *options);
int ti_floor(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Forecast Oscillator */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: fosc */
int ti_fosc_start(TI_REAL const *options);
int ti_fosc(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Hull Moving Average */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: hma */
int ti_hma_start(TI_REAL const *options);
int ti_hma(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Kaufman Adaptive Moving Average */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: kama */
int ti_kama_start(TI_REAL const *options);
int ti_kama(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Klinger Volume Oscillator */
/* Type: indicator */
/* Input arrays: 4    Options: 2    Output arrays: 1 */
/* Inputs: high, low, close, volume */
/* Options: short period, long period */
/* Outputs: kvo */
int ti_kvo_start(TI_REAL const *options);
int ti_kvo(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Lag */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: lag */
int ti_lag_start(TI_REAL const *options);
int ti_lag(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Linear Regression */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: linreg */
int ti_linreg_start(TI_REAL const *options);
int ti_linreg(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Linear Regression Intercept */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: linregintercept */
int ti_linregintercept_start(TI_REAL const *options);
int ti_linregintercept(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Linear Regression Slope */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: linregslope */
int ti_linregslope_start(TI_REAL const *options);
int ti_linregslope(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Natural Log */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: ln */
int ti_ln_start(TI_REAL const *options);
int ti_ln(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Base-10 Log */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: log10 */
int ti_log10_start(TI_REAL const *options);
int ti_log10(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Moving Average Convergence/Divergence */
/* Type: indicator */
/* Input arrays: 1    Options: 3    Output arrays: 3 */
/* Inputs: real */
/* Options: short period, long period, signal period */
/* Outputs: macd, macd_signal, macd_histogram */
int ti_macd_start(TI_REAL const *options);
int ti_macd(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Market Facilitation Index */
/* Type: indicator */
/* Input arrays: 3    Options: 0    Output arrays: 1 */
/* Inputs: high, low, volume */
/* Options: none */
/* Outputs: marketfi */
int ti_marketfi_start(TI_REAL const *options);
int ti_marketfi(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Mass Index */
/* Type: indicator */
/* Input arrays: 2    Options: 1    Output arrays: 1 */
/* Inputs: high, low */
/* Options: period */
/* Outputs: mass */
int ti_mass_start(TI_REAL const *options);
int ti_mass(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Maximum In Period */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: max */
int ti_max_start(TI_REAL const *options);
int ti_max(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Mean Deviation Over Period */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: md */
int ti_md_start(TI_REAL const *options);
int ti_md(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Median Price */
/* Type: overlay */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: high, low */
/* Options: none */
/* Outputs: medprice */
int ti_medprice_start(TI_REAL const *options);
int ti_medprice(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Money Flow Index */
/* Type: indicator */
/* Input arrays: 4    Options: 1    Output arrays: 1 */
/* Inputs: high, low, close, volume */
/* Options: period */
/* Outputs: mfi */
int ti_mfi_start(TI_REAL const *options);
int ti_mfi(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Minimum In Period */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: min */
int ti_min_start(TI_REAL const *options);
int ti_min(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Momentum */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: mom */
int ti_mom_start(TI_REAL const *options);
int ti_mom(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Mesa Sine Wave */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 2 */
/* Inputs: real */
/* Options: period */
/* Outputs: msw_sine, msw_lead */
int ti_msw_start(TI_REAL const *options);
int ti_msw(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Multiplication */
/* Type: simple */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: real, real */
/* Options: none */
/* Outputs: mul */
int ti_mul_start(TI_REAL const *options);
int ti_mul(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Normalized Average True Range */
/* Type: indicator */
/* Input arrays: 3    Options: 1    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: period */
/* Outputs: natr */
int ti_natr_start(TI_REAL const *options);
int ti_natr(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Negative Volume Index */
/* Type: indicator */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: close, volume */
/* Options: none */
/* Outputs: nvi */
int ti_nvi_start(TI_REAL const *options);
int ti_nvi(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* On Balance Volume */
/* Type: indicator */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: close, volume */
/* Options: none */
/* Outputs: obv */
int ti_obv_start(TI_REAL const *options);
int ti_obv(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Percentage Price Oscillator */
/* Type: indicator */
/* Input arrays: 1    Options: 2    Output arrays: 1 */
/* Inputs: real */
/* Options: short period, long period */
/* Outputs: ppo */
int ti_ppo_start(TI_REAL const *options);
int ti_ppo(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Parabolic SAR */
/* Type: overlay */
/* Input arrays: 2    Options: 2    Output arrays: 1 */
/* Inputs: high, low */
/* Options: acceleration factor step, acceleration factor maximum */
/* Outputs: psar */
int ti_psar_start(TI_REAL const *options);
int ti_psar(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Positive Volume Index */
/* Type: indicator */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: close, volume */
/* Options: none */
/* Outputs: pvi */
int ti_pvi_start(TI_REAL const *options);
int ti_pvi(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Qstick */
/* Type: indicator */
/* Input arrays: 2    Options: 1    Output arrays: 1 */
/* Inputs: open, close */
/* Options: period */
/* Outputs: qstick */
int ti_qstick_start(TI_REAL const *options);
int ti_qstick(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Rate of Change */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: roc */
int ti_roc_start(TI_REAL const *options);
int ti_roc(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Rate of Change Ratio */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: rocr */
int ti_rocr_start(TI_REAL const *options);
int ti_rocr(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Round */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: round */
int ti_round_start(TI_REAL const *options);
int ti_round(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Relative Strength Index */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: rsi */
int ti_rsi_start(TI_REAL const *options);
int ti_rsi(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Sine */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: sin */
int ti_sin_start(TI_REAL const *options);
int ti_sin(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Hyperbolic Sine */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: sinh */
int ti_sinh_start(TI_REAL const *options);
int ti_sinh(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Simple Moving Average */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: sma */
int ti_sma_start(TI_REAL const *options);
int ti_sma(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Square Root */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: sqrt */
int ti_sqrt_start(TI_REAL const *options);
int ti_sqrt(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Standard Deviation Over Period */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: stddev */
int ti_stddev_start(TI_REAL const *options);
int ti_stddev(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Standard Error Over Period */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: stderr */
int ti_stderr_start(TI_REAL const *options);
int ti_stderr(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Stochastic Oscillator */
/* Type: indicator */
/* Input arrays: 3    Options: 3    Output arrays: 2 */
/* Inputs: high, low, close */
/* Options: %k period, %k slowing period, %d period */
/* Outputs: stoch_k, stoch_d */
int ti_stoch_start(TI_REAL const *options);
int ti_stoch(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Stochastic RSI */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: stochrsi */
int ti_stochrsi_start(TI_REAL const *options);
int ti_stochrsi(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Subtraction */
/* Type: simple */
/* Input arrays: 2    Options: 0    Output arrays: 1 */
/* Inputs: real, real */
/* Options: none */
/* Outputs: sub */
int ti_sub_start(TI_REAL const *options);
int ti_sub(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Sum Over Period */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: sum */
int ti_sum_start(TI_REAL const *options);
int ti_sum(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Tangent */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: tan */
int ti_tan_start(TI_REAL const *options);
int ti_tan(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Hyperbolic Tangent */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: tanh */
int ti_tanh_start(TI_REAL const *options);
int ti_tanh(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Triple Exponential Moving Average */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: tema */
int ti_tema_start(TI_REAL const *options);
int ti_tema(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Degree Conversion */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: degrees */
int ti_todeg_start(TI_REAL const *options);
int ti_todeg(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Radian Conversion */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: radians */
int ti_torad_start(TI_REAL const *options);
int ti_torad(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* True Range */
/* Type: indicator */
/* Input arrays: 3    Options: 0    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: none */
/* Outputs: tr */
int ti_tr_start(TI_REAL const *options);
int ti_tr(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Triangular Moving Average */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: trima */
int ti_trima_start(TI_REAL const *options);
int ti_trima(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Trix */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: trix */
int ti_trix_start(TI_REAL const *options);
int ti_trix(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vector Truncate */
/* Type: simple */
/* Input arrays: 1    Options: 0    Output arrays: 1 */
/* Inputs: real */
/* Options: none */
/* Outputs: trunc */
int ti_trunc_start(TI_REAL const *options);
int ti_trunc(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Time Series Forecast */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: tsf */
int ti_tsf_start(TI_REAL const *options);
int ti_tsf(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Typical Price */
/* Type: overlay */
/* Input arrays: 3    Options: 0    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: none */
/* Outputs: typprice */
int ti_typprice_start(TI_REAL const *options);
int ti_typprice(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Ultimate Oscillator */
/* Type: indicator */
/* Input arrays: 3    Options: 3    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: short period, medium period, long period */
/* Outputs: ultosc */
int ti_ultosc_start(TI_REAL const *options);
int ti_ultosc(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Variance Over Period */
/* Type: math */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: var */
int ti_var_start(TI_REAL const *options);
int ti_var(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Vertical Horizontal Filter */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: vhf */
int ti_vhf_start(TI_REAL const *options);
int ti_vhf(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Variable Index Dynamic Average */
/* Type: overlay */
/* Input arrays: 1    Options: 3    Output arrays: 1 */
/* Inputs: real */
/* Options: short period, long period, alpha */
/* Outputs: vidya */
int ti_vidya_start(TI_REAL const *options);
int ti_vidya(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Annualized Historical Volatility */
/* Type: indicator */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: volatility */
int ti_volatility_start(TI_REAL const *options);
int ti_volatility(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Volume Oscillator */
/* Type: indicator */
/* Input arrays: 1    Options: 2    Output arrays: 1 */
/* Inputs: volume */
/* Options: short period, long period */
/* Outputs: vosc */
int ti_vosc_start(TI_REAL const *options);
int ti_vosc(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Volume Weighted Moving Average */
/* Type: overlay */
/* Input arrays: 2    Options: 1    Output arrays: 1 */
/* Inputs: close, volume */
/* Options: period */
/* Outputs: vwma */
int ti_vwma_start(TI_REAL const *options);
int ti_vwma(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Williams Accumulation/Distribution */
/* Type: indicator */
/* Input arrays: 3    Options: 0    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: none */
/* Outputs: wad */
int ti_wad_start(TI_REAL const *options);
int ti_wad(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Weighted Close Price */
/* Type: overlay */
/* Input arrays: 3    Options: 0    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: none */
/* Outputs: wcprice */
int ti_wcprice_start(TI_REAL const *options);
int ti_wcprice(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Wilders Smoothing */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: wilders */
int ti_wilders_start(TI_REAL const *options);
int ti_wilders(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Williams %R */
/* Type: indicator */
/* Input arrays: 3    Options: 1    Output arrays: 1 */
/* Inputs: high, low, close */
/* Options: period */
/* Outputs: willr */
int ti_willr_start(TI_REAL const *options);
int ti_willr(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Weighted Moving Average */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: wma */
int ti_wma_start(TI_REAL const *options);
int ti_wma(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);





/* Zero-Lag Exponential Moving Average */
/* Type: overlay */
/* Input arrays: 1    Options: 1    Output arrays: 1 */
/* Inputs: real */
/* Options: period */
/* Outputs: zlema */
int ti_zlema_start(TI_REAL const *options);
int ti_zlema(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs);



#ifdef __cplusplus
}
#endif

#endif /*__TI_INDICATORS_H__*/
