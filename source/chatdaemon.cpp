#include <mysql_driver.h>
#include <mysql_connection.h>

#include <sstream>
#include <functional>
#include <cstring>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "chatdaemon.hpp"

static int findCookie(void *cls, enum MHD_ValueKind kind, const char *key, const char *value)
{
	if(!strcmp(key,"Cookie"))
	{
		*static_cast<std::string*>(cls) = std::string(value,strlen(value));
		return MHD_NO;
	}
	return MHD_YES;
}

static void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
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

static std::string formResponse(const Database::Table *table)
{
	std::string answer;
	
	answer = "[";
	answer += "[";
	const Database::Row *row = table->getHeader();
	for(int i = 0; i < row->getSize(); ++i)
	{
		if(i)
		{
			answer += ",";
		}
		std::string value = row->getValue(i);
		replaceAll(value,"\\","\\\\");
		replaceAll(value,"'","\\'");
		answer += '\'' + value + '\'';
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
			std::string value = row->getValue(i);
			replaceAll(value,"\\","\\\\");
			replaceAll(value,"'","\\'");
			answer += '\'' + value + '\'';
		}
		answer += "]";
	}
	answer += "]";
	
	return answer;
}

static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

static std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

bool ChatDaemon::authenticate(const std::string &cookie)
{
	std::vector<std::string> values = split(cookie,';');
	for(unsigned int i = 1; i < values.size(); ++i)
	{
		if(values[i].size() > 0)
		{
			values[i].erase(0,1);
		}
	}
	std::string name, passwd;
	for(std::string &i : values)
	{
		std::vector<std::string> pair = split(i,'=');
		if(pair.size() < 2)
		{
			continue;
		}
		// printf("%s\n",i.data());
		if(pair[0] == "username")
		{
			name = pair[1];
		}
		else
		if(pair[0] == "password")
		{
			passwd = pair[1];
		}
	}
	//printf("%s %s\n",name.data(),passwd.data());
	
	bool exists;
	try
	{
		exists = db->getUserByNameAndPassword(name,passwd)->getRowsNumber() > 0;
	}
	catch(sql::SQLException &e)
	{
		exists = false;
	}
	
	return exists;
}

std::pair<std::string,std::vector<std::string>> ChatDaemon::parseRequest(std::string request)
{
	replaceAll(request,"\\","\\\\");
	replaceAll(request,"'","\\'");
	
	std::string com, args;
	
	int lpos = request.find("(");
	int rpos = request.find(")");
	
	com = request.substr(0,lpos);
	args = request.substr(lpos+1,rpos-lpos-1);
	
	// std::cout << com << std::endl;
	
	std::vector<std::string> argv = split(args,',');
	
	for(std::string &i : argv)
	{
		replaceAll(i,"#",",");
	}
	
	return std::pair<std::string,std::vector<std::string>>(com,argv);
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
	// printf ("GET responded from %s\n", url);
	
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
	// printf ("POST responded from %s\n", url);
	
	std::string cookie;
	MHD_get_connection_values (con, MHD_HEADER_KIND, &findCookie, static_cast<void*>(&cookie));
	
	
	std::string answer;
	
	std::string query;
	try
	{
		query = std::string(static_cast<const char*>(data), size);
		
		auto pair = parseRequest(query);
		
		std::unique_ptr<Database::Table> table_ptr;
		bool is_table = false;
		int success = -1;
		
		std::string com = pair.first;
		std::vector<std::string> argv = pair.second;
		
		if(com == "addUser" && argv.size() >= 4)
		{
			success = db->addUser(argv[0],argv[1],argv[2],argv[3]);
		}
		else
		if(com == "getUserByNameAndPassword" && argv.size() >= 2)
		{
			table_ptr = db->getUserByNameAndPassword(argv[0],argv[1]); is_table = true;
		}
		else
		if(authenticate(cookie))
		{
			if(com == "getAllUsers")
			{
				table_ptr = db->getAllUsers(); is_table = true;
			}
			else
			if(com == "getAllConversations")
			{
				table_ptr = db->getAllConversations(); is_table = true;
			}
			else
			if(com == "getUserById" && argv.size() >= 1)
			{
				table_ptr = db->getUserById(argv[0]); is_table = true;
			}
			else
			if(com == "getUserByName" && argv.size() >= 1)
			{
				table_ptr = db->getUserByName(argv[0]); is_table = true;
			}
			else
			if(com == "getConversationById" && argv.size() >= 1)
			{
				table_ptr = db->getConversationById(argv[0]); is_table = true;
			}
			else
			if(com == "getConversationByName" && argv.size() >= 1)
			{
				table_ptr = db->getConversationByName(argv[0]); is_table = true;
			}
			else
			if(com == "updateUserNames" && argv.size() >= 3)
			{
				success = db->updateUserNames(argv[0],argv[1],argv[2]);
			}
			else
			if(com == "updateUserPassword" && argv.size() >= 2)
			{
				success = db->updateUserPassword(argv[0],argv[1]);
			}
			else
			if(com == "addConversation" && argv.size() >= 1)
			{
				success = db->addConversation(argv[0]);
			}
			else
			if(com == "getAllMessagesWithAuthor" && argv.size() >= 1)
			{
				table_ptr = db->getAllMessagesWithAuthor(argv[0]); is_table = true;
			}
			else
			if(com == "getNewMessagesWithAuthor" && argv.size() >= 2)
			{
				table_ptr = db->getNewMessagesWithAuthor(argv[0],argv[1]); is_table = true;
			}
			else
			if(com == "addMessage" && argv.size() >= 3)
			{
				success = db->addMessage(argv[0],argv[1],argv[2]);
			}
			else
			{
				success = -1;
				answer = "Unknown command";
			}
		}
		else
		{
			success = -2;
			answer = "Authentication error";
		}
		db->commit();
		
		if(is_table)
		{
			Database::Table *table = table_ptr.get();
			answer = formResponse(table);
		}
		else
		{
			if(success == 0)
			{
				answer = "Done";
			}
			else
			{
				if(answer.size() == 0)
				{
					answer = "Error";
				}
			}
		}
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
