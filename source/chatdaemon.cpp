#include <mysql_driver.h>
#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "chatdaemon.hpp"

int ChatDaemon::print_out_key (void *cls, enum MHD_ValueKind kind, const char *key, const char *value)
{
	printf ("%s: %s\n", key, value);
	return MHD_YES;
}

bool ChatDaemon::checkExtension(const std::string &fn, const std::string &ext)
{
	for(unsigned int i = 1; i <= ext.size(); ++i)
	{
		if(ext[ext.size() - i] != fn[fn.size() - i])
		{
			return false;
		}
	}
	return true;
}

ChatDaemon::ChatDaemon(ChatDatabase *database) : 
	Daemon(), db(database)
{
	
}

ChatDaemon::~ChatDaemon()
{
	
}

int ChatDaemon::respondGet(MHD_Connection *con, const char *url)
{
	printf ("GET responded from %s\n", url);
	MHD_get_connection_values (con, MHD_HEADER_KIND, &print_out_key, NULL);
	
	std::string path(url);
	
	if(path == "/")
	{
		path = "/index.html";
	}
	
	if(checkExtension(path,".html"))
	{
		sendFile(con,("client" + path).data(),"text/html");
	}
	else
	if(checkExtension(path,".js"))
	{
		sendFile(con,("client" + path).data(),"application/javascript");
	}
	else
	if(checkExtension(path,".css"))
	{
		sendFile(con,("client" + path).data(),"text/css");
	}
	else
	if(checkExtension(path,".png"))
	{
		sendFile(con,("client" + path).data(),"image/png");
	}
	else
	{
		sendFile(con,"client/notfound.html","text/html");
	}
	
	return YES;
}

int ChatDaemon::respondPost(MHD_Connection *con, const char *url, void *data, int size)
{
	printf ("GET responded from %s\n", url);
	MHD_get_connection_values (con, MHD_HEADER_KIND, &print_out_key, NULL);
	
	std::string query;
	std::string answer;
	try
	{
		query = std::string(static_cast<const char*>(data), size);
		
		Database::Table *table = db->executeQuery(query);
		db->commit();
		
		answer = "[";
		answer += "[";
		const Database::Row *row = table->getHeader();
		for(int i = 0; i < row->getSize(); ++i)
		{
			if(i)
			{
				answer += ",";
			}
			answer += '\'' + row->getValue(i) + '\'';
		}
		answer += "]";
		for(int j = 0; j < table->getRowsNumber(); ++j)
		{
			answer += ",";
			answer += "[";
			const Database::Row *row = table->getRow(j);
			for(int i = 0; i < row->getSize(); ++i)
			{
				if(i)
				{
					answer += ",";
				}
				answer += '\'' + row->getValue(i) + '\'';
			}
			answer += "]";
		}
		answer += "]";
		delete table;
	}
	catch(sql::SQLException &e)
	{
		answer = std::string(e.what());
		if(!answer.size())
		{
			answer = "Done";
		}
	}
	
	sendData(con,const_cast<char*>(answer.data()),answer.size(),"text/plain");
	
	return YES;
}
