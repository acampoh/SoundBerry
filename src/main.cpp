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

#include <fcgiapp.h>
#include <thread>

#include <unistd.h>

#include "Player.h"
#include "Request.h"
#include "soundcloud/SoundCloud.h"

const int kMaxThreads = 2;
const std::string kInitFile = "soundberry.conf";

void ProcessFCGI(int socket)
{
	FCGX_Request* cgiRequest = new FCGX_Request();
	FCGX_InitRequest(cgiRequest, socket, 0);

	Request request;

	while(true)
	{
		FCGX_Accept_r(cgiRequest);

		char** envp = cgiRequest->envp;
		for ( ; *envp; ++envp)
		{
			printf("%s\n", *envp);
		}

		request.parse(cgiRequest);
		if (request.isCorrect())
		{
			Response response = sc::SoundCloud::sharedInstance()->executeRequest(request);
			response.doResponse(cgiRequest->out);
		}
	}

	delete cgiRequest;
}

int main(int argc, char **argv)
{
	sc::SoundCloud::sharedInstance()->init(kInitFile);

	FCGX_Init();
	int socket = FCGX_OpenSocket(":2026", kMaxThreads);

	std::thread threads[kMaxThreads];
	for (int i = 0; i < kMaxThreads; ++i)
	{
		threads[i] = std::thread(ProcessFCGI, socket);
	}

	while (true)
	{
		Player::sharedInstance()->update(60);
		sleep(60);
	}
}
