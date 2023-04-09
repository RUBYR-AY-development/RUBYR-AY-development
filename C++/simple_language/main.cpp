#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lexer.h"

#define FILE_NAME "SRC.txt"
static std::string LINE;
static std::ifstream _FILE(FILE_NAME);
static std::vector<std::string> LINES;

void open_contents()
{
	while (getline(_FILE, LINE))
		LINES.push_back(LINE);
	_FILE.close();
}

int main(int argc, char** argv)
{
	open_contents();
	LEXER lex(LINES);

}
