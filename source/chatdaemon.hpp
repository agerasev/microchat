#pragma once

#include <vector>
#include <string>
#include <memory>
#include <utility>

#include "daemon.hpp"

#include "chatdatabase.hpp"

class ChatDaemon : public Daemon
{
private:
	ChatDatabase *db;
	
	bool authenticate(const std::string &cookie);
	std::pair<std::string,std::vector<std::string>> parseRequest(std::string request);

public:
	ChatDaemon(ChatDatabase *database);
	~ChatDaemon();
	
	int respondGet(MHD_Connection *con, const char *url) override;
	int respondPost(MHD_Connection *con, const char *url, void *data, int size) override;
};
