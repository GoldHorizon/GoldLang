#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>

#include "lexer.h"
#include "reporting.h"
#include "globals.h"

int current_line_num = 0;
std::mutex file_mutex;
std::mutex token_mutex;

re_type::~re_type() {
    delete re;
}

lexer::lexer() {
    // C style regex
    // Comments
    auto re_comment = new std::regex ("^//.*");
    re_list.push_back(new re_type {re_comment, token_type::COMMENT});

    // Create keywords
    auto keywords = {"^return", 
                     "^print", 
                     "^assert", 
                     "^if", 
                     "^for", 
                     "^while", 
                     "^switch", 
                     "^case", 
                     "^break", 
                     "^continue",
                     "^struct",
                     "^int",
                     "^bool",
                     "^float",
                     "^string"};

    for (auto word : keywords) {
        re_list.push_back(new re_type {new std::regex (word), token_type::KEYWORD});
    }

    // Identifiers
    auto re_identifier = new std::regex ("^[[:alpha:]]\\w*");
    re_list.push_back(new re_type {re_identifier, token_type::IDENTIFIER});

    // Constants
    auto re_number     = new std::regex ("^[[:digit:]]+(\\.[[:digit:]]*)?");
    auto re_hex_number = new std::regex ("^0x[[:xdigit:]]+");
    re_list.push_back(new re_type {re_number, token_type::CONSTANT});
    re_list.push_back(new re_type {re_hex_number, token_type::CONSTANT});

    // Strings
    auto re_string = new std::regex ("^\"[^\"]*\"");
    re_list.push_back(new re_type {re_string, token_type::STRING});

    // Symbols
    auto re_symbols = new std::regex ("^[;{}()\\[\\]]");
    re_list.push_back(new re_type {re_symbols, token_type::SYMBOL});

    // Operators
    auto re_operators = new std::regex ("^(==)|[:=,+-/*%]");
    re_list.push_back(new re_type {re_operators, token_type::OPERATOR});

}

lexer::~lexer() {
    for (auto d : tokens)  
		for (auto t : d)
			delete t;

    for (auto r : re_list) delete r;
}

void lexer::read_file(std::string file_name) {
    std::ifstream file;
    std::string line;

    file.open(file_name);

    if (!file) {
        report_error("Could not open file %\n", file_name);
        return;
    }

	// Multi-thread here!
	if (thread_count > 0) {
		report_debug("Thread count = %\n", thread_count + 1);

		report_debug("Creating job threads\n");
		std::thread* jobs = new std::thread[thread_count];

		report_debug("Assigning job threads\n");
		for (int i = 0; i < thread_count; i++)
			jobs[i] = std::thread(&lexer::thread_file, this, std::ref(file));

		// Make this main thread help do some of the work
		thread_file(file);

		report_debug("Joining job threads\n");
		for (int i = 0; i < thread_count; i++)
			if (jobs[i].joinable()) jobs[i].join();

		report_debug("Deleting job threads\n");
		delete [] jobs;
	} else {
		// Make this main thread do all the work
		thread_file(file);
	}

	// Cleanup empty token deques, if they exist
	auto it = tokens.begin();

	while (it != tokens.end()) {
		if (it->size() == 0)
			it = tokens.erase(it);
		else
			++it;
	}

    file.close();
}

void lexer::thread_file(std::ifstream& file) {
	std::deque<token*> line_tokens;
	std::string line_str;
	int line_num;

	while (file) {
		line_tokens.clear();
		//line_str = "";

		// Protect the file from being read out of order.
		file_mutex.lock();

			if (!file) {
				file_mutex.unlock();
				return;
			}

			line_num = ++current_line_num;
			std::getline(file, line_str);

		file_mutex.unlock();
		// End mutex protection

		// If we got an empty line, just move on to another line
		if (line_str == "") continue;

		tokenize_line(line_tokens, line_str, line_num);

		// Protect token vector from being manipulated by other threads
		token_mutex.lock();

			if (line_num - 1 == tokens.size()) {
				// Simply push new tokens onto end of vector
				tokens.push_back(line_tokens);

			} else  {
				// May have to resize the vector
				if (line_num - 1 > tokens.size())
					tokens.resize(line_num);

				// Then insert tokens
				tokens[line_num - 1] = line_tokens;
			}

		token_mutex.unlock();
		// Unlock token vector mutex
	}
}

void lexer::tokenize_line(std::deque<token*>& line_tokens, std::string line, int line_num) {
	int column_num = 0;
	// Read tokens from line
	if (line.length() > 0) {
		std::string current_line = line;

		while (current_line[0] == ' ' || current_line[0] == '\t') {
			// @cleanup: This is SUPER inefficient I feel. 
			// Maybe use pointer to beginning of string, increment to search?
			current_line = current_line.substr(1);
		}

		while (current_line != "") {
			bool matched = false;
			std::smatch re_match;

			for (auto re_t : re_list) {
				if (std::regex_search(current_line, re_match, *(re_t->re))) {
					if (re_t->type != token_type::NONE && re_t->type != token_type::COMMENT)
						line_tokens.push_back(new token(re_t->type, re_match.str(), line_num, column_num));

					column_num += re_match.length();
					current_line = re_match.suffix();
					matched = true;
					break;
				}
			}

			if (!matched) {
				report_error("Error: Couldn't match on line %: %\n", line_num, current_line);
				break;
			}

			while (current_line[0] == ' ' || current_line[0] == '\t') {
				// @cleanup: Same as above
				current_line = current_line.substr(1);
			}
		}
	}
}

void lexer::push_token(token* t) {
    tokens.back().push_back(t);
}

void lexer::push_tokens(std::deque<token*> t) {
	tokens.push_back(t);
}

void lexer::print_tokens() {
	int line_num = 0;

	report_message("Tokens:\n");

	for (auto d : tokens) {
		++line_num;

		report_message("\tLine %:\t", d.front()->line);
		for (auto t : d) {
			report_message("%", t->str);
		}
		report_message("\n");
	}
}
