#pragma once

#include "daemon.hpp"

#include "chatdatabase.hpp"

class ChatDaemon : public Daemon
{
private:
	static int print_out_key (void *cls, enum MHD_ValueKind kind, const char *key, const char *value);
	static bool checkExtension(const std::string &fn, const std::string &ext);
	
private:
	ChatDatabase *db;

public:
	ChatDaemon(ChatDatabase *database);
	~ChatDaemon();
	
	int respondGet(MHD_Connection *con, const char *url) override;
	int respondPost(MHD_Connection *con, const char *url, void *data, int size) override;
};
