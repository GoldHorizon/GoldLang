#include <iostream>
#include <chrono>

#include "lexer.h"
#include "parser.h"
#include "reporting.h"
#include "clock.h"

uint16_t warning_count;
uint16_t error_count;

int main (int argc, char** argv) {

    lexer* l = new lexer;

	timer::start();
    l->read_file("start.g");
	timer::stop();

    if (warning_count > 0 || error_count > 0) {
        report_message("\nWarnings: % | Errors: %\n", warning_count, error_count);
        return 1;
    } else {
		report_message("Lexer time taken: % milliseconds\n", timer::time());
		l->print_tokens();
	}

    parser* p = new parser(l->tokens);

	timer::start();
    p->build_tree();
	timer::stop();

    if (warning_count > 0 || error_count > 0) {
        report_message("\nWarnings: % | Errors: %\n", warning_count, error_count);
        return 1;
    } else {
		report_message("Parser time taken: % milliseconds\n", timer::time());
	}

    p->print_tree();

	delete p;
	delete l;

	report_message("Memory freed\n");

	return 0;
}
