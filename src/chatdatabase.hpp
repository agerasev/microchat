#pragma once

#include <string>

#include "database.hpp"

class ChatDatabase : public Database
{
public:
	ChatDatabase() : 
		Database("microchat","localhost","","microchatdb")
	{
		
	}
	
	virtual ~ChatDatabase()
	{
		
	}
};
