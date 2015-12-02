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

#ifndef ENUM_H_
#define ENUM_H_

#include <map>
#include <string>

#include "InvertedIndex.h"

namespace MethodType
{
	enum Enum
	{
		Get,
		Post,
		Put,
		Delete,

		Count
	};
}

namespace Method
{
	static const std::string StringForEnum[MethodType::Count] =
	{
		"GET",
		"POST",
		"PUT",
		"DELETE",
	};

	static std::map<std::string, MethodType::Enum> EnumForString = CreateInvertedIndex<MethodType::Enum>(StringForEnum, MethodType::Count);
}

namespace EndPointType
{
	enum Enum
	{
		ConnectCallback,
		Connect,
		Login,
		Enqueue,
		Play,

		Count
	};
}

namespace EndPoint
{
	static const std::string StringForEnum[EndPointType::Count] =
	{
		"connback",
		"connect",
		"connect/login",
		"enqueue",
		"play"
	};

	static std::map<std::string, EndPointType::Enum> EnumForString = CreateInvertedIndex<EndPointType::Enum>(StringForEnum, EndPointType::Count);
}

#endif
