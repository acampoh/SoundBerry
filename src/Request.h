/*
 * Request.h
 *
 *  Created on: 23/10/2015
 *      Author: acampoh
 */

#ifndef SRC_REQUEST_H_
#define SRC_REQUEST_H_

#include "CurlWrapper.h"
#include "Enums.h"

struct FCGX_Request;

class Request
{
public:
	Request();
	virtual ~Request();

	void parse(const FCGX_Request *cgiRequest);
	bool isCorrect() const { return m_isOk; }
	HttpParams getData() const { return m_params; }
	EndPointType::Enum getEndPoint() const { return m_endPoint; }

private:
	void clean() { m_isOk = false; }

	MethodType::Enum m_method;
	EndPointType::Enum m_endPoint;

	bool m_isOk;
	HttpParams m_params;
};

#endif /* SRC_REQUEST_H_ */
