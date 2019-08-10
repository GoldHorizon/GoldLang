#include <fstream>
#include <iostream>
#include <thread>

#include "lexer.h"
#include "reporting.h"

const int THREAD_COUNT = std::thread::hardware_concurrency();

re_type::~re_type() {
    delete re;
}

lexer::lexer() {
    // C style regex
    // Comments
    auto re_comment = new std::regex ("^//.*");
    re_list.push_back(new re_type {re_comment, token_type::NONE});

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
                     "^struct"};

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

    //// Original regex
    //std::regex re_comment("^//.*");
    //std::regex re_comment_block("");
    //std::regex re_bracket("^[{}]");
    //std::regex re_parens("^[()]");
    //std::regex re_semicolon("^;");
    //std::regex re_colon("^:");
    //std::regex re_equals("^=");
    //std::regex re_comma("^,");
    //std::regex re_operation("^[\\+\\-\\/\\*\\%]");
    //std::regex re_print("^print");
    //std::regex re_return("^return");

    //// Order of regular expressions to check in line
    //re_list.push_back({re_string, tkn_type::string});
    //re_list.push_back({re_comment, tkn_type::comment});
    //re_list.push_back({re_bracket, tkn_type::bracket});
    //re_list.push_back({re_parens, tkn_type::parens});
    //re_list.push_back({re_semicolon, tkn_type::semicolon});
    //re_list.push_back({re_colon, tkn_type::colon});
    //re_list.push_back({re_equals, tkn_type::equals});
    //re_list.push_back({re_comma, tkn_type::comma});
    //re_list.push_back({re_operation, tkn_type::operation});
    //re_list.push_back({re_print, tkn_type::print});
    //re_list.push_back({re_return, tkn_type::ret});
    //re_list.push_back({re_identifier, tkn_type::identifier});
    //re_list.push_back({re_number, tkn_type::number});
    //re_list.push_back({re_hex_number, tkn_type::hex_number});
}

lexer::~lexer() {
    for (auto t : tokens)  delete t;
    for (auto r : re_list) delete r;
}

void lexer::read_file(std::string file_name) {
    //report_message("Begin reading file '%'\n", file_name);
    std::ifstream file;
    std::string line;
    int line_num = 1;

    file.open(file_name);

    if (!file) {
        // @todo Make into error report
        report_error("Could not open file %\n", file_name);
        return;
    }

    // @todo: Make each line work concurrently on another thread
    std::getline(file, line);
    while (file) {
		tokenize_line(line, line_num);
        std::getline(file, line);
        line_num++;
    }

    file.close();
}

std::vector<token*> lexer::tokenize_line(std::string line, int line_num) {
	std::vector<token*> line_tokens;

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
					if (re_t->type != token_type::NONE) {
						//report_message("Found: %\n", re_match.str());

						line_tokens.push_back(new token(re_t->type, re_match.str(), line_num, column_num));
					}

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

	return line_tokens;
}

void lexer::push_token(token* t) {
    tokens.push_back(t);
}

void lexer::push_tokens(std::vector<token*> t) {
	for (auto tok : t)
		tokens.push_back(tok);
}
