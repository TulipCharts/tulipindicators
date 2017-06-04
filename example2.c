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


#include "indicators.h"
#include <stdio.h>
#include <stdlib.h>


int main()
{
    printf("This program is an example of looping through\n");
    printf("each of the available indicators.\n\n");

    /* Set info to first indicators in array. */
    const ti_indicator_info *info = ti_indicators;

    /* The last item is all zeros, so we'll stop when we get there. */
    while (info->name != 0) {
        int i;

        printf("%s (%s) has type %d with: %d inputs, %d options, %d outputs.\n",
                info->name,
                info->full_name,
                info->type,
                info->inputs,
                info->options,
                info->outputs
              );


        printf("   inputs: ");
        for (i = 0; i < info->inputs; ++i)
            printf("%s%s", i ? ", " : "", info->input_names[i]);
        printf("\n");

        printf("   options: ");
        for (i = 0; i < info->options; ++i)
            printf("%s%s", i ? ", " : "", info->option_names[i]);
        printf("\n");

        printf("   outputs: ");
        for (i = 0; i < info->outputs; ++i)
            printf("%s%s", i ? ", " : "", info->output_names[i]);
        printf("\n");


        /* To use this indicator, call the start function like:
         * info->start(options);
         * and then call the actual indicator function like:
         * info->indicator(size, inputs, options, outputs);
         */



        printf("\n");

        ++info; /* Next indicator. */
    }

    return 0;
}
