#include "pst_manager.h"
#include <stdio.h>
#include <cassert>

#include "../indicators.h"
#include "pti_rsi.h"
// #include "pti_sma.h"

// TODO bind idct_type to idct_constructor

pst_manager::pst_manager()
    : idct_types_({
        "pti_rsi"
//        ,"pti_sma"
    }) {
}

int pst_manager::get_idct_no(const char* id, const char* type, TI_REAL const *options) {
    int period = (int)options[0];
    // printf("---> id=%s, type=%s, period=%d\n", id, type, period);

    // check indicator's type 
    if (idct_types_.find(type) == idct_types_.cend()) {
        return -1;
    }
    const ti_indicator_info* ti_info = ti_find_indicator(type);
    if (0 == ti_info) {
        return -2;
    }

    // check indicator's id
    auto result = idct_map_.emplace(id, idct_array_.size());
    if (result.second) {
        create_idct(ti_info, options);
    }
    return result.first->second;
}

void pst_manager::create_idct(const ti_indicator_info* ti_info, TI_REAL const *options) {
    pst_interface* pst_idct = nullptr;
    if (0 == strcmp("pti_rsi", ti_info->name)) {
        pst_idct = new pti_rsi(ti_info, options);
    }
    /*else if (0 == strcmp("pti_sma", ti_info->name)) {
        pst_idct = new pti_sma(ti_info, options);
    }*/
    assert(pst_idct != nullptr);
    idct_array_.emplace_back(pst_idct);
}

int pst_manager::update_idct(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs) {
    int idct_no = (int)options[0];
    // printf("---> idct_no=%d\n", idct_no);
    return (!idct_array_.empty() && idct_no < idct_array_.size()) 
        ? idct_array_.at(idct_no)->update(size, inputs, options, outputs) 
        : TI_INVALID_OPTION;    
}