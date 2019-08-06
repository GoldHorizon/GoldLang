#pragma once

// Class for parser, generates AST for further usage
#include <vector>
#include <deque>

#include "token.h"
#include "ast_nodes.h"

class parser {
    public:
        parser(std::deque<token*>& token_list);
        void build_tree();

        ast::code*          create_code();
        ast::func_def*      create_func_definition();
        ast::func_call*     create_func_call();
        ast::return_call*   create_return();

    private:
        token* eat_token();
        token* front_token();

        bool check_token        (token_type t, std::string s = "", int index = 0);
        bool check_keyword      (std::string s = "", int index = 0);
        bool check_identifier   (std::string s = "", int index = 0);
        bool check_symbol       (std::string s = "", int index = 0);
        bool check_operator     (std::string s = "", int index = 0);

        ast::root* tree;
        std::deque<token*> tokens;
};

