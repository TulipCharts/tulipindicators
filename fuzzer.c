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
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


double optionsd[] = {-20,-1,0,.1,.5,.7,1,2,2.5,3,4,5,6,7,8,9,10,11,17,18,19,20,21,22,23,24,25,26,100};
double dummy_in[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
double dummy_in0[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
double dummy_ot[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};



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
    for (k = 0; k < size; ++k) printf(" %f", inputs[0][k]); \
    printf("\nOptions:"); \
    for (k = 0; k < info->options; ++k) printf(" %f", options[k]); \
    for (j = 0; j < info->outputs; ++j) { \
        printf("\nOutput[%d]:", j); \
        for (k = 0; k < size; ++k) printf(" %f", outputs[j][k]); \
    } \
 \
} while (0)


void check_output(const ti_indicator_info *info, int size, TI_REAL const *const *inputs, TI_REAL const * options, TI_REAL *const *outputs) {

    int s;
    s = info->start(options);

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
                    if (op > max * 1.5 + 2 || op < min * 0.5 - 2) {
                        DUMP_STATE();
                        printf("\nERROR Output is out of range for input: input: %f output: %f\n", in, op);
                        assert(0);
                    }
                    break;
            }
        }

    }
}


void stress(const ti_indicator_info *info) {
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

    int choices = sizeof(optionsd) / sizeof(double);
    int size = sizeof(dummy_in) / sizeof(double);

    int j, count = 0;
    do {
        ++count;

        for (j = 0; j < opt_count; ++j) {
            options[j] = optionsd[options_index[j]];
            printf(" %f", options[j]);
        }

        int r;

        printf(" 0"); r = info->indicator(0, inputs, options, outputs); if (r == TI_OKAY) check_output(info, 0, inputs, options, outputs);
        printf(" 1"); r = info->indicator(1, inputs, options, outputs); if (r == TI_OKAY) check_output(info, 1, inputs, options, outputs);
        printf(" 2"); r = info->indicator(2, inputs, options, outputs); if (r == TI_OKAY) check_output(info, 2, inputs, options, outputs);
        printf(" 3"); r = info->indicator(3, inputs, options, outputs); if (r == TI_OKAY) check_output(info, 3, inputs, options, outputs);
        printf(" %d", size); r = info->indicator(size, inputs, options, outputs); if (r == TI_OKAY) check_output(info, size, inputs, options, outputs);
        printf(" 0s"); r = info->indicator(size, inputs0, options, outputs); if (r == TI_OKAY) check_output(info, size, inputs0, options, outputs);

        printf("\r                                                  \r");

        j = 0;
        do {
            options_index[j] = (options_index[j] + 1) % choices;
        } while (options_index[j++] == 0);

    } while (j <= opt_count);

    assert(.1 > fabs(count - (pow(choices, opt_count))));
}


int main(int argc, char *argv[])
{
    banner();

    if (argc > 1) {
        const ti_indicator_info *info = ti_find_indicator(argv[1]);
        if (info) stress(info);
    } else {
        const ti_indicator_info *info = ti_indicators;
        do {
            stress(info);
        } while ((++info)->name);
    }

    printf("\r                                                  \rDone\n");

    return 0;
}
