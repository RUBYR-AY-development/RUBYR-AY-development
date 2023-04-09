#pragma once
#include <iostream>
#include <vector>

enum TOKEN_TYPES
{
	LEFT_PAREN, // (
	RIGHT_PAREN, // )
	DOLLAR_SIGN, // $
	QUOTATION_DOUBLE, // "
	QUOTATION_SINGLE, // '
	DOT, // .
	SEMICOLON, // ;
	COLON, // :
	PLUS, // +
	MINUS, // -
	STAR, // *
	FRONT_SLASH, // /
	AMPERSAND, // &
	EOL, // END OF LINE
	GREATER_THAN, // >
	LESS_THAN, // <
	EQUAL, // =
	COMMA, // ,
	LEFT_CURLY_BRACE, // {
	RIGHT_CURLY_BRACE, // }
};

struct TOKEN
{
	std::string type;
	std::string value;

	auto operator<=>(const TOKEN&) const = default;
};


class LEXER
{
	/*
	* this->TXT is the text for each LINES loop cycle
	*/

	public:
		LEXER(std::vector<std::string> LINES);
	private:
		std::string TXT;
		std::vector<std::string> LINES;
		std::vector<TOKEN> TOKENS;
		std::string CUR_TOK;
		int position = 0;
		int cycle = 0;
		bool in_string = false;
		bool ignore_all_tokens = false; // used for comments

		char CURRENT()
		{
			if (this->position > this->TXT.size())
				return '\0';
			return this->TXT[this->position];
		}

		void ADVANCE()
		{
			this->position++;
		}

		void ADVANCE_NEXT_TXT()
		{
			this->ADD_CUR_TOK();
			this->ignore_all_tokens = false;
			this->position = 0;
			this->cycle++;
			if (this->cycle < this->LINES.size())
			{
				// will advance to next line
				this->TXT = this->LINES[this->cycle];
				this->CUR_TOK = "";
			}
		}

		// TOKEN
		void ADD_CUR_TOK();
		void ADD_TOKEN(TOKEN_TYPES TYPE);
};
