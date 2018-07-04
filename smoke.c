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

#include "utils/minctest.h"
#include "indicators.h"
#include "utils/buffer.h"
#include <string.h>
#include <stdlib.h>

int tested[TI_INDICATOR_COUNT] = {0};

/* Compare two arrays, assert each element is equal. */
void compare_arrays(TI_REAL *a, TI_REAL *b, int size_a, int size_b) {
    int i;

    lequal(size_a, size_b);
    if (size_a != size_b) {
        printf("Size mismatch.\n");
    }

    for (i = 0; i < size_a; ++i) {
        lfequal(a[i], b[i]);
    }
}


/*Return next non-comment, non-blank line.*/
char *next_line(FILE *fp) {
    static char buf[1024];
    while (fgets(buf, 1024, fp)) {
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
        printf("Couldn't find indicator %s\n", name);
        lok(0);
        return;
    }


    const int ind_num = (int)(info - ti_indicators);
    if (count)
        tested[ind_num] = 1;


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
        inputs[i] = malloc(sizeof(TI_REAL) * 512);
        input_size = get_array(fp, inputs[i]);
    }

    int answer_size = 0;
    for (i = 0; i < info->outputs; ++i) {
        answers[i] = malloc(sizeof(TI_REAL) * 512);
        outputs[i] = malloc(sizeof(TI_REAL) * 512);
        answer_size = get_array(fp, answers[i]);
    }

    const int ret = info->indicator(input_size, (TI_REAL const *const *)inputs, options, outputs);
    lok(ret == TI_OKAY);

    int output_size = input_size - info->start(options);
    if (output_size < 0) output_size = 0;
    for (i = 0; i < info->outputs; ++i) {
        compare_arrays(answers[i], outputs[i], answer_size, output_size);
    }

    for (i = 0; i < info->inputs; ++i) free(inputs[i]);
    for (i = 0; i < info->outputs; ++i) free(answers[i]);
    for (i = 0; i < info->outputs; ++i) free(outputs[i]);

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
        lrun(name, test_ind_name(fp, name, count));
    }

    fclose(fp);
}


void test_version() {
    lok(strcmp(TI_VERSION, ti_version()) == 0);
    lok(TI_BUILD == ti_build());
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


int main() {

    printf("TI TEST SUITE\n");
    lrun("buffer", test_buffer());
    lrun("version", test_version());
    test("tests/untest.txt", 0);
    test("tests/atoz.txt", 1);
    test("tests/extra.txt", 1);


    int i;
    for (i = 0; i < TI_INDICATOR_COUNT; ++i) {
        if (!tested[i]) {
            printf("WARNING: no test for %s\n", ti_indicators[i].name);
        }
    }



    if (lfails == 0) {
        printf("ALL TESTS PASSED (%d/%d)\n", ltests, ltests);
    } else {
        printf("%d TESTS FAILED (of %d)\n", lfails, ltests);
    }

    return lfails != 0;
}

