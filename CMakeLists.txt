cmake_minimum_required(VERSION 3.14.1)
project(tulipindicators VERSION 0.9 LANGUAGES C)

## Library
file(GLOB src utils/*.c indicators/*.c)
add_library(indicators ${src} indicators_index.c)
target_link_libraries(indicators m)

set_target_properties(indicators PROPERTIES PUBLIC_HEADER "indicators.h")
install(TARGETS indicators
    PUBLIC_HEADER DESTINATION "include"
    ARCHIVE DESTINATION "lib")

target_compile_options(indicators PUBLIC
   $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
        -Wall -Wextra -Wshadow -Wconversion -std=c99 -pedantic -Wno-declaration-after-statement -O2 -g>
   $<$<C_COMPILER_ID:MSVC>:
        /W4>
)

# file(COPY indicators.tcl DESTINATION ${CMAKE_SOURCE_DIR})
execute_process(
    COMMAND tclsh indicators.tcl
    OUTPUT indicators.h indicators_index.c
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

 # Samples
add_executable(example1 example1.c)
target_link_libraries(example1 indicators)

add_executable(example2 example2.c)
target_link_libraries(example2 indicators)

add_executable(sample sample.c)
target_link_libraries(sample indicators)

# file(GLOB src-h utils/*.h indicators/*.h)
# add_custom_command(
#     OUTPUT tiamalgamation.c
#     COMMAND ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS} -E -P -DTI_SKIP_SYSTEM_HEADERS -D__TI_INDICATORS_H__ -D__BUFFER_H__ -D__MINMAX_H__ ${src} | cat - ${src-h} > tiamalgamation.c
# )

# CC -E -P $CFLAGS -DTI_SKIP_SYSTEM_HEADERS -D__TI_INDICATORS_H__ -D__BUFFER_H__ -D__MINMAX_H__ |
# cat <(CC -E -P $CFLAGS -DTI_SKIP_SYSTEM_HEADERS -D__TI_INDICATORS_H__ -D__BUFFER_H__ -D__MINMAX_H__) <(**/*.h) > tiamalgamation.c

## Tests
enable_testing()

# Copy tests files
file(CREATE_LINK ${CMAKE_SOURCE_DIR}/tests ${CMAKE_BINARY_DIR}/tests SYMBOLIC)

add_executable(smoke smoke.c)
target_link_libraries(smoke indicators)
add_test(NAME smoke COMMAND smoke)

add_executable(fuzzer fuzzer.c)
target_link_libraries(fuzzer indicators)
add_test(NAME fuzzer COMMAND fuzzer)

add_executable(benchmark benchmark.c)
target_link_libraries(benchmark indicators ta_lib)
add_test(NAME benchmark COMMAND benchmark)

add_executable(benchmark2 benchmark2.c)
target_link_libraries(benchmark2 indicators)
add_test(NAME benchmark2 COMMAND benchmark2)

add_custom_target(check COMMAND CTEST_OUTPUT_ON_FAILURE=1 ${CMAKE_CTEST_COMMAND})
