/*
 * Utils.cpp
 *
 *  Created on: 23/10/2015
 *      Author: acampoh
 */

#include "Utils.h"

#include <string>

std::string utils::Tokenize(std::string& string, const std::string& delimiter)
{
	int delimPos = string.find(delimiter);
	std::string token = string.substr(0, delimPos);

	if (delimPos >= 0)
	{
		string = string.substr(delimPos + 1, string.length() - (delimPos + 1));
	}
	else
	{
		string = "";
	}

	return token;
}
