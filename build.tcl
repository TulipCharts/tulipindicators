# Tulip Indicators
# https://tulipindicators.org/
# Copyright (c) 2010-2022 Tulip Charts LLC
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





#This script takes the files in the /templates directory
#and fills them in with the indicator and candle stick pattern
#recognition info.




set version 0.9.1
set build [clock seconds]






puts "Generate interface for indicator library."


set indicators {}
#type name func_name inputs options outputs extra_features

#TODO it would be nice to normalize some of the out and option names, but that would be a breaking change

#Moving averages
lappend indicators [list overlay "Arnaud Legoux Moving Average" alma 1 3 1 {real} {period offset sigma} {alma} {beta}]
lappend indicators [list overlay "Double Exponential Moving Average" dema 1 1 1 {real} {period} {dema}]
lappend indicators [list overlay "Exponential Moving Average" ema 1 1 1 {real} {period} {ema}]
lappend indicators [list overlay "Hull Moving Average" hma 1 1 1 {real} {period} {hma}]
lappend indicators [list overlay "MESA Adaptive Moving Average" mama 1 2 2 {real} {{fast limit} {slow limit}} {mama fama} {ref stream beta}]
lappend indicators [list overlay "Simple Moving Average" sma 1 1 1 {real} {period} {sma} {stream}]
lappend indicators [list overlay "Triple Exponential Moving Average" tema 1 1 1 {real} {period} {tema}]
lappend indicators [list overlay "Triangular Moving Average" trima 1 1 1 {real} {period} {trima}]
lappend indicators [list overlay "Variable Index Dynamic Average" vidya 1 3 1 {real} {{short_period} {long_period} alpha} {vidya}]
lappend indicators [list overlay "Volume Weighted Moving Average" vwma 2 1 1 {close volume} {period} {vwma}]
lappend indicators [list overlay "Volume Weighted Average Price" vwap 4 1 1 {high low close volume} {period} {vwap} {stream ref beta}]
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
lappend indicators [list overlay "Acceleration Bands" abands 3 1 3 {high low close} {period} {abands_lower abands_upper abands_middle} {ref beta}]
lappend indicators [list overlay "Bollinger Bands" bbands 1 2 3 {real} {period stddev} {bbands_lower bbands_middle bbands_upper}]
lappend indicators [list overlay "Donchian Channel" dc 1 1 2 {real} {period} {dc_lower dc_upper} {stream beta}]
lappend indicators [list overlay "Kaufman Adaptive Moving Average" kama 1 1 1 {real} {period} {kama}]
lappend indicators [list overlay "Keltner Channel" kc 3 2 3 {high low close} {period multiple} {kc_lower kc_middle kc_upper} {stream beta}]
lappend indicators [list overlay "Projection Bands" pbands 3 1 2 {high low close} {period} {pbands_lower pbands_upper} {ref stream beta}]
lappend indicators [list overlay "Price Channel" pc 2 1 2 {high low} {period} {pc_low pc_high} {stream beta}]
lappend indicators [list overlay "Parabolic SAR" psar 2 2 1 {high low} {{acceleration_factor_step} {acceleration_factor_maximum}} {psar}]

