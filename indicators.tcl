# Tulip Indicators
# https://tulipindicators.org/
# Copyright (c) 2010-2018 Tulip Charts LLC
# Lewis Van Winkle (LV@tulipcharts.org)
#
# This file is part of Tulip Indicators.
#
# Tulip Indicators is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by the
# Free Software Foundation, either version 3 of the License, or (at your
# option) any later version.
#
# Tulip Indicators is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
# for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Tulip Indicators.  If not, see <http://www.gnu.org/licenses/>.


set license "/*
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
"

set version 0.8.4
set build [clock seconds]


rename open open_old
proc open {n m} {
    puts "Writing $n"
    set k [open_old $n $m]
    fconfigure $k -translation {auto lf}
    return $k
}



puts "Generate interface for indicator library."


set indicators {}
#type name func_name inputs options outputs

#Moving averages
lappend indicators [list overlay "Double Exponential Moving Average" dema 1 1 1 {real} {period} {dema}]
lappend indicators [list overlay "Exponential Moving Average" ema 1 1 1 {real} {period} {ema}]
lappend indicators [list overlay "Hull Moving Average" hma 1 1 1 {real} {period} {hma}]
lappend indicators [list overlay "Simple Moving Average" sma 1 1 1 {real} {period} {sma}]
lappend indicators [list overlay "Triple Exponential Moving Average" tema 1 1 1 {real} {period} {tema}]
lappend indicators [list overlay "Triangular Moving Average" trima 1 1 1 {real} {period} {trima}]
lappend indicators [list overlay "Variable Index Dynamic Average" vidya 1 3 1 {real} {{short period} {long period} alpha} {vidya}]
lappend indicators [list overlay "Volume Weighted Moving Average" vwma 2 1 1 {close volume} {period} {vwma}]
lappend indicators [list overlay "Wilders Smoothing" wilders 1 1 1 {real} {period} {wilders}]
lappend indicators [list overlay "Weighted Moving Average" wma 1 1 1 {real} {period} {wma}]
lappend indicators [list overlay "Zero-Lag Exponential Moving Average" zlema 1 1 1 {real} {period} {zlema}]

#Line fitting
lappend indicators [list overlay "Linear Regression" linreg 1 1 1 {real} {period} {linreg}]
lappend indicators [list indicator "Linear Regression Intercept" linregintercept 1 1 1 {real} {period} {linregintercept}]
lappend indicators [list indicator "Linear Regression Slope" linregslope 1 1 1 {real} {period} {linregslope}]
lappend indicators [list overlay "Time Series Forecast" tsf 1 1 1 {real} {period} {tsf}]
lappend indicators [list indicator "Forecast Oscillator" fosc 1 1 1 {real} {period} {fosc}]

#Special moving averages and other overlays
lappend indicators [list overlay "Bollinger Bands" bbands 1 2 3 {real} {period stddev} {bbands_lower bbands_middle bbands_upper} {input input input}]
lappend indicators [list overlay "Kaufman Adaptive Moving Average" kama 1 1 1 {real} {period} {kama}]
lappend indicators [list overlay "Parabolic SAR" psar 2 2 1 {high low} {{acceleration factor step} {acceleration factor maximum}} {psar}]

