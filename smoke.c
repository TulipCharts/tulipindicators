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

#include "indicators.h"
#include "utils/buffer.h"
#include "utils/minmax.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*********** TYPEDEFS ************/

enum {OK, FAILURES_OCCURED, PARSING_ERROR, VERSION_MISMATCH};

/*********** GLOBALS ************/

int tested[TI_INDICATOR_COUNT] = {0};
int failed_cnt = 0;

/************ PARSING PRIMITIVES *************/

char *read_line(FILE *fp) {
    static char buf[65536];
    while (fgets(buf, 65536, fp)) {
        if (buf[0] == '#' || buf[0] == '\n' || buf[0] == '\r') { continue; }
        return buf;
    }
    return 0;
}

int read_array(FILE *fp, TI_REAL *s) {
    char *line = read_line(fp);
    if (!line) {
        printf("seems like an unexpected eof\n");
        exit(PARSING_ERROR);
    }

    if (line[0] != '{') {
        printf("bad input: expected array, got '%s'\n", line);
        exit(PARSING_ERROR);
    }

    TI_REAL *inp = s;
    char *num = strtok(&line[1], ",}\r\n");
    while (num) {
        *inp++ = atof(num);
        num = strtok(0, ",}\r\n");
    }

    return (int)(inp - s);
}

/*********** UTILITIES *************/

int equal_reals(TI_REAL a, TI_REAL b) {
    return fabs(a - b) < 0.001;
}

int equal_arrays(const TI_REAL *a, const TI_REAL *b, int size_a, int size_b) {
    if (size_a != size_b) { return 0; }

    int i;
    for (i = 0; i < size_a; ++i) {
        if (!equal_reals(a[i], b[i])) { return 0; };
    }
    return 1;
}

void print_array(const TI_REAL *a, int size) {
    printf("[%i] = {", size);
    int i;
    for (i = 0; i < size-1; ++i) {
        printf("%.3f,", a[i]);
    }
    if (size) { printf("%.3f", a[size-1]); }
    printf("}");
}

/*********** PARSING, TESTING, REPORTING ************/

int compare_answers(const ti_indicator_info *info, TI_REAL *answers[], TI_REAL *outputs[], int answer_size, int output_size) {
    int i;
    int fails = 0;
    for (i = 0; i < info->outputs; ++i) {
        if (!equal_arrays(answers[i], outputs[i], answer_size, output_size)) {
            ++failed_cnt;
            ++fails;
            printf("output '%s' mismatch\n", info->output_names[i]);
            printf("> expected: "); print_array(answers[i], answer_size); printf("\n");
            printf("> got:      "); print_array(outputs[i], output_size); printf("\n");
        }
    }
    return fails;
}


