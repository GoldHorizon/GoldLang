#pragma once

#include <string>

enum class token_type {
    NONE,
    KEYWORD,
    IDENTIFIER,
    CONSTANT,
    STRING,
    SYMBOL,
    OPERATOR
};

//enum class tkn_type {
//    unknown,
//    whitespace,
//    string,
//    comment,
//    comment_block,
//    bracket,
//    parens,
//    semicolon,
//    colon,
//    equals,
//    comma,
//    operation,
//    identifier,
//    number,
//    hex_number,
//    print,
//    ret,
//    _auto
//};

struct token {
    token() : type(token_type::NONE), str(""), line(0), column(0) {}
    token(token_type t, std::string s, int l, int c) :
        type(t), str(s), line(l), column(c) {}

    token_type type;
    std::string str;
    int line;
    int column;
};
