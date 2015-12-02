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
