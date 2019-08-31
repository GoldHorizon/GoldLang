#include <vector>
#include <iostream>
#include <chrono>

#include "interface.h"
#include "lexer.h"
#include "parser.h"
#include "reporting.h"
#include "clock.h"

int main (int argc, char** argv) {
	// Initialize global variables to default value
	init_vars();

	std::vector<char*> file_list;
	read_arguments(file_list, argc, argv);

	if (file_list.size() == 0) {
		report_message("No input files passed in, exiting...\n");
	} else {
		// Read each file individually (for now. Eventually use a linker [?] to combine the generated code)
		for (auto file : file_list) {
			std::string file_str(file);

			report_message("Compiling file %\n", file_str);

			// Lexing
			lexer* l = new lexer;

			timer::start();
			l->read_file(file_str);
			timer::stop();

			if (warning_count > 0 || error_count > 0) {
				report_message("\nWarnings: % | Errors: %\n", warning_count, error_count);
				return 1;
			} else {
				report_message("Lexer time taken: % milliseconds\n", timer::time());
				if (debug_mode)
					l->print_tokens();
			}

			// Parsing
			parser* p = new parser(l->tokens);

			timer::start();
			p->build_tree();
			timer::stop();

			if (warning_count > 0 || error_count > 0) {
				report_message("\nWarnings: % | Errors: %\n", warning_count, error_count);
				return 1;
			} else {
				report_message("Parser time taken: % milliseconds\n", timer::time());
				if (debug_mode)
					p->print_tree();
			}

			// Eventually put this data in some other object to use when linking

			// Free memory
			delete p;
			delete l;

			report_debug("Memory freed\n");

		}
	}

	return 0;
}
