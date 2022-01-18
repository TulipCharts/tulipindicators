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



//This allows some indicators to be run from the command line.
//It's mostly intended to facilitate testing.

//TODO This utility isn't memory safe. Currently for testing only!


#include "indicators.h"
#include "candles.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define maxarray 8192
#define maxline 131072


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
void run(const char *name, const char *in, const char *out) {

    /*Find indicator from name.*/
    const ti_indicator_info *info = ti_find_indicator(name);

    if (!info) {
        fprintf(stderr, "(ERROR) Couldn't find indicator %s\n", name);
        return;
    }

    FILE *fin = fopen(in, "r");
    if (!fin) {
        fprintf(stderr, "(ERROR) Couldn't open %s for input.\n", in);
        return;
    }

    char *line = next_line(fin);

    TI_REAL options[TI_MAXINDPARAMS];
    TI_REAL *o = options;
    const char *s = strtok(line, " \n\r");
    if (s) {
        do {
            *o = atof(s);
            ++o;
        } while ((s = strtok(0, " \n\r")));
    }

    if ((int)(o-options) != info->options) {
        fprintf(stderr, "(ERROR) Invalid number of options for %s. Expected %d, got %d\n", name, info->options, (int)(o-options));
        return;
    }

    int i;

    TI_REAL *inputs[TI_MAXINDPARAMS] = {0};
    TI_REAL *outputs[TI_MAXINDPARAMS] = {0};

    int input_size = 0;
    for (i = 0; i < info->inputs; ++i) {
        inputs[i] = malloc(sizeof(TI_REAL) * maxarray);
        input_size = get_array(fin, inputs[i]);
    }
    fclose(fin);

    for (i = 0; i < info->outputs; ++i) {
        outputs[i] = malloc(sizeof(TI_REAL) * maxarray);
    }

    {
        const int ret = info->indicator(input_size, (TI_REAL const *const *)inputs, options, outputs);
        if (ret != TI_OKAY) {
            fprintf(stderr, "(ERROR) Return value for %s of %d.\n", name, ret);
            return;
        }
    }



    FILE *fout = fopen(out, "w");
    if (!fout) {
        fprintf(stderr, "(ERROR) Couldn't open %s for output.\n", out);
        return;
    }

    const int out_size = input_size - info->start(options);
    for (i = 0; i < info->outputs; ++i) {
        fprintf(fout, "{");
        for (int j = 0; j < out_size; ++j) {
            fprintf(fout, "%f%s", outputs[i][j], j == out_size-1 ? "" : ",");
        }
        fprintf(fout, "}\n");
    }

    fclose(fout);



    for (i = 0; i < info->inputs; ++i) free(inputs[i]);
    for (i = 0; i < info->outputs; ++i) free(outputs[i]);
}



int main(int argc, char *argv[]) {

    if (argc < 4) {
        printf("Usage: cli indicator_name infile outfile\n");
    }

    const char *name = argv[1];
    const char *in = argv[2];
    const char *out = argv[3];

    run(name, in, out);

    return 0;
}

