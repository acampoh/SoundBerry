/*
 * CurlWrapper.h
 *
 *  Created on: 22/8/2015
 *      Author: acampoh
 */

#ifndef SRC_CURLWRAPPER_H_
#define SRC_CURLWRAPPER_H_

#include <string>

#include "Enums.h"
#include "Response.h"

typedef void CURL;
typedef std::map<std::string, std::string> HttpParams;

class CurlWrapper
{
public:
	CurlWrapper();
	virtual ~CurlWrapper();

	void init();
	Response doRequest(MethodType::Enum method, const std::string& url, const HttpParams& body = HttpParams());

private:
	CURL* m_handler;
};

#endif /* SRC_CURLWRAPPER_H_ */
