#pragma once
#include "Helper.h"


string Helper::intToString(int i)
{
	stringstream ss;
	ss<<i;
	return ss.str();
}

string Helper::charToString(char c)
{
	stringstream ss;
	ss<<c;
	return ss.str();
}

int Helper::stringToInt(string s)
{
	//to implement
	return -1;
}

bool Helper::isNumber(string s)
{
    auto it = s.begin();
    while (it != s.end() && isdigit(*it)) 
		it++;
    return !s.empty() && it == s.end();
}