#Momentum
lappend indicators [list indicator "Accumulation/Distribution Line" ad 4 0 1 {high low close volume} {} {ad}]
lappend indicators [list indicator "Accumulation/Distribution Oscillator" adosc 4 2 1 {high low close volume} {{short_period} {long_period}} {adosc}]
lappend indicators [list indicator "Absolute Price Oscillator" apo 1 2 1 {real} {{short_period} {long_period}} {apo}]
lappend indicators [list indicator "Aroon" aroon 2 1 2 {high low} {period} {aroon_down aroon_up}]
lappend indicators [list indicator "Aroon Oscillator" aroonosc 2 1 1 {high low} {period} {aroonosc}]
lappend indicators [list indicator "Awesome Oscillator" ao 2 0 1 {high low} {} {ao}]
lappend indicators [list indicator "Balance of Power" bop 4 0 1 {open high low close} {} {bop}]
lappend indicators [list indicator "Chaikin Money Flow" cmf 4 1 1 {high low close volume} {period} {cmf} {beta}]
lappend indicators [list indicator "Chande Momentum Oscillator" cmo 1 1 1 {real} {period} {cmo}]
lappend indicators [list indicator "Commodity Channel Index" cci 3 1 1 {high low close} {period} {cci}]
lappend indicators [list indicator "Coppock Curve" copp 1 3 1 {real} {{short_period} {long_period} {smooth_period}} {copp} {ref beta}] ;# TODO fix and enable stream ;#TODO review
lappend indicators [list indicator "Detrended Price Oscillator" dpo 1 1 1 {real} {period} {dpo}]
lappend indicators [list indicator "Ease of Movement" emv 3 0 1 {high low volume} {} {emv}]
lappend indicators [list indicator "Fisher Transform" fisher 2 1 2 {high low} {period} {fisher fisher_signal}]
lappend indicators [list indicator "Force Index" fi 2 1 1 {close volume} {period} {fi} {ref stream beta}]
lappend indicators [list indicator "Klinger Volume Oscillator" kvo 4 2 1 {high low close volume} {{short_period} {long_period}} {kvo}]
lappend indicators [list indicator "Know Sure Thing" kst 1 8 2 {real} {roc1 roc2 roc3 roc4 ma1 ma2 ma3 ma4} {kst kst_signal} {ref beta}]
lappend indicators [list indicator "Market Facilitation Index" marketfi 3 0 1 {high low volume} {} {marketfi}]
lappend indicators [list indicator "Mass Index" mass 2 1 1 {high low} {period} {mass}]
lappend indicators [list indicator "Money Flow Index" mfi 4 1 1 {high low close volume} {period} {mfi}]
lappend indicators [list indicator "Moving Average Convergence/Divergence" macd 1 3 3 {real} {{short_period} {long_period} {signal_period}} {macd macd_signal macd_histogram}]
lappend indicators [list indicator "Negative Volume Index" nvi 2 0 1 {close volume} {} {nvi}]
lappend indicators [list indicator "On Balance Volume" obv 2 0 1 {close volume} {} {obv}]
lappend indicators [list indicator "Percentage Price Oscillator" ppo 1 2 1 {real} {{short_period} {long_period}} {ppo}]
lappend indicators [list indicator "Polarized Fractal Efficiency" pfe 1 2 1 {real} {period {smooth_period}} {pfe} {ref stream beta}]
lappend indicators [list indicator "Positive Volume Index" pvi 2 0 1 {close volume} {} {pvi}]
lappend indicators [list indicator "Projection Oscillator" posc 3 2 1 {high low close} {period {smooth_period}} {posc} {stream ref beta}]
lappend indicators [list indicator "Qstick" qstick 2 1 1 {open close} {period} {qstick}]
lappend indicators [list indicator "Recursive Moving Trend Average" rmta 1 2 1 {real} {period beta} {rmta} {beta}]
lappend indicators [list indicator "Relative Strength Index" rsi 1 1 1 {real} {period} {rsi}]
lappend indicators [list indicator "Relative Volatility Index" rvi 1 2 1 {real} {period {smooth_period}} {rvi} {stream beta}] ;#TODO suspect
lappend indicators [list indicator "Stochastic Momentum Index" smi 3 3 1 {high low close} {{q_period} {r_period} {s_period}} {smi} {stream ref beta}]
lappend indicators [list indicator "True Strength Index" tsi 1 2 1 {real} {period {smooth_period}} {tsi} {stream ref beta}]
lappend indicators [list indicator "Relative Momentum Index" rmi 1 2 1 {real} {period {lookback_period}} {rmi} {ref stream beta}] ;#TODO suspect.
lappend indicators [list indicator "Stochastic Oscillator" stoch 3 3 2 {high low close} {{k_period} {k_slowing_period} {d_period}} {stoch_k stoch_d}]
lappend indicators [list indicator "Stochastic RSI" stochrsi 1 1 1 {real} {period} {stochrsi}]
lappend indicators [list indicator "Trix" trix 1 1 1 {real} {period} {trix}]
lappend indicators [list indicator "Williams Accumulation/Distribution" wad 3 0 1 {high low close} {} {wad}]
lappend indicators [list indicator "Williams %R" willr 3 1 1 {high low close} {period} {willr}]
lappend indicators [list indicator "Ultimate Oscillator" ultosc 3 3 1 {high low close} {{short_period} {medium_period} {long_period}} {ultosc}]
lappend indicators [list indicator "Vertical Horizontal Filter" vhf 1 1 1 {real} {period} {vhf}]
lappend indicators [list indicator "Volume Oscillator" vosc 1 2 1 {volume} {{short_period} {long_period}} {vosc}]

