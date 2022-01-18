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



/*VERSION*/

#include "indicators.h"


const char* ti_version(void) {return TI_VERSION;}
long int ti_build(void) {return TI_BUILD;}
int ti_indicator_count(void) {return TI_INDICATOR_COUNT;}


/*INDICATORS*/



struct ti_stream {
    int index;
    int progress;
};



int ti_stream_run(ti_stream *stream, int size, TI_REAL const *const *inputs, TI_REAL *const *outputs) {
    return ti_indicators[stream->index].stream_run(stream, size, inputs, outputs);
}

ti_indicator_info *ti_stream_get_info(ti_stream *stream) {
    return ti_indicators + stream->index;
}

int ti_stream_get_progress(ti_stream *stream) {
    return stream->progress;
}

void ti_stream_free(ti_stream *stream) {
    ti_indicators[stream->index].stream_free(stream);
}



const ti_indicator_info *ti_find_indicator(const char *name) {
    int imin = 0;
    int imax = sizeof(ti_indicators) / sizeof(ti_indicator_info) - 2;

    /*Binary search.*/
    while (imax >= imin) {
        const int i = (imin + ((imax-imin)/2));
        const int c = strcmp(name, ti_indicators[i].name);
        if (c == 0) {
            return ti_indicators + i;
        } else if (c > 0) {
            imin = i + 1;
        } else {
            imax = i - 1;
        }
    }

    return 0;
}


