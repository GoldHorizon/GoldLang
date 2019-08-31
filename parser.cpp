#include <iostream>
#include <stack>

#include "parser.h"
#include "reporting.h"

parser::parser(std::vector<std::deque<token*>>& token_list) {
    tokens = token_list;
}

void parser::build_tree() {
    report_debug("Begin parsing...\n");
    
    tree = new ast::root;
    
    tree->statement_list = create_code();
    
}

void parser::print_tree() {
    if (tree != nullptr) {
		report_message("\n---- NODE TREE ----\n\n");
        tree->print();
		report_message("\n--- END OF TREE ---\n\n");
	}
}

void parser::eat_token() {
    if (tokens.size() > 0) {
		// Don't delete tokens here, lexer still holds them
        //delete tokens.front();
        tokens.front().pop_front();

		if (tokens.front().size() == 0) tokens.erase(tokens.begin());
    }
}

token* parser::get_token(int i) {
	if (i < tokens.front().size()) return tokens.front()[i];
	else {
		auto it = tokens.begin();

		while (i >= it->size() && it != tokens.end()) {
			i -= it->size();
			++it;
		}

		if (it != tokens.end())
			return (*it)[i];
		else 
			return nullptr;
	}
}

token* parser::front_token() {
    return get_token();
}

ast::code* parser::create_code() {
    
    auto root = new ast::code;
	bool block = false; // Whether this is just a code block (perhaps as part of a definition of a function or struct), or just straight code in the file.
    
    if (check_symbol("{")) {
        eat_token();
		block = true;
    }
    
	report_debug("Starting code loop\n");
    while (!check_symbol("}")) {
		// Determine what type of token is first, then look ahead to see whats next
		report_debug("  Front symbol: %\n", front_token()->str);
        if (check_identifier()) {
            if (check_operator(":", 1))
            {
                if (check_symbol("(", 2)) {
                    // Function definition
                    
                    root->statement_list.push_back(create_func_definition());
                    
                } else if (check_token(token_type::IDENTIFIER, "", 2) || check_token(token_type::KEYWORD, "", 2) || check_token(token_type::STRING, "", 2) ||check_token(token_type::CONSTANT, "" , 2)) {
					// Variable definition

                    root->statement_list.push_back(create_var_definition());
                } else {
					// Need: struct definitions
                    report_error("Couldn't find correct definition at line %\n", front_token()->line);
                }
            }
            
            else if (check_operator("=", 1)) {
				// Assignment
            	//report_message("Found assignment\n");

				//root->statement_list.push_back(create_var_assignment());

				// DEBUG (To get rid of tokens for now)
				while (front_token()->str != ";") eat_token();
				eat_token();
            }
            
            else if (check_symbol("(", 1)) {
                //report_message("Found function call\n");
                
                root->statement_list.push_back(create_func_call());
            }
            
            else {
                report_error("Invalid statement found! Line %, token - %\n", front_token()->line, get_token(1)->str);
                return nullptr;
            }
        } else if (check_keyword("print")) {
            //report_message ("Found print...\n");
            
            root->statement_list.push_back(create_func_call());
        } else if (check_keyword("return")) {
            //report_message ("Found return...\n");
            
            root->statement_list.push_back(create_return());
        } else {
            report_error ("Can't work with first two tokens: '%' and '%'\n", get_token(0)->str, get_token(1)->str);
        }

		if (error_count > 0) return nullptr;
    }
    return root;
}