void run_one(FILE *fp, const char* target_name, int is_regression_test) {
    char *line = read_line(fp);
    if (!line) { return; }
    if (line[0] < 'a' || line[0] > 'z') {
        printf("expected indicator name, got %s\n", line);
        exit(PARSING_ERROR);
    }
    char *name = strtok(line, " \n\r");
    int skip_this = target_name && strcmp(name, target_name) != 0;

    int any_failures_here = 0;

    const ti_indicator_info *info = ti_find_indicator(name);
    if (!info) {
        printf("unknown indicator %s\n", name);
        exit(PARSING_ERROR);
    }

    if (!is_regression_test) { tested[(int)(info - ti_indicators)] = 1; }

    TI_REAL options[TI_MAXINDPARAMS];
    TI_REAL *o = options;
    const char *s;
    while ((s = strtok(0, " \n\r"))) { *o++ = atof(s); }

    if (o-options != info->options) {
        printf("options number mismatch: expected %i, got %i\n", (int)(o-options), info->options);
        failed_cnt += 1;
        any_failures_here = 1;
        goto cleanup;
    }

    int i;

    TI_REAL *inputs[TI_MAXINDPARAMS] = {0};
    TI_REAL *answers[TI_MAXINDPARAMS] = {0};
    TI_REAL *outputs[TI_MAXINDPARAMS] = {0};
    TI_REAL *outputs_ref[TI_MAXINDPARAMS] = {0};
    TI_REAL *outputs_stream_1[TI_MAXINDPARAMS] = {0};
    TI_REAL *outputs_stream_all[TI_MAXINDPARAMS] = {0};

    int input_size = 0;
    for (i = 0; i < info->inputs; ++i) {
        inputs[i] = malloc(sizeof(TI_REAL) * 4096);
        input_size = read_array(fp, inputs[i]);
    }

    const int output_size = MAX(0, input_size - info->start(options));
    int answer_size = 0;
    for (i = 0; i < info->outputs; ++i) {
        answers[i] = malloc(sizeof(TI_REAL) * (size_t)output_size);
        outputs[i] = malloc(sizeof(TI_REAL) * (size_t)output_size);
        outputs_ref[i] = malloc(sizeof(TI_REAL) * (size_t)output_size);
        outputs_stream_1[i] = malloc(sizeof(TI_REAL) * (size_t)output_size);
        outputs_stream_all[i] = malloc(sizeof(TI_REAL) * (size_t)output_size);
        answer_size = read_array(fp, answers[i]);
    }

    if (skip_this) { goto cleanup; }


    {
        printf("running \t%-16s... ", info->name);
        const clock_t ts_start = clock();
        const int ret = info->indicator(input_size, (const double * const*)inputs, options, outputs);
        const clock_t ts_end = clock();

        if (ret != TI_OKAY) {
            printf("return code %i\n", ret);
            failed_cnt += 1;
            any_failures_here = 1;
        } else {
            any_failures_here += compare_answers(info, answers, outputs, answer_size, output_size);
        }

        printf("%4dμs\n", (int)((ts_end - ts_start) / (double)CLOCKS_PER_SEC * 1000000.0));
    }


    if (info->indicator_ref) {
        printf("running \t%s%-*s... ", info->name, 16-strlen(info->name), "_ref");
        const clock_t ts_start = clock();
        const int ret = info->indicator_ref(input_size, (const double * const*)inputs, options, outputs_ref);
        const clock_t ts_end = clock();

        if (ret != TI_OKAY) {
            printf("return code %i\n", ret);
            failed_cnt += 1;
            any_failures_here = 1;
        } else {
            any_failures_here += compare_answers(info, answers, outputs_ref, answer_size, output_size);
        }

        printf("%4dμs\n", (int)((ts_end - ts_start) / (double)CLOCKS_PER_SEC * 1000000.0));
    }


    if (info->stream_new) {
        printf("running \t%s%-*s... ", info->name, 16-strlen(info->name), "_stream_1");
        const clock_t ts_start = clock();

        ti_stream *stream = 0;
        int new_ret = info->stream_new(options, &stream);
        if (new_ret != TI_OKAY || !stream) {
            printf("stream_new failure.\n");
            failed_cnt += 1;
            any_failures_here = 1;

        } else {
            int bar;

            TI_REAL *ins[TI_MAXINDPARAMS] = {0};
            TI_REAL *outs[TI_MAXINDPARAMS] = {0};

            for (bar = 0; bar < input_size; ++bar) {

                for (i = 0; i < info->inputs; ++i) {
                    ins[i] = inputs[i] + bar;
                }

                for (i = 0; i < info->outputs; ++i) {
                    outs[i] = outputs_stream_1[i] + ti_stream_get_progress(stream);
                }

                const int ret = info->stream_run(stream, 1, (const double * const*)ins, outs);

                //TODO should we check ret? Is it possible for a stream indicator to fail?
                assert(ret == TI_OKAY);
            }

            info->stream_free(stream);
        }
        const clock_t ts_end = clock();

        any_failures_here += compare_answers(info, answers, outputs_stream_1, answer_size, output_size);
        printf("%4dμs\n", (int)((ts_end - ts_start) / (double)CLOCKS_PER_SEC * 1000000.0));
    }


    if (info->stream_new) {
        printf("running \t%s%-*s... ", info->name, 16-strlen(info->name), "_stream_all");
        const clock_t ts_start = clock();

        ti_stream *stream = 0;
        int new_ret = info->stream_new(options, &stream);
        if (new_ret != TI_OKAY || !stream) {
            printf("stream_new failure.\n");
            failed_cnt += 1;
            any_failures_here = 1;

        } else {
            int bar;

            const int ret = info->stream_run(stream, input_size, (const double * const*)inputs, outputs_stream_all);

            //TODO should we check ret? Is it possible for a stream indicator to fail?
            assert(ret == TI_OKAY);
        }

        info->stream_free(stream);
        const clock_t ts_end = clock();

        any_failures_here += compare_answers(info, answers, outputs_stream_all, answer_size, output_size);
        printf("%4dμs\n", (int)((ts_end - ts_start) / (double)CLOCKS_PER_SEC * 1000000.0));
    }


cleanup:
    for (i = 0; i < info->inputs; ++i) {
        free(inputs[i]);
    }
    for (i = 0; i < info->outputs; ++i) {
        free(answers[i]);
        free(outputs[i]);
        free(outputs_ref[i]);
        free(outputs_stream_1[i]);
        free(outputs_stream_all[i]);
    }
}

