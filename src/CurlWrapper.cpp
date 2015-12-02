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

#include "CurlWrapper.h"

#include <string>

#include <curl/curl.h>
#include <curl/easy.h>

#include "Enums.h"

namespace
{
	static size_t getResponse(char *ptr, size_t size, size_t nmemb, void *userdata)
	{
		size_t realSize = size * nmemb;

		std::string* data = static_cast<std::string*>(userdata);
		data->append(ptr, realSize);

		return realSize;
	}

	std::string getQueryStringFromParams(CURL* handle, std::map<std::string, std::string> params)
	{
		std::string queryString;
		bool isFirst = true;
		if (!params.empty())
		{
			for (auto it : params)
			{
				queryString += (isFirst) ? '?' : '&';
				isFirst = false;

				int keyLength;
				int valueLength;
				char* origKey = curl_easy_unescape(handle, it.first.c_str(), it.first.size(), &keyLength);
				char* origValue = curl_easy_unescape(handle, it.second.c_str(), it.second.size(), &valueLength);

				queryString += std::string(origKey) + '=' + std::string(origValue);

				curl_free(origKey);
				curl_free(origValue);
			}
		}

		return queryString;
	}

	std::string getDataFromParams(CURL* handle, std::map<std::string, std::string> params)
	{
		std::string body;
		bool isFirst = true;
		if (!params.empty())
		{
			for (auto it : params)
			{
				if (!isFirst)
				{
					body += '&';
				}

				isFirst = false;

				int keyLength;
				int valueLength;
				char* origKey = curl_easy_unescape(handle, it.first.c_str(), it.first.size(), &keyLength);
				char* origValue = curl_easy_unescape(handle, it.second.c_str(), it.second.size(), &valueLength);

				body += std::string(origKey) + '=' + std::string(origValue);

				curl_free(origKey);
				curl_free(origValue);
			}
		}

		return body;
	}
}

CurlWrapper::CurlWrapper()
: m_handler(nullptr)
{
	curl_global_init(CURL_GLOBAL_DEFAULT);
}

CurlWrapper::~CurlWrapper()
{
	curl_global_cleanup();
}

void CurlWrapper::init()
{
	m_handler = curl_easy_init();
}

Response CurlWrapper::doRequest(MethodType::Enum method, const std::string& url, const std::map<std::string, std::string>& body)
{
	curl_easy_reset(m_handler);
	std::string response;
	std::string paramUrl = url;

	struct curl_slist* headers = nullptr; // init to NULL is important
	curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
	curl_slist_append(headers, "Transfer-Encoding: chunked");
	curl_easy_setopt(m_handler, CURLOPT_HTTPHEADER, headers);

	std::string strBody;

	switch(method)
	{
		case MethodType::Get:
			paramUrl += getQueryStringFromParams(m_handler, body);
			break;
		case MethodType::Post:
			curl_easy_setopt(m_handler, CURLOPT_POST, 1L);
			strBody = getDataFromParams(m_handler, body);
			curl_easy_setopt(m_handler, CURLOPT_POSTFIELDS, strBody.c_str());
			break;
		case MethodType::Put:
			curl_easy_setopt(m_handler, CURLOPT_CUSTOMREQUEST, "PUT");
			strBody = getDataFromParams(m_handler, body);
			curl_easy_setopt(m_handler, CURLOPT_POSTFIELDS, strBody.c_str());
			break;
		case MethodType::Delete:
			curl_easy_setopt(m_handler, CURLOPT_CUSTOMREQUEST, "DELETE");
			strBody = getDataFromParams(m_handler, body);
			curl_easy_setopt(m_handler, CURLOPT_POSTFIELDS, strBody.c_str());
			break;
		default:
			//Do nothing
			break;
	}

	curl_easy_setopt(m_handler, CURLOPT_CONNECTTIMEOUT, 15);
	curl_easy_setopt(m_handler, CURLOPT_TIMEOUT, 5);
	curl_easy_setopt(m_handler, CURLOPT_URL, paramUrl.c_str());
	curl_easy_setopt(m_handler, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(m_handler, CURLOPT_WRITEFUNCTION, getResponse);
	curl_easy_setopt(m_handler, CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(m_handler, CURLOPT_NOSIGNAL, 1);

	long responseCode = 400;
	curl_easy_perform(m_handler);
	curl_easy_getinfo (m_handler, CURLINFO_RESPONSE_CODE, &responseCode);

	int length = 0;
	char* unescapedResponse = curl_easy_unescape(m_handler, response.c_str(), response.length(), &length);
	response = unescapedResponse;

	curl_free(unescapedResponse);
	curl_slist_free_all(headers);

	return Response(responseCode, response);
}

std::string CurlWrapper::getRedirectUrl(const std::string& originalUrl, const std::map<std::string, std::string>& body)
{
	curl_easy_reset(m_handler);
	std::string paramUrl = originalUrl;

	paramUrl += getQueryStringFromParams(m_handler, body);
	curl_easy_setopt(m_handler, CURLOPT_CONNECTTIMEOUT, 15);
	curl_easy_setopt(m_handler, CURLOPT_TIMEOUT, 5);
	curl_easy_setopt(m_handler, CURLOPT_URL, paramUrl.c_str());
	curl_easy_setopt(m_handler, CURLOPT_FOLLOWLOCATION, 0);
	curl_easy_setopt(m_handler, CURLOPT_NOSIGNAL, 1);

	curl_easy_perform(m_handler);
	char* finalUrl = nullptr;
	curl_easy_getinfo (m_handler, CURLINFO_REDIRECT_URL, &finalUrl);

	std::string redirectUrl(finalUrl);

	return redirectUrl;
}
