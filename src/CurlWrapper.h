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
	std::string getRedirectUrl(const std::string& originalUrl, const std::map<std::string, std::string>& body);

private:
	CURL* m_handler;
};

#endif /* SRC_CURLWRAPPER_H_ */
