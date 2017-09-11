#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <list>

std::string trim(const std::string& str,
	const std::string& whitespace = " \t");
void split(const std::string &s, char delim, std::vector<std::string>& elems);
void split(const std::string &s, char delim, std::list<std::string>& elems);