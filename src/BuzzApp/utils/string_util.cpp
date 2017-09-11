#pragma once
#include "string_util.hpp"

std::string trim(const std::string& str,
	const std::string& whitespace)
{
	const auto strBegin = str.find_first_not_of(whitespace);
	if (strBegin == std::string::npos)
		return ""; // no content

	const auto strEnd = str.find_last_not_of(whitespace);
	const auto strRange = strEnd - strBegin + 1;

	return str.substr(strBegin, strRange);
}

template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

void split(const std::string &s, char delim, std::vector<std::string>& elems) {
	split(s, delim, std::back_inserter(elems));
}

void split(const std::string &s, char delim, std::list<std::string>& elems) {
	split(s, delim, std::back_inserter(elems));
}