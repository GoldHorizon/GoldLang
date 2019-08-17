#include <stdlib.h>
#include <cstring>

#include "globals.h"
#include "interface.h"

void read_arguments(std::vector<char*>& files, int argc, char** argv) {
	int i = 1;
	while (i < argc) {
		report_debug("Reading argument '%' next\n", std::string(argv[i]));
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
					i += read_option(argv[i], nullptr);
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
	int dash_count = 0;
	while (option[0] == '-') {
		++option;
		++dash_count;
	}

	if (dash_count == 1) {
		switch (option[0]) {
			case 'h': 
			{ // Display help text
				option_print_help();
			} break;

			case 't':
			{ // Override thread count
				option_thread_count(option, value);
				options_ate = 2;
			} break;

			case 'd': 
			{ // Override debug mode
				option_toggle_debug();
			} break;

			default:
			{
				report_error("Invalid option '%'\n", "???");
			}
		}
	} else if (dash_count >= 2) {
		if (strcmp(option, "help") == 0) {
			option_print_help();
		} else if (strcmp(option, "thread-count") == 0) {
			option_thread_count(option, value);
			// Don't eat a second option, since the argument
			// should be considered as one string
			//options_ate = 2;
		} else if (strcmp(option, "debug") == 0) {
			option_toggle_debug();
		} else
			report_error("Invalid option '%'\n", std::string(option));
	}

	return options_ate;
}

void option_print_help() {
	report_message("goldlang\n\n");
	report_message("Usage: gold [options] file(s)\n\n");
	report_message("Options:\n");
	report_message("\t-h or --help:\t\tShow this help message\n");
	report_message("\t-t or --thread-count:\tChange number of threads used, 0 for no parallel processing\n");
	report_message("\t-d or --debug:\t\tTurn on debug mode\n");
}

void option_thread_count(char* option, char* value) {
	if (value == nullptr) {
		report_error("Invalid count for thread count option\n");
		return;
	}

	thread_count = atoi(value) - 1;
	if (thread_count <= 0) {
		thread_count = 0;
		report_message("Disabled multithreading\n");
	} else report_message("Set thread count to %\n", thread_count);
}

void option_toggle_debug() {
	debug_mode = !debug_mode;

	if (debug_mode)
		report_message("Set debug mode to be true\n");
	else
		report_message("Set debug mode to be false\n");
}
