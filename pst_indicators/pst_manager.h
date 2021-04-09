#pragma once
#include <set>
#include <map>
#include <vector>
#include <string>

#include "pst_interface.h"

class pst_manager {
public:
    pst_manager();
    int get_idct_no(const char* id, const char* type, TI_REAL const *options);
    int update_idct(int size, TI_REAL const *const *inputs, TI_REAL const *options, TI_REAL *const *outputs);

private:
    void create_idct(const ti_indicator_info* ti_info, TI_REAL const *options);
    
    typedef std::set<std::string> IDCT_SET;
    const IDCT_SET idct_types_;

    // indicator instance id : no
    typedef std::map<std::string, int> IDCT_MAP;
    IDCT_MAP idct_map_;

    typedef std::vector<pst_interface*> IDCT_ARRAY;
    IDCT_ARRAY idct_array_;
};