#pragma once

// File with all type info

enum class type {
    t_null,
    t_bool,
    t_int,
    t_float,
    t_string
};

// Useful ever?
class type_base {
    public:
        virtual ~type_base() = 0;

    protected:
        type data_type;
};

class type_bool : public type_base {
    
};

class type_int : public type_base {

};

class type_float : public type_base {

};

class type_string : public type_base {

};
