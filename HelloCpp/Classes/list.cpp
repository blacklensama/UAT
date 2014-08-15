#include "list.h"
#include "iostream"

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
		str = _list.back();
		_list.pop_back();
	}
	pthread_mutex_unlock(&mutex);
	return str;
}