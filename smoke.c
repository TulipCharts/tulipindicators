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

#include "indicators.h"
#include "candles.h"
#include "utils/minctest.h"
#include "utils/buffer.h"
#include "utils/localbuffer.h"
#include <string.h>
#include <stdlib.h>

#define maxline 4096
#define maxarray 1024

int tested_ind[TI_INDICATOR_COUNT] = {0};
int tested_cnd[TC_CANDLE_COUNT] = {0};

/* Compare two arrays, assert each element is equal. */
void compare_arrays(TI_REAL *a, TI_REAL *b, int size_a, int size_b) {
    int i;

    lequal(size_a, size_b);
    if (size_a != size_b) {
        printf("Size mismatch.\n");
    }

    const int f = lfails;

    for (i = 0; i < size_a; ++i) {
        lfequal(a[i], b[i]);
    }

    if (lfails != f) {
        printf("Expected      Got\n");
        for (i = 0; i < size_a; ++i) {
            printf("%8.3f %8.3f\n", a[i], b[i]);
        }
    }
}


/*Return next non-comment, non-blank line.*/
char *next_line(FILE *fp) {
    static char buf[maxline];
    while (fgets(buf, maxline, fp)) {
        /*Skip Comments*/
        if (buf[0] == '#') continue;

        /*Skip blank lines*/
        if (buf[0] == 10 || buf[0] == 13) continue;

        return buf;
    }

    return 0;
}


/*Read in float array.*/
int get_array(FILE *fp, TI_REAL *s) {
    char *line = next_line(fp);

    if (line[0] != '{') {
        printf("Bad input\n");
        lok(0);
        return 0;
    }

//#pragma warning(disable:4996) //MSVC
    char *num = strtok(line+1, ",}\r\n");

    if (!num) {
        return 0;
    }

    TI_REAL *inp = s;

    do {
        *inp = atof(num);
        ++inp;
    } while ((num = strtok(0, ",}\r\n")));

    return (int)(inp - s);
}