void run_tests(const char *fname, const char* target_name, int is_regression_test) {
    printf("# test suite %s:\n", fname);
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        printf("failed to open\n");
        exit(PARSING_ERROR);
    }

    while (!feof(fp)) {
        run_one(fp, target_name, is_regression_test);
    }

    printf("\n");
    fclose(fp);
}

/*************** LIBRARY TESTS *****************/

void test_buffer() {
    printf("running \t%-16s... ", "buffer");
    #define buffer_size 3
    ti_buffer *b = ti_buffer_new(buffer_size);
    const TI_REAL input[] = {5,5,1,1,3,1,2,3};
    const TI_REAL sum[] = {5,10,11,7,5,5,6,6};
    const TI_REAL output[] = {1,2,3};

    #define input_size sizeof(input)/sizeof(input[0])
    TI_REAL answers_sum[input_size] = {0};

    clock_t ts_start = clock();
    int i;
    for (i = 0; i < (int)(input_size); ++i) {
        ti_buffer_push(b, input[i]);
        answers_sum[i] = b->sum;
    }

    TI_REAL answers_output[buffer_size] = {0};
    for (i = 0; i < buffer_size; ++i) {
        answers_output[i] = ti_buffer_get(b, i-buffer_size+1);
    }
    TI_REAL ts_end = clock();

    int any_failures_here = 0;
    if (!equal_arrays(output, answers_output, buffer_size, buffer_size)) {
        printf("output mismatch: \n");
        printf("> expected: "); print_array(output, buffer_size); printf("\n");
        printf("> got:      "); print_array(answers_output, buffer_size); printf("\n");
        any_failures_here = 1;
    }
    if (!equal_arrays(sum, answers_sum, input_size, input_size)) {
        printf("sum mismatch: \n");
        printf("> expected: "); print_array(sum, input_size); printf("\n");
        printf("> got:      "); print_array(answers_sum, input_size); printf("\n");
        any_failures_here = 1;
    }

    ti_buffer_free(b);
    if (any_failures_here) { exit(FAILURES_OCCURED); }
    printf("%4dμs\n", (int)((ts_end - ts_start) / (double)CLOCKS_PER_SEC * 1000000.0));
}

/************** ENTRY POINT ***************/

int main(int argc, const char** argv) {
    if (strcmp(TI_VERSION, ti_version()) != 0) {
        printf("library version mismatch: header %s, binary %s\n", TI_VERSION, ti_version());
        exit(VERSION_MISMATCH);
    }
    if (TI_BUILD != ti_build()) {
        printf("build version mismatch: header %i, binary %li\n", TI_BUILD, ti_build());
        exit(VERSION_MISMATCH);
    }

    const char* target_name = argc > 1 ? argv[1] : 0;

    if (!target_name) {
        printf("# utils:\n");
        test_buffer();
        printf("\n");
    }


    run_tests("tests/untest.txt", target_name, 1);
    run_tests("tests/atoz.txt", target_name, 0);
    run_tests("tests/extra.txt", target_name, 0);

    if (!target_name) {
        int i;
        for (i = 0; i < TI_INDICATOR_COUNT; ++i) {
            if (!tested[i]) { printf("WARNING: no test for %s\n", ti_indicators[i].name); }
        }
    }

    if (failed_cnt == 0) {
        printf("ALL TESTS PASSED\n");
    } else {
        printf(" %d TEST%s FAILED\n", failed_cnt, failed_cnt > 1 ? "s" : "");
    }

    return failed_cnt ? FAILURES_OCCURED : OK;
}
