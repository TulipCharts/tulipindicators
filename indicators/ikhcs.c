/*
 * Tulip Indicators
 * https://tulipindicators.org/
 * Copyright (c) 2010-2018 Tulip Charts LLC
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



int ti_ikhcs_start(TI_REAL const *options) {
      return (int)options[0] - 1;
}


int ti_ikhcs(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs) {

      TI_REAL *output = outputs[0];
      
      int start = ti_max_start(options);
      const int out_size = size - start;
      TI_REAL *input = (double*)inputs[0] + start;

      int i;
      for(i = 0; i < out_size; ++i) {
            *output++ = *input++;
      }

      assert(output - outputs[0] == size - ti_ikhcs_start(options));
      return TI_OKAY;

}