/*Read in options, inputs, answers, and test.*/
void test_ind_name(FILE *fp, const char *name, int count) {

    /*Find indicator from name.*/
    const ti_indicator_info *info = ti_find_indicator(name);

    if (!info) {
        printf("(WARN) Couldn't find indicator %s\n", name);
        printf("---------------------------------------------");

        //Skip to next indicator
        static char buf[maxline];
        while (fgets(buf, maxline, fp) && (buf[0] == '{' || buf[0] == '#'));

        return;
    }


    const int ind_num = (int)(info - ti_indicators);
    if (count)
        tested_ind[ind_num] = 1;


    TI_REAL options[TI_MAXINDPARAMS];
    TI_REAL *o = options;
    const char *s;
    while((s = strtok(0, " \n\r"))) {
        *o = atof(s);
        ++o;
    }

    lequal((int)(o-options), info->options);

    int i;

    TI_REAL *inputs[TI_MAXINDPARAMS] = {0};
    TI_REAL *answers[TI_MAXINDPARAMS] = {0};
    TI_REAL *outputs[TI_MAXINDPARAMS] = {0};

    int input_size = 0;
    for (i = 0; i < info->inputs; ++i) {
        inputs[i] = malloc(sizeof(TI_REAL) * maxarray);
        input_size = get_array(fp, inputs[i]);
    }

    int answer_size = 0;
    for (i = 0; i < info->outputs; ++i) {
        answers[i] = malloc(sizeof(TI_REAL) * maxarray);
        outputs[i] = malloc(sizeof(TI_REAL) * maxarray);
        answer_size = get_array(fp, answers[i]);
    }

    {
        const int ret = info->indicator(input_size, (TI_REAL const *const *)inputs, options, outputs);
        lok(ret == TI_OKAY);

        int output_size = input_size - info->start(options);
        if (output_size < 0) output_size = 0;
        for (i = 0; i < info->outputs; ++i) {
            compare_arrays(answers[i], outputs[i], answer_size, output_size);
        }
    }

    if (info->indicator_ref) {

        for (i = 0; i < info->outputs; ++i) {
            memset(outputs[i], 0, sizeof(TI_REAL) * maxarray);
        }

        const int ret = info->indicator_ref(input_size, (TI_REAL const *const *)inputs, options, outputs);
        lok(ret == TI_OKAY);

        int output_size = input_size - info->start(options);
        if (output_size < 0) output_size = 0;
        for (i = 0; i < info->outputs; ++i) {
            compare_arrays(answers[i], outputs[i], answer_size, output_size);
        }
    }

    if (info->stream_new) {

        const int steps[] = {1, 2, 3, 4, 5, 7, 13, 100, maxarray};
        const int step_count = (sizeof(steps)/sizeof(int));

        for (int step_i = 0; step_i < step_count; ++step_i) {
            const int step = steps[step_i];

            //Clear outputs
            for (i = 0; i < info->outputs; ++i) {
                memset(outputs[i], 0, sizeof(TI_REAL) * maxarray);
            }

            //Create stream
            ti_stream *stream = 0;
            const int stream_ret = info->stream_new(options, &stream);
            lok(stream_ret == TI_OKAY);
            lok(stream != 0);

            for (int bar = 0; bar < input_size; bar += step) {

                TI_REAL *ins[TI_MAXINDPARAMS] = {0};
                TI_REAL *outs[TI_MAXINDPARAMS] = {0};

                for (i = 0; i < info->inputs; ++i) {
                    ins[i] = inputs[i] + bar;
                }

                for (i = 0; i < info->outputs; ++i) {
                    int progress = ti_stream_get_progress(stream);
                    if (progress < 0) progress = 0;
                    outs[i] = outputs[i] + progress;
                }

                int stride = step;
                if (stride + bar >= input_size) stride = input_size - bar;
                const int ret = info->stream_run(stream, stride, (const double * const*)ins, outs);
                lok(ret == TI_OKAY);
            }

            int output_size = input_size - info->start(options);
            if (output_size < 0) output_size = 0;
            lequal(ti_stream_get_progress(stream), output_size);
            for (i = 0; i < info->outputs; ++i) {
                compare_arrays(answers[i], outputs[i], answer_size, output_size);
            }

            info->stream_free(stream);
        }
    }

    for (i = 0; i < info->inputs; ++i) free(inputs[i]);
    for (i = 0; i < info->outputs; ++i) free(answers[i]);
    for (i = 0; i < info->outputs; ++i) free(outputs[i]);

}



/*Read in inputs, answers, and test.*/
void test_candles(FILE *fp, int count) {

    TI_REAL *inputs[TI_MAXINDPARAMS] = {0};

    int input_size = 0;
    for (int i = 0; i < 4; ++i) {
        inputs[i] = malloc(sizeof(TI_REAL) * maxarray);
        input_size = get_array(fp, inputs[i]);
    }


    tc_config config;
    tc_config_set_to_default(&config);

    tc_result *output = tc_result_new();

    tc_run(TC_ALL, input_size, (TC_REAL const *const *)inputs, &config, output);



    static char buf[maxline];
    do {
        if (feof(fp)) break;

        fgets(buf, maxline, fp);
        if (buf[0] != '!' && (buf[0] < 'a' || buf[0] > 'z')) break;

        char *name = strtok(buf, " ");

        int negate = 0;
        if (buf[0] == '!') {
            negate = 1;
            ++name;
        }


        const tc_candle_info *info = tc_find_candle(name);
        lok(info);

        char *num;
        while ((num = strtok(0, " "))) {
            const int pos = atoi(num);

            //printf("\n%s %d\n", name, pos);
            if (negate)
                lok(!TC_SET_CHECK(tc_result_at(output, pos), info->pattern));
            else
                lok(TC_SET_CHECK(tc_result_at(output, pos), info->pattern));
        }

        const int cnd_num = (int)(info - tc_candles);
        if (count)
            tested_cnd[cnd_num] = 1;

    } while (1);





    tc_result_free(output);
}



