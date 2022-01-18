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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include "utils/minmax.h"

#include "indicators.h"
#include "candles.h"

#ifdef TA_LIB
#include <ta-lib/ta_abstract.h>
#include <ta-lib/ta_func.h>
#endif

#define INSIZE 4000 //Size of input arrays

#define TESTING_MODE
#ifdef TESTING_MODE
#define LOOPS 1
#define PSTART 4
#define PEND 150
#else
#define LOOPS 100 //How many times to loop in single test
#define PSTART 4 //Period starting value
#define PEND 150 //Period ending value
#endif

int bestof = 1; //How many repeats for benchmark. Override on command line.

int fails = 0; //Track how many comparisons fail.

#define MAX_IN_SERIES 5
#define MAX_OUT_SERIES 3

TI_REAL in[MAX_IN_SERIES][INSIZE];
TI_REAL out[MAX_OUT_SERIES][INSIZE];
TI_REAL outref[MAX_OUT_SERIES][INSIZE];
TI_REAL outstream1[MAX_OUT_SERIES][INSIZE];
TI_REAL outstreamall[MAX_OUT_SERIES][INSIZE];
tc_result *outcnd = 0;
int ind_offset;

#ifdef TA_LIB
TI_REAL outta[MAX_OUT_SERIES][INSIZE];
int outtaint[INSIZE];
int ta_offset;
#endif


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
        assert(high[i] >= open[i]);
        assert(high[i] >= close[i]);

        assert(low[i] <= low[i]);
        assert(low[i] <= open[i]);
        assert(low[i] <= close[i]);
    }

    /* This is a hack, since ta-lib obv uses volume[0] as starting value and
     * ti obv uses 0 as starting value. */
    volume[0] = 0;
}




#define GOAL_RUN 0
#define GOAL_SETUP 1
#define GOAL_FREE 2


/* Interface to run a benchmark. */
typedef int (*run_fun)(const ti_indicator_info *info, TI_REAL *options, int goal);


#ifdef TA_LIB

/* Chart of names we should cross reference to talib. */
typedef struct ind_cross_ref {
    char *name;
    char *rename;
    ti_indicator_function adapter;
} ind_cross_ref;

