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

void Response::doResponse(FCGX_Stream* stream)
{
	FCGX_FPrintF(stream, "Status: %i\r\n\r\n", m_httpCode);
	FCGX_FPrintF(stream, "%s\r\n\r\n", m_data.c_str());
}

