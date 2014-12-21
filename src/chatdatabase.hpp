#pragma once

#include <string>

#include "database.hpp"

class ChatDatabase : public Database
{
private:
	
	void createTableIfNotExists(
		const std::string &name, 
		const std::string &desc
	)
	{
		execute(
			"create table if not exists " + 
			name + "(" + desc + ");"
		);
	}
	
	/* Tests which tables exist and creates if needed */
	void initTables()
	{
		createTableIfNotExists(
			"accounts",
			"id int auto_increment not null,"
			"username varchar(64) not null,"
			"password varchar(32) not null,"
			"first_name varchar(32),"
			"last_name varchar(32),"
			"unique(username),"
			"primary key(id)"
		);
		
		createTableIfNotExists(
			"conversations",
			"id int auto_increment not null,"
			"name varchar(64),"
			"messages_number int not null,"
			"primary key(id)"
		);
		
		createTableIfNotExists(
			"messages",
			"id int auto_increment not null,"
			"author_id int not null,"
			"conversation_id int not null,"
			"text varchar(4096),"
			"primary key(id),"
			"foreign key(author_id) references accounts(id),"
			"foreign key(conversation_id) references conversations(id)"
		);
		
		commit();
	}
	
	void dropTableIfExists(std::string name)
	{
		execute("drop table if exists " + name + ";");
	}
	
	void removeTables()
	{
		dropTableIfExists("messages");
		dropTableIfExists("conversations");
		dropTableIfExists("accounts");
		
		commit();
	}

public:
	ChatDatabase() : 
		Database("microchat","localhost","","microchatdb")
	{
		initTables();
	}
	
	virtual ~ChatDatabase()
	{
		
	}
};