ind_cross_ref ind_cross_refs[] = {
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


/* Cross reference candle functions manually. */
typedef TA_RetCode (*TA_CdlFunction)(int startIdx, int endIdx, const double inOpen[], const double inHigh[], const double inLow[], const double inClose[], int *outBegIdx, int *outNBElement, int outInteger[]);

typedef struct cnd_cross_ref {
    tc_set pattern;
    TA_CdlFunction tacdl;
    int count; /* 0 to count any, -1 to only count negatives, +1 to only count positives */
} cnd_cross_ref;

cnd_cross_ref cnd_cross_refs[] = {
    //{TC_ABANDONED_BABY_BEAR, 0, 0},
    //{TC_ABANDONED_BABY_BULL, 0, 0},
    //{TC_BIG_BLACK_CANDLE, 0, 0},
    //{TC_BIG_WHITE_CANDLE, 0, 0},
    {TC_BLACK_MARUBOZU, TA_CDLMARUBOZU, -1},
    {TC_DOJI, TA_CDLDOJI, 0},
    {TC_DRAGONFLY_DOJI, TA_CDLDRAGONFLYDOJI, 0},
    {TC_ENGULFING_BEAR, TA_CDLENGULFING, -1},
    {TC_ENGULFING_BULL, TA_CDLENGULFING, 1},
    //{TC_EVENING_DOJI_STAR, 0, 0},
    //{TC_EVENING_STAR, 0, 0},
    //{TC_FOUR_PRICE_DOJI, 0, 0},
    {TC_GRAVESTONE_DOJI, TA_CDLGRAVESTONEDOJI, 0},
    {TC_HAMMER, TA_CDLHAMMER, 0},
    {TC_HANGING_MAN, TA_CDLHANGINGMAN, 0},
    {TC_INVERTED_HAMMER, TA_CDLINVERTEDHAMMER, 0},
    {TC_LONG_LEGGED_DOJI, TA_CDLLONGLEGGEDDOJI, 0},
    {TC_MARUBOZU, TA_CDLMARUBOZU, 0},
    //{TC_MORNING_DOJI_STAR, 0, 0},
    //{TC_MORNING_STAR, 0, 0},
    //{TC_SPINNING_TOP, 0, 0},
    //{TC_STAR, 0, 0},
    //{TC_THREE_BLACK_CROWS, 0, 0},
    //{TC_THREE_WHITE_SOLDIERS, 0, 0},
    {TC_WHITE_MARUBOZU, TA_CDLMARUBOZU, 1},
    {0, 0, 0}
};


void setup_talib() {
    TA_Initialize();

    /* Try to match TA-Lib settings to tulip indicators. */
    tc_config config;
    tc_config_set_to_default(&config);

    TA_SetCandleSettings(TA_BodyDoji, TA_RangeType_HighLow, config.period, config.body_none);
    TA_SetCandleSettings(TA_BodyShort, TA_RangeType_RealBody, config.period, config.body_short);
    TA_SetCandleSettings(TA_BodyLong, TA_RangeType_RealBody, config.period, config.body_long);

    TA_SetCandleSettings(TA_ShadowVeryShort, TA_RangeType_HighLow, config.period, config.wick_none);
    TA_SetCandleSettings(TA_ShadowLong, TA_RangeType_RealBody, config.period, config.wick_long);

    TA_SetCandleSettings(TA_Near, TA_RangeType_HighLow, config.period, config.near);
}


/* Search for a talib function by name. */
const TA_FuncInfo *find_ta(const char *ti_name) {

        char name[64];
        strcpy(name, ti_name);

        char *n = name;

        /* Check if a different name is used. */
        ind_cross_ref *c = ind_cross_refs;
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


/* Interface for talib. */
#define CHECKR(msg) do {if (r != TA_SUCCESS) {printf("   *ERROR %s %d*\n", (msg), r); return 1;}} while (0)
int run_ta(const ti_indicator_info *info_cast, TI_REAL *options, int goal) {
    const TA_FuncInfo *info = (const TA_FuncInfo *)info_cast;
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

#endif //TA_LIB



void ti_setup(const ti_indicator_info *info, TI_REAL **inputs) {
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
}


int run_ti(const ti_indicator_info *info, TI_REAL *options, int goal) {
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

        ti_setup(info, inputs);

    } else if (goal == GOAL_FREE) {
    } else {
        assert(0);
    }

    return 0;
}


int run_ti_ref(const ti_indicator_info *info, TI_REAL *options, int goal) {
    static TI_REAL *inputs[] = {0, 0, 0, 0, 0};
    static TI_REAL *outputs[] = {outref[0], outref[1], outref[2], outref[3], outref[4]};

    if (goal == GOAL_RUN) {
        /* Set options, save offset. */
        ind_offset = info->start(options);

        /* Run it. */
        const int ret = info->indicator_ref(INSIZE, (TI_REAL const *const *)inputs, options, outputs);
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

        ti_setup(info, inputs);

    } else if (goal == GOAL_FREE) {
    } else {
        assert(0);
    }

    return 0;
}


int run_ti_stream1(const ti_indicator_info *info, TI_REAL *options, int goal) {
    static TI_REAL *inputs[] = {0, 0, 0, 0, 0};
    static TI_REAL *outputs[] = {outstream1[0], outstream1[1], outstream1[2], outstream1[3], outstream1[4]};

    if (goal == GOAL_RUN) {
        /* Set options, save offset. */
        ind_offset = info->start(options);


        TI_REAL *ins[5] = {0};
        TI_REAL *outs[5] = {0};

        /* Run it. */
        ti_stream *stream;
        const int sret = info->stream_new(options, &stream);
        assert(stream);
        assert(sret == TI_OKAY);

        for (int i = 0; i < INSIZE; ++i) {

            for (unsigned int j = 0; j < (sizeof(inputs)/sizeof(TI_REAL*)); ++j) {
                ins[j] = inputs[j] + i;
            }

            int k = ti_stream_get_progress(stream);
            if (k < 0) k = 0;
            for (unsigned int j = 0; j < (sizeof(outputs)/sizeof(TI_REAL*)); ++j) {
                outs[j] = outstream1[j] + k;
            }

            const int ret = info->stream_run(stream, 1, (TI_REAL const *const *)ins, outs);

            if (ret != TI_OKAY) {
                printf("   *ERROR* (%d)\n", ret);
                printf("options:\n");
                for (int j = 0; j < info->options; ++j) {
                    printf("%d = %f\n", j, options[j]);
                }
                return 1;
            }


        }

        info->stream_free(stream);


    } else if (goal == GOAL_SETUP) {

        ti_setup(info, inputs);

    } else if (goal == GOAL_FREE) {
    } else {
        assert(0);
    }

    return 0;
}


int run_ti_streamall(const ti_indicator_info *info, TI_REAL *options, int goal) {
    static TI_REAL *inputs[] = {0, 0, 0, 0, 0};
    static TI_REAL *outputs[] = {outstreamall[0], outstreamall[1], outstreamall[2], outstreamall[3], outstreamall[4]};

    if (goal == GOAL_RUN) {
        /* Set options, save offset. */
        ind_offset = info->start(options);

        /* Run it. */
        ti_stream *stream;
        const int sret = info->stream_new(options, &stream);
        assert(stream);
        assert(sret == TI_OKAY);

        const int ret = info->stream_run(stream, INSIZE, (TI_REAL const *const *)inputs, outputs);
        info->stream_free(stream);

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

        ti_setup(info, inputs);

    } else if (goal == GOAL_FREE) {
    } else {
        assert(0);
    }

    return 0;
}


/* Option setter functions. */

//TODO These should have less constants, instead relying on some manipulation of period
//TODO It seems there are too many functions here. It's likely a few could be combined.

void simple_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    (void)ti;
    options[0] = period;
}

void fast_slow_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    (void)ti;
    options[0] = period;
    options[1] = period + 10;
}

