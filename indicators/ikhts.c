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



int ti_ikhts_start(TI_REAL const *options) {
      return (int)options[0] - 1;
}


int ti_ikhts(int size,
      TI_REAL const *const *inputs,
      TI_REAL const *options,
      TI_REAL *const *outputs) {
      TI_REAL *output = outputs[0];

      const int out_size = size - ti_max_start(options);

      TI_REAL *buff = malloc(sizeof(TI_REAL) * (unsigned long) out_size * 2UL);
      TI_REAL *max = &buff[0];
      TI_REAL *min = &buff[out_size];
      TI_REAL *tmp_outs[1];
      tmp_outs[0] = max;
      
      int ret = ti_max(size, &inputs[0], options, tmp_outs);
      assert(ret == TI_OKAY);

      tmp_outs[0] = min;
      ret = ti_min(size, &inputs[1], options, tmp_outs);
      assert(ret == TI_OKAY);

      
      int i;
      for (i = 0; i < out_size; ++i) {
            *output++ = (*max++ + *min++)/2.0;
      }
      
      free(buff);
      assert(output - outputs[0] == size - ti_ikhts_start(options));
      return TI_OKAY;

}
