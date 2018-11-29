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
#include <time.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include "utils/minmax.h"

#include "indicators.h"
#include <ta-lib/ta_abstract.h>
#include <ta-lib/ta_func.h>

#define INSIZE 4000

#define TESTING_MODE1
#ifdef TESTING_MODE
#define LOOPS 1
#define PSTART 14
#define PEND 14
#else
#define LOOPS 100
#define PSTART 4
#define PEND 150
#endif


#define MAX_IN_SERIES 5
#define MAX_OUT_SERIES 3


TI_REAL in[MAX_IN_SERIES][INSIZE];
TI_REAL out[MAX_OUT_SERIES][INSIZE];
int ind_offset;

TI_REAL outta[MAX_OUT_SERIES][INSIZE];
int ta_offset;

int bestof = 1;

/* Generate some fake bar data to test on. */
void generate_inputs() {
    TI_REAL *open = in[0];
    TI_REAL *high = in[1];
    TI_REAL *low = in[2];
    TI_REAL *close = in[3];
    TI_REAL *volume = in[4];

    open[0] = 100;

    srand(22);

    int i;
    for (i = 0; i < INSIZE; ++i) {

        const TI_REAL diff1 = (((TI_REAL)(rand()) / RAND_MAX - .5) + .01) * 2.5;
        const TI_REAL diff2 = (((TI_REAL)(rand()) / RAND_MAX - .5) + .01) * 2.5;
        const TI_REAL diff3 = (((TI_REAL)(rand()) / RAND_MAX)) * 0.5;
        const TI_REAL diff4 = (((TI_REAL)(rand()) / RAND_MAX)) * 0.5;
        const TI_REAL vol = (((TI_REAL)(rand()) / RAND_MAX)) * 10000 + 500;

        if (i) {
            open[i] = open[i-1] + diff1;
        }

        close[i] = open[i] + diff2;
        high[i] = open[i] > close[i] ? open[i] + diff3 : close[i] + diff3;
        low[i] = open[i] < close[i] ? open[i] - diff4 : close[i] - diff4;
        volume[i] = vol;


        /*printf("%.2f %.2f %.2f %.2f\n", open[i], high[i], low[i], close[i]);*/

        assert(open[i] <= high[i]);
        assert(close[i] <= high[i]);

        assert(open[i] >= low[i]);
        assert(close[i] >= low[i]);

        assert(high[i] >= low[i]);
    }

    /* This is a hack, since ta-lib obv uses volume[0] as starting value and
     * ti obv uses 0 as starting value. */
    volume[0] = 0;
}


/* Chart of names we should cross reference to talib. */
typedef struct cross {
    char *name;
    char *rename;
    ti_indicator_function adapter;
} cross;

cross cross_ref[] = {
    {"cmo", "XXX", 0}, /* talib uses EMA, we use SMA. */
    {"dm", "plus_dm", 0},
    {"di", "plus_di", 0},
    {"linreg", "linearreg", 0},
    {"linregintercept", "linearreg_intercept", 0},
    {"linregslope", "linearreg_slope", 0},
    {"mul", "mult", 0},
    {"roc", "rocp", 0},
    {"tr", "trange", 0},
    {"wcprice", "wclprice", 0},
    {"psar", "sar", 0},
    {0, 0, 0}
};


