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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#include "indicators.h"


/* Program takes an indicator name as an argument, and generates
 * a small example of applying the indicator to some actual numbers.
 */


#define INSIZE 15
TI_REAL out[5][INSIZE];

/* Example data on IBM. */
const char *datet[] = {"2005-11-01","2005-11-02","2005-11-03","2005-11-04","2005-11-07","2005-11-08","2005-11-09","2005-11-10","2005-11-11","2005-11-14","2005-11-15","2005-11-16","2005-11-17","2005-11-18","2005-11-21"};
TI_REAL date[] = {51101,51102,51103,51104,51107,51108,51109,51110,51111,51114,51115,51116,51117,51118,51121};
TI_REAL open[] = {81.85,81.2,81.55,82.91,83.1,83.41,82.71,82.7,84.2,84.25,84.03,85.45,86.18,88,87.6};
TI_REAL high[] = {82.15,81.89,83.03,83.3,83.85,83.9,83.33,84.3,84.84,85,85.9,86.58,86.98,88,87.87};
TI_REAL low[] = {81.29,80.64,81.31,82.65,83.07,83.11,82.49,82.3,84.15,84.11,84.03,85.39,85.76,87.17,87.01};
TI_REAL close[] = {81.59,81.06,82.87,83,83.61,83.15,82.84,83.99,84.55,84.36,85.53,86.54,86.89,87.77,87.29};
TI_REAL volume[] = {5653100,6447400,7690900,3831400,4455100,3798000,3936200,4732000,4841300,3915300,6830800,6694100,5293600,7985800,4807900};
TI_REAL alternative[] = {.2,.3,.4,.3,.5,.7,.75,.9,.9,1,1,.2,.1,-.1,-.5};



int main(int argc, char **argv) {
    const ti_indicator_info *info = ti_indicators;

    if (argc < 2) {
        printf("No indicator given.\n");
        printf("Example:\n");
        printf("\tsample ma 5");
        return 1;
    }

    if (strcmp(argv[1], "--version") == 0) {
        printf("TI VERSION: %s, TI BUILD: %d\n", TI_VERSION, TI_BUILD);
        return 0;
    }

    if (strcmp(argv[1], "--list") == 0) {
        do {
            if (info->type == TI_TYPE_OVERLAY)
                printf("type overlay ");
            else if (info->type == TI_TYPE_INDICATOR)
                printf("type indicator ");
            else if (info->type == TI_TYPE_MATH)
                printf("type math ");
            else if (info->type == TI_TYPE_SIMPLE)
                printf("type simple ");
            else
                printf("type unknown ");

            printf("name %s ", info->name);
            printf("full_name {%s} ", info->full_name);

            int i;
            printf("inputs {");
            for (i = 0; i < info->inputs; ++i)
                printf("%s%s", i ? " " : "", info->input_names[i]);
            printf("} ");

            printf("options {");
            for (i = 0; i < info->options; ++i)
                printf("%s{%s}", i ? " " : "", info->option_names[i]);
            printf("} ");

            printf("outputs {");
            for (i = 0; i < info->outputs; ++i)
                printf("%s{%s}", i ? " " : "", info->output_names[i]);
            printf("}");

            printf("\n");


        } while ((++info)->name);

        return 0;
    }



    info = ti_find_indicator(argv[1]);
    if (!info) {
        printf("Couldn't find indicator: %s\n", argv[1]);
        return 1;
    }


    TI_REAL const *inputs[] = {0, 0, 0, 0, 0};
    TI_REAL *outputs[] = {out[0], out[1], out[2], out[3], out[4]};

    int o=0, h=0, l=0, c=0, r=0, r2=0, v=0, a=0;


    /* Setup the inputs. */
    int j;
    for (j = 0; j < info->inputs; ++j) {
        if (strcmp(info->input_names[j], "open") == 0) {
            inputs[j] = open; o = 1;
        } else if (strcmp(info->input_names[j], "high") == 0) {
            inputs[j] = high; h = 1;
        } else if (strcmp(info->input_names[j], "low") == 0) {
            inputs[j] = low; l = 1;
        } else if (strcmp(info->input_names[j], "close") == 0) {
            inputs[j] = close; c = 1;
        } else if (strcmp(info->input_names[j], "volume") == 0) {
            inputs[j] = volume; v = 1;
        } else if (strcmp(info->input_names[j], "real") == 0) {
            if (!r) {
                inputs[j] = close; r = 1;
            } else {
                inputs[j] = open; r2 = 1;
            }
        } else {
            assert(0);
        };
    }


    /* See if we need alternative inputs (for the indicators that can't take large numbers. */
    const char *alts[] = {"acos", "asin", "atan", "cosh", "sinh", "tanh", "todeg", 0};
    {
        const char **alt = alts;
        while (*alt) {
            if (strcmp(*alt, info->name) == 0) {

                r=0;
                a=1;

                for (j = 0; j < info->inputs; ++j)
                    inputs[j] = alternative;

                break;
            }
            ++alt;
        }
    }


    /* Set options, save offset. */
    TI_REAL options[10];
    {
        int i;
        for (i = 0; i < info->options; ++i) {
            if (argc < 3 + i) {
                printf("*ERROR NOT ENOUGH OPTIONS*\n");
                return 1;
            }
            options[i] = atof(argv[2+i]);
        }
    }

    int start = info->start(options);


    /* Run it. */
    const int ret = info->indicator(INSIZE, inputs, options, outputs);
    if (ret == TI_OKAY) {

        int i, k;
        int bad = 0;

        printf("date        ");
        if (o) printf(" open   ");
        if (h) printf(" high   ");
        if (l) printf(" low    ");
        if (c) printf(" close  ");
        if (v) printf(" volume ");
        if (r) printf(" input  ");
        if (r2) printf(" input2  ");
        if (a) printf(" input  ");

        for (i = 0; i < info->outputs; ++i)
            printf(" %s", info->output_names[i]);

        printf("\n");

        for (i = 0; i < INSIZE; ++i) {
            printf("%s", datet[i]);
            if (o) printf(" %8.2f", open[i]);
            if (h) printf(" %8.2f", high[i]);
            if (l) printf(" %8.2f", low[i]);
            if (c) printf(" %8.2f", close[i]);
            if (v) printf(" %8.0f", volume[i]);
            if (r) printf(" %8.2f", close[i]);
            if (r2) printf(" %8.2f", open[i]);
            if (a) printf(" %8.2f", alternative[i]);
            if (i >= start) {
                for (k = 0; k < info->outputs; ++k)
                {
                    if (out[k][i-start] != out[k][i-start]) bad = 1;
                    printf(" %8.3f", out[k][i-start]);
                }
            }
            printf("\n");
        }

        if (bad) {
            printf("\nERROR NaN in outputs (%s).\n", info->name);
            return 1;
        }

        return 0;
    } else {
        if (ret == TI_INVALID_OPTION) {
            printf("*ERROR INVALID OPTION*\n");
        } else {
            printf("*ERROR*\n");
        }
        return 1;
    }
}

