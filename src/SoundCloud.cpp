/*
 * SoundCloud.cpp
 *
 *  Created on: 22/8/2015
 *      Author: acampoh
 */

#include "SoundCloud.h"

#include <fstream>

#include "json/json.h"

namespace
{
	const std::string k_connectUrl = "https://soundcloud.com/connect";
	const std::string k_apiUrl = "https://api.soundcloud.com";
	const std::string k_redirectUrl = "http://127.0.0.1:2015/connback";
}

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

	m_wrapper.init();
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
