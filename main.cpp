#include <vector>
#include <iostream>
#include <chrono>

#include "interface.h"
#include "lexer.h"
#include "parser.h"
#include "reporting.h"
#include "clock.h"

int main (int argc, char** argv) {

	init_vars();

	std::vector<char*> file_list;
	read_arguments(file_list, argc, argv);

	for (auto file : file_list) {
		std::string file_str(file);

		report_message("Compiling file %\n", file_str);

		lexer* l = new lexer;

		timer::start();
		l->read_file(file_str);
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
			p->print_tree();
		}

		delete p;
		delete l;

		report_debug("Memory freed\n");

	}

	return 0;
}
