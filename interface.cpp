#include <stdlib.h>

#include "globals.h"
#include "interface.h"

void read_arguments(std::vector<char*>& files, int argc, char** argv) {
	int i = 1;
	while (i < argc) {
		if (argv[i][0] == '-') {
			if (argv[i][1] == '-') {
				// Extended option (i.e. --threads=4)
				char* temp = argv[i];

				while (*temp != '\0' && *temp != '=') ++temp;
				while (*temp == '=') {
					*temp = '\0';
					++temp;
				}

				if (*temp == '\0')
					report_error("Invalid extended argument '%'\n", argv[i]);
				else
					i += read_option(argv[i], temp);

				continue;
			} else if (i + 1 < argc && argv[i + 1][0] != '-') {
				// Option ('-t 4')
				i += read_option(argv[i], argv[i + 1]);
				continue;
			} else {
				// Option ('-d')
				i += read_option(argv[i], nullptr);
				continue;
			}
		} else {
			// File to compile
			files.push_back(argv[i]);
			++i;
			continue;
		}
	}	
}

int read_option(char* option, char* value) {
	int options_ate = 1;
	while (option[0] == '-') ++option;

	switch (option[0]) {
		case 'h': 
		{ // Display help text
			print_help();
		} break;

		case 't':
		{ // Override thread count
			if (value == nullptr) {
				report_error("Invalid count for thread count option");
				break;
			}

			thread_count = atoi(value) - 1;
			if (thread_count <= 0) {
				thread_count = 0;
				report_message("Disabled multithreading\n");
			} else report_message("Set thread count to %\n", thread_count);

			options_ate = 2;
		} break;

		case 'd': 
		{ // Override debug mode
			debug_mode = !debug_mode;

			if (debug_mode)
				report_message("Set debug mod{e} to be true\n");
			else
				report_message("Set debug mode to be false\n");
		} break;

		default:
		{
			report_error("Invaid option '%'\n", "???");
		}
	}

	return options_ate;
}

void print_help() { 
	report_message("\ngoldlang\n\n");
	report_message("Usage: gold [options] file(s)\n\n");
}
