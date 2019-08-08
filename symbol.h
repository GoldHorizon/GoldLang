#include "types.h"

union symbol_data {
    int* int_data;
    float* float_data;
    bool* bool_data;
    std::string* str_data;
};

struct symbol {

    type s_type;
};