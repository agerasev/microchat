#include <iostream>
#include <fstream>
#include <string>

#include "daemon.hpp"

class MyDaemon : public Daemon
{
private:
	char *page_data;
	int page_length;

public:
	MyDaemon(const char *page_name) : Daemon()
	{
		std::ifstream t(page_name);
		t.seekg(0, std::ios::end);
		page_length = t.tellg();
		page_data = new char [page_length];
		t.seekg(0);
		t.read(page_data, page_length);
	}
	
	~MyDaemon()
	{
		page_length = 0;
		delete page_data;
	}
	
	int answer(Daemon::Connection &conn) override
	{
		printf("request received.\n");
		
		struct MHD_Response *response;
		int ret;

		response = MHD_create_response_from_buffer (page_length, static_cast<void*>(page_data), MHD_RESPMEM_PERSISTENT);

		ret = MHD_queue_response (conn.connection, MHD_HTTP_OK, response);
		MHD_destroy_response (response);

		return ret;
	}
};

int main ()
{
	printf("starting server ...\n");

	MyDaemon daemon("index.html");

	printf("server started.\n");
	
	std::string com;
	while(com != "exit")
	{
		std::getline(std::cin,com);
	}
	
	printf("stopping server ...\n");
  
  return 0;
}