void alma_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    (void)ti;
    options[0] = period;
    options[1] = 0.5;
    options[2] = 1;
}

void bbands_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    (void)ti;
    options[0] = period;
    options[1] = 1.0;
#ifdef TA_LIB
    if (!ti) {
        options[2] = 1.0;
        options[3] = TA_MAType_SMA;
    }
#endif
}

void ce_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    (void)ti;
    options[0] = period;
    options[1] = 3;
}

void copp_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    (void)ti;
    options[0] = 11;
    options[1] = 14;
    options[2] = period;
}

void kc_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    (void)ti;
    options[0] = period;
    options[1] = 0.77;
}

void kst_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    (void)ti;
    for (int i = 0; i < 4; ++i) {
        options[i+4] = options[i] = period + period / 4. * i;
    }
}

void macd_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    (void)ti;
    options[0] = period;
    options[1] = period + 10;

    options[2] = period + 1;
}

void mama_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    (void)ti;
    //TODO base something on period
    (void)period;
    options[0] = 0.5;
    options[1] = 0.05;
}

void pfe_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    (void)ti;
    options[0] = period;
    options[1] = 5;
}

void posc_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    (void)ti;
    options[0] = period;
    options[1] = 3;
}

void ppo_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    (void)ti;
    options[0] = period;
    options[1] = period + 10;

#ifdef TA_LIB
    if (!ti) {
        options[2] = TA_MAType_EMA;
    }
#endif
}

void psar_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    (void)ti;
    options[0] = 1/period;
    options[1] = options[0] * 10;
}

