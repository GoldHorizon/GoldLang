#include <iostream> 

#include "ast_nodes.h"
#include "reporting.h"

using namespace ast;

// Node (base)
node::~node() {}

// Root
root::~root() {
    if (statement_list) delete statement_list;
}

void root::print(int t) {
    report_message("\n---- NODE TREE ----\n\n");
    statement_list->print(t + 1);
    report_message("\n--- END OF TREE ---\n");
}

// Statement
statement::~statement() {
    if (lhs) delete lhs;
}

// Definition
definition::~definition() {}

// Expression
expression::~expression() {}

// Function definition
func_def::~func_def() {
    if (rhs_params) delete rhs_params;
    if (rhs_code) delete rhs_code;
}

void func_def::print(int t) {
    report_tabs(t);
    report_message("FUNCTION % (", lhs->token_info.str);
    
    // Parameters
    rhs_params->print(0);
    report_message(") RETURNS % {\n", type_str(rhs_ret_type));
    
    // Code
    for (auto s : rhs_code->statement_list) {
        s->print(t + 1);
    }
    
    report_tabs(t);
    report_message("}\n");
    
}

// Variable definition
var_def::~var_def() {
    if (rhs) delete rhs;
}

// Function call
func_call::~func_call() {
    if (rhs) delete rhs;
}

void func_call::print(int t) {
    report_tabs(t);
    report_message("% (", lhs->token_info.str);
    
    // Parameters
    rhs->print(0);
    
    report_message(");\n");
}

// Return call
return_call::~return_call() {
    if (rhs) delete rhs;
}

void return_call::print(int t) {
    report_tabs(t);
    report_message("RETURN ");
    
    // Expression
    //rhs->print(0);
    report_message("?"); // TEMP, @cleanup
    
    report_message(";\n");
}

// Parameters
parameters::~parameters() {
    for (auto it : identifier_list)
        delete it;
}

void parameters::print(int t) {
    bool comma = false;
    for (auto p : identifier_list) {
        if (!comma)
            comma = true;
        else
            report_message(", ");
        
        report_message("%", p->token_info.str);
    } 
}

// Code
code::~code() {
    for (auto it : statement_list)
        delete it;
}

void code::print(int t) {
    for (auto s : statement_list) {
        s->print(t);
    }
}

// Identifier
void identifier::print(int t) {
    
}

// Number
void number::print(int t) {
    
}

// Evaluate
eval::~eval() {
    if (inside) delete inside;
}

void eval::print(int t) {
    
}