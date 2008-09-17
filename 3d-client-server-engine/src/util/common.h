#ifndef _COMMON_H
#define _COMMON_H

#include <string>
#include <vector>

using std::string;
using std::vector;

void splitString(const string& str, vector<string>& tokens,
		const string& delimiters);

#endif
