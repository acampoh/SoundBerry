#include <fcgiapp.h>
#include <thread>

#include <unistd.h>

#include "Request.h"
#include "SoundCloud.h"

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
			Response response;
			switch (request.getEndPoint())
			{
				case EndPointType::Connect:
					response = SoundCloud::sharedInstance()->connect();
					break;
				case EndPointType::Login:
					response = SoundCloud::sharedInstance()->login(request.getData());
					break;
				case EndPointType::ConnectCallback:
					response = SoundCloud::sharedInstance()->connectCallback(request.getData());
					break;
				default:
					//Do Nothing
					break;
			}

			response.doResponse(cgiRequest->out);
		}
	}

	delete cgiRequest;
}

int main(int argc, char **argv)
{
	SoundCloud::sharedInstance()->init(kInitFile);

	FCGX_Init();
	int socket = FCGX_OpenSocket(":2026", kMaxThreads);

	std::thread threads[kMaxThreads];
	for (int i = 0; i < kMaxThreads; ++i)
	{
		threads[i] = std::thread(ProcessFCGI, socket);
	}

	while (true)
	{
		sleep(60);
	}
}
