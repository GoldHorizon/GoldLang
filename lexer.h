#pragma once

// Custom lexer class for the compiler, written from scratch
#include <string>
#include <vector>
#include <deque>
#include <regex>

#include "token.h"

struct re_type {
    re_type(std::regex* r, token_type t) :
        re(r), type(t) {}

    ~re_type();

    std::regex* re;
    token_type type;
};

class lexer {
    public:
        lexer();
        ~lexer();
        void read_file(std::string file_name);
		std::vector<token*> tokenize_line (std::string line, int line_num);

        void push_token(token* t);
        void push_tokens(std::vector<token*> t);

        std::deque<token*> tokens;
    private:

        std::vector<re_type*> re_list;
};
