/*
 * Response.h
 *
 *  Created on: 24/10/2015
 *      Author: acampoh
 */

#ifndef SRC_RESPONSE_H_
#define SRC_RESPONSE_H_

#include <string>

struct FCGX_Stream;

class Response
{
public:
	Response();
	Response(int httpCode, const std::string& data);
	virtual ~Response();

	int getHttpCode() const { return m_httpCode; }
	std::string getData() const { return m_data; }
	void doResponse(FCGX_Stream* stream);
private:
	int m_httpCode;
	std::string m_data;
};

#endif /* SRC_RESPONSE_H_ */
