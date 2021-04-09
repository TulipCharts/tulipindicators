#pragma once
#include "../indicators.h"

class pst_interface {
public:
    pst_interface(const ti_indicator_info* ti_info, TI_REAL const *options) 
        : ti_info_(ti_info)
        , start_(ti_info->start(options)) 
    {}
    virtual int update(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) = 0;
    virtual ~pst_interface() {}
    
protected:
    const ti_indicator_info* ti_info_;
    const int start_;
};