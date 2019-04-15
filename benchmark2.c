/*
 * Tulip Indicators
 * https://tulipindicators.org/
 * Copyright (c) 2018 Tulip Charts LLC
 * Ilya Pikulin (ilya.pikulin@gmail.com)
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

#include "indicators.h"
#include "utils/testing.h"

#define INSIZE 4000
#define MIN_PERIOD 4
#define MAX_PERIOD 150
#define LOOPS 10

TI_REAL g_inputs[TI_MAXINDPARAMS][INSIZE];

TI_REAL *const open = g_inputs[0];
TI_REAL *const high = g_inputs[1];
TI_REAL *const low = g_inputs[2];
TI_REAL *const close = g_inputs[3];
TI_REAL *const volume = g_inputs[4];

void generate_inputs() {
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

        assert(open[i] <= high[i]);
        assert(close[i] <= high[i]);

        assert(open[i] >= low[i]);
        assert(close[i] >= low[i]);

        assert(high[i] >= low[i]);
    }
}

/************ SPECIAL OPTION SETTERS *************/

void simple_option_setter(double period, double *options) {
    options[0] = period;
}

void macd_option_setter(double period, double *options) {
    options[0] = period;
    options[1] = period + 10;

    options[2] = period + 1;
}

void ppo_option_setter(double period, double *options) {
    options[0] = period;
    options[1] = period + 10;
}

void psar_option_setter(double period, double *options) {
    options[0] = 1/period;
    options[1] = options[0] * 10;
}

void fast_slow_option_setter(double period, double *options) {
    options[0] = period;
    options[1] = period + 10;
}

void stoch_option_setter(double period, double *options) {
    options[0] = period;
    options[1] = 3;
    options[2] = 4;
}

void stochrsi_option_setter(double period, double *options) {
    options[0] = period;
    options[1] = period;
    options[2] = period;
    options[3] = 1;
}

void ultosc_option_setter(double period, double *options) {
    options[0] = period;
    options[1] = period * 2;
    options[2] = period * 4;
}

void vidya_option_setter(double period, double *options) {
    options[0] = period;
    options[1] = period + 10;
    options[2] = .2;
}

void ce_option_setter(double period, double *options) {
    options[0] = period;
    options[1] = 3;
}

void mama_option_setter(double period, double *options) {
    options[0] = 0.5;
    options[1] = 0.05;
}

