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

#include "../indicators.h"

/* 
 * Implementation of Arnaud Legoux Moving Average
 * Originally created by Arnaud Legoux, www.arnaudlegoux.com
 * C implementation for Tulip Indicators by Johan Henriksson, johan@stackpoint.se
 */

int ti_alma_start(TI_REAL const *options) {
    const int window = (int)options[0];
    return window - 1;
}

int ti_alma(int size, TI_REAL const * const *inputs, TI_REAL const *options, TI_REAL * const *outputs) {
    const TI_REAL *input = inputs[0];
    const int window = (int)options[0];
    const TI_REAL offset = options[1];
    const TI_REAL sigma = options[2];

    int i, j;
    TI_REAL *output = outputs[0];

    // validate options
    if (window < 1) return TI_INVALID_OPTION;
    if (sigma <= 0) return TI_INVALID_OPTION;
    if ((offset < 0) || (offset > 1)) return TI_INVALID_OPTION;

    if (size <= ti_sma_start(options)) return TI_OKAY;

    // precompute weights
    TI_REAL *weights = (TI_REAL*)malloc(sizeof(TI_REAL) * (size_t)window);
    assert(weights != NULL); // break tests
    if (weights == NULL) {
        // todo: out of memory error code. 
        // for now it'll be considered a too large window size
        return TI_INVALID_OPTION;
    }

    const TI_REAL m = floor(offset * (window - 1));
    const TI_REAL s = window / sigma;
    TI_REAL norm = 0;
    for(i = 0; i < window; i++) {
        weights[i] = exp(-1 * pow(i - m, 2) / (2 * pow(s, 2))); // guassian weight
        norm += weights[i]; // aggregate sum for normalization
    }
    for(i = 0; i < window; i++) {
        weights[i] /= norm; // normalize weights
    }

    // compute moving average
    for(i = window-1; i < size; i++) {
        TI_REAL sum = 0;
        for (j = 0; j < window; j++) {
            sum += input[i - window + j + 1] * weights[j];
        }
        *output++ = sum;
    }
    
    // free computed weights
    free(weights);

    assert(output - outputs[0] == size - ti_sma_start(options));
    return TI_OKAY;
}
