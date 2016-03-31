#ifndef STRUTIL_H
#define STRUTIL_H

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <regex>
#include <iostream>

template <typename T>
inline bool parse(const std::string &str, T &out)
{
	std::stringstream ss(str);
	if ((ss >> out).fail() || !(ss >> std::ws).eof())
		return false;
	return true;
}

// trim from start
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}

bool isNumber(const std::string &s)
{
	double tmp = 0.0;
	return parse<double>(s, tmp);//c >= 48 && c <= 57;
}

bool isOperator(const std::string &str)
{
	int slen = str.length();
	return	str == "^" || str == "*" || str == "/" || str == "%" ||
		str == "+" || str == "-" ||
		str == "<<" || str == ">>" ||
		str == "<" || str == ">" ||
		str == "<=" || str == ">=" ||
		str == "==" || str == "!=" ||
		str == "&&" || str == "||" ||
		str == "(" || str == ")";
}

bool isOperatorStart(const char &c)
{
	return	c == '^' || c == '*' || c == '/' || c == '%' ||
		c == '+' || c == '-' ||
		c == '<' || c == '>' ||
		c == '=' || c == '!' ||
		c == '&' || c == '|' ||
		c == '(' ||c == ')';
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

std::string removeWhitespace(const std::string &s) {
	std::string workingStr = "";

	for (size_t i = 0; i < s.length(); i++) {

		char c = s[i];
		if (c == '\"') {  // in string
			i++;
			while (s[i] != '\"')
			{
				if (s[i] != '\t' && s[i] != '\n')
					workingStr += s[i]; i++;
			}
		}
		else {
			if (c != '\t' && c != '\n')
			{
				if (i > 0)
				{
					if (!(c == ' ' && s[i - 1] == ' '))
						workingStr += s[i];
				}
				else
					workingStr += s[i];
			}
		}
	}
	//cout << workingStr << "\n";
	return workingStr;
}

std::vector<std::string> splitExpression(const std::string & s) {
	std::vector<std::string> res;

	std::string workingStr = "";

	bool inComment = false;

	for (size_t i = 0; i<s.length(); i++) {

		char c = s[i];

		if (s[i] == ' ') {
			if (workingStr != "") {
				res.push_back(std::string(workingStr));
				workingStr = "";
			}
		}
		else if (i < s.length()-1 && ((s[i] == '&' && s[i + 1] == '&') ||
			(s[i] == '|' && s[i + 1] == '|') ||
			(s[i] == '=' && s[i + 1] == '=') ||
			(s[i] == '!' && s[i + 1] == '=') ||
			(s[i] == '>' && s[i + 1] == '=') ||
			(s[i] == '<' && s[i + 1] == '=')))
		{
			if (workingStr != "") {
				res.push_back(std::string(workingStr));
				workingStr = "";
			}
			workingStr += s[i];
			workingStr += s[++i];
			res.push_back(std::string(workingStr));
			workingStr = "";
		}
		else if (s[i] == '+' || 
			s[i] == '/' || 
			s[i] == '*' || 
			s[i] == '>' || 
			s[i] == '<' || 
			s[i] == '(' || 
			s[i] == ')' || 
			(s[i] == '-' && i < s.length()-1 && s[i+1] == ' ')) // this is to make sure that (2 != -1) doesn't become (2 != - 1)
		{
			if (workingStr != "") {
				res.push_back(std::string(workingStr));
				workingStr = "";
			}
			workingStr += s[i];
			res.push_back(std::string(workingStr));
			workingStr = "";
		}
		else
		{
			workingStr += s[i];
		}
		
	}

	res.push_back(std::string(workingStr));
	workingStr = "";

	return res;

}

std::vector<std::string> codeSplit(const std::string & s) {
	std::vector<std::string> res;

	std::string workingStr = "";

	bool inComment = false;

	for (size_t i = 0; i<s.length(); i++) {

		char c = s[i];


		if (c == ';') {
			workingStr = removeWhitespace(workingStr);
			res.push_back(std::string(workingStr));
			workingStr = "";
		}
		else if (c == '{' || c == '}') { // begin comment / close or end brace

			workingStr = removeWhitespace(workingStr);
			res.push_back(std::string(workingStr));
			workingStr = "";
			workingStr += s[i];
			res.push_back(std::string(workingStr));
			workingStr = "";
		}

		else if (c == '/'  && s[i + 1] == '*') { // begin comment
			i+=2; // to skip the * character also
			while (!(s[i] == '/' && s[i-1] == '*')) { i++; }
		}
		

		else if (c == '/'  && s[i + 1] == '/') { // one line comment
			workingStr = removeWhitespace(workingStr);
			res.push_back(std::string(workingStr));
			workingStr = "";
			while (s[i] != '\n') { i++; }
		}
		else {
			workingStr += s[i];
		}
	}

	workingStr = removeWhitespace(workingStr);
	res.push_back(std::string(workingStr));
	workingStr = "";

	return res;

}

std::vector<std::string> splitQuotes(const std::string &s, char delim) {
	std::vector<std::string> res;

	std::string workingStr = "";

	for (size_t i = 0; i<s.length(); i++) {

		char c = s[i];
		if (c == delim) {
			res.push_back(std::string(workingStr));
			workingStr = "";
		}
		else if (c == '\"') {
			i++;
			while (s[i] != '\"') { workingStr += s[i]; i++; }
		}
		else {
			workingStr += s[i];
		}
	}
	res.push_back(workingStr);

	return res;
}

#endif