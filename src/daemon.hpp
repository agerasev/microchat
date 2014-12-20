#pragma once

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>

#include <microhttpd.h>

class Daemon
{
public:
	class Connection
	{
	public:
		struct MHD_Connection *connection;
		const char *url;
		const char *method;
		const char *version;
		const char *upload_data;
		size_t *upload_data_size;
		void **con_cls;
	};
	
private:
	static int answer_wrapper(void *cls, struct MHD_Connection *connection, 
	                          const char *url, 
	                          const char *method, const char *version, 
	                          const char *upload_data, 
	                          size_t *upload_data_size, void **con_cls)
	{
		Daemon *object = static_cast<Daemon*>(cls);
		Connection conn = {connection,url,method,version,upload_data,upload_data_size,con_cls};
		return object->answer(conn);
	}

private:
	unsigned int _port;
	struct MHD_Daemon *_daemon;

public:
	Daemon(unsigned int port = 8888) :
		_port(port), _daemon(MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, _port, NULL, NULL, &answer_wrapper, static_cast<void*>(this), MHD_OPTION_END))
	{
	
	}
	
	virtual ~Daemon()
	{
		MHD_stop_daemon (_daemon);
	}

	virtual int answer(Connection &connection) = 0;
	
};
