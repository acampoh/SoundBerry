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

#ifndef SRC_SOUNDCLOUD_H_
#define SRC_SOUNDCLOUD_H_

#include  <vector>

#include "../CurlWrapper.h"
#include "../Request.h"
#include "../Response.h"

namespace sc
{
	class Track;

	class SoundCloud
	{
	public:
		static SoundCloud* sharedInstance();

		virtual ~SoundCloud();

		void init(const std::string &initFile);
		Response executeRequest(const Request &request);

		std::string getOAuthToken() const { return m_oauthToken; };

	protected:
		SoundCloud();

		Response connect();
		Response connectCallback(const HttpParams &requestData);
		Response login(const HttpParams &requestData);
		Response getUserData();
		Response enqueue(const HttpParams &requestData);
		Response play(const HttpParams &requestData);

		void saveCredentials();
		void loadCredentials();
		bool checkCredentials() const;

		CurlWrapper m_wrapper;

		std::string m_clientId;
		std::string m_clientSecret;

		std::string m_user;
		std::string m_passwd;
		std::string m_oauthToken;
		std::string m_refreshToken;

		std::vector<Track*> m_queue;
	};
}

#endif /* SRC_SOUNDCLOUD_H_ */