void bench(const ti_indicator_info *info) {
    void (*options_setter)(double period, double *options) = simple_option_setter;
    if (strcmp(info->name, "apo") == 0) { options_setter = ppo_option_setter; }
    if (strcmp(info->name, "macd") == 0) { options_setter = macd_option_setter; }
    if (strcmp(info->name, "ppo") == 0) { options_setter = ppo_option_setter; }
    if (strcmp(info->name, "psar") == 0) { options_setter = psar_option_setter; }
    if (strcmp(info->name, "adosc") == 0) { options_setter = fast_slow_option_setter; }
    if (strcmp(info->name, "kvo") == 0) { options_setter = fast_slow_option_setter; }
    if (strcmp(info->name, "stoch") == 0) { options_setter = stoch_option_setter; }
    if (strcmp(info->name, "stochrsi") == 0) { options_setter = stochrsi_option_setter; }
    if (strcmp(info->name, "ultosc") == 0) { options_setter = ultosc_option_setter; }
    if (strcmp(info->name, "vosc") == 0) { options_setter = fast_slow_option_setter; }
    if (strcmp(info->name, "vidya") == 0) { options_setter = vidya_option_setter; }
    if (strcmp(info->name, "mama") == 0) { options_setter = mama_option_setter; }

    TI_REAL *inputs[TI_MAXINDPARAMS];
    for (int i = 0; i < info->inputs; ++i) {
        if (strcmp(info->input_names[i], "open") == 0) { inputs[i] = open; }
        else if (strcmp(info->input_names[i], "high") == 0) { inputs[i] = high; }
        else if (strcmp(info->input_names[i], "low") == 0) { inputs[i] = low; }
        else if (strcmp(info->input_names[i], "close") == 0) { inputs[i] = close; }
        else if (strcmp(info->input_names[i], "real") == 0) { inputs[i] = close; }
        else if (strcmp(info->input_names[i], "volume") == 0) { inputs[i] = volume; }
        else { assert(0 && "unsupported input"); };
    }

    int elapsed_plain = 0;
    int elapsed_ref = 0;
    int elapsed_stream_1 = 0;
    int elapsed_stream_all = 0;


    for (int i = 0; i < LOOPS; ++i) {
        for (int period = MIN_PERIOD; period <= MAX_PERIOD; ++period) {
            TI_REAL outputs_mem[4][TI_MAXINDPARAMS][INSIZE];
            TI_REAL options[TI_MAXINDPARAMS];

            TI_REAL *outputs[TI_MAXINDPARAMS];
            TI_REAL *outputs_ref[TI_MAXINDPARAMS];
            TI_REAL *outputs_stream_all[TI_MAXINDPARAMS];
            TI_REAL *outputs_stream_1[TI_MAXINDPARAMS];
            for (int i = 0; i < info->outputs; ++i) {
                outputs[i] = outputs_mem[0][i];
                outputs_ref[i] = outputs_mem[1][i];
                outputs_stream_all[i] = outputs_mem[2][i];
                outputs_stream_1[i] = outputs_mem[3][i];
            }
            options_setter(period, options);

            const int OUTSIZE = INSIZE - info->start(options);

            clock_t start_ts, end_ts;

            {
                start_ts = clock();
                const int ret = info->indicator(INSIZE, inputs, options, outputs);
                end_ts = clock();
                elapsed_plain += end_ts - start_ts;
                if (ret != TI_OKAY) {
                    printf("%s returned %i, exiting\n", info->name, ret);
                    exit(2);
                }
            }

            if (info->indicator_ref) {
                start_ts = clock();
                const int ret = info->indicator_ref(INSIZE, inputs, options, outputs_ref);
                end_ts = clock();
                elapsed_ref += end_ts - start_ts;
                if (ret != TI_OKAY) {
                    printf("%s_ref returned %i, exiting\n", info->name, ret);
                    exit(2);
                }
                int ok = !compare_answers(info, outputs, outputs_ref, OUTSIZE, OUTSIZE);
                if (!ok) { printf("%s_ref mismatched, exiting\n", info->name); exit(1); }
            }

            if (info->stream_new) {
                ti_stream *stream;
                const int ret = info->stream_new(options, &stream);
                start_ts = clock();
                info->stream_run(stream, INSIZE, inputs, outputs_stream_all);
                end_ts = clock();
                info->stream_free(stream);
                elapsed_stream_all += end_ts - start_ts;
                if (ret != TI_OKAY) {
                    printf("%s_stream_new returned %i, exiting\n", info->name, ret);
                    exit(2);
                }
                int ok = !compare_answers(info, outputs, outputs_stream_all, OUTSIZE, OUTSIZE);
                if (!ok) { printf("%s_stream_all mismatched, exiting\n", info->name); exit(1); }
            }

            if (info->stream_new) {
                ti_stream *stream;
                const int ret = info->stream_new(options, &stream);
                TI_REAL *inputs_[TI_MAXINDPARAMS] = {0};
                TI_REAL *outputs_[TI_MAXINDPARAMS] = {0};
                start_ts = clock();
                for (int bar = 0; bar < INSIZE; ++bar) {
                    for (int j = 0; j < info->inputs; ++j) {
                        inputs_[j] = inputs[j] + bar;
                    }
                    for (int j = 0; j < info->outputs; ++j) {
                        outputs_[j] = outputs_stream_1[j] + ti_stream_get_progress(stream);
                    }
                    const int ret = info->stream_run(stream, 1, inputs_, outputs_);
                    if (ret != TI_OKAY) {
                        printf("%s_stream_new returned %i, exiting\n", info->name);
                        exit(2);
                    }
                }
                end_ts = clock();
                elapsed_stream_1 += end_ts - start_ts;
                int ok = !compare_answers(info, outputs, outputs_stream_1, OUTSIZE, OUTSIZE);
                if (!ok) { printf("%s_stream_1 mismatched, exiting\n", info->name); exit(1); }
                info->stream_free(stream);
            }
        }
    }

    #define MS(elapsed) (int)(elapsed * 1000. / CLOCKS_PER_SEC)
    const int iterations = LOOPS * (MAX_PERIOD - MIN_PERIOD + 1);
    #define PERFORMANCE(elapsed) (int)(0 ? 0 : (iterations * INSIZE) / MS(elapsed) / 1000.)

    // mfps = million floats per second
    printf("Benchmark %15s%s\t%5dms\t%5dmfps\n", info->name, "           ", MS(elapsed_plain), PERFORMANCE(elapsed_plain));
    if (info->indicator_ref) { printf("Benchmark %15s%s\t%5dms\t%5dmfps\n", info->name, "_ref       ", MS(elapsed_ref), PERFORMANCE(elapsed_ref)); }
    if (info->stream_new) { printf("Benchmark %15s%s\t%5dms\t%5dmfps\n", info->name,    "_stream_all", MS(elapsed_stream_all), PERFORMANCE(elapsed_stream_all)); }
    if (info->stream_new) { printf("Benchmark %15s%s\t%5dms\t%5dmfps\n", info->name,    "_stream_1  ", MS(elapsed_stream_1), PERFORMANCE(elapsed_stream_1)); }
}

int main(int argc, char** argv) {
    generate_inputs();

    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            ti_indicator_info *info = ti_find_indicator(argv[i]);
            if (!info) {
                printf("indicator %s not found\n", argv[i]);
                exit(3);
            }
            bench(info);
        }
    } else {
        ti_indicator_info *info = ti_indicators;
        for (int i = 0; i < TI_INDICATOR_COUNT; ++i) {
            bench(info + i);
        }
    }
}
