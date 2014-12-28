#include "chatdatabase.hpp"

ChatDatabase::ChatDatabase() : 
	Database("microchat","localhost","","microchatdb")
{
	
}

ChatDatabase::~ChatDatabase()
{
	
}

std::unique_ptr<Database::Table> ChatDatabase::getAllUsers()
{
	return executeQuery(
		"select id, username, first_name, last_name from accounts;"
	);
}
std::unique_ptr<Database::Table> ChatDatabase::getAllConversations()
{
	return executeQuery(
		"select id, name from conversations;"
	);
}
	
std::unique_ptr<Database::Table> ChatDatabase::getUserById(
	const std::string &id
)
{
	return executeQuery(
		"select username, first_name, last_name from accounts"
		" where id = " + id + ";"
	);
}
std::unique_ptr<Database::Table> ChatDatabase::getUserByName(
	const std::string &name
)
{
	return executeQuery(
		"select id, first_name, last_name from accounts"
		" where username = '" + name + "';"
	);
}
std::unique_ptr<Database::Table> ChatDatabase::getUserByNameAndPassword(
	const std::string &name, 
	const std::string &password
)
{
	return executeQuery(
		"select id, first_name, last_name from accounts"
		" where username = '" + name + "' and password = '" + password + "';"
	);
}

std::unique_ptr<Database::Table> ChatDatabase::getConversationById(
	const std::string &id
)
{
	return executeQuery(
		"select name from conversations"
		" where id = " + id + ";"
	);
}

std::unique_ptr<Database::Table> ChatDatabase::getConversationByName(
	const std::string &name
)
{
	return executeQuery(
		"select id from conversations"
		" where name = '" + name + "';"
	);
}

bool ChatDatabase::updateUserNames(
	const std::string &id,
	const std::string &first_name, 
	const std::string &last_name
)
{
	return execute(
		"update accounts set first_name = '" + first_name + 
		"', last_name = '" + last_name + "' where id = " + id + ";"
	);
}
bool ChatDatabase::updateUserPassword(
	const std::string &id,
	const std::string &password
)
{
	return execute(
		"update accounts set password = '" + password + 
		"' where id = " + id + ";"
	);
}
bool ChatDatabase::addUser(
	const std::string &username, 
	const std::string &password, 
	const std::string &first_name, 
	const std::string &last_name
)
{
	return execute(
		"insert into accounts(username,password,first_name,last_name) values('" +
		username + "','" + password + "','" + first_name + "','" + last_name + "');"
	);
}

bool ChatDatabase::addConversation(
	const std::string &name
)
{
	return execute(
		"insert into conversations(name,last_update) values('" + name + "',now());"
	);
}

std::unique_ptr<Database::Table> ChatDatabase::getAllMessagesWithAuthor(
	const std::string &conv_id
)
{
	return executeQuery(
		"select b.id, concat(a.first_name,' ',a.last_name) as author, b.text, b.send_time"
		" from accounts as a, messages as b where a.id = b.author_id and"
		" conversation_id = " + conv_id + 
		" order by send_time;"
	);
}
std::unique_ptr<Database::Table> ChatDatabase::getNewMessagesWithAuthor(
	const std::string &conv_id, 
	const std::string &time
)
{
	return executeQuery(
		"select b.id, concat(a.first_name,' ',a.last_name) as author, b.text, b.send_time"
		" from accounts as a, messages as b where a.id = b.author_id and"
		" conversation_id = " + conv_id + 
		" and b.send_time > '" + time + "' order by send_time;"
	);
}
bool ChatDatabase::addMessage(
	const std::string &author_id, 
	const std::string &conv_id, 
	const std::string &text
)
{
	return execute(
		"insert into messages(author_id,conversation_id,text,send_time) values(" + 
		author_id + "," +
		conv_id + ",'" +
		text + "',now());"
	);
}
