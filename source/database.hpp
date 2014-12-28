#pragma once

#include <memory>
#include <vector>
#include <string>

#include <mysql_driver.h>
#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

class Database
{
public:
	class Row
	{
	private:
		std::vector<std::string> data;
		
	public:
		Row(int size);
		void setValue(int pos, std::string value);
		std::string getValue(int pos) const;
		int getSize() const;
		
	};
	
	class Table
	{
	private:
		Row head;
		std::vector<Row> data;
		int row_size;
		
	public:
		Table(const Row &header);
		const Row *getHeader() const;
		void pushRow(const Row &row);
		const Row *getRow(int pos) const;
		int getRowsNumber() const;
		int getRowsSize() const;
		
	};
	
private:
	std::unique_ptr<Table> convert(sql::ResultSet *res);
	
private:
	sql::Driver* driver;
	sql::Connection *con;
	sql::Statement *stmt;
	
public:

	Database(const char *user, const char *url, const char *pass, const char *database);
	virtual ~Database();
	
	std::unique_ptr<Table> executeQuery(const std::string &query);
	bool execute(const std::string &query);
	
	void commit();
};