#Momentum
lappend indicators [list indicator "Accumulation/Distribution Line" ad 4 0 1 {high low close volume} {} {ad}]
lappend indicators [list indicator "Accumulation/Distribution Oscillator" adosc 4 2 1 {high low close volume} {{short period} {long period}} {adosc}]
lappend indicators [list indicator "Absolute Price Oscillator" apo 1 2 1 {real} {{short period} {long period}} {apo}]
lappend indicators [list indicator "Aroon" aroon 2 1 2 {high low} {period} {aroon_down aroon_up}]
lappend indicators [list indicator "Aroon Oscillator" aroonosc 2 1 1 {high low} {period} {aroonosc}]
lappend indicators [list indicator "Awesome Oscillator" ao 2 0 1 {high low} {} {ao}]
lappend indicators [list indicator "Balance of Power" bop 4 0 1 {open high low close} {} {bop}]
lappend indicators [list indicator "Chande Momentum Oscillator" cmo 1 1 1 {real} {period} {cmo}]
lappend indicators [list indicator "Commodity Channel Index" cci 3 1 1 {high low close} {period} {cci}]
lappend indicators [list indicator "Detrended Price Oscillator" dpo 1 1 1 {real} {period} {dpo}]
lappend indicators [list indicator "Ease of Movement" emv 3 0 1 {high low volume} {} {emv}]
lappend indicators [list indicator "Fisher Transform" fisher 2 1 2 {high low} {period} {fisher fisher_signal}]
lappend indicators [list indicator "Klinger Volume Oscillator" kvo 4 2 1 {high low close volume} {{short period} {long period}} {kvo}]
lappend indicators [list indicator "Market Facilitation Index" marketfi 3 0 1 {high low volume} {} {marketfi}]
lappend indicators [list indicator "Mass Index" mass 2 1 1 {high low} {period} {mass}]
lappend indicators [list indicator "Money Flow Index" mfi 4 1 1 {high low close volume} {period} {mfi}]
lappend indicators [list indicator "Moving Average Convergence/Divergence" macd 1 3 3 {real} {{short period} {long period} {signal period}} {macd macd_signal macd_histogram}]
lappend indicators [list indicator "Negative Volume Index" nvi 2 0 1 {close volume} {} {nvi}]
lappend indicators [list indicator "On Balance Volume" obv 2 0 1 {close volume} {} {obv}]
lappend indicators [list indicator "Percentage Price Oscillator" ppo 1 2 1 {real} {{short period} {long period}} {ppo}]
lappend indicators [list indicator "Positive Volume Index" pvi 2 0 1 {close volume} {} {pvi}]
lappend indicators [list indicator "Qstick" qstick 2 1 1 {open close} {period} {qstick}]
lappend indicators [list indicator "Relative Strength Index" rsi 1 1 1 {real} {period} {rsi}]
lappend indicators [list indicator "Stochastic Oscillator" stoch 3 3 2 {high low close} {{%k period} {%k slowing period} {%d period}} {stoch_k stoch_d}]
lappend indicators [list indicator "Stochastic RSI" stochrsi 1 1 1 {real} {period} {stochrsi}]
lappend indicators [list indicator "Trix" trix 1 1 1 {real} {period} {trix}]
lappend indicators [list indicator "Williams Accumulation/Distribution" wad 3 0 1 {high low close} {} {wad}]
lappend indicators [list indicator "Williams %R" willr 3 1 1 {high low close} {period} {willr}]
lappend indicators [list indicator "Ultimate Oscillator" ultosc 3 3 1 {high low close} {{short period} {medium period} {long period}} {ultosc}]
lappend indicators [list indicator "Vertical Horizontal Filter" vhf 1 1 1 {real} {period} {vhf}]
lappend indicators [list indicator "Volume Oscillator" vosc 1 2 1 {volume} {{short period} {long period}} {vosc}]

#DX ADX ADXR etc
lappend indicators [list indicator "Directional Movement" dm 2 1 2 {high low} {period} {plus_dm minus_dm}]
lappend indicators [list indicator "Directional Movement Index" dx 3 1 1 {high low close} {period} {dx}]
lappend indicators [list indicator "Directional Indicator" di 3 1 2 {high low close} {period} {plus_di minus_di}]
lappend indicators [list indicator "Average Directional Movement Index" adx 3 1 1 {high low close} {period} {adx}]
lappend indicators [list indicator "Average Directional Movement Rating" adxr 3 1 1 {high low close} {period} {adxr}]

#Waves
lappend indicators [list indicator "Mesa Sine Wave" msw 1 1 2 {real} {period} {msw_sine msw_lead}]

#Rate of return, rate of change, momentum indicators
lappend indicators [list indicator "Momentum" mom 1 1 1 {real} {period} {mom}]
lappend indicators [list indicator "Rate of Change" roc 1 1 1 {real} {period} {roc}]
lappend indicators [list indicator "Rate of Change Ratio" rocr 1 1 1 {real} {period} {rocr}]

#Math functions
lappend indicators [list math "Lag" lag 1 1 1 {real} {period} {lag}]
lappend indicators [list math "Maximum In Period" max 1 1 1 {real} {period} {max}]
lappend indicators [list math "Minimum In Period" min 1 1 1 {real} {period} {min}]
lappend indicators [list math "Sum Over Period" sum 1 1 1 {real} {period} {sum}]
lappend indicators [list math "Standard Deviation Over Period" stddev 1 1 1 {real} {period} {stddev}]
lappend indicators [list math "Standard Error Over Period" stderr 1 1 1 {real} {period} {stderr}]
lappend indicators [list math "Mean Deviation Over Period" md 1 1 1 {real} {period} {md}]
lappend indicators [list math "Variance Over Period" var 1 1 1 {real} {period} {var}]

