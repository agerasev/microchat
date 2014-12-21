#pragma once

#include "database.hpp"

class ChatDatabase : Database
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
