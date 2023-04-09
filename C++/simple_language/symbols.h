#pragma once
#include <iostream>
#include <unordered_map>
#include <set>
#include <vector>

std::set<std::string> KEYWORD_SYMBOLS =
{
	"var",
	"if",
	"end",
	"else",
	"struct"
};

// ["function name"] = {param one type, param two type, ...}
std::unordered_map<std::string, const std::vector<std::string>> BUILTIN_FUNCS =
{
	{"display",{"string"}},
};
