cmake_minimum_required(VERSION 3.5)
project(tulipindicators VERSION 0.8.4 LANGUAGES C)

## Library
add_library(indicators 
    utils/buffer.c
    indicators_index.c
    indicators/abs.c
    indicators/acos.c
    indicators/ad.c
    indicators/add.c
    indicators/adosc.c
    indicators/adx.c
    indicators/adxr.c
    indicators/ao.c
    indicators/apo.c
    indicators/aroon.c
    indicators/aroonosc.c
    indicators/asin.c
    indicators/atan.c
    indicators/atr.c
    indicators/avgprice.c
    indicators/bbands.c
    indicators/bop.c
    indicators/cci.c
    indicators/ceil.c
    indicators/cmo.c
    indicators/cos.c
    indicators/cosh.c
    indicators/crossany.c
    indicators/crossover.c
    indicators/cvi.c
    indicators/decay.c
    indicators/dema.c
    indicators/di.c
    indicators/div.c
    indicators/dm.c
    indicators/dpo.c
    indicators/dx.c
    indicators/edecay.c
    indicators/ema.c
    indicators/emv.c
    indicators/exp.c
    indicators/fisher.c
    indicators/floor.c
    indicators/fosc.c
    indicators/hma.c
    indicators/kama.c
    indicators/kvo.c
    indicators/lag.c
    indicators/linreg.c
    indicators/linregintercept.c
    indicators/linregslope.c
    indicators/ln.c
    indicators/log10.c
    indicators/macd.c
    indicators/marketfi.c
    indicators/mass.c
    indicators/max.c
    indicators/md.c
    indicators/medprice.c
    indicators/mfi.c
    indicators/min.c
    indicators/mom.c
    indicators/msw.c
    indicators/mul.c
    indicators/natr.c
    indicators/nvi.c
    indicators/obv.c
    indicators/ppo.c
    indicators/psar.c
    indicators/pvi.c
    indicators/qstick.c
    indicators/roc.c
    indicators/rocr.c
    indicators/round.c
    indicators/rsi.c
    indicators/sin.c
    indicators/sinh.c
    indicators/sma.c
    indicators/sqrt.c
    indicators/stddev.c
    indicators/stderr.c
    indicators/stoch.c
    indicators/stochrsi.c
    indicators/sub.c
    indicators/sum.c
    indicators/tan.c
    indicators/tanh.c
    indicators/tema.c
    indicators/todeg.c
    indicators/torad.c
    indicators/tr.c
    indicators/trima.c
    indicators/trix.c
    indicators/trunc.c
    indicators/tsf.c
    indicators/typprice.c
    indicators/ultosc.c
    indicators/var.c
    indicators/vhf.c
    indicators/vidya.c
    indicators/volatility.c
    indicators/vosc.c
    indicators/vwma.c
    indicators/wad.c
    indicators/wcprice.c
    indicators/wilders.c
    indicators/willr.c
    indicators/wma.c
    indicators/zlema.c)

set_target_properties(indicators PROPERTIES PUBLIC_HEADER "indicators.h")
install(TARGETS indicators
        PUBLIC_HEADER DESTINATION "include"
        ARCHIVE DESTINATION "lib"  
)

# Samples
add_executable(example1 example1.c)
target_link_libraries(example1 indicators)

add_executable(example2 example2.c)
target_link_libraries(example2 indicators)

add_executable(sample sample.c)
target_link_libraries(sample indicators)

## Tests
enable_testing()

# Copy tests files
file(COPY tests DESTINATION ${CMAKE_BINARY_DIR})

add_executable(smoke smoke.c)
target_link_libraries(smoke indicators)
add_test(NAME smoke COMMAND smoke)

add_executable(fuzzer fuzzer.c)
target_link_libraries(fuzzer indicators)
add_test(NAME fuzzer COMMAND fuzzer)
