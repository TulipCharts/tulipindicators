#pragma once
#include "pst_interface.h"
#include <vector>
#include <stdio.h>
#include <assert.h>

class pti_rsi : public pst_interface {
public:
    pti_rsi(const ti_indicator_info* ti_info, TI_REAL const *options)
        : pst_interface(ti_info, options)
        , period_((int)options[0]) {
    }
    
    virtual int update(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) override {
        assert(size > 0);
        const TI_REAL *input = inputs[0];
        TI_REAL *output = outputs[0];
        for (int i=0; i<size; ++i) {
            output[i] = update_one(input[i]);
        }
        return TI_OKAY;
    }

private:
    TI_REAL update_one(TI_REAL input) {
        if (input_.empty()) { // 0
            TI_REAL output = TI_NULL_VAL;
            input_.push_back(input);
            smooth_up_.push_back(0);
            smooth_down_.push_back(0);
            output_.push_back(output);  
            return output;
        }
        else {
            int sz = input_.size();
            int end = sz-1;
            TI_REAL per = 1.0 / ((TI_REAL)period_);
            TI_REAL upward = input > input_[end] ? input - input_[end] : 0;
            TI_REAL downward = input < input_[end] ? input_[end] - input: 0;
            TI_REAL smooth_up, smooth_down, output;
            if (sz < period_) { // sum    
                smooth_up = smooth_up_[end] + upward;
                smooth_down = smooth_down_[end] + downward;
                output = TI_NULL_VAL;
            }
            else if (sz == period_) { // avg
                smooth_up = (smooth_up_[end] + upward) * per;
                smooth_down = (smooth_down_[end] + downward) * per;
                output = 100.0 * (smooth_up / (smooth_up + smooth_down));
            }
            else { // move avg
                smooth_up = (upward-smooth_up_[end]) * per + smooth_up_[end]; 
                smooth_down = (downward-smooth_down_[end]) * per + smooth_down_[end];
                output = 100.0 * (smooth_up / (smooth_up + smooth_down));
            }
            input_.push_back(input);
            smooth_up_.push_back(smooth_up); 
            smooth_down_.push_back(smooth_down);
            output_.push_back(output);
            return output;
        }
    }

    virtual ~pti_rsi() override {
    }

private:
    const int period_;
    typedef std::vector<double> DOUBLE_ARRAY;
    DOUBLE_ARRAY input_;
    DOUBLE_ARRAY smooth_up_;
    DOUBLE_ARRAY smooth_down_;
    DOUBLE_ARRAY output_;

};

/*
    // MEMO: easy to port from ti, but error.
    
    // int update_batch(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    //     assert(size > 0);
    //     const TI_REAL *input = inputs[0];
    //     TI_REAL *output = outputs[0];

    //     
    //     //|---------------start_zone-------------------|
    //     //|---pre_input_size---|---start_remain--------|
    //     //            #case1   |---input_in_start---|
    //     //                     |---input_size-------|
    //     //            #case2   |---input_in_start------|
    //     //                     |---input_size----------|
    //     //            #case3   |---input_in_start------|---input_out_start---|
    //     //                     |------------------input_size-----------------|

    //     //|---------------start_zone-------------------|
    //     //|---------------pre_input_size---------------|
    //     //            #case4                           |---input_out_start---|
    //     //                                            |---input_size--------|

    //     //|---------------start_zone-------------------|
    //     //|---------------pre_input_size----------------------|
    //     //            #case5                                  |---input_out_start---|
    //     //                                                    |---input_size--------|
    //     

    //     int pre_input_size = input_.size();
    //     int start_remain = (start_ > pre_input_size) ? (start_ - input_.size()) : 0;
    //     int input_out_start = (size > start_remain) ? (size - start_remain) : 0;
    //     int input_in_start = size - input_out_start;

    //     assert(start_ <= pre_input_size + start_remain);
    //     assert(start_remain >= 0);
    //     assert(start_remain >= input_in_start);
    //     assert(input_in_start >= 0);
    //     assert(input_out_start >= 0);
    //     assert(input_in_start + input_out_start == size);

    //     // some of input in start zone;
    //     if (input_in_start > 0) {
    //         input_.insert(input_.end(), input, input+input_in_start);
    //         output_.insert(output_.end(), input_in_start, TI_NULL_VAL);
    //         for (int i=0; i<input_in_start; ++i) {
    //             output[i] = TI_NULL_VAL;
    //         }
    //     }

    //     // some of input out of start zone
    //     if (input_out_start > 0) {
    //         input += input_in_start;
    //         output += input_in_start;

    //         input_.insert(input_.end(), input, input+input_out_start);
    //         int raw_size = period_+ input_out_start;

    //         TI_REAL const *raw_input = input_.data() + pre_input_size + input_in_start - period_;
    //         TI_REAL const *raw_inputs[] = { raw_input };
    //         TI_REAL *raw_outputs[] = { output };

    //         int ti_result = raw_update(raw_size, raw_inputs, 0, raw_outputs);
    //         if (TI_OKAY == ti_result) {
    //             output_.insert(output_.end(), output, output+input_out_start);
    //         }
    //         return ti_result;
    //     }
    //     return TI_OKAY;
    // }

    // int raw_update(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    //     const TI_REAL *input = inputs[0];
    //     const int period = period_; // for pst
    //     TI_REAL *output = outputs[0];
    //     const TI_REAL per = 1.0 / ((TI_REAL)period);

    //     if (period < 1) return TI_INVALID_OPTION;

    //     // not enough inputs
    //     if (size <= start_) return TI_OKAY; // for pst

    //     TI_REAL smooth_up = 0, smooth_down = 0;

    //     int i;
    //     for (i = 1; i <= period; ++i) {
    //         const TI_REAL upward = input[i] > input[i-1] ? input[i] - input[i-1] : 0;
    //         const TI_REAL downward = input[i] < input[i-1] ? input[i-1] - input[i] : 0;
    //         smooth_up += upward;
    //         smooth_down += downward;
    //     }

    //     smooth_up /= period;
    //     smooth_down /= period;
    //     *output++ = 100.0 * (smooth_up / (smooth_up + smooth_down));

    //     for (i = period+1; i < size; ++i) {
    //         const TI_REAL upward = input[i] > input[i-1] ? input[i] - input[i-1] : 0;
    //         const TI_REAL downward = input[i] < input[i-1] ? input[i-1] - input[i] : 0;

    //         smooth_up = (upward-smooth_up) * per + smooth_up;
    //         smooth_down = (downward-smooth_down) * per + smooth_down;

    //         *output++ = 100.0 * (smooth_up / (smooth_up + smooth_down));
    //     }

    //     assert(output - outputs[0] == size - start_); // for pst
    //     return TI_OKAY;
    // }
*/