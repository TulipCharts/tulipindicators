#include "../indicators.h"
#include "pst_manager.h"

pst_manager g_pst_manager;

int pst_ti_init(const char* id, const char* type, TI_REAL const *options) {
    return g_pst_manager.get_idct_no(id, type, options);
}

int pst_ti_rsi(int size,
    TI_REAL const *const *inputs,
    TI_REAL const *options,
    TI_REAL *const *outputs) {
    return g_pst_manager.update_idct(size, inputs, options, outputs);
}

// int pst_ti_sma(int size,
//     TI_REAL const *const *inputs,
//     TI_REAL const *options,
//     TI_REAL *const *outputs) {
//     return g_pst_manager.update_idct(size, inputs, options, outputs);
// }
