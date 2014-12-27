#pragma once

#include <microhttpd.h>

class Daemon
{
public:
	
	static const int YES = MHD_YES;
	static const int NO  = MHD_NO;
	
private:

	struct PostProcessor
	{
		void *data;
		int size;
	};
	
	static int request_received(
		void *cls, struct MHD_Connection *connection, 
		const char *url, 
		const char *method, const char *version, 
		const char *upload_data, 
		size_t *upload_data_size, void **con_cls
	);
	
	static int request_completed(
		void *cls, struct MHD_Connection *connection, 
		void **con_cls, enum MHD_RequestTerminationCode toe
	);
	
	/* data is heap-allocated and you must free it */
	static void load_file(const char *path, void **data, int *len);

private:
	unsigned int _port;
	struct MHD_Daemon *_daemon;

public:
	Daemon(unsigned int port = 8888);
	
	virtual ~Daemon();
	
	int sendData(MHD_Connection *con, void *data, int size, const char *mime);
	
	/* Sends file content */
	int sendFile(MHD_Connection *con, const char *name, const char *mime);
	
	virtual int respondGet(MHD_Connection *con, const char *url) = 0;
	virtual int respondPost(MHD_Connection *con, const char *url, void *data, int size) = 0;
};
