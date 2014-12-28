#pragma once

#include <memory>

#include "database.hpp"

class ChatDatabase : public Database
{
public:
	ChatDatabase();
	virtual ~ChatDatabase();
	
	std::unique_ptr<Database::Table> getAllUsers();
	std::unique_ptr<Database::Table> getAllConversations();
	
	std::unique_ptr<Database::Table> getUserById(const std::string &id);
	std::unique_ptr<Database::Table> getUserByName(const std::string &name);
	std::unique_ptr<Database::Table> getUserByNameAndPassword(
		const std::string &name, 
		const std::string &password
	);
	
	std::unique_ptr<Database::Table> getConversationById(
		const std::string &id
	);
	std::unique_ptr<Database::Table> getConversationByName(
		const std::string &name
	);
	
	bool updateUserNames(
		const std::string &id,
		const std::string &first_name, 
		const std::string &last_name
	);
	bool updateUserPassword(
		const std::string &id,
		const std::string &password
	);
	bool addUser(
		const std::string &username, 
		const std::string &password, 
		const std::string &first_name, 
		const std::string &last_name
	);
	bool addConversation(
		const std::string &name
	);
	std::unique_ptr<Database::Table> getAllMessagesWithAuthor(
		const std::string &conv_id
	);
	std::unique_ptr<Database::Table> getNewMessagesWithAuthor(
		const std::string &conv_id, 
		const std::string &time
	);
	bool addMessage(
		const std::string &author_id, 
		const std::string &conv_id, 
		const std::string &text
	);
};
