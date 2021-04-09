#include "indicators.h"
#include <stdio.h>

// TODO move to unit test.

#define INSIZE 15
#define ARRAY_SIZE 5

/* Example data on IBM. */
extern const char *datet[];
extern TI_REAL date[];
extern TI_REAL open[];
extern TI_REAL high[];
extern TI_REAL low[];
extern TI_REAL close[];
extern TI_REAL volume[];
extern TI_REAL alternative[];

int exec_pst_ti_init(const char* pst_id
    , const char* pst_type
    , const TI_REAL pst_options[]) {
    int pst_no = pst_ti_init(pst_id, pst_type, pst_options);
    printf("\npst_ti_init(%s, %s, %d)-> %d\n",
           pst_id, pst_type, (int)pst_options[0], pst_no);
    return pst_no;
}

void init_inputs(const ti_indicator_info *info
    ,TI_REAL const *inputs[]) {
    int r=0;
    int j;
    for (j = 0; j < info->inputs; ++j) {
        if (strcmp(info->input_names[j], "open") == 0) {
            inputs[j] = open;
        } else if (strcmp(info->input_names[j], "high") == 0) {
            inputs[j] = high;
        } else if (strcmp(info->input_names[j], "low") == 0) {
            inputs[j] = low;
        } else if (strcmp(info->input_names[j], "close") == 0) {
            inputs[j] = close;
        } else if (strcmp(info->input_names[j], "volume") == 0) {
            inputs[j] = volume;
        } else if (strcmp(info->input_names[j], "real") == 0) {
            if (!r) {
                inputs[j] = close; r = 1;
            } else {
                inputs[j] = open;
            }
        } else {
            assert(0);
        };
    } // for fields
}

int cmp_outputs(TI_REAL const out_ti[][INSIZE]
              , TI_REAL const out_pti[][INSIZE]) {

    /*for (int t=0; t<INSIZE; ++t) {
        if (is_null_value(out_ti[0][t]) == 0) {
            printf("ti[%d]null, ", t);        
        }
        else {
            printf("ti[%d]%8.3f, ", t, out_ti[0][t]);    
        }
    }
    printf("\n");             
    for (int pt=0; pt<INSIZE; ++pt) {
        if (is_null_value(out_pti[0][pt]) == 0) {
            printf("pti[%d]null, ", pt);        
        }
        else {
            printf("pti[%d]%8.3f, ", pt, out_pti[0][pt]);    
        }
    }
    printf("\n");*/

    for (int a=0; a<ARRAY_SIZE; ++a) {
        TI_REAL const *ti = out_ti[a];
        TI_REAL const *pti = out_pti[a];
        int start = 0;
        for (int i=0; i<INSIZE; ++i) {
            TI_REAL pti_val = pti[i];
            if (is_null_value(pti_val) == 0) {
                ++start;
                continue;
            }
            TI_REAL ti_val = ti[i-start];
            if (ti_val != pti_val) {
                /*for (int t=0; t<INSIZE; ++t) {
                    if (is_null_value(ti[t]) == 0) {
                        printf("ti[%d]null, ", t);        
                    }
                    else {
                        printf("ti[%d]%8.3f, ", t, ti[t]);    
                    }
                }
                printf("\n");
                
                for (int pt=0; pt<INSIZE; ++pt) {
                    if (is_null_value(pti[pt]) == 0) {
                         printf("pti[%d]null, ", pt);        
                    }
                    else {
                        printf("pti[%d]%8.3f, ", pt, pti[pt]);    
                    }
                }
                printf("\n"); */

                printf("[error] start=%d, pti[%d][%i](%8.3f) != ti[%d][%d](%8.3f)\n", 
                    start,
                    a, i, pti_val,
                    a, i-start, ti_val);
                return -1;
            }
        } // for values
    } // for arrays
    return 0;
}

void print_head(const ti_indicator_info *info) {
    printf("date           ");
    int i = 0;
    while (info->input_names[i]) {
        printf("%s\t", info->input_names[i]);
        ++i;
    }
    i = 0;
    while (info->output_names[i]) {
        printf("%s\t", info->output_names[i]);
        ++i;
    }
    printf("\n");
}

