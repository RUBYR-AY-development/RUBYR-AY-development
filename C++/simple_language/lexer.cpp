#include <iostream>
#include <vector>
#include "lexer.h"
#include "symbols.h"

void LEXER::ADD_CUR_TOK()
{
	if (this->CUR_TOK != "" && !this->ignore_all_tokens)
	{
		if (KEYWORD_SYMBOLS.count(this->CUR_TOK))
		{
			this->TOKENS.push_back(TOKEN{ "KEYWORD",this->CUR_TOK });
			this->CUR_TOK = "";
			return;
		}
		if (BUILTIN_FUNCS.count(this->CUR_TOK))
		{
			this->TOKENS.push_back(TOKEN{ "FUNCTION",this->CUR_TOK });
			this->CUR_TOK = "";
			return;
		}
		

		// if everything else is false
		this->TOKENS.push_back(TOKEN{ "IDENTIFIER",this->CUR_TOK });
		this->CUR_TOK = "";
		return;
	}
}

void LEXER::ADD_TOKEN(TOKEN_TYPES TYPE)
{
	switch (TYPE)
	{
	case 0: // (
		if (!this->ignore_all_tokens)
		{
			if (!this->in_string)
			{
				this->ADD_CUR_TOK();
				this->TOKENS.push_back(TOKEN{ "LEFT_PAREN","(" });
			}
			else
				this->CUR_TOK += "(";
		}
		break;
	case 1: // )
		if (!this->ignore_all_tokens)
		{
			if (!this->in_string)
			{
				this->ADD_CUR_TOK();
				this->TOKENS.push_back(TOKEN{ "RIGHT_PAREN",")" });
			}
			else
				this->CUR_TOK += ")";
		}
		break;
	case 2: // $
		if (!this->ignore_all_tokens)
		{
			if (!this->in_string)
			{
				this->TOKENS.push_back(TOKEN{ "KEYWORD_IDENTIFIER","$" });
			}
			else
				this->CUR_TOK += "$";
		}
		break;
	case 3: // "
		if (!this->ignore_all_tokens)
		{
			if (this->in_string)
			{
				// string completed
				this->TOKENS.push_back(TOKEN{ "STRING_LITERAL",this->CUR_TOK });
				this->CUR_TOK = "";
			}
			this->in_string = !this->in_string;
			this->TOKENS.push_back(TOKEN{ "QUOTATION_DOUBLE","\"" });
		}
		break;
	case 4: // '
		if (!this->ignore_all_tokens)
		{
			this->TOKENS.push_back(TOKEN{ "QUOTATION_SINGLE","'" });
		}
		break;
	case 5: // .
		if (!this->ignore_all_tokens)
		{
			if (!this->in_string)
			{
				this->ADD_CUR_TOK();
				this->TOKENS.push_back(TOKEN{ "DOT","." });
			}
			else
				this->CUR_TOK += ".";
		}
		break;
	case 6: // ;
		if (!this->ignore_all_tokens)
		{
			if (!this->in_string)
			{
				this->TOKENS.push_back(TOKEN{ "SEMICOLON",";" });
			}
			else
				this->CUR_TOK += ";";
		}
		break;
	case 7: // :
		if (!this->ignore_all_tokens)
		{
			if (!this->in_string)
			{
				this->ADD_CUR_TOK();
				this->TOKENS.push_back(TOKEN{ "COLON",":" });
			}
			else
				this->CUR_TOK += ":";
		}
		break;
	case 8: // +
		if (!this->ignore_all_tokens)
		{
			if (!this->in_string)
			{
				if (this->TOKENS.back() == TOKEN{ "PLUS","+" })
				{
					this->TOKENS.pop_back();
					this->TOKENS.push_back(TOKEN{ "INCREMENT_ONE","++" });
				}
				else
				{
					this->ADD_CUR_TOK();
					this->TOKENS.push_back(TOKEN{ "PLUS","+" });
				}
			}
			else
				this->CUR_TOK += "+";
		}
		break;
	case 9: // -
		if (!this->ignore_all_tokens)
		{
			if (!this->in_string)
			{
				if (this->TOKENS.back() == TOKEN{ "MINUS","-" })
				{
					this->TOKENS.pop_back();
					this->TOKENS.push_back(TOKEN{ "DECREMENT_ONE","--" });
				}
				else
				{
					this->ADD_CUR_TOK();
					this->TOKENS.push_back(TOKEN{ "MINUS","-" });
				}
			}
			else
				this->CUR_TOK += "-";
		}
		break;
	case 10: // *
		if (!this->ignore_all_tokens)
		{
			if (!this->in_string)
			{
				this->ADD_CUR_TOK();
				this->TOKENS.push_back(TOKEN{ "STAR","*" });
			}
			else
				this->CUR_TOK += "*";
		}
		break;
	case 11: // /
		if (!this->in_string)
		{
			// PREVIOUS TOKEN CHECK
			if (this->TOKENS.back() == TOKEN{ "FRONT_SLASH","/" })
			{
				// check if its a comment
				this->ignore_all_tokens = true;

				// get rid of the previous frontslash
				this->TOKENS.pop_back();
			}
			else
			{
				this->ADD_CUR_TOK();
				this->TOKENS.push_back(TOKEN{ "FRONT_SLASH","/" });
			}
		}
		else
			this->CUR_TOK += "/";
		break;
	case 12: // &
		if (!this->ignore_all_tokens)
		{
			if (!this->in_string)
			{
				this->ADD_CUR_TOK();
				this->TOKENS.push_back(TOKEN{ "AMPERSAND","&" });
			}
			else
				this->CUR_TOK += "&";
		}
		break;
	case 13: // EOL
		this->TOKENS.push_back(TOKEN{ "EOL","EOL" });
		break;
	case 14: // >
		if (!this->ignore_all_tokens)
		{
			if (!this->in_string)
			{
				this->ADD_CUR_TOK();
				this->TOKENS.push_back(TOKEN{ "GREATER_THAN",">" });
			}
			else
				this->CUR_TOK += ">";
		}
		break;
	case 15: // <
		if (!this->ignore_all_tokens)
		{
			if (!this->in_string)
			{
				this->ADD_CUR_TOK();
				this->TOKENS.push_back(TOKEN{ "LESS_THAN","<" });
			}
			else
				this->CUR_TOK += "<";
		}
		break;
	case 16: // =
		if (!this->ignore_all_tokens)
		{
			if (!this->in_string)
			{
				auto bacc = this->TOKENS.back();
				if (bacc == TOKEN{ "EQUAL","=" })
				{
					this->TOKENS.pop_back();
					this->TOKENS.push_back(TOKEN{ "EQUAL_EQUAL","==" });
				}
				// greater or eq
				else if (bacc == TOKEN{ "GREATER_THAN",">" })
				{
					this->TOKENS.pop_back();
					this->TOKENS.push_back(TOKEN{ "GREATER_OR_EQ",">=" });
				}
				// less or eq
				else if (bacc == TOKEN{ "LESS_THAN","<" })
				{
					this->TOKENS.pop_back();
					this->TOKENS.push_back(TOKEN{ "LESS_OR_EQ","<=" });
				}
				// regular equal
				else
				{
					this->ADD_CUR_TOK();
					this->TOKENS.push_back(TOKEN{ "EQUAL","=" });
				}
			}
			else
				this->CUR_TOK += "=";
		}
		break;
	case 17: // ,
		if (!this->ignore_all_tokens)
		{
			if (!this->in_string)
			{
				this->ADD_CUR_TOK();
				this->TOKENS.push_back(TOKEN{ "COMMA","," });
			}
			else
				this->CUR_TOK += ",";
		}
		break;
	case 18: // {
		if (!this->ignore_all_tokens)
		{
			if (!this->in_string)
			{
				this->ADD_CUR_TOK();
				this->TOKENS.push_back(TOKEN{ "LEFT_CURLY_BRACE","{" });
			}
			else
				this->CUR_TOK += "{";
		}
		break;
	case 19: // }
		if (!this->ignore_all_tokens)
		{
			if (!this->in_string)
			{
				this->ADD_CUR_TOK();
				this->TOKENS.push_back(TOKEN{ "RIGHT_CURLY_BRACE","}" });
			}
			else
				this->CUR_TOK += "}";
		}
		break;
	}
}

