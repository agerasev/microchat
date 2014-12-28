#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>

#include "chatdaemon.hpp"
#include "chatdatabase.hpp"

int main ()
{
	printf("starting server ...\n");
	
	ChatDatabase database;
	ChatDaemon daemon(&database);

	printf("server started.\nenter 'exit' to stop server.\n");
	
	std::string com;
	while(com != "exit")
	{
		std::getline(std::cin,com);
	}
	
	printf("stopping server ...\n");
  
  return 0;
}
