#include <iostream>
#include <sstream>
#include <stdexcept>

#include <vector>
#include <string>

#include <mysql_driver.h>
#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "database.hpp"

using namespace std;
using namespace sql;

Database::Row::Row(int size)
{
	data.resize(size);
}

void Database::Row::setValue(int pos, string value)
{
	data[pos] = value;
}

string Database::Row::getValue(int pos) const
{
	return data[pos];
}

int Database::Row::getSize() const
{
	return data.size();
}
	
Database::Table::Table(const Database::Row &header) :
	head(header), row_size(header.getSize())
{
	
}

const Database::Row *Database::Table::getHeader() const
{
	return &head;
}

void Database::Table::pushRow(const Database::Row &row)
{
	if(row.getSize() != row_size)
	{
		cerr << "[error] row size doesn't match table size" << endl;
		return;
	}
	data.push_back(row);
}

const Database::Row *Database::Table::getRow(int pos) const
{
	return &data[pos];
}

int Database::Table::getRowsNumber() const
{
	return data.size();
}

int Database::Table::getRowsSize() const
{
	return row_size;
}

/* table is heap allocated and must be deleted */
Database::Table *Database::convert(ResultSet *res)
{
	ResultSetMetaData *resmd = res->getMetaData();
	
	int columns_number = resmd->getColumnCount();
	Row header(columns_number);
	for (int i = 1; i <= columns_number; i++) {
		header.setValue(i-1,resmd->getColumnName(i));
	}
	Table *table = new Table(header);
	
	while (res->next())
	{
		Row row(columns_number);
		for (int i = 1; i <= columns_number; i++) 
		{
			row.setValue(i-1,res->getString(i));
		}
		table->pushRow(row);
	}
	
	return table;
}

Database::Database(const char *user, const char *url, const char *pass, const char *database)
{
	driver = get_driver_instance();
	con = driver->connect(url, user, pass);
	con->setSchema(database);
	stmt = con->createStatement();
}

Database::~Database()
{
	delete stmt;
	delete con;
}

/* Returns heap-allocated table which must be deleted */
Database::Table *Database::executeQuery(const string &query)
{
	ResultSet *res = stmt->executeQuery(query.data());
	Table *table = convert(res);
	delete res;
	return table;
}

bool Database::execute(const string &query)
{
	return stmt->execute(query.data());
}

void Database::commit()
{
	con->commit();
}