#DX ADX ADXR etc
lappend indicators [list indicator "Directional Movement" dm 2 1 2 {high low} {period} {plus_dm minus_dm}]
lappend indicators [list indicator "Directional Movement Index" dx 2 1 1 {high low} {period} {dx}]
lappend indicators [list indicator "Directional Indicator" di 3 1 2 {high low close} {period} {plus_di minus_di}]
lappend indicators [list indicator "Average Directional Movement Index" adx 2 1 1 {high low} {period} {adx}]
lappend indicators [list indicator "Average Directional Movement Rating" adxr 2 1 1 {high low} {period} {adxr}]

#Waves
lappend indicators [list indicator "Mesa Sine Wave" msw 1 1 2 {real} {period} {msw_sine msw_lead}]

#Rate of return, rate of change, momentum indicators
lappend indicators [list indicator "Momentum" mom 1 1 1 {real} {period} {mom}]
lappend indicators [list indicator "Rate of Change" roc 1 1 1 {real} {period} {roc}]
lappend indicators [list indicator "Rate of Change Ratio" rocr 1 1 1 {real} {period} {rocr}]

#Math functions
lappend indicators [list math "Lag" lag 1 1 1 {real} {period} {lag}]
lappend indicators [list math "Maximum In Period" max 1 1 1 {real} {period} {max} {ref}]
lappend indicators [list math "Minimum In Period" min 1 1 1 {real} {period} {min} {ref}]
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
lappend indicators [list indicator "Average True Range" atr 3 1 1 {high low close} {period} {atr} {stream ref}]
lappend indicators [list indicator "Chandelier Exit" ce 3 2 2 {high low close} {period multiple} {ce_high ce_low} {ref beta}] ;#TODO fix and enable stream ;#TODO review
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

# Ichimoku
lappend indicators [list overlay "Ichimoku Tenkan-Sen" ikhts 2 1 1 {high low} {period} {ikhts} {beta}]


#Remove beta indicators
set ind_all $indicators
set indicators {}
foreach ind $ind_all {
    set flags [lindex $ind 9]
    if {[lsearch $flags beta] != -1} {
        puts "Skipping beta indicator [lindex $ind 1]"
    } else {
        lappend indicators $ind
    }
}


#Sort indicators by name
proc ci {a b} {
    return [string compare [lindex $a 2] [lindex $b 2]]
}
set indicators [lsort -command ci $indicators]





set ti_version "
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

set ti_version_define "

#define TI_VERSION \"$version\"
#define TI_BUILD $build

"




#Make indicator functions definitions

set fun_args_start {TI_REAL const *options}
set fun_args "int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs"
set fun_stream_new_args {TI_REAL const *options, ti_stream **stream}
set fun_stream_run_args "ti_stream *stream, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs"
set fun_stream_free_args "ti_stream *stream"

set index 0

set indicators_h {}
set indicators_c {}

foreach func $indicators {
    lassign $func type fn n in opt out in_names opt_names out_names


    if {$in != [llength $in_names]} {
        puts "WARNING: Bad in names array: $n"
        exit
    }
    if {$opt != [llength $opt_names]} {
        puts "WARNING: Bad option array: $n"
        exit
    }
    if {$out != [llength $out_names]} {
        puts "WARNING: Bad out names array: $n"
        exit
    }

    foreach o [list {*}$in_names {*}$opt_names {*}$out_names] {
        if {[string first " " $o] != -1} {
            puts "WARNING: Space in in/opt/out: $n $o"
        }
        if {[string first "%" $o] != -1} {
            puts "WARNING: % in in/opt/out: $n $o"
        }
    }

    set start "int ti_[set n]_start($fun_args_start)"
    set fun "int ti_[set n]($fun_args)"


    append indicators_h "/* $fn */\n"
    append indicators_h "/* Type: $type */\n"
    append indicators_h "/* Input arrays: $in    Options: $opt    Output arrays: $out */\n"
    append indicators_h "/* Inputs: [join $in_names {, }] */\n"
    if {[llength $opt_names]} {
        append indicators_h "/* Options: [join $opt_names {, }] */\n"
    } else {
        append indicators_h "/* Options: none */\n"
    }
    append indicators_h "/* Outputs: [join $out_names {, }] */\n"
    append indicators_h "#define TI_INDICATOR_[string toupper $n]_INDEX $index\n"
    append indicators_h "$start;\n"
    append indicators_h "$fun;\n"


    if {[llength $func] > 9} {
        set extra [lindex $func 9]
        if {[lsearch $extra ref] != -1} {
            append indicators_h "int ti_[set n]_ref($fun_args);\n"
        }
        if {[lsearch $extra stream] != -1} {
            append indicators_h "int ti_[set n]_stream_new($fun_stream_new_args);\n"
            append indicators_h "int ti_[set n]_stream_run($fun_stream_run_args);\n"
            append indicators_h "void ti_[set n]_stream_free($fun_stream_free_args);\n"
        }
    }

    incr index
}



