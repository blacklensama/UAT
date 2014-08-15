#pragma once

#include <list>
#include "socket.h"
#include <string>

class singletion
{
public:
	static singletion* Instance();
	void add();
	void add(std::string str);
	std::string get();
protected:
	singletion();
private:
	pthread_mutex_t mutex;
	static singletion* _instance;
	std::list<std::string> _list;
	int num;
};