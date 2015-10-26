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

		Count
	};
}

namespace EndPoint
{
	static const std::string StringForEnum[EndPointType::Count] =
	{
		"connback",
		"connect",
		"connect/login"
	};

	static std::map<std::string, EndPointType::Enum> EnumForString = CreateInvertedIndex<EndPointType::Enum>(StringForEnum, EndPointType::Count);
}

#endif
