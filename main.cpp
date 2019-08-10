#include <iostream>

#include "lexer.h"
#include "parser.h"
#include "reporting.h"

extern uint16_t warning_count;
extern uint16_t error_count;

int main (int argc, char** argv) {

    lexer* l = new lexer;
    l->read_file("start.g");

    parser* p = new parser(l->tokens);
    p->build_tree();

    p->print_tree();

    if (warning_count > 0 || error_count > 0) {
        report_message("Warnings: % | Errors: %\n", warning_count, error_count);
        return 1;
    } else {
		report_message("Compiler finished with no issues!\n");
	}

	delete p;
	report_message("Parser memory freed\n");
	
	delete l;
	report_message("Lexer memory freed\n");
}