append indicators_c "struct ti_indicator_info ti_indicators\[\] = {"

set func_names {}
foreach func $indicators {
    lassign $func type n fn in opt out in_names opt_names out_names
    lappend func_names $n

    set ref 0
    set stream_new 0
    set stream_run 0
    set stream_free 0

    if {[llength $func] > 9} {
        set extra [lindex $func 9]
        foreach option $extra {
            switch $option {
                ref {set ref ti_[set fn]_ref}
                stream {
                    set stream_new ti_[set fn]_stream_new
                    set stream_run ti_[set fn]_stream_run
                    set stream_free ti_[set fn]_stream_free
                }
            }

        }
    }

    set type "TI_TYPE_[string toupper $type]"

    set in_names "{\"[join $in_names {","}]\",0}"
    set opt_names "{\"[join $opt_names {","}]\",0}"
    set out_names "{\"[join $out_names {","}]\",0}"

    append indicators_c "    {\"$fn\", \"$n\", ti_[set fn]_start, ti_[set fn], $ref, $type, $in, $opt, $out, $in_names, $opt_names, $out_names, $stream_new, $stream_run, $stream_free},"
}

append indicators_c "    {0,0,0,0,0,0,0,0,0,{0,0},{0,0},{0,0},0,0,0}"

append indicators_c "};"

















set candles {}

lappend candles [list {Big Black Candle} {BLACK(i) && BODY_LONG(i)}]
lappend candles [list {Big White Candle} {WHITE(i) && BODY_LONG(i)}]

lappend candles [list {Doji} {BODY_NONE(i)}]
lappend candles [list {Four Price Doji} {BODY_NONE(i) && WICK_UPPER_NONE(i) && WICK_LOWER_NONE(i)}]
lappend candles [list {Dragonfly Doji} {BODY_NONE(i) && WICK_UPPER_NONE(i) && WICK_LOWER_LONG(i)}]
lappend candles [list {Gravestone Doji} {BODY_NONE(i) && WICK_UPPER_LONG(i) && WICK_LOWER_NONE(i)}]
lappend candles [list {Long Legged Doji} {BODY_NONE(i) && WICK_UPPER_LONG(i) && WICK_LOWER_LONG(i)}]

lappend candles [list {Morning Star} {i>=2 && BLACK(i-2) && BODY_LONG(i-2) && GAP_DOWN(i-1) && BODY_SHORT(i-1) && WHITE(i) && GAP_UP(i) && (close[i] >= close[i-2])}]
lappend candles [list {Evening Star} {i>=2 && WHITE(i-2) && BODY_LONG(i-2) && GAP_UP(i-1) && BODY_SHORT(i-1) && BLACK(i) && GAP_DOWN(i) && (close[i] <= close[i-2])}]
lappend candles [list {Morning Doji Star} {i>=2 && BLACK(i-2) && BODY_LONG(i-2) && GAP_DOWN(i-1) && BODY_NONE(i-1) && WHITE(i) && GAP_UP(i) && (close[i] >= close[i-2])}]
lappend candles [list {Evening Doji Star} {i>=2 && WHITE(i-2) && BODY_LONG(i-2) && GAP_UP(i-1) && BODY_NONE(i-1) && BLACK(i) && GAP_DOWN(i) && (close[i] <= close[i-2])}]
lappend candles [list {Abandoned Baby Bull} {i>=2 && BLACK(i-2) && BODY_LONG(i-2) && STAR_DOWN(i-1) && BODY_NONE(i-1) && WHITE(i) && STAR_UP(i) && (close[i] >= close[i-2])}]
lappend candles [list {Abandoned Baby Bear} {i>=2 && WHITE(i-2) && BODY_LONG(i-2) && STAR_UP(i-1) && BODY_NONE(i-1) && BLACK(i) && STAR_DOWN(i) && (close[i] <= close[i-2])}]

lappend candles [list {Star} {i>=1 && STAR(i)}]

lappend candles [list {Hammer} {i>=1 && BODY_SHORT(i) && WICK_UPPER_NONE(i) && WICK_LOWER_LONG(i) && BOTTOM(i) <= low[i-1] + OPT_NEAR()}]
lappend candles [list {Hanging Man} {i>=1 && BODY_SHORT(i) && WICK_UPPER_NONE(i) && WICK_LOWER_LONG(i) && BOTTOM(i) >= high[i-1] - OPT_NEAR()}]
lappend candles [list {Inverted Hammer} {i>=1 && BODY_SHORT(i) && WICK_UPPER_LONG(i) && WICK_LOWER_NONE(i) && GAP_DOWN(i)}]

