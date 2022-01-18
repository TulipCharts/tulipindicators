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


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "indicators.h"
#include "candles.h"


double optionsd_small[] = {-20,-1,0,.7,1,2,5,20,100};
double optionsd_large[] = {-5,0,5};

double dummy_in[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
double dummy_in0[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
double dummy_ot[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

const int input_size = sizeof(dummy_in) / sizeof(double);
const int sizes[] = {0,1,2,3,4,8,20};
const int sizes_count = sizeof(sizes) / sizeof(int);


void banner() {
    printf("  ______ _    _ __________________ _____  \n");
    printf(" |  ____| |  | |___  /___  /  ____|  __ \\ \n");
    printf(" | |__  | |  | |  / /   / /| |__  | |__) |\n");
    printf(" |  __| | |  | | / /   / / |  __| |  _  / \n");
    printf(" | |    | |__| |/ /__ / /__| |____| | \\ \\ \n");
    printf(" |_|     \\____//_____/_____|______|_|  \\_\\\n\n");

    printf("This program tries each indicator with a lot of options.\n");
    printf("It's good for checking for crashes or assertion failures.\n");
    printf("Use with a memory tool to check for memory errors in ti.\n\n");
}

#define DUMP_STATE() \
do { \
    int k, j; \
    printf("\nInputs:"); \
    for (k = 0; k < input_size; ++k) printf(" %f", inputs[0][k]); \
    printf("\nOptions:"); \
    for (k = 0; k < info->options; ++k) printf(" %f", options[k]); \
    for (j = 0; j < info->outputs; ++j) { \
        printf("\nOutput[%d]:", j); \
        for (k = 0; k < input_size; ++k) printf(" %f", outputs[j][k]); \
    } \
 \
} while (0)

int errors_cnt = 0;

void check_output(const ti_indicator_info *info, int size, TI_REAL const *const *inputs, TI_REAL const * options, TI_REAL *const *outputs) {

    const int s = info->start(options);

    int o;
    for (o = 0; o < info->outputs; ++o) {

        TI_REAL max = 0;
        TI_REAL min = 0;

        int i;
        for (i = 0; i < size - s; ++i) {
            const TI_REAL op = outputs[o][i];
            const TI_REAL in = inputs[0][i+s];

            max = in > max ? in : max;
            min = in < min ? in : min;

            switch (info->type) {

                default:
                    break;

                case TI_TYPE_OVERLAY:
                    if ((op > 4 && op > max * 1.5 + 2) || (op < -4 && op < min * 0.5 - 3)) {
                        DUMP_STATE();
                        printf("\nERROR Output is out of range for input: input: %f output: %f\n", in, op);
                        errors_cnt += 1;
                        assert(0);
                    }
                    break;
            }
        }

    }
}


void stress_indicator(const ti_indicator_info *info) {
    const int opt_count = info->options;
    printf("%s (%s) (%d options)\n", info->name, info->full_name, opt_count);


    const double *inputs[TI_MAXINDPARAMS] = {0};
    const double *inputs0[TI_MAXINDPARAMS] = {0};
    double *outputs[TI_MAXINDPARAMS] = {0};

    int i;
    for (i = 0; i < TI_MAXINDPARAMS; ++i) {
        inputs[i] = i < info->inputs ? dummy_in : 0;
        inputs0[i] = i < info->inputs ? dummy_in0 : 0;
        outputs[i] = i < info->outputs ? dummy_ot : 0;
    }


    int options_index[TI_MAXINDPARAMS+1] = {0};
    double options[TI_MAXINDPARAMS];

    int choices;
    TI_REAL *optionsd;
    if (opt_count <= 4) {
        choices = sizeof(optionsd_small) / sizeof(double);
        optionsd = optionsd_small;
    } else {
        choices = sizeof(optionsd_large) / sizeof(double);
        optionsd = optionsd_large;
    }


    int j, count = 0;
    do {
        ++count;

        for (j = 0; j < opt_count; ++j) {
            options[j] = optionsd[options_index[j]];
            printf(" %f", options[j]);
        }

        //Check on zero input
        const int rz = info->indicator(input_size, inputs0, options, outputs);
        if (rz == TI_OKAY) check_output(info, input_size, inputs0, options, outputs);

        //Check on dummy input, with different input sizes
        for (int s = 0; s < sizes_count; ++s) {
            const int size = sizes[s];
            printf(" %d", size);
            const int r = info->indicator(size, inputs, options, outputs);
            if (r == TI_OKAY) check_output(info, size, inputs, options, outputs);

            if (info->indicator_ref) {
                printf(" r");
                const int rr = info->indicator_ref(size, inputs, options, outputs);
                if (rr == TI_OKAY) check_output(info, size, inputs, options, outputs);
            }


            if (info->stream_new) {
                ti_stream *stream;
                const int rs = info->stream_new(options, &stream);
                if (rs == TI_OKAY && stream) {
                    printf(" s");
                    int rsc = info->stream_run(stream, size, inputs, outputs);
                    if (rsc == TI_OKAY) check_output(info, size, inputs, options, outputs);

                    //Just run it a few more times on the same inputs
                    for (int repeat = 0; repeat < 5; ++repeat) {
                        rsc = info->stream_run(stream, size, inputs, outputs);
                        //Doesn't really make sense to check outputs, since the
                        //start is no longer valid.
                    }

                    info->stream_free(stream);
                }
            }

        }

        printf("\r                                                                          \r");

        j = 0;
        do {
            options_index[j] = (options_index[j] + 1) % choices;
        } while (options_index[j++] == 0);

    } while (j <= opt_count);

    assert(.1 > fabs(count - (pow(choices, opt_count))));
}


void stress_candle(const tc_candle_info *info) {
    printf("%s (%s)\n", info->name, info->full_name);

    const double *inputs[4] = {dummy_in, dummy_in, dummy_in, dummy_in};
    const double *inputs0[4] = {dummy_in0, dummy_in0, dummy_in0, dummy_in0};

    tc_config config;
    tc_config_set_to_default(&config);

    tc_result *output = tc_result_new();

    //Check on dummy input, with different input sizes
    for (int s = 0; s < sizes_count; ++s) {
        const int size = sizes[s];

        info->candle(size, inputs, &config, output);
        assert(tc_result_count(output) <= size);
        assert(tc_result_pattern_count(output) <= size);

        for (int i = 0; i < tc_result_count(output); ++i) {
            tc_hit hit = tc_result_get(output, i);
            assert(hit.patterns != TC_ALL);
            assert(hit.index >= 0);
            assert(hit.index < size);
        }
        for (int i = 0; i < size; ++i) {
            tc_set patterns = tc_result_at(output, i);
            assert(patterns != TC_ALL);
        }

        info->candle(size, inputs0, &config, output);
        assert(tc_result_count(output) <= size);
        assert(tc_result_pattern_count(output) <= size);
    }

    tc_result_free(output);

}


int main(int argc, char *argv[])
{
    banner();

    if (argc > 1) {
        const ti_indicator_info *ind_info = ti_find_indicator(argv[1]);
        if (ind_info) {
            stress_indicator(ind_info);
        }

        const tc_candle_info *cnd_info = tc_find_candle(argv[1]);
        if (cnd_info) {
            stress_candle(cnd_info);
        }
    } else {

        const ti_indicator_info *ind_info = ti_indicators;
        do {
            stress_indicator(ind_info);
        } while ((++ind_info)->name);

        const tc_candle_info *cnd_info = tc_candles;
        do {
            stress_candle(cnd_info);
        } while ((++cnd_info)->name);
    }

    printf("\r                                                  \rDone\n");

    return errors_cnt ? 1 : 0;
}
