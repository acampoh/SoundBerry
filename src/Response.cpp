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

#include "Response.h"

#include <fcgiapp.h>

Response::Response()
: m_httpCode(404)
{
}

Response::Response(int httpCode, const std::string& data)
: m_httpCode(httpCode)
, m_data(data)
{
}

Response::~Response()
{
}

void Response::addHeader(const std::string& key, const std::string& value)
{
	m_headers[key] = value;
}

void Response::doResponse(FCGX_Stream* stream)
{
	FCGX_FPrintF(stream, "Status: %i\n", m_httpCode);
	for (auto it : m_headers)
	{
		FCGX_FPrintF(stream, "%s: %s\n", it.first.c_str(), it.second.c_str());
	}

	FCGX_FPrintF(stream, "Content-Length: %i\r\n\r\n", m_data.size() + 4);
	FCGX_FPrintF(stream, "%s\r\n\r\n", m_data.c_str());
}

