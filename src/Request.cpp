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

#include "Request.h"

#include <fcgiapp.h>

#include "Utils.h"

namespace
{
	std::string GetFCGIEnv(const char* varName, char** envp)
	{
		const char* value = FCGX_GetParam(varName, envp);
		return (value != nullptr) ? value : "";
	}

	HttpParams ParseQueryString(const std::string& queryString)
	{
		HttpParams params;

		size_t pos = 0;
		if (!queryString.empty())
		{
			while (pos != std::string::npos)
			{
				size_t currentPos = queryString.find('&', pos);
				std::string element = queryString.substr(pos, currentPos - pos);
				pos = (currentPos == std::string::npos) ? currentPos : currentPos + 1;
				size_t separatorPos = element.find('=');
				if (separatorPos != std::string::npos)
				{
					std::string key = element.substr(0, separatorPos);
					std::string value = element.substr(separatorPos + 1, element.length() - (separatorPos + 1));

					params[key] = value;
				}
			}
		}

		return params;
	}
}

Request::Request()
: m_method(MethodType::Get)
, m_endPoint(EndPointType::Count)
, m_isOk(false)
{
}

Request::~Request()
{
}

void Request::parse(const FCGX_Request *cgiRequest)
{
	clean();

	std::string method = GetFCGIEnv("REQUEST_METHOD", cgiRequest->envp);
	m_method = (Method::EnumForString.find(method) != Method::EnumForString.end())  ? Method::EnumForString[method] : MethodType::Count;

	std::string url = GetFCGIEnv("REQUEST_URI", cgiRequest->envp);

	if (url.length() <= 1)
	{
		return;
	}

	url = url.substr(1);
	std::string endPoint = utils::Tokenize(url, "?"); // stripping the possible QueryString

	m_endPoint = (EndPoint::EnumForString.find(endPoint) != EndPoint::EnumForString.end()) ? EndPoint::EnumForString[endPoint] : EndPointType::Count;
	m_params = ParseQueryString(GetFCGIEnv("QUERY_STRING", cgiRequest->envp));
	size_t contentLength = atoi(GetFCGIEnv("CONTENT_LENGTH", cgiRequest->envp).c_str());

	std::string message;
	char temp[1024];

	while (message.length() != contentLength)
	{
		FCGX_GetLine(temp, 1024, cgiRequest->in);
		message += temp;
	}

	HttpParams bodyParams = ParseQueryString(message);
	m_params.insert(bodyParams.begin(), bodyParams.end());

	m_isOk = true;
}
