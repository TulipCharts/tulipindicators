//
// Created by dewe on 3/21/23.
//
#include "../indicators.h"
#include <math.h>
#include "assert.h"
#include "string.h"

//
//int ti_shift_start(TI_REAL const *options) {
//    return (int)fabs(options[0]);
//}
//
//int ti_shift(int size, TI_REAL const * const *inputs, TI_REAL const *options, TI_REAL * const *outputs) {
//    const TI_REAL *input = inputs[0];
//    TI_REAL *output = outputs[0];
//    const int shift = (int)options[0];
//
//    if (size <= ti_shift_start(options)) {
//        return TI_OKAY;
//    }
//
//    if (shift < 0) {
//        memcpy(output, input, sizeof(TI_REAL) * (size + shift));
//    } else {
//        memcpy(output + shift, input, sizeof(TI_REAL) * (size - shift));
//    }
//
//    assert(output - outputs[0] == size - ti_shift_start(options));
//    return TI_OKAY;
//}
