/*
 * Copyright 2015 Adrián del Campo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
