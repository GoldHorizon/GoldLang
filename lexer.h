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

		void thread_file(std::ifstream& file);
		void tokenize_line(std::deque<token*>& line_tokens, std::string line, int line_num);

        void push_token(token* t);
		void push_tokens(std::deque<token*> t);

		void print_tokens();

        std::vector<std::deque<token*>> tokens;
    private:

        std::vector<re_type*> re_list;
};
