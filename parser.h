#pragma once

// Class for parser, generates AST for further usage
#include <vector>
#include <stack>
#include <deque>
#include <unordered_map>

#include "token.h"
#include "ast_nodes.h"
#include "symbol.h"

using umap = std::unordered_map<std::string, symbol*>;

class parser {
    public:
    parser(std::vector<std::deque<token*>>& token_list);
    void build_tree();
    void print_tree();
    
    // Temporary interpretation method
    int run_tree();
    
    ast::code*          create_code();
    ast::func_def*      create_func_definition();
    ast::var_def*       create_var_definition();
    ast::expression*    create_expression();
    ast::expression*    create_expression(std::stack<token*>& expr_stack);
    ast::func_call*     create_func_call();
    ast::return_call*   create_return();
    
    private:
    void eat_token();
    token* get_token(int i = 0);
    token* front_token();

	int token_count();
    
    bool check_token        (token_type t, std::string s = "", int index = 0);
    bool check_keyword      (std::string s = "", int index = 0);
    bool check_identifier   (std::string s = "", int index = 0);
    bool check_symbol       (std::string s = "", int index = 0);
    bool check_operator     (std::string s = "", int index = 0);
    
    ast::root* tree;
    std::vector<std::deque<token*>> tokens;
    
    umap symbol_table;
};

