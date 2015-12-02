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

#ifndef SRC_RESPONSE_H_
#define SRC_RESPONSE_H_

#include <map>
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

	void addHeader(const std::string& key, const std::string& value);
	void doResponse(FCGX_Stream* stream);
private:
	std::map<std::string, std::string> m_headers;
	int m_httpCode;
	std::string m_data;
};

#endif /* SRC_RESPONSE_H_ */
