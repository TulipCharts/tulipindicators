cmake_minimum_required(VERSION 3.13.4)
project(tulipindicators VERSION 0.9 LANGUAGES C)

## Library
file(GLOB src utils/*.c indicators/*.c)
add_library(indicators SHARED ${src} indicators.h indicators_index.c)
target_link_libraries(indicators PUBLIC
	"$<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:$<$<CONFIG:DEBUG>:asan;ubsan>;m>")
target_link_options(indicators PRIVATE $<$<C_COMPILER_ID:MSVC>:/PROFILE:linker /STACK:10000000>)

set_target_properties(indicators PROPERTIES PUBLIC_HEADER "indicators.h")
install(TARGETS indicators
    PUBLIC_HEADER DESTINATION "include"
    ARCHIVE DESTINATION "lib")

target_compile_options(indicators PUBLIC
   $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
        -Wall -Wextra -Wshadow -Wconversion -std=c99 -pedantic -g3 -ggdb -O2
        	$<$<CONFIG:DEBUG>:
        		-fsanitize=undefined -fsanitize=address -fsanitize=leak>
        	$<$<CONFIG:RELEASE>:
        		-ffast-math -march=native>>
   $<$<C_COMPILER_ID:MSVC>:
        /W4 /O2>
)

# file(COPY indicators.tcl DESTINATION ${CMAKE_SOURCE_DIR})
execute_process(
    COMMAND tclsh indicators.tcl
    OUTPUT indicators.h indicators_index.c
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Samples
# failed to build on MSVC:
# add_executable(example1 example1.c)
# target_link_libraries(example1 indicators)

add_executable(example2 example2.c)
target_link_libraries(example2 indicators)

add_executable(sample sample.c)
target_link_libraries(sample indicators)

# not ready yet:
# file(GLOB src-h utils/*.h indicators/*.h)
# add_custom_command(
#     OUTPUT tiamalgamation.c
#     COMMAND ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS} -E -P -DTI_SKIP_SYSTEM_HEADERS -D__TI_INDICATORS_H__ -D__BUFFER_H__ -D__MINMAX_H__ ${src} | cat - ${src-h} > tiamalgamation.c
# )

# CC -E -P $CFLAGS -DTI_SKIP_SYSTEM_HEADERS -D__TI_INDICATORS_H__ -D__BUFFER_H__ -D__MINMAX_H__ |
# cat <(CC -E -P $CFLAGS -DTI_SKIP_SYSTEM_HEADERS -D__TI_INDICATORS_H__ -D__BUFFER_H__ -D__MINMAX_H__) <(**/*.h) > tiamalgamation.c

## Tests
enable_testing()

add_executable(smoke smoke.c)
target_link_libraries(smoke indicators)
add_test(NAME smoke COMMAND smoke WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(fuzzer fuzzer.c)
target_link_libraries(fuzzer indicators)
add_test(NAME fuzzer COMMAND fuzzer)

add_executable(benchmark benchmark.c)
set_target_properties(benchmark PROPERTIES EXCLUDE_FROM_ALL 1)
target_link_libraries(benchmark indicators ta_lib)
#add_test(NAME benchmark COMMAND benchmark)

add_executable(benchmark2 benchmark2.c)
target_link_libraries(benchmark2 indicators)
add_test(NAME benchmark2 COMMAND benchmark2)

add_custom_target(check COMMAND CTEST_OUTPUT_ON_FAILURE=1 LD_PRELOAD=libasan.so ${CMAKE_CTEST_COMMAND})