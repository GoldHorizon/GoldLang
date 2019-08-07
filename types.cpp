#include "types.h"

std::string type_str(type t) {
    std::string str;

    switch (t) {
        case type::t_null: str = "null";        break;
        case type::t_bool: str = "bool";        break;
        case type::t_int: str = "int";          break;
        case type::t_float: str = "float";      break;
        case type::t_string: str = "string";    break;
        default: str = "invalid_type";
    }

    return str;
}