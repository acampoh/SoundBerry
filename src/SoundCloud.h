/*
 * SoundCloud.h
 *
 *  Created on: 22/8/2015
 *      Author: acampoh
 */

#ifndef SRC_SOUNDCLOUD_H_
#define SRC_SOUNDCLOUD_H_

#include "CurlWrapper.h"
#include "Response.h"

class SoundCloud
{
public:
	static SoundCloud* sharedInstance();

	virtual ~SoundCloud();

	void init(const std::string &initFile);

	Response connect();
	Response connectCallback(const HttpParams &requestData);
	Response login(const HttpParams &requestData);
	Response getUserData();

	std::string getOAuthToken() const { return m_oauthToken; };

protected:
	SoundCloud();

	void requestAuthToken();

	CurlWrapper m_wrapper;

	std::string m_clientId;
	std::string m_clientSecret;

	std::string m_user;
	std::string m_passwd;
	std::string m_oauthToken;
};

#endif /* SRC_SOUNDCLOUD_H_ */
