#pragma once

#include <list>
#include "socket.h"
#include <string>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

class singletion
{
public:
	static singletion* Instance();
	void add();
	void add(std::string str);
	std::string get();
	std::string getResult();
	void setResult(std::string str);
	void* node;
protected:
	singletion();
private:
	pthread_mutex_t mutex, mutex1;
	static singletion* _instance;
	std::list<std::string> _list;
	std::string result;
	int num;
};

class Command
{
public:
	Command(std::string str);
	std::string operation;
	std::string tag;
	std::list<std::string> checkPoint;
	void print();
protected:
private:
};

