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

#include "SoundCloud.h"

#include <fstream>
#include <set>

#include "../json/json.h"
#include "../Player.h"
#include "Track.h"

namespace
{
	const std::string k_connectUrl = "https://soundcloud.com/connect";
	const std::string k_apiUrl = "https://api.soundcloud.com";
	const std::string k_redirectUrl = "http://127.0.0.1:2015/connback";
	const std::string k_credentialsUrl = "http://127.0.0.1:2015/connect";

	const std::string k_credentialsFile = "soundCloudCredentials.json";
}

namespace sc
{
	SoundCloud* SoundCloud::sharedInstance()
	{
		static SoundCloud sharedInstance;
		return &sharedInstance;
	}

	SoundCloud::SoundCloud()
	{
	}

	SoundCloud::~SoundCloud()
	{
		for (auto it : m_queue)
		{
			delete it;
		}

		m_queue.clear();
	}

	void SoundCloud::init(const std::string &initFile)
	{
		Json::Reader reader(Json::Features::strictMode());
		Json::Value root;
		std::ifstream stream;

		stream.open(initFile);
		reader.parse(stream, root, false);
		stream.close();

		m_clientId = root["client_id"].asString();
		m_clientSecret = root["client_secret"].asString();

		loadCredentials();

		m_wrapper.init();
	}

	Response SoundCloud::executeRequest(const Request &request)
	{
		Response response;
		if (!checkCredentials() && request.getEndPoint() > EndPointType::Login)
		{
			response = Response(302, "");
			response.addHeader("Location", k_credentialsUrl);

			return response;
		}

		switch (request.getEndPoint())
		{
			case EndPointType::Connect:
				response = connect();
				break;
			case EndPointType::Login:
				response = login(request.getData());
				break;
			case EndPointType::ConnectCallback:
				response = connectCallback(request.getData());
				break;
			case EndPointType::Enqueue:
				response = enqueue(request.getData());
				break;
			case EndPointType::Play:
				response = play(request.getData());
				break;
			default:
				//Do Nothing
				break;
		}

		return response;
	}

	Response SoundCloud::connect()
	{
		HttpParams params;
		params["client_id"] = m_clientId;
		params["redirect_uri"] = "http://127.0.0.1:2015/connback";
		params["response_type"] = "code";

		return m_wrapper.doRequest(MethodType::Get, k_connectUrl, params);
	}

	Response SoundCloud::connectCallback(const HttpParams &requestData)
	{
		if (requestData.find("error") != requestData.end())
		{
			return Response(400, requestData.find("error_description")->second);
		}

		return Response(200, "");
	}

	Response SoundCloud::login(const HttpParams &requestData)
	{
		if (requestData.find("username") == requestData.end())
		{
			return Response(400, "Bad request!");
		}

		m_user   = requestData.find("username")->second;
		m_passwd = requestData.find("password")->second;

		HttpParams params;

		params["username"] = m_user;
		params["password"] = m_passwd;
		params["client_id"] = m_clientId;
		params["client_secret"] = m_clientSecret;
		params["redirect_uri"] = k_redirectUrl;
		params["grant_type"] = "password";

		Response response = m_wrapper.doRequest(MethodType::Post, k_apiUrl + "/oauth2/token", params);
		if (response.getHttpCode() == 200)
		{
			Json::Value json;
			Json::Reader(Json::Features::strictMode()).parse(response.getData(), json);

			m_oauthToken = json["access_token"].asString();
			m_refreshToken = json["refresh_token"].asString();

			saveCredentials();

			return getUserData();
		}

		return response;
	}

	Response SoundCloud::getUserData()
	{
		HttpParams params;

		params["oauth_token"] = m_oauthToken;
		return m_wrapper.doRequest(MethodType::Get, k_apiUrl + "/me", params);
	}

	Response SoundCloud::enqueue(const HttpParams& requestData)
	{
		HttpParams params;

		params["oauth_token"] = m_oauthToken;
		auto query = requestData.find("query");
		if (query != requestData.end())
		{
			params["q"] = requestData.find("query")->second;
		}

		Response response = m_wrapper.doRequest(MethodType::Get, k_apiUrl + "/tracks", params);
		Json::Value json;
		Json::Reader(Json::Features::strictMode()).parse(response.getData(), json);

		for (int i = 0; i < json.size(); ++i)
		{
			Track* t = new Track();
			t->loadFromJson(json[i]);

			m_queue.push_back(t);
		}

		char info[64];
		snprintf(info, 64, "enqueued %i tracks, total %ui", json.size(), m_queue.size());

		return Response(200, info);
	}

	Response SoundCloud::play(const HttpParams& requestData)
	{
		HttpParams params;

		params["oauth_token"] = m_oauthToken;
		Player::sharedInstance()->play(m_wrapper.getRedirectUrl(m_queue[0]->getStreamUrl(), params));

		return Response(200, "");
	}

	void SoundCloud::saveCredentials()
	{
		Json::Value credentials;

		credentials["auth_token"] = m_oauthToken;
		credentials["refresh_token"] = m_refreshToken;

		std::ofstream stream(k_credentialsFile);
		Json::StyledStreamWriter writer;

		writer.write(stream, credentials);
		stream.close();
	}

	void SoundCloud::loadCredentials()
	{
		Json::Value json;

		std::ifstream stream(k_credentialsFile);
		Json::Reader(Json::Features::strictMode()).parse(stream, json);
		stream.close();

		m_oauthToken = json["auth_token"].asString();
		m_refreshToken = json["refresh_token"].asString();
	}

	bool SoundCloud::checkCredentials() const
	{
		return !m_oauthToken.empty() && !m_refreshToken.empty();
	}
}
