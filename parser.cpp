#include <iostream>

#include "parser.h"
#include "reporting.h"

parser::parser(std::deque<token*>& token_list) {
    tokens = token_list;
}

void parser::build_tree() {
    report_message("Begin parsing...\n");

    tree = new ast::root;

    tree->statement_list = create_code();

}

void parser::print_tree() {
    if (tree != nullptr)
        tree->print();
}

token* parser::eat_token() {
    auto t = tokens.front();

    tokens.pop_front();

    return t;
}

token* parser::front_token() {
    return tokens.front();
}

ast::code* parser::create_code() {

    auto root = new ast::code;

    int index = 0;

    // Consider removing this, may not be necessary, simply for safety
    if (check_symbol("{")) {
        ++index;

        report_message("Note: Line % - Code begins with {, skipping that token\n", front_token()->line);
    }

    report_message("Begin code tokens\n");

    // @todo: Need a way for index to advance correct amount each time we look at a new statement
    while (!check_symbol("}")) {
        report_message("Token: % at %:%\n", front_token()->str, front_token()->line, front_token()->column);
        
        if (check_identifier()) {
            if (check_operator(":", 1))
            {
                if (check_symbol("(", 2)) {

                    root->statement_list.push_back(create_func_definition());
                } else if (check_identifier("", 2)) {
                    root->statement_list.push_back(create_var_definition());
                } else {
                    report_error("Couldn't find correct definition at line %\n", front_token()->line);
                }
            }

            //else if (check_operator("=", 1)) {

            //}

            else if (check_symbol("(", 1)) {
                report_message("Found function call\n");

                root->statement_list.push_back(create_func_call());
            }

            else {
            report_error("Invalid definition statement found! Line %, token - %\n", front_token()->line, tokens[1]->str);
            return nullptr;
            }
        } else if (check_keyword("print")) {
            report_message ("Found print...\n");

            root->statement_list.push_back(create_func_call());
        } else if (check_keyword("return")) {
            report_message ("Found return...\n");

            root->statement_list.push_back(create_return());
        } else {
            report_error ("Can't work with first two tokens: '%' and '%'\n", tokens[0]->str, tokens[1]->str);
        }

        if (error_count > 0) {
            if (root) delete root;
            return nullptr;
        }
    }
    return root;
}

ast::return_call* parser::create_return() {
    auto root = new ast::return_call;

    root->lhs = new ast::identifier;
    root->rhs = nullptr;

    // Temporary @todo
    while (!check_symbol(";")) eat_token();
          
    eat_token();

    return root;
}

ast::func_def* parser::create_func_definition() {
    auto root = new ast::func_def;

    root->lhs = new ast::identifier;
    root->rhs_params = new ast::parameters;
    root->rhs_ret_type = type::t_null;
    root->rhs_code = new ast::code;

    // Func name
    root->lhs->token_info = *front_token();
    std::cout << "Function name: " << root->lhs->token_info.str << std::endl;
    eat_token();

    // Parameters
    eat_token(); eat_token(); // Remove : and (
    
    // Function parameters
    while (!check_symbol(")")) {
        auto id = new ast::identifier;

        if (front_token()->type != token_type::KEYWORD) {
            report_error("Expected type in parameters on line % - Token %\n", front_token()->line, front_token()->str);

            if (root) 
                delete root;

            delete id;
            return nullptr;
        }
        auto tstr = eat_token()->str;
        id->token_info = *eat_token();
        if (tstr == "bool") id->type_info = type::t_bool;
        else if (tstr == "int") id->type_info = type::t_int;
        else if (tstr == "float") id->type_info = type::t_float;
        else if (tstr == "string") id->type_info = type::t_string;
        else {
            report_error("Invalid type in parameters on line %\n", front_token()->line);

            if (root) 
                delete root;

            delete id;
            return nullptr;
        }

        root->rhs_params->identifier_list.push_back(id);

        if (check_operator(",")) eat_token();
    }

    eat_token(); // Eat ')'

    // Return type
    std::cout << "\tReturn type: " << front_token()->str << std::endl;
    if (front_token()->str == "bool")
        root->rhs_ret_type = type::t_bool;
    if (front_token()->str == "int")
        root->rhs_ret_type = type::t_int;
    if (front_token()->str == "float")
        root->rhs_ret_type = type::t_float;
    if (front_token()->str == "string")
        root->rhs_ret_type = type::t_string;
    
    eat_token();

    // Code
    report_message("Grabbing code...\n");
    eat_token(); // Remove {

    root->rhs_code = create_code();

    return root;
}

ast::var_def* parser::create_var_definition() {
    auto root = new ast::var_def;

    // @todo

    return root;
}

ast::func_call* parser::create_func_call() {
    auto root = new ast::func_call;

    root->lhs = new ast::identifier;
    root->rhs = new ast::parameters;

    root->lhs->token_info = *eat_token();

    if (check_symbol("(")) eat_token();
    else {
        report_error("Expected '(' after % on line % - Got %\n", root->lhs->token_info.str, root->lhs->token_info.line, front_token()->str);
        return nullptr;
    }

    // Function parameters
    while (!check_symbol(")")) {
        auto id = new ast::identifier;

        id->token_info = *eat_token();
        switch (id->token_info.type) {
            case token_type::STRING:
                id->type_info = type::t_string;
                break;
            //case token_type::CONSTANT:
            default:
                id->type_info = type::t_null;
        }

        root->rhs->identifier_list.push_back(id);

        if (check_operator(",")) eat_token();
    }

    eat_token();

    if (!check_symbol(";")) {
        report_error("Expected ';' after function call on line %\n", front_token()->line);

        if (root)
            delete root;

        while (!check_symbol(";")) eat_token();
        eat_token();

        return nullptr;
    }

    eat_token();

    return root;
}

bool parser::check_token(token_type t, std::string s, int index) {
    return (tokens[index]->type == t && (s == "" || tokens[index]->str == s));
}

bool parser::check_keyword  (std::string s, int index) {
    return check_token(token_type::KEYWORD, s, index);
}

bool parser::check_identifier   (std::string s, int index) {
    return check_token(token_type::IDENTIFIER, s, index);
}

bool parser::check_symbol   (std::string s, int index) {
    return check_token(token_type::SYMBOL, s, index);
}

bool parser::check_operator (std::string s, int index) {
    return check_token(token_type::OPERATOR, s, index);
}