lappend candles [list {Marubozu} {BODY_LONG(i) && WICK_UPPER_NONE(i) && WICK_LOWER_NONE(i)}]
lappend candles [list {White Marubozu} {BODY_LONG(i) && WICK_UPPER_NONE(i) && WICK_LOWER_NONE(i) && WHITE(i)}]
lappend candles [list {Black Marubozu} {BODY_LONG(i) && WICK_UPPER_NONE(i) && WICK_LOWER_NONE(i) && BLACK(i)}]

lappend candles [list {Spinning Top} {BODY_SHORT(i) && WICK_UPPER_LONGER_THAN_BODY(i) && WICK_LOWER_LONGER_THAN_BODY(i)}]

lappend candles [list {Engulfing Bull} {i>=1 && WHITE(i) && BLACK(i-1) && close[i] > open[i-1] && open[i] < close[i-1]}]
lappend candles [list {Engulfing Bear} {i>=1 && BLACK(i) && WHITE(i-1) && close[i] < open[i-1] && open[i] > close[i-1]}]

lappend candles [list {Three White Soldiers} {i>=2 && WHITE(i-2) && WHITE(i-1) && WHITE(i) && OPEN_INSIDE(i-1) && OPEN_INSIDE(i)}]
lappend candles [list {Three Black Crows} {i>=2 && BLACK(i-2) && BLACK(i-1) && BLACK(i) && OPEN_INSIDE(i-1) && OPEN_INSIDE(i)}]



proc ci {a b} {
    return [string compare [lindex $a 0] [lindex $b 0]]
}
set candles [lsort -command ci $candles]




set tc_version "
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

set tc_version_define "

#define TC_VERSION \"$version\"
#define TC_BUILD $build

"



set tc_defines "#define TC_CANDLE_COUNT [llength $candles] /* Total number of recognized candle patterns. */\n\n"
append tc_defines "#define TC_NONE (0)\n"
append tc_defines "#define TC_ALL ((1L<<[llength $candles])-1)\n\n"
set tc_patterns {}
set tc_function_defs {}
set tc_functions {}
set tc_short {}
set tc_index {}
set i 0

foreach candle $candles {
    lassign $candle name code

    set cname "TC_[string toupper [string map {{ } {_}} $name]]"
    set lname "[string tolower [string map {{ } {_}} $name]]"
    append tc_defines "#define $cname (1L<<$i)\n"

    append tc_patterns "
    /* $name */
    if (($cname & patterns) && $code) {
        HIT($cname);
    }
"

    append tc_function_defs "int tc_[set lname](int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output);\n"

    append tc_functions "int tc_[set lname](int size, TC_REAL const *const *inputs, tc_config const *options, tc_result *output) {
        SETUP
        LOOP(
            if ($code) {
                HIT($cname);
            }
        );
        return TC_OKAY;
    }\n\n"

    append tc_short "if (patterns == $cname) return tc_[set lname](size, inputs, options, output);\n"

    lappend tc_index "{\"$lname\", \"$name\", $cname, tc_[set lname]}"

    incr i
}

lappend tc_index "{0,0,0,0}"

set tc_index [join $tc_index ",\n"]

















proc fill {in_name out_name replace} {
    set in [open $in_name r]
    set template [read $in]
    close $in


    set template [string map $replace $template]

    set out [open $out_name w]
    fconfigure $out -translation {auto lf}
    puts $out $template
    close $out

    puts "Wrote $out_name"
}



fill templates/indicators.h indicators.h [list /*VERSION*/ $ti_version /*VERSION_DEFINE*/ $ti_version_define /*INDICATORS*/ $indicators_h]
fill templates/indicators.c indicators.c [list /*VERSION*/ $ti_version /*INDICATORS*/ $indicators_c]


fill templates/candles.h candles.h [list /*VERSION*/ $tc_version /*VERSION_DEFINE*/ $tc_version_define /*DEFINES*/ $tc_defines /*FUNCTIONS*/ $tc_function_defs]
fill templates/candles.c candles.c [list /*VERSION*/ $tc_version /*SHORT*/ $tc_short /*PATTERNS*/ $tc_patterns /*FUNCTIONS*/ $tc_functions /*INDEX*/ $tc_index]

