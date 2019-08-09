// All nodes used by the AST
#pragma once

#include <vector>
#include <string>

#include "types.h"
#include "token.h"

namespace ast {
    
    struct node {
        virtual ~node() = 0;
        virtual void print(int t = 0) = 0;
    };
    
    struct code;
    struct identifier;
    struct parameters;
    struct root : public node {
        ~root();
        virtual void print(int t = 0);
        
        code* statement_list;
    };
    
    struct statement : public node {
        virtual ~statement() = 0;
        
        identifier* lhs;
    };
    
    struct definition : public statement {
        virtual ~definition() = 0;
    };
    
    struct expression : public node {
        virtual ~expression() = 0;
    };
    
    struct func_def : public definition {
        ~func_def();
        virtual void print(int t = 0);
        
        parameters* rhs_params;
        type rhs_ret_type;
        code* rhs_code;
    };
    
    struct func_call : public statement {
        ~func_call();
        virtual void print(int t = 0);
        
        parameters* rhs;
    };
    
    struct var_def : public definition {
        ~var_def();
        virtual void print(int t = 0);
        
        expression* rhs;
    };
    
    //struct assignment : public statement {
    //  expression* rhs;
    //};
    
    struct return_call : public statement {
        ~return_call();
        virtual void print(int t = 0);
        
        expression* rhs;
    };
    
    struct parameters : public node {
        ~parameters();
        virtual void print(int t = 0);
        
        std::vector<expression*> parameter_list;
    };
    
    struct code : public node {
        ~code();
        virtual void print(int t = 0);
        
        std::vector<statement*> statement_list;
    };
    
    struct identifier : public expression {
        virtual void print(int t = 0);
        
        token token_info;
        type type_info;
    };
    
    struct number : public expression {
        virtual void print(int t = 0);
        
        token token_info;
        type type_info;
    };
    
    struct eval : public expression {
        ~eval();
        virtual void print(int t = 0);
        
        expression* inside;
    };
    
}