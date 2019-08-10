#include <iostream>
#include <chrono>

#include "lexer.h"
#include "parser.h"
#include "reporting.h"
#include "clock.h"

extern uint16_t warning_count;
extern uint16_t error_count;

int main (int argc, char** argv) {

    lexer* l = new lexer;

	timer::start();
    l->read_file("start.g");
	timer::stop();

	l->print_tokens();
	report_message("Time taken: % milliseconds\n", timer::time());

    if (warning_count > 0 || error_count > 0) {
        report_message("Warnings: % | Errors: %\n", warning_count, error_count);
        return 1;
    }

    parser* p = new parser(l->tokens);
    p->build_tree();

    if (warning_count > 0 || error_count > 0) {
        report_message("Warnings: % | Errors: %\n", warning_count, error_count);
        return 1;
    }

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