void print_body_ti(const ti_indicator_info *info
    , int size , TI_REAL const *inputs[]
    , int start, TI_REAL *outputs[]) {
    int ln;
    for (ln=0; ln<size; ++ln) {
        printf("%s  ", datet[ln]);
        {
            int in = 0;
            while (info->input_names[in]) {
                printf("%8.2f\t", inputs[in][ln]);
                ++in;
            } // for in
        }
        if (start <= ln) {
            int out = 0;
            while (info->output_names[out]) {
                printf("%8.3f\t", outputs[out][ln-start]);
                ++out;
            } // for out
        }
        printf("\n");
    } // for line
}

void print_body_pti(const ti_indicator_info *info
    , int size , TI_REAL const *inputs[]
    , int start, TI_REAL *outputs[]) {
    int ln;
    for (ln=0; ln<size; ++ln) {
        printf("%s  ", datet[ln]);
        {
            int in = 0;
            while (info->input_names[in]) {
                printf("%8.2f\t", inputs[in][ln]);
                ++in;
            } // for in
        }
        {
            int out = 0;
            while (info->output_names[out]) {
                if (TI_OKAY == is_null_value(outputs[out][ln])) {
                    printf("null\t");
                }
                else {
                    printf("%8.3f\t", outputs[out][ln]);
                }
                ++out;
            } // for out
        }
        printf("\n");
    } // for line
}

int test_pst_ti_init() {
    printf("\n\n");
    printf("### test pst_ti_init ###\n");

    const char *pst_id = "init_rsi_0";
    const char *pst_type = "pti_rsi";
    TI_REAL pst_options[1] = {3.0};
    int pst_no = exec_pst_ti_init(pst_id, pst_type, pst_options);
    if (pst_no != 0) {
        return -1;
    }

    pst_no = exec_pst_ti_init(pst_id, pst_type, pst_options);
    if (pst_no != 0) {
        return -1;
    }
    
    pst_id = "init_rsi_1";
    pst_no = exec_pst_ti_init(pst_id, pst_type, pst_options);
    if (pst_no != 1) {
        return -1;
    }

    pst_id = "init_rsi_2";
    pst_type = "pti_rsi";
    pst_no = exec_pst_ti_init(pst_id, pst_type, pst_options);
    if (pst_no != 2) {
        return -1;
    }

    pst_id = "init_BAD_z";
    pst_type = "pti_BAD";
    pst_no = exec_pst_ti_init(pst_id, pst_type, pst_options);
    if (pst_no >= 0) {
        return -1;
    }

    return 0;
}

int test_ti_indicator(const char* id
        , const char* type
        , TI_REAL const * options
        , TI_REAL out[][INSIZE]) {
    printf("\n\n");
    printf("### test %s, %s ###\n", id, type);

    const ti_indicator_info *info = ti_find_indicator(type);
    if (info == NULL) {
        printf("[error] fail to find %s\n", type);
        return -1;
    }
    int pst_start = info->start(options);
    printf("info->start = %d\n", pst_start);

    TI_REAL const *inputs[ARRAY_SIZE] = {0, 0, 0, 0, 0};
    init_inputs(info, inputs);

    // TI_REAL out[5][INSIZE];
    TI_REAL* outputs[ARRAY_SIZE] = {out[0], out[1], out[2], out[3], out[4]};

    int result = info->indicator(INSIZE, inputs, options, outputs);
    printf("%s -> %d\n", type, result);

    if (result == TI_OKAY) {
        print_head(info);
        print_body_ti(info, INSIZE, inputs, pst_start, outputs);
        return 0;
    }
    else {
        printf("[error] fail to exec %s -> %d\n", type, result);
        return -1;
    }
}

