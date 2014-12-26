#pragma once

#include "daemon.hpp"
#include "chatdatabase.hpp"

class ChatDaemon : public Daemon
{
private:
	static int print_out_key (void *cls, enum MHD_ValueKind kind, const char *key, const char *value)
	{
		printf ("%s: %s\n", key, value);
		return MHD_YES;
	}
	
	static bool checkExtension(const std::string &fn, const std::string &ext)
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
	
private:
	ChatDatabase *db;

public:
	ChatDaemon(ChatDatabase *database) : 
		Daemon(), db(database)
	{
		
	}
	
	~ChatDaemon()
	{
		
	}
	
	int respondGet(MHD_Connection *con, const char *url) override
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
			sendFile(con,("res" + path).data(),"text/html");
		}
		else
		if(checkExtension(path,".js"))
		{
			sendFile(con,("res" + path).data(),"application/javascript");
		}
		else
		if(checkExtension(path,".css"))
		{
			sendFile(con,("res" + path).data(),"text/css");
		}
		else
		if(checkExtension(path,".png"))
		{
			sendFile(con,("res" + path).data(),"image/png");
		}
		else
		{
			sendFile(con,"res/notfound.html","text/html");
		}
		
		return YES;
	}
	
	int respondPost(MHD_Connection *con, const char *url, void *data, int size) override
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
		catch(SQLException &e)
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
};