/* Search for a talib function by name. */
const TA_FuncInfo *find_ta(const char *ti_name) {

        char name[64];
        strcpy(name, ti_name);

        char *n = name;

        /* Check if a different name is used. */
        cross *c = cross_ref;
        while (c->name) {
            if (strcmp(c->name, n) == 0) {
                if (c->adapter) {
                    /* Use adapter function instead. */
                    assert(0);
                } else {
                    /* Replace name. */
                    strcpy(name, c->rename);
                }
                break;
            }
            ++c;
        }


        /* TALIB uses upper case names. */
        while (*n) {
            *n = (char)toupper(*n);
            ++n;
        }


        /* Find it. */
        TA_FuncHandle const *handle;
        const TA_FuncInfo *tainfo;

        int r = TA_GetFuncHandle(name, &handle);
        if (r == TA_SUCCESS) {
            r = TA_GetFuncInfo(handle, &tainfo);
            if (r == TA_SUCCESS) {
                return tainfo;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
}



#define GOAL_RUN 0
#define GOAL_SETUP 1
#define GOAL_FREE 2




/* Interface for talib. */
#define CHECKR(msg) do {if (r != TA_SUCCESS) {printf("   *ERROR %s %d*\n", (msg), r); return 1;}} while (0)
int run_ta(const TA_FuncInfo *info, double *options, int goal) {
    static TA_ParamHolder *ph = 0;
    int r, i;

    if (goal == GOAL_RUN) {

        /* Setup options. */
        for (i = 0; i < (int)info->nbOptInput; ++i) {
            const TA_OptInputParameterInfo *option;
            TA_GetOptInputParameterInfo(info->handle, (unsigned int)i, &option);

            if (option->type == TA_OptInput_RealRange)
                r = TA_SetOptInputParamReal(ph, (unsigned int)i, options[i]);
            else if (option->type == TA_OptInput_IntegerRange)
                r = TA_SetOptInputParamInteger(ph, (unsigned int)i, (int)options[i]);
            else {
                r = TA_SetOptInputParamInteger(ph, (unsigned int)i, (int)options[i]);
            }
            CHECKR("setting options");
        }


        /* Run it. */
        int onb;
        r = TA_CallFunc(ph, 0, INSIZE-1, &ta_offset, &onb);
        CHECKR("running");


    } else if (goal == GOAL_SETUP) {

        /* Setup the inputs, outputs. */
        r = TA_ParamHolderAlloc(info->handle, &ph);
        CHECKR("holder alloc");

        for(i = 0; i < (int)info->nbInput; ++i)
        {
            const TA_InputParameterInfo *input;
            TA_GetInputParameterInfo(info->handle, (unsigned int)i, &input);

            if (input->type == TA_Input_Price) {
                r = TA_SetInputParamPricePtr(ph, (unsigned int)i, in[0], in[1], in[2], in[3], in[4], 0);
                CHECKR("set price");
            } else if (input->type == TA_Input_Real) {
                r = TA_SetInputParamRealPtr(ph, (unsigned int)i, in[3]);
                CHECKR("set real");
            } else if (input->type == TA_Input_Integer) {
                assert(0);
            } else {
                assert(0);
            }
        }


        assert(info->nbOutput);
        for(i = 0; i < (int)info->nbOutput; ++i)
        {
            r = TA_SetOutputParamRealPtr(ph, (unsigned int)i, outta[i]);
            CHECKR("setting outputs");
        }

    } else if (goal == GOAL_FREE) {

        TA_ParamHolderFree(ph);

    } else {

        assert(0);

    }

    return 0;
}


int run_ti(const ti_indicator_info *info, double *options, int goal) {
    static TI_REAL *inputs[] = {0, 0, 0, 0, 0};
    static TI_REAL *outputs[] = {out[0], out[1], out[2], out[3], out[4]};

    if (goal == GOAL_RUN) {
        /* Set options, save offset. */
        ind_offset = info->start(options);

        /* Run it. */
        const int ret = info->indicator(INSIZE, (TI_REAL const *const *)inputs, options, outputs);
        if (ret != TI_OKAY) {
            printf("   *ERROR* (%d)\n", ret);
            printf("options:\n");
            int i;
            for (i = 0; i < info->options; ++i) {
                printf("%d = %f\n", i, options[i]);
            }
            return 1;
        }

    } else if (goal == GOAL_SETUP) {

        /* Setup the inputs. */
        int j;
        for (j = 0; j < info->inputs; ++j) {
            if (strcmp(info->input_names[j], "open") == 0) {
                inputs[j] = in[0];
            } else if (strcmp(info->input_names[j], "high") == 0) {
                inputs[j] = in[1];
            } else if (strcmp(info->input_names[j], "low") == 0) {
                inputs[j] = in[2];
            } else if (strcmp(info->input_names[j], "close") == 0) {
                inputs[j] = in[3];
            } else if (strcmp(info->input_names[j], "real") == 0) {
                inputs[j] = in[3];
            } else if (strcmp(info->input_names[j], "volume") == 0) {
                inputs[j] = in[4];
            } else {
                assert(0);
            };
        }

    } else if (goal == GOAL_FREE) {
    } else {
        assert(0);
    }

    return 0;
}

void macd_option_setter(double period, double *options, int ti) {
    ti=ti;
    options[0] = period;
    options[1] = period + 10;

    options[2] = period + 1;
}

void ppo_option_setter(double period, double *options, int ti) {
    ti=ti;
    options[0] = period;
    options[1] = period + 10;

    if (!ti) {
        options[2] = TA_MAType_EMA;
    }
}
void psar_option_setter(double period, double *options, int ti) {
    ti=ti;
    options[0] = 1/period;
    options[1] = options[0] * 10;
}

void fast_slow_option_setter(double period, double *options, int ti) {
    ti=ti;
    options[0] = period;
    options[1] = period + 10;
}

void stoch_option_setter(double period, double *options, int ti) {
    /* TI: k period, k slowing, d period. */
    /* TA-LIB: k period, k slowing, k ma type, d period, d ma type */
    if (ti) {
        options[0] = period;
        options[1] = 3;
        options[2] = 4;
    } else {
        options[0] = period;
        options[1] = 3;
        options[2] = TA_MAType_SMA;
        options[3] = 4;
        options[4] = TA_MAType_SMA;
    }
}

void stochrsi_option_setter(double period, double *options, int ti) {
    ti=ti;
    options[0] = period;
    options[1] = period;
    options[2] = period;
    options[3] = 1;
}

void ultosc_option_setter(double period, double *options, int ti) {
    ti=ti;
    options[0] = period;
    options[1] = period * 2;
    options[2] = period * 4;
}

void vidya_option_setter(double period, double *options, int ti) {
    ti=ti;
    options[0] = period;
    options[1] = period + 10;
    options[2] = .2;
}

double stochrsi_output_adjust(double a) {
    /* ta-lib uses stochrsi * 100 */
    return a * 0.01;
}


void bench(FILE *log, const ti_indicator_info *info) {

    /* Clear results, so there is no carry over.
     * Also insures that if both fail an equal comparison is impossible. */

    {
        int i;
        for (i = 0; i < MAX_OUT_SERIES; ++i) {
            memset(out[i], 0xF1, sizeof(out[i]));
            memset(outta[i], 0xF2, sizeof(outta[i]));
        }
    }


    /* Grab talib function to compare to. */
    const TA_FuncInfo *tainfo = find_ta(info->name);


    /* Options storage. */
    /* NB some talib functions take an extra scalar option, so we set the default to 1. */
    double options[] = {1,1,1,1,1,1};


    /* Some functions need a different option generator strategy.
     * Messy, but can't really be helped. */
    void (*options_setter)(double period, double *options, int ti) = 0;

    if (strcmp(info->name, "apo") == 0) options_setter = ppo_option_setter;
    if (strcmp(info->name, "macd") == 0) options_setter = macd_option_setter;
    if (strcmp(info->name, "ppo") == 0) options_setter = ppo_option_setter;
    if (strcmp(info->name, "psar") == 0) options_setter = psar_option_setter;
    if (strcmp(info->name, "adosc") == 0) options_setter = fast_slow_option_setter;
    if (strcmp(info->name, "kvo") == 0) options_setter = fast_slow_option_setter;
    if (strcmp(info->name, "stoch") == 0) options_setter = stoch_option_setter;
    if (strcmp(info->name, "stochrsi") == 0) options_setter = stochrsi_option_setter;
    if (strcmp(info->name, "ultosc") == 0) options_setter = ultosc_option_setter;
    if (strcmp(info->name, "vosc") == 0) options_setter = fast_slow_option_setter;
    if (strcmp(info->name, "vidya") == 0) options_setter = vidya_option_setter;

    double (*output_adjust)(double period) = 0;
    if (strcmp(info->name, "stochrsi") == 0) output_adjust = stochrsi_output_adjust;

    if (info) {
        if (log) fprintf(log, ",\n  \"%s\" => array(\n", info->name);
        if (log) fprintf(log, "    \"name\" => \"%s\"", info->full_name);

        int best_e = 1e9, best_p = -1e9;
        int best;
        for (best = 0; best < bestof; ++best) {
            printf("Benchmark %12s%s", info->name, "      ");
            int i, period;
            int iterations = 0;
            const clock_t start = clock();

            run_ti(info, 0, GOAL_SETUP);

            for (i = 0; i < LOOPS; ++i) {
                for (period = PSTART; period <= PEND; ++period) {
                    if (options_setter)
                        options_setter(period, options, 1);
                    else
                        options[0] = period;

                    const int ret = run_ti(info, options, GOAL_RUN);
                    if (ret) {
                        return;
                    }
                    ++iterations;
                }
            }

            run_ti(info, 0, GOAL_FREE);

            const int elapsed = (clock() - start) * 1000 / CLOCKS_PER_SEC;
            const int performance = elapsed == 0 ? 0 : (iterations * INSIZE) / elapsed / 1000;

            /*Million floats per second input.*/
            if (elapsed)
                printf("\t%5dms\t%5dmfps\n", elapsed, performance);
            else
                printf("\n");

            best_e = MIN(elapsed, best_e);
            best_p = MAX(performance, best_p);
        }

        if (log) fprintf(log, ",\n    \"ti\" => array(\"elapsed\" => %d, \"performance\" => %d)", best_e, best_p);
    }


    if (tainfo) {

        int best_e = 1e9, best_p = -1e9;
        int best;
        for (best = 0; best < bestof; ++best) {
            printf("Benchmark %12s%s", tainfo->name, "-talib");
            int i, period;
            int iterations = 0;

            const clock_t start = clock();

            run_ta(tainfo, 0, GOAL_SETUP);

            for (i = 0; i < LOOPS; ++i) {
                for (period = PSTART; period <= PEND; ++period) {
                    if (options_setter)
                        options_setter(period, options, 0);
                    else
                        options[0] = period;

                    const int ret = run_ta(tainfo, options, GOAL_RUN);
                    if (ret) return;
                    ++iterations;
                }
            }

            run_ta(tainfo, 0, GOAL_FREE);

            int elapsed = (clock() - start) * 1000 / CLOCKS_PER_SEC;
            int performance = elapsed == 0 ? 0 : (iterations * INSIZE) / elapsed / 1000;

            /*Million floats per second input.*/
            if (elapsed)
                printf("\t%5dms\t%5dmfps\n", elapsed, performance);
            else
                printf("\n");

            best_e = MIN(elapsed, best_e);
            best_p = MAX(performance, best_p);
        }

        if (log) fprintf(log, ",\n    \"talib\" => array(\"elapsed\" => %d, \"performance\" => %d)", best_e, best_p);
    }


    if (info && tainfo) {
        /* As sanity check, see that a few values actually match
         * between the libraries.
         * We don't expect the first few values to match, there
         * is not agreement on how or when to initialize many indicators.
         * They should even out and match at the end though. */


        /* Messy, but sometimes the outputs are in a different order. */
        int remap[] = {0,1,2,3,4};
        if (strcmp(info->name, "bbands") == 0) {remap[0] = 2; remap[2] = 0;}
        if (strcmp(info->name, "dm") == 0) {remap[0] = 0; remap[1] = -1;}
        if (strcmp(info->name, "di") == 0) {remap[0] = 0; remap[1] = -1;}

        int i, j;

        /*
        for (i = 0; i < 10; ++i)
            printf("check %.2f %.2f\n", out[0][i], outta[remap[0]][i]);

        printf("Checking %d outputs.\n", info->outputs);
        */

        for (j = 0; j < info->outputs; ++j) {
            int end = INSIZE/2;
            if (remap[j] == -1) continue; /* Skip output. */

            /*if (ta_offset == ind_offset) end = INSIZE-ta_offset;*/
            for (i = 0; i < end; ++i) {
                double a = out[j][INSIZE-1-i-ind_offset];
                double b = outta[remap[j]][INSIZE-1-i-ta_offset];

                if (output_adjust) b = output_adjust(b);

                double diff = fabs(a-b);
                if (diff > 0.0001) {
                    printf("Results disagree.\n");
                    printf("    series         (diff)   ti   ta\n");
                    printf("    series %d, %4d (%f) %.5f %.5f\n", j, i, diff, a, b);
                    break;
                }
            }
        }
    }

    if (log) fprintf(log, "\n  )");
    printf("\n");
}






int main(int argc, char **argv) {
    printf("Tulip Charts Indicator benchmark.\n");
    printf("Using real size of: %d.\n\n", sizeof(TI_REAL));

    generate_inputs();

    const ti_indicator_info *info = ti_indicators;

    if (argc > 1 && !isdigit(argv[1][0])) {
        /* Benchmark functions given on command line. */

        if (argc > 2) {
            bestof = atoi(argv[2]);
            if (!bestof) bestof = 1;
        }

        info = ti_find_indicator(argv[1]);
        if (!info) {
            printf("Couldn't find indicator: %s\n", argv[1]);
            return 1;
        } else {
            bench(0, info);
        }

    } else {

        if (argc > 1) {
            bestof = atoi(argv[1]);
        }

        FILE *log = fopen("docs/benchmark.php", "w");
        if (!log) {
            printf("Couldn't open results file for output. Make sure ./docs/ exists.\n");
            exit(1);
        }

        fprintf(log, "<?php\n");
        fprintf(log, "#GENERATED BY benchmark.c\n");
        fprintf(log, "#DO NOT MODIFY DIRECTLY\n");
        fprintf(log, "$benchmark = array(\n");
        fprintf(log, "  \"worksize\" => %d,", LOOPS * (PEND-PSTART+1) * INSIZE);
        fprintf(log, "  \"bestof\" => %d", bestof);

        /* Benchmark all. */
        do {

            /* Skip indicators that are very slow. */
            if (strcmp(info->name, "msw") == 0) continue;

            bench(log, info);
        } while ((++info)->name);

        fprintf(log, ");");

        fclose(log);
    }


    /*
    int i;
    for (i = 0; i < 10; ++i) {
        printf("%.2f %.2f %.2f %.2f %.2f\n", in[0][i], in[1][i], in[2][i], in[3][i], in[4][i]);
    }
    */


    return 0;
}

