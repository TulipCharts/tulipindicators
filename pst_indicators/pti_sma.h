#pragma once
#include "pst_interface.h"
#include <vector>
#include <stdio.h>

class pti_sma : public pst_interface {
public:
    pti_sma(const ti_indicator_info* ti_info, TI_REAL const *options)
        : pst_interface(ti_info, options)
        , period_((int)options[0]) {
        printf("---> %s, period_=%d\n", __FUNCTION__, period_);
    }
    
    virtual int update(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) override {
        printf("---> %s, period_=%d\n", __FUNCTION__, period_);
        const TI_REAL *input = inputs[0];
        const int period = period_; // for pst
        TI_REAL *output = outputs[0];
        const TI_REAL scale = 1.0 / period;

        if (period < 1) return TI_INVALID_OPTION;
        if (size <= start_) return TI_OKAY; // for pst

        TI_REAL sum = 0;

        int i;
        for (i = 0; i < period; ++i) {
            sum += input[i];
        }

        *output++ = sum * scale;

        for (i = period; i < size; ++i) {
            sum += input[i];
            sum -= input[i-period];
            *output++ = sum * scale;
        }

        assert(output - outputs[0] == size - start_); // for pst
        return TI_OKAY;
        // return 1129;
    }

    virtual ~pti_sma() override {

    }

private:
    const int period_;
    typedef std::vector<double> DOUBLE_ARRAY;
    DOUBLE_ARRAY input_;
    DOUBLE_ARRAY output_;
};