ast::return_call* parser::create_return() {
    auto root = new ast::return_call;
    
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
	report_debug("Defining function '%'\n", root->lhs->token_info.str);
    eat_token();
    
    // Parameters
    eat_token(); eat_token(); // Remove : and (
    
    // Function parameters
    while (!check_symbol(")")) {
        auto p = new ast::parameter;
        
        if (front_token()->type != token_type::KEYWORD) {
            report_error("Expected type in parameters on line % - Token %\n", front_token()->line, front_token()->str);
            
            if (root) 
                delete root;
            
            delete p;
            return nullptr;
        }

        auto tstr = front_token()->str;
        eat_token();
        p->name = new ast::identifier;
        p->name->token_info = *front_token();
        eat_token();
        
        // Switch on type string
        if (tstr == "bool") p->param_type = type::t_bool;
        else if (tstr == "int") p->param_type = type::t_int;
        else if (tstr == "float") p->param_type = type::t_float;
        else if (tstr == "string") p->param_type = type::t_string;
        else {
            report_error("Invalid type in parameters on line %\n", front_token()->line);
            
            if (root) 
                delete root;
            
            delete p;
            return nullptr;
        }
        
        root->rhs_params->parameter_list.push_back(p);
        
        if (check_operator(",")) eat_token();
    }
    
    eat_token(); // Eat ')'
    
    // Return type
	report_debug("  Return type: %\n", front_token()->str);
    if (front_token()->str == "bool")
        root->rhs_ret_type = type::t_bool;
    if (front_token()->str == "int")
        root->rhs_ret_type = type::t_int;
    if (front_token()->str == "float")
        root->rhs_ret_type = type::t_float;
    if (front_token()->str == "string")
        root->rhs_ret_type = type::t_string;
    
    // @todo: account/allow for pointer/array types (2 or more tokens)
    
    eat_token(); // Eat the type token
    
    // Code
    report_debug("Grabbing code...\n");
    //eat_token(); // Don't eat '{', create_code looks for it
    
    root->rhs_code = create_code();
    
    return root;
}

ast::var_def* parser::create_var_definition() {
    auto root = new ast::var_def;
    
    root->lhs = new ast::identifier;
    
    // Variable name
    root->lhs->token_info = *front_token();
    eat_token(); eat_token(); // Eat var name and ':'
    
    // Get expression details
	report_debug("Creating expression\n");
    root->rhs = create_expression();
    
    if (check_symbol(";")) {
        eat_token();
    } else {
		//DEBUG
		while (front_token()->str != ";") eat_token();
		eat_token();
		
		report_error("Didn't find ';' after expression on line %\n", front_token()->line);
	}

	return root;
}

ast::expression* parser::create_expression() {
    // Maybe scan to ';', then work backwards?
    std::stack<token*> expr_stack;
    std::stack<token*> op_stack;

	for (int i = 0; i < token_count(); ++i) {
		auto t = get_token(i);
	
		report_debug("Expr token %\n", t->str);

		if (t->str == ";") break;

		if (check_operator(t->str))
			op_stack.push(t);
		else
			expr_stack.push(t);
	}

    // Eat all tokens until the ';' at the end
    while (front_token()->str != ";") eat_token();

	// DEBUG
	if (!(op_stack.empty() || op_stack.empty()))
		report_message("Top OP: % - Top EXPR: %\n", op_stack.top(), expr_stack.top());

    //return expr;
	return nullptr;
}

ast::func_call* parser::create_func_call() {
    auto root = new ast::func_call;
    
    root->lhs = new ast::identifier;
    root->rhs = new ast::parameters;
    
    root->lhs->token_info = *front_token();
    eat_token();
    
    if (check_symbol("(")) eat_token();
    else {
        report_error("Expected '(' after % on line % - Got %\n", root->lhs->token_info.str, root->lhs->token_info.line, front_token()->str);
        return nullptr;
    }
    
    // Function parameters
    while (!check_symbol(")")) {
        auto p = new ast::parameter;
        
        p->name = new ast::identifier;
        p->name->token_info = *front_token();
        eat_token();
        
        switch (front_token()->type) {
            case token_type::STRING:
            p->param_type = type::t_string;
            break;
            //case token_type::CONSTANT:
            default:
            p->param_type = type::t_null;
        }
        
        root->rhs->parameter_list.push_back(p);
        
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

int parser::token_count() {
	int count = 0;
	for (auto d : tokens)
		count += d.size();

	return count;
}

bool parser::check_token(token_type t, std::string s, int index) {
    return (get_token(index)->type == t && (s == "" || get_token(index)->str == s));
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