#Price average functions
lappend indicators [list overlay "Average Price" avgprice 4 0 1 {open high low close} {} {avgprice}]
lappend indicators [list overlay "Median Price" medprice 2 0 1 {high low} {} {medprice}]
lappend indicators [list overlay "Typical Price" typprice 3 0 1 {high low close} {} {typprice}]
lappend indicators [list overlay "Weighted Close Price" wcprice 3 0 1 {high low close} {} {wcprice}]

#Volatility
lappend indicators [list indicator "Average True Range" atr 3 1 1 {high low close} {period} {atr}]
lappend indicators [list indicator "Normalized Average True Range" natr 3 1 1 {high low close} {period} {natr}]
lappend indicators [list indicator "True Range" tr 3 0 1 {high low close} {} {tr}]
lappend indicators [list indicator "Annualized Historical Volatility" volatility 1 1 1 {real} {period} {volatility}]
lappend indicators [list indicator "Chaikins Volatility" cvi 2 1 1 {high low} {period} {cvi}]

#Utility functions
lappend indicators [list math "Crossany" crossany 2 0 1 {real real} {} {crossany}]
lappend indicators [list math "Crossover" crossover 2 0 1 {real real} {} {crossover}]
lappend indicators [list math "Linear Decay" decay 1 1 1 {real} {period} {decay}]
lappend indicators [list math "Exponential Decay" edecay 1 1 1 {real} {period} {edecay}]

#Simple functions taking two inputs
lappend indicators [list simple "Vector Addition" add 2 0 1 {real real} {} {add}]
lappend indicators [list simple "Vector Subtraction" sub 2 0 1 {real real} {} {sub}]
lappend indicators [list simple "Vector Multiplication" mul 2 0 1 {real real} {} {mul}]
lappend indicators [list simple "Vector Division" div 2 0 1 {real real} {} {div}]

#Simple functions taking one input
lappend indicators [list simple "Vector Absolute Value" abs 1 0 1 {real} {} {abs}]
lappend indicators [list simple "Vector Arccosine" acos 1 0 1 {real} {} {acos}]
lappend indicators [list simple "Vector Arcsine" asin 1 0 1 {real} {} {asin}]
lappend indicators [list simple "Vector Arctangent" atan 1 0 1 {real} {} {atan}]
lappend indicators [list simple "Vector Ceiling" ceil 1 0 1 {real} {} {ceil}]
lappend indicators [list simple "Vector Cosine" cos 1 0 1 {real} {} {cos}]
lappend indicators [list simple "Vector Hyperbolic Cosine" cosh 1 0 1 {real} {} {cosh}]
lappend indicators [list simple "Vector Exponential" exp 1 0 1 {real} {} {exp}]
lappend indicators [list simple "Vector Floor" floor 1 0 1 {real} {} {floor}]
lappend indicators [list simple "Vector Natural Log" ln 1 0 1 {real} {} {ln}]
lappend indicators [list simple "Vector Base-10 Log" log10 1 0 1 {real} {} {log10}]
lappend indicators [list simple "Vector Round" round 1 0 1 {real} {} {round}]
lappend indicators [list simple "Vector Sine" sin 1 0 1 {real} {} {sin}]
lappend indicators [list simple "Vector Hyperbolic Sine" sinh 1 0 1 {real} {} {sinh}]
lappend indicators [list simple "Vector Square Root" sqrt 1 0 1 {real} {} {sqrt}]
lappend indicators [list simple "Vector Tangent" tan 1 0 1 {real} {} {tan}]
lappend indicators [list simple "Vector Hyperbolic Tangent" tanh 1 0 1 {real} {} {tanh}]
lappend indicators [list simple "Vector Degree Conversion" todeg 1 0 1 {real} {} {degrees}]
lappend indicators [list simple "Vector Radian Conversion" torad 1 0 1 {real} {} {radians}]
lappend indicators [list simple "Vector Truncate" trunc 1 0 1 {real} {} {trunc}]




proc ci {a b} {
    return [string compare [lindex $a 2] [lindex $b 2]]
}
set indicators [lsort -command ci $indicators]




#Make main header file
set h [open indicators.h w]
set guard __TI_INDICATORS_H__
puts $h "$license

/*
 *
 * Version $version
 * Header Build $build
 *
 */

 /*
  *
  * This file is generated. Do not modify directly.
  *
  */

#ifndef $guard
#define $guard


#define TI_VERSION \"$version\"
#define TI_BUILD $build


#ifndef TI_SKIP_SYSTEM_HEADERS
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#endif

#ifdef __cplusplus
extern \"C\" {
#endif


const char* ti_version();
long int ti_build();


"

      set fun_args_start {TI_REAL const *options}
      set fun_args "int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs"

      puts $h "

#define TI_REAL double

