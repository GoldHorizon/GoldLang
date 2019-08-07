#include "ast_nodes.h"

using namespace ast;

// Node (base)
node::~node() {}

// Root
root::~root() {
    delete statement_list;
}

void root::print() {
    
}

// Statement
statement::~statement() {
    delete lhs;
}

// Definition
definition::~definition() {}

// Expression
expression::~expression() {}

// Function definition
func_def::~func_def() {
    delete rhs_params;
    delete rhs_code;
}

void func_def::print() {

}

// Function call
func_call::~func_call() {
    delete rhs;
}

void func_call::print() {

}

// Return call
return_call::~return_call() {
    delete rhs;
}

void return_call::print() {

}

// Parameters
parameters::~parameters() {
    for (auto it : identifier_list)
        delete it;
}

void parameters::print() {

}

// Code
code::~code() {
    for (auto it : statement_list)
        delete it;
}

void code::print() {

}

// Identifier
void identifier::print() {

}

// Number
void number::print() {

}

// Evaluate
eval::~eval() {
    delete inside;
}

void eval::print() {

}