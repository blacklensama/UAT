#pragma once

#include <list>
#include "socket.h"
#include <string>
#include <vector>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "cocos2d.h"

class Command
{
public:
	Command(std::string str);
	std::string operation;
	std::string tag;
	std::vector<std::string> checkPoint;
	void print();
protected:
private:
};

class ResultJson
{
public:
	std::string result;
	std::list<std::string> checkResult;
	std::string ToJson();
protected:
private:
};

class singletion
{
public:
	static singletion* Instance();
	void add();
	void add(std::string str);
	std::string get();
	std::string getResult();
	void setResult(std::string str);
	cocos2d::CCNode* node;
	ResultJson execute(Command c);
	std::vector<std::string> split(std::string value, std::string str);
protected:
	singletion();
private:
	pthread_mutex_t mutex, mutex1;
	static singletion* _instance;
	std::list<std::string> _list;
	std::string result;
	int num;
};