LEXER::LEXER(std::vector<std::string> LINES)
{
	this->LINES = LINES;
	this->TXT = this->LINES[this->cycle];

	// CYCLE FOR EACH LINE
	while (this->cycle < LINES.size())
	{
		
		// CYCLE FOR EACH CHARACTER
		while (this->CURRENT() != '\0')
		{
			switch (this->CURRENT())
			{
				case '(': {this->ADD_TOKEN(LEFT_PAREN); break; }
				case ')': {this->ADD_TOKEN(RIGHT_PAREN); break; }
				case '$': {this->ADD_TOKEN(DOLLAR_SIGN); break; }
				case '"': {this->ADD_TOKEN(QUOTATION_DOUBLE); break; }
				case '\'': {this->ADD_TOKEN(QUOTATION_SINGLE); break; }
				case '.': {this->ADD_TOKEN(DOT); break; }
				case ';': {this->ADD_TOKEN(SEMICOLON); break; }
				case ':': {this->ADD_TOKEN(COLON); break; }
				case '+': {this->ADD_TOKEN(PLUS); break; }
				case '-': {this->ADD_TOKEN(MINUS); break; }
				case '*': {this->ADD_TOKEN(STAR); break; }
				case '/': {this->ADD_TOKEN(FRONT_SLASH); break; }
				case '&': {this->ADD_TOKEN(AMPERSAND); break; }
				case '>': {this->ADD_TOKEN(GREATER_THAN); break; }
				case '<': {this->ADD_TOKEN(LESS_THAN); break; }
				case '=': {this->ADD_TOKEN(EQUAL); break; }
				case ',': {this->ADD_TOKEN(COMMA); break; }
				case '{': {this->ADD_TOKEN(LEFT_CURLY_BRACE); break; }
				case '}': {this->ADD_TOKEN(RIGHT_CURLY_BRACE); break; }

				case ' ':
				{
					if (this->in_string)
						this->CUR_TOK += this->CURRENT();
					else
						this->ADD_CUR_TOK();
					break;
				}
				default:
				{
					this->CUR_TOK += this->CURRENT();
					break;
				}
			}
			
			this->ADVANCE();
		}

		this->ADVANCE_NEXT_TXT();
		this->ADD_TOKEN(EOL);
	}

	for (auto x : this->TOKENS)
		std::cout << "[" << x.type << "] " << x.value << std::endl;
}