int test_pti_indicator(const char* pst_id
        , const char* pst_type
        , TI_REAL const* pst_options
        , int step
        , TI_REAL const out_ref[][INSIZE]) {
    printf("\n\n");
    printf("### test %s, %s ###\n", pst_id, pst_type);

    const ti_indicator_info *pst_info = ti_find_indicator(pst_type);
    if (pst_info == NULL) {
        printf("[error] fail to find %s\n", pst_type);
        return -1;
    }
    int pst_start = pst_info->start(pst_options);
    // printf("pst_info->start = %d\n", pst_start);

    int pst_no = exec_pst_ti_init(pst_id, pst_type, pst_options);
    if (pst_no < 0) {
        printf("[error] fail to pst_ti_init(%s, %s)->%d\n", pst_id, pst_type, pst_no);
        return -2;
    }
    TI_REAL pst_idct_options[1] = {(double)pst_no};

    TI_REAL const *inputs[ARRAY_SIZE] = {0, 0, 0, 0, 0};
    init_inputs(pst_info, inputs);

    TI_REAL out[ARRAY_SIZE][INSIZE] = {0};
    TI_REAL *outputs[ARRAY_SIZE] = {out[0], out[1], out[2], out[3], out[4]};

    int result = TI_OKAY;
    // bs: big step
    int pos = 0;
    for (int bs=0; bs<INSIZE; bs+=step) {
        TI_REAL const *step_inputs[ARRAY_SIZE] = {0, 0, 0, 0, 0};
        TI_REAL const *step_input;
        for (int i=0; i<ARRAY_SIZE; ++i) {   
            if (inputs[i] != 0) {
                step_input = inputs[i] + bs;
                step_inputs[i] = step_input;
            }
        }
        int real_step = (pos+step)<=INSIZE ? step : (INSIZE-pos);
        pos += real_step;

        TI_REAL *step_outputs[ARRAY_SIZE] = {
            out[0]+bs, out[1]+bs, out[2]+bs, out[3]+bs, out[4]+bs
        };
        
        // printf("\nstep=%d, real_step=%d, %s(%d) -> %d\n", bs, real_step, pst_type, pst_no, result);
        result = pst_info->indicator(real_step, step_inputs, pst_idct_options, step_outputs);
        if (result != TI_OKAY) {
            break;
        }
    } // for bs
    if (result == TI_OKAY) {
        print_head(pst_info);
        print_body_pti(pst_info, INSIZE, inputs, pst_start, outputs);

        if (cmp_outputs(out_ref, out) != 0) {
            printf("[error] fail to compare result with ti & pst_ti\n");
            return -1;
        }
        return 0;
    }
    else {
        printf("[error] fail to exec %s(%d) -> %d\n", pst_type, pst_no, result);
        return -1;
    }
}

int test_indicator_count() {
    printf("\n\n");
    printf("### test ti_count ###\n");
    int id = TI_INDICATOR_COUNT-1;
    const char* ti_name = ti_indicators[id].name;
    if (ti_name == 0) {
        return -1;
    }
    printf("ti_indicators[%d]=%s\n", id, ti_name);

    id = TI_INDICATOR_COUNT;
    ti_name = ti_indicators[id].name;
    if (ti_name != 0) {
        return -1;
    }
    printf("ti_indicators[%d]=null\n", id);
    return 0;
}

int test_pst_all()
{
    if (0 != test_indicator_count()) {
        return -1;
    }

    if (0 != test_pst_ti_init()) {
        return -2;
    }

    TI_REAL const pst_options[] = {5.0};    
    TI_REAL out_rsi_0[ARRAY_SIZE][INSIZE] = {0};
    
    if (0 != test_ti_indicator("idct_rsi_0", "rsi", pst_options, out_rsi_0)) {
        return -10;
    }

    if (0 != test_pti_indicator("idct_rsi_s1", "pti_rsi", pst_options, 1, out_rsi_0)) {
        return -11;
    }
    
    if (0 != test_pti_indicator("idct_rsi_s2", "pti_rsi", pst_options, 2, out_rsi_0)) {
        return -12;
    }

    if (0 != test_pti_indicator("idct_rsi_s5", "pti_rsi", pst_options, 5, out_rsi_0)) {
        return -13;
    }

    if (0 != test_pti_indicator("idct_rsi_s7", "pti_rsi", pst_options, 7, out_rsi_0)) {
        return -14;
    }

    if (0 != test_pti_indicator("idct_rsi_s9", "pti_rsi", pst_options, 9, out_rsi_0)) {
        return -15;
    }

    if (0 != test_pti_indicator("idct_rsi_s15", "pti_rsi", pst_options, 15, out_rsi_0)) {
        return -16;
    }

    printf("\n\n");
    return 0;
}