void test(const char *fname, int count) {
    /*Read in test values from external file.*/
    printf("%s:\n", fname);
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        lok(0);
        return;
    }

    char *line;

    while ((line = next_line(fp))) {

        /*Looking for function name.*/
        if (line[0] < 'a' || line[0] > 'z') {
            lok(0);
            continue;
        }

        /*Found function*/
        char *name = strtok(line, " \n\r");

        if (!strcmp(name, "candle")) {
            lrun(name, test_candles(fp, count));
        } else {
            lrun(name, test_ind_name(fp, name, count));
        }
    }

    fclose(fp);
}


void test_version() {
    lok(strcmp(TI_VERSION, ti_version()) == 0);
    lok(TI_BUILD == ti_build());
    lok(TI_INDICATOR_COUNT == ti_indicator_count());

    lok(strcmp(TC_VERSION, tc_version()) == 0);
    lok(TC_BUILD == tc_build());
    lok(TC_CANDLE_COUNT == tc_candle_count());

    lok(strcmp(TI_VERSION, TC_VERSION) == 0);
    lequal(TI_BUILD, TC_BUILD);
}


void test_buffer() {
    ti_buffer *b = ti_buffer_new(3);
    ti_buffer_push(b, 5.0); lfequal(b->sum, 5.0);
    ti_buffer_push(b, 5.0); lfequal(b->sum, 10.0);
    ti_buffer_push(b, 1.0); lfequal(b->sum, 11.0);
    ti_buffer_push(b, 1.0); lfequal(b->sum, 7.0);
    ti_buffer_push(b, 3.0); lfequal(b->sum, 5.0);
    ti_buffer_push(b, 1.0); lfequal(b->sum, 5.0);
    ti_buffer_push(b, 2.0); lfequal(b->sum, 6.0);
    ti_buffer_push(b, 3.0); lfequal(b->sum, 6.0);

    lfequal(ti_buffer_get(b, 0), 3.0);
    lfequal(ti_buffer_get(b, -1), 2.0);
    lfequal(ti_buffer_get(b, -2), 1.0);
    lfequal(ti_buffer_get(b, -3), 3.0);


    ti_buffer_free(b);
}


void test_localbuffer() {
    //TODO localbuffer needs much more testing

    typedef struct s {
        BUFFERS(
            BUFFER(a)
        )

        TI_REAL padding[5];
    } s;

    //TODO why does it break without the = {0}
    s stream = {0};

    BUFFER_INIT(&stream, a, 5);

    lequal(BUFFERS_SIZE(&stream), 5);

    BUFFER_PUSH(&stream, a, 100);
    TI_REAL get;
    BUFFER_AT(get, &stream, a, 0);
    lfequal(get, 100);

    //TODO more testing

}


int main() {

    printf("TI TEST SUITE\n");
    lrun("buffer", test_buffer());
    lrun("localbuffer", test_localbuffer());
    lrun("version", test_version());
    test("tests/untest.txt", 0);
    test("tests/atoz.txt", 1);
    test("tests/extra.txt", 1);
    test("tests/candles.txt", 1);


    for (int i = 0; i < TI_INDICATOR_COUNT; ++i) {
        if (!tested_ind[i]) {
            printf("WARNING: no test for %s\n", ti_indicators[i].name);
        }
    }
    for (int i = 0; i < TC_CANDLE_COUNT; ++i) {
        if (!tested_cnd[i]) {
            printf("WARNING: no test for %s\n", tc_candles[i].name);
        }
    }



    if (lfails == 0) {
        printf("ALL TESTS PASSED (%d/%d)\n", ltests, ltests);
    } else {
        printf("%d TESTS FAILED (of %d)\n", lfails, ltests);
    }

    return lfails != 0;
}

