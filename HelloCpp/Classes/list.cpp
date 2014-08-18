#include "list.h"
#include "iostream"
#include "cocos2d.h"
USING_NS_CC;

singletion* singletion::_instance = 0;

singletion* singletion::Instance()
{
	if (_instance == NULL)
	{
		_instance = new singletion();
	}
	return _instance;
}

void singletion::add()
{
	pthread_mutex_lock(&mutex);
	num++;
	std::cout << num << std::endl;
	pthread_mutex_unlock(&mutex);
}

singletion::singletion()
{
	num = 0;
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutex1, NULL);
}

void singletion::add(std::string str)
{
	pthread_mutex_lock(&mutex);
	_list.push_back(str);
	pthread_mutex_unlock(&mutex);
}

std::string singletion::get()
{
	std::string str = ""; 
	pthread_mutex_lock(&mutex);
	if (!_list.empty())
	{
		str = _list.front();
		_list.pop_front();
	}
	pthread_mutex_unlock(&mutex);
	return str;
}

void singletion::setResult(std::string str)
{
	pthread_mutex_lock(&mutex1);
	result = str;
	pthread_mutex_unlock(&mutex1);
}

std::string singletion::getResult()
{
	std::string str = "";
	pthread_mutex_lock(&mutex1);
	str = result;
	result = "";
	pthread_mutex_unlock(&mutex1);
	return str;
}

Command::Command(std::string str)
{
	rapidjson::Document doc;
	doc.Parse<0>(str.c_str());
	if (doc.HasMember("select") == true)
	{
		operation = "select";
		tag = doc["tag"].GetString();
	}else if (doc.HasMember("click") == true)
	{
		operation = "click";
	}else if (doc.HasMember("check") == true)
	{
		operation = "check";
	}
}

void Command::print()
{
	std::cout << "operation = " << operation << std::endl;
	std::cout << "tag = " << tag << std::endl;
}