void rmi_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    (void)ti;
    options[0] = period;
    options[1] = 3;
}

void rmta_option_setter(TI_REAL period,  TI_REAL *options, int ti) {
    (void)ti;
    options[0] = period;
    options[1] = 1 - (2. / (period + 1));
}

void rvi_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    (void)ti;
    options[0] = period;
    options[1] = 10;
}

void smi_option_setter(TI_REAL period, TI_REAL* options, int ti) {
    (void)ti;
    options[0] = period;
    options[1] = 25;
    options[2] = 3;
}

void stoch_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    /* TI: k period, k slowing, d period. */
    /* TA-LIB: k period, k slowing, k ma type, d period, d ma type */
    if (ti) {
        options[0] = period;
        options[1] = 3;
        options[2] = 4;
    } else {
#ifdef TA_LIB
        options[0] = period;
        options[1] = 3;
        options[2] = TA_MAType_SMA;
        options[3] = 4;
        options[4] = TA_MAType_SMA;
#endif
    }
}

void stochrsi_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    (void)ti;
    options[0] = period;
    options[1] = period;
    options[2] = period;
    options[3] = 1;
}

void tsi_option_setter(TI_REAL period, TI_REAL* options, int ti) {
    (void)ti;
    options[0] = period;
    options[1] = 3;
}

void ultosc_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    (void)ti;
    options[0] = period;
    options[1] = period * 2;
    options[2] = period * 4;
}

void vidya_option_setter(TI_REAL period, TI_REAL *options, int ti) {
    (void)ti;
    options[0] = period;
    options[1] = period + 10;
    options[2] = .2;
}







/* Output adjustment functions. */

TI_REAL stochrsi_output_adjust(TI_REAL a) {
    /* ta-lib uses stochrsi * 100 */
    return a * 0.01;
}







void check_outputs(const ti_indicator_info *info, const char* what, TI_REAL compare[MAX_OUT_SERIES][INSIZE]) {

    /* Messy, but sometimes the outputs are in a different order. */
    int remap[] = {0,1,2,3,4};

    int nanok = 0;
    if (strcmp(info->name, "asin") == 0) nanok = 1;
    if (strcmp(info->name, "acos") == 0) nanok = 1;

#ifdef TA_LIB
    int ta = 0;
    if (strcmp("ta", what) == 0) ta = 1;
    TI_REAL (*output_adjust)(TI_REAL period) = 0;
    if (ta) {
        if (strcmp(info->name, "stochrsi") == 0) output_adjust = stochrsi_output_adjust;

        if (strcmp(info->name, "bbands") == 0) {remap[0] = 2; remap[2] = 0;}
        if (strcmp(info->name, "dm") == 0) {remap[0] = 0; remap[1] = -1;}
        if (strcmp(info->name, "di") == 0) {remap[0] = 0; remap[1] = -1;}
    }

#endif

    int i, j;

    /*
       for (i = 0; i < 10; ++i)
       printf("check %.2f %.2f\n", out[0][i], outta[remap[0]][i]);

       printf("Checking %d outputs.\n", info->outputs);
       */

    for (j = 0; j < info->outputs; ++j) {
        int end = INSIZE - ind_offset;
#ifdef TA_LIB
        if (ta) end = INSIZE/2;
#endif
        if (remap[j] == -1) continue; /* Skip output. */

        /*if (ta_offset == ind_offset) end = INSIZE-ta_offset;*/
        for (i = 0; i < end; ++i) {
            TI_REAL a = out[j][INSIZE-1-i-ind_offset];
            TI_REAL b = compare[j][INSIZE-1-i-ind_offset];

#ifdef TA_LIB
            if (ta) {
                b = compare[remap[j]][INSIZE-1-i-ta_offset];
            }

            if (output_adjust) b = output_adjust(b);
#endif

            TI_REAL diff = fabs(a-b);
            if (diff > 0.0001 || (!nanok && diff != diff)) {
                ++fails;
                printf("Results disagree.\n");
                printf("    series         (diff)   ti   %s\n", what);
                printf("    series %d, %4d (%f) %.5f %.5f\n", j, i, diff, a, b);
                break;
            }
        }
    }
}




void bench_run(FILE *log, const ti_indicator_info *info, const void *run_info, run_fun fun, const char *name) {

    /* Options storage. */
    /* NB some talib functions take an extra scalar option, so we set the default to 1. */
    TI_REAL options[] = {1,1,1,1,1,1,1,1,1,1};


    /* Some functions need a different option generator strategy.
     * Messy, but can't really be helped. */
    void (*options_setter)(TI_REAL period, TI_REAL *options, int ti) = 0;


    if (strcmp(info->name, "alma") == 0) { options_setter = alma_option_setter; }
    if (strcmp(info->name, "adosc") == 0) { options_setter = fast_slow_option_setter; }
    if (strcmp(info->name, "apo") == 0) { options_setter = ppo_option_setter; }
    if (strcmp(info->name, "bbands") == 0) { options_setter = bbands_option_setter; }
    if (strcmp(info->name, "copp") == 0) { options_setter = copp_option_setter; }
    if (strcmp(info->name, "kc") == 0) { options_setter = kc_option_setter; }
    if (strcmp(info->name, "kst") == 0) { options_setter = kst_option_setter; }
    if (strcmp(info->name, "kvo") == 0) { options_setter = fast_slow_option_setter; }
    if (strcmp(info->name, "macd") == 0) { options_setter = macd_option_setter; }
    if (strcmp(info->name, "mama") == 0) { options_setter = mama_option_setter; }
    if (strcmp(info->name, "pfe") == 0) { options_setter = pfe_option_setter; }
    if (strcmp(info->name, "posc") == 0) { options_setter = posc_option_setter; }
    if (strcmp(info->name, "ppo") == 0) { options_setter = ppo_option_setter; }
    if (strcmp(info->name, "psar") == 0) { options_setter = psar_option_setter; }
    if (strcmp(info->name, "rvi") == 0) { options_setter = rvi_option_setter; }
    if (strcmp(info->name, "rmi") == 0) { options_setter = rmi_option_setter; }
    if (strcmp(info->name, "rmta") == 0) { options_setter = rmta_option_setter; }
    if (strcmp(info->name, "smi") == 0) { options_setter = smi_option_setter; }
    if (strcmp(info->name, "stoch") == 0) { options_setter = stoch_option_setter; }
    if (strcmp(info->name, "stochrsi") == 0) { options_setter = stochrsi_option_setter; }
    if (strcmp(info->name, "ultosc") == 0) { options_setter = ultosc_option_setter; }
    if (strcmp(info->name, "vidya") == 0) { options_setter = vidya_option_setter; }
    if (strcmp(info->name, "vosc") == 0) { options_setter = fast_slow_option_setter; }


    int best_e = 1e9, best_p = -1e9;
    int best;
    for (best = 0; best < bestof; ++best) {
        printf("Benchmark %25s-%-16s ", info->name, name);
        int i, period;
        int iterations = 0;
        const clock_t start = clock();

        fun(run_info, 0, GOAL_SETUP);

        for (i = 0; i < LOOPS; ++i) {
            for (period = PSTART; period <= PEND; ++period) {
                if (options_setter)
                    options_setter(period, options, info == run_info);
                else
                    options[0] = period;

                const int ret = fun(run_info, options, GOAL_RUN);
                if (ret) {
                    return;
                }
                ++iterations;
            }
        }

        fun(run_info, 0, GOAL_FREE);

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

    if (log) fprintf(log, ",\n    \"%s\" => array(\"elapsed\" => %d, \"performance\" => %d)", name, best_e, best_p);
}



void bench_indicator(FILE *log, const ti_indicator_info *info) {

    /* Clear results, so there is no carry over.
     * Also insures that if both fail an equal comparison is impossible. */
    {
        int i;
        for (i = 0; i < MAX_OUT_SERIES; ++i) {
            memset(out[i], 0xF1, sizeof(out[i]));
            memset(outref[i], 0xF2, sizeof(outref[i]));
            memset(outstream1[i], 0xF3, sizeof(outstream1[i]));
            memset(outstreamall[i], 0xF4, sizeof(outstreamall[i]));
#ifdef TA_LIB
            memset(outta[i], 0xF5, sizeof(outta[i]));
#endif
        }
    }


#ifdef TA_LIB
    /* Grab talib function to compare to. */
    const TA_FuncInfo *tainfo = find_ta(info->name);
#endif


    if (info) {
        if (log) fprintf(log, ",\n  \"%s\" => array(\n", info->name);
        if (log) fprintf(log, "    \"name\" => \"%s\"", info->full_name);

        bench_run(log, info, info, run_ti, "ti");
    }

    if (info && info->indicator_ref) {
        bench_run(log, info, info, run_ti_ref, "ti_ref");
        check_outputs(info, "ref", outref);
    }

    if (info && info->stream_new) {
        bench_run(log, info, info, run_ti_stream1, "ti_stream1");
        check_outputs(info, "stream1", outstream1);

        bench_run(log, info, info, run_ti_streamall, "ti_streamall");
        check_outputs(info, "streamall", outstreamall);
    }


#ifdef TA_LIB
    if (tainfo) {

        bench_run(log, info, tainfo, run_ta, "talib");

        if (info && tainfo) {
            /* As sanity check, see that a few values actually match
             * between the libraries.
             * We don't expect the first few values to match, there
             * is not agreement on how or when to initialize many indicators.
             * They should even out and match at the end though. */

            check_outputs(info, "ta", outta);
        }
    }
#endif


    if (log) fprintf(log, "\n  )");
    printf("\n");
}


void bench_run_candle(FILE *log, const tc_candle_info *info) {

    const char* name = "ti";
    tc_set pattern = info->pattern;

    int best_e = 1e9, best_p = -1e9;
    int best;
    for (best = 0; best < bestof; ++best) {
        printf("Benchmark %25s-%-16s ", info->name, name);
        int i, period;
        int iterations = 0;
        const clock_t start = clock();

        const double *inputs[4] = {in[0], in[1], in[2], in[3]};
        tc_config config;
        tc_config_set_to_default(&config);


        for (i = 0; i < LOOPS; ++i) {
            for (period = PSTART; period <= PEND; ++period) {

                const int ret = tc_run(pattern, INSIZE, inputs, &config, outcnd);
                if (ret) {
                    return;
                }
                ++iterations;
            }
        }


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

    if (log) fprintf(log, ",\n    \"%s\" => array(\"elapsed\" => %d, \"performance\" => %d)", name, best_e, best_p);


}


#ifdef TA_LIB

void bench_run_candle_talib(FILE *log, const tc_candle_info *info, const cnd_cross_ref *c) {

    const char* name = "talib";

    int best_e = 1e9, best_p = -1e9;
    int best;
    for (best = 0; best < bestof; ++best) {
        printf("Benchmark %25s-%-16s ", info->name, name);
        int i, period;
        int iterations = 0;
        const clock_t start = clock();

        for (i = 0; i < LOOPS; ++i) {
            for (period = PSTART; period <= PEND; ++period) {

                int count;
                int ret = c->tacdl(0, INSIZE-1, in[0], in[1], in[2], in[3], &ta_offset, &count, outtaint);
                if (ret) {
                    return;
                }
                ++iterations;
            }
        }

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

    if (log) fprintf(log, ",\n    \"%s\" => array(\"elapsed\" => %d, \"performance\" => %d)", name, best_e, best_p);

}

#endif


void bench_candle(FILE *log, const tc_candle_info *info) {

    outcnd = tc_result_new();
    assert(outcnd);

    if (info) {
        if (log) fprintf(log, ",\n  \"%s\" => array(\n", info->name);
        if (log) fprintf(log, "    \"name\" => \"%s\"", info->full_name);

        bench_run_candle(log, info);



#ifdef TA_LIB

    /*See if we have a comparable TA-Lib function.*/
    cnd_cross_ref *c = cnd_cross_refs;
    while (c->pattern && c->pattern != info->pattern) {
        ++c;
    }
    if (c->pattern) {

        memset(outtaint, 0xE1, sizeof(outtaint));
        bench_run_candle_talib(log, info, c);

        /*Check that at least some outputs match.*/

        int matches = 0;
        int tc_extra = 0;
        int talib_extra = 0;

        for (int i = 0; i < tc_result_count(outcnd); ++i) {
            tc_hit h = tc_result_get(outcnd, i);

            int j = h.index - ta_offset;

            if ((c->count == 0 && outtaint[j] != 0)
                    || (c->count == -1 && outtaint[j] < 0)
                    || (c->count == 1 && outtaint[j] > 0)) {
                ++matches;
            } else {
                ++tc_extra;
            }
        }

        /*Now attempt to find extra ta_lib matches.*/
        for (int i = 0; i < INSIZE - ta_offset; ++i) {
            if ((c->count == 0 && outtaint[i] != 0)
                    || (c->count == -1 && outtaint[i] < 0)
                    || (c->count == 1 && outtaint[i] > 0)) {
                if (tc_result_at(outcnd, i + ta_offset) != info->pattern)
                    ++talib_extra;
            }
        }

        printf("\t\t%d matches, %d tc_extra, %d talib_extra\n", matches, tc_extra, talib_extra);
    }

#endif


    tc_result_free(outcnd);

    }





    if (log) fprintf(log, "\n  )");
    printf("\n");
}



int main(int argc, char **argv) {
    printf("Tulip Charts Indicator benchmark.\n");
    printf("Using real size of: %d.\n\n", sizeof(TI_REAL));

    generate_inputs();

#ifdef TA_LIB
    setup_talib();
#endif

    const ti_indicator_info *ind_info = ti_indicators;
    const tc_candle_info *cnd_info = tc_candles;

    const int candles_only = argc > 1 && !strcmp(argv[1], "tc");

    if (argc > 1 && !isdigit(argv[1][0]) && !candles_only) {
        /* Benchmark functions given on command line. */

        if (argc > 2) {
            bestof = atoi(argv[2]);
            if (!bestof) bestof = 1;
        }

        ind_info = ti_find_indicator(argv[1]);
        if (ind_info) bench_indicator(0, ind_info);

        cnd_info = tc_find_candle(argv[1]);
        if (cnd_info) bench_candle(0, cnd_info);

    } else {

        if (argc > 1 && isdigit(argv[1][0])) {
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

        if (!candles_only) {
            /* Benchmark all. */
            do {
                /* Skip indicators that are very slow. */
                if (strcmp(ind_info->name, "msw") == 0) continue;

                bench_indicator(log, ind_info);
            } while ((++ind_info)->name);
        }

        do {
            bench_candle(log, cnd_info);
        } while ((++cnd_info)->name);

        const tc_candle_info cnd_all = {.name = "all", .full_name = "All Candle Patterns", .pattern = TC_ALL};
        bench_candle(log, &cnd_all);

        fprintf(log, ");");

        fclose(log);
    }


    /*
    int i;
    for (i = 0; i < 10; ++i) {
        printf("%.2f %.2f %.2f %.2f %.2f\n", in[0][i], in[1][i], in[2][i], in[3][i], in[4][i]);
    }
    */

    if (!fails) {
        printf("All comparisons passed.\n");
    } else {
        printf("%d comparisons failed.\n", fails);
    }


    return 0;
}

