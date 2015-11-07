/*
 * Response.cpp
 *
 *  Created on: 24/10/2015
 *      Author: acampoh
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
	FCGX_FPrintF(stream, "Content-Length: %i\r\n\r\n", m_data.size());

	FCGX_FPrintF(stream, "%s\r\n\r\n", m_data.c_str());
}

