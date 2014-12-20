#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>

#include "daemon.hpp"

class MyDaemon : public Daemon
{
private:
	static int print_out_key (void *cls, enum MHD_ValueKind kind, const char *key, const char *value)
	{
		printf ("%s: %s\n", key, value);
		return MHD_YES;
	}
	
	/* you must free data */
	static void load_file(const char *path, void **data, int *len)
	{
		FILE *file = fopen(path,"r");
		if(file)
		{
			char *string;
			int size;
			
			fseek(file, 0L, SEEK_END);
			size = ftell(file);
			fseek(file, 0L, SEEK_SET);
			
			string = new char[size + 1];
			fread(string, size, 1, file);
			fclose(file);

			string[size] = '\0';
			
			*data = static_cast<void*>(string);
			*len = size;
		}
		else
		{
			*data = nullptr;
			*len = 0;
		}
	}

public:
	MyDaemon() : Daemon()
	{
		
	}
	
	~MyDaemon()
	{
		
	}
	
	int answer(Daemon::Connection &conn) override
	{
		/* printing information */
		printf ("New %s request for %s using version %s\n", conn.method, conn.url, conn.version);
		MHD_get_connection_values (conn.connection, MHD_HEADER_KIND, &print_out_key, NULL);
		
		/* loading requested page */
		void *page_data = nullptr;
		int page_length = 0;
		
		std::string url(conn.url);
		if(url.length() < 2)
		{
			url = "/index.html";
		}
		load_file(("res" + url).data(),&page_data,&page_length);
		
		/* sending response */
		MHD_Response *response = nullptr;
		if(page_data)
		{
			response = MHD_create_response_from_buffer (page_length, page_data, MHD_RESPMEM_MUST_FREE);
			int ret = MHD_queue_response (conn.connection, MHD_HTTP_OK, response);
			MHD_destroy_response (response);
			return ret;
		}
		else
		{
			load_file("res/notfound.html",&page_data,&page_length);
			response = MHD_create_response_from_buffer (page_length, page_data, MHD_RESPMEM_MUST_FREE);
			MHD_queue_response (conn.connection, 404, response);
			MHD_destroy_response(response);
			return MHD_YES;
		}
	}
};

int main ()
{
	printf("starting server ...\n");

	MyDaemon daemon;

	printf("server started.\n");
	
	std::string com;
	while(com != "exit")
	{
		std::getline(std::cin,com);
	}
	
	printf("stopping server ...\n");
  
  return 0;
}