#define TI_INDICATOR_COUNT [llength $indicators] /* Total number of indicators. */

#define TI_OKAY                    0
#define TI_INVALID_OPTION          1

#define TI_TYPE_OVERLAY            1 /* These have roughly the same range as the input data. */
#define TI_TYPE_INDICATOR          2 /* Everything else (e.g. oscillators). */
#define TI_TYPE_MATH               3 /* These aren't so good for plotting, but are useful with formulas. */
#define TI_TYPE_SIMPLE             4 /* These apply a simple operator (e.g. addition, sin, sqrt). */
#define TI_TYPE_COMPARATIVE        5 /* These are designed to take inputs from different securities. i.e. compare stock A to stock B.*/

#define TI_MAXINDPARAMS 10 /* No indicator will use more than this many inputs, options, or outputs. */


typedef int (*ti_indicator_start_function)($fun_args_start);
typedef int (*ti_indicator_function)($fun_args);

typedef struct ti_indicator_info {
    char *name;
    char *full_name;
    ti_indicator_start_function start;
    ti_indicator_function indicator;
    int type, inputs, options, outputs;
    char *input_names\[TI_MAXINDPARAMS\];
    char *option_names\[TI_MAXINDPARAMS\];
    char *output_names\[TI_MAXINDPARAMS\];
} ti_indicator_info;


/*Complete array of all indicators. Last element is 0,0,0,0...*/
extern ti_indicator_info ti_indicators\[\];


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





"

file mkdir indicators
file mkdir docs

foreach func $indicators {
    lassign $func type fn n in opt out in_names opt_names out_names

    if {$in != [llength $in_names]} {
        puts "WARNING: Bad in names array: $n"
    }
    if {$opt != [llength $opt_names]} {
        puts "WARNING: Bad option array: $n"
    }
    if {$out != [llength $out_names]} {
        puts "WARNING: Bad out names array: $n"
    }

    set start "int ti_[set n]_start($fun_args_start)"
    set fun "int ti_[set n]($fun_args)"

    set imp [file join indicators $n.c]
    if {![file exists $imp]} {
        set o [open $imp w]
        puts $o $license
        puts $o {#include "../indicators.h"}
        puts $o "\n\n"

        puts $o "$start {"
        puts $o ""
        puts $o "}\n\n"

        puts $o "$fun {"
        puts $o ""
        puts $o "}"

        close $o
    }


    set prototype {}

    append prototype

    append prototype "/* $fn */\n"
    append prototype "/* Type: $type */\n"
    append prototype "/* Input arrays: $in    Options: $opt    Output arrays: $out */\n"
    append prototype "/* Inputs: [join $in_names {, }] */\n"
    if {[llength $opt_names]} {
        append prototype "/* Options: [join $opt_names {, }] */\n"
    } else {
        append prototype "/* Options: none */\n"
    }
    append prototype "/* Outputs: [join $out_names {, }] */\n"
    append prototype "$start;\n"
    append prototype "$fun;"

    puts $h "\n\n$prototype\n\n\n"
}


puts $h "#ifdef __cplusplus"
puts $h "}"
puts $h "#endif"
puts $h "\n#endif /*$guard*/"




#Make helper functions and index
set idx [open indicators_index.c w]

puts $idx "$license


/*
 *
 * Version $version
 * Header Build $build
 *
 */

/*
 *
 * This file is generated. Do not modify directly.
 *
 */

"


puts $idx "#include \"indicators.h\"\n\n"
puts $idx "
const char* ti_version() {return TI_VERSION;}
long int ti_build() {return TI_BUILD;}
"

puts $idx "\n\n\nstruct ti_indicator_info ti_indicators\[\] = {"

set func_names {}
foreach func $indicators {
    lassign $func type n fn in opt out in_names opt_names out_names
    lappend func_names $n

    set type "TI_TYPE_[string toupper $type]"

    set in_names "{\"[join $in_names {","}]\",0}"
    set opt_names "{\"[join $opt_names {","}]\",0}"
    set out_names "{\"[join $out_names {","}]\",0}"

    puts $idx "    {\"$fn\", \"$n\", ti_[set fn]_start, ti_[set fn], $type, $in, $opt, $out, $in_names, $opt_names, $out_names},"
}

puts $idx "    {0,0,0,0,0,0,0,0,{0,0},{0,0},{0,0}}"

puts $idx "};"

puts $idx {

const ti_indicator_info *ti_find_indicator(const char *name) {
    int imin = 0;
    int imax = sizeof(ti_indicators) / sizeof(ti_indicator_info) - 2;

    /*Binary search.*/
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

}



close $idx
