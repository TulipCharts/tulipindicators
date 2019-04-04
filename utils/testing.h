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


#ifndef __TESTING_H__
#define __TESTING_H__

#include "../indicators.h"
#include <stdio.h>

int equal_reals(TI_REAL a, TI_REAL b);

int equal_arrays(const TI_REAL *a, const TI_REAL *b, int size_a, int size_b);

void print_array(const TI_REAL *a, int size);

int compare_answers(const ti_indicator_info *info, TI_REAL *answers[], TI_REAL *outputs[], int answer_size, int output_size);

#endif