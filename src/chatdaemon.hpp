#pragma once

#include "daemon.hpp"

class ChatDaemon : public Daemon
{
private:
	static int print_out_key (void *cls, enum MHD_ValueKind kind, const char *key, const char *value)
	{
		printf ("%s: %s\n", key, value);
		return MHD_YES;
	}

public:
	ChatDaemon() : Daemon()
	{
		
	}
	
	~ChatDaemon()
	{
		
	}
	
	int respondGet(MHD_Connection *con, const char *url) override
	{
		printf ("GET responded from %s\n", url);
		// MHD_get_connection_values (con, MHD_HEADER_KIND, &print_out_key, NULL);
		
		std::string path(url);
		
		if(path == "/")
		{
			path = "/index.html";
		}
		
		sendFile(con,("res" + path).data());
		
		return YES;
	}
	
	int respondPost(MHD_Connection *con, const char *url, void *data, int size) override
	{
		printf ("POST responded\n");
		// MHD_get_connection_values (con, MHD_HEADER_KIND, &print_out_key, NULL);
		
		sendData(con,data,size);
		
		return YES;
	}
};
