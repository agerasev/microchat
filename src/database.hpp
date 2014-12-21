#pragma once

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <vector>
#include <string>

#include "mysql_driver.h"
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;
using namespace sql;

class Database
{
public:
	class Row
	{
	private:
		vector<string> data;
		
	public:
		Row(int size)
		{
			data.resize(size);
		}
		
		void setValue(int pos, string value)
		{
			data[pos] = value;
		}
		
		string getValue(int pos) const
		{
			return data[pos];
		}
		
		int getSize() const
		{
			return data.size();
		}
	};
	
	class Table
	{
	private:
		Row head;
		vector<Row> data;
		int row_size;
		
	public:
		Table(const Row &header) :
			head(header), row_size(header.getSize())
		{
			
		}
		
		const Row *getHeader() const
		{
			return &head;
		}
		
		void pushRow(const Row &row)
		{
			if(row.getSize() != row_size)
			{
				cerr << "[error] row size doesn't match table size" << endl;
				return;
			}
			data.push_back(row);
		}
		
		const Row *getRow(int pos) const
		{
			return &data[pos];
		}
		
		int getRowsNumber() const
		{
			return data.size();
		}
		
		int getRowsSize() const
		{
			return row_size;
		}
	};
	
private:
	
	/* table is heap allocated and must be deleted */
	Table *convert(ResultSet *res)
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

private:
	Driver* driver;
	Connection *con;
	Statement *stmt;
	
public:

	Database(const char *user, const char *url, const char *pass, const char *database)
	{
		driver = get_driver_instance();
		con = driver->connect(url, user, pass);
		con->setSchema(database);
		stmt = con->createStatement();
	}
	
	virtual ~Database()
	{
		delete stmt;
		delete con;
	}
	
	/* Returns heap-allocated table which must be deleted */
	Table *executeQuery(const string &query)
	{
		ResultSet *res = stmt->executeQuery(query.data());
		Table *table = convert(res);
		delete res;
		return table;
	}
	
	bool execute(const string &query)
	{
		return stmt->execute(query.data());
	}
	
	void commit()
	{
		con->commit();
	}
};
