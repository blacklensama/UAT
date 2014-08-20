#include "list.h"
#include "iostream"
#include "cocos2d.h"
#include "BlackJack.h"
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
	node = NULL;
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

std::vector<std::string> singletion::split(std::string value, std::string str)
{
	std::vector<std::string> temp;
	int i = 0;
	do 
	{
		std::string tmp;
		i = value.find(str);
		if (i == -1)
		{
			tmp = value.substr(0, value.length());
			temp.push_back(tmp);
			break;
		}
		tmp = value.substr(0, i);
		value.erase(0, i + 1);
		temp.push_back(tmp);
	} while (1);
	return temp;
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
	std::string s = doc["operate"].GetString();
	if (s == "select")
	{
		operation = "select";
		tag = doc["tag"].GetString();
	}else if (s == "click")
	{
		operation = "click";
	}else if (s == "check")
	{
		operation = "check";
		for (auto i = doc.MemberonBegin(); i != doc.MemberonEnd(); i++)
		{
			if (i->value.GetString() != std::string("check"))
			{
				checkPoint.push_back(i->value.GetString());
			}
		}
	}
}

void Command::print()
{
	std::cout << "operation = " << operation << std::endl;
	std::cout << "tag = " << tag << std::endl;
	for (int i = 0; i<checkPoint.size(); i++)
	{
		std::cout << "result" << i << " = " << checkPoint[i] << std::endl;
	}
}

std::string ResultJson::ToJson()
{
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember("result", result.c_str(), allocator);
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::cout << buffer.GetString() << std::endl;
	return buffer.GetString();
}

ResultJson singletion::execute(Command c)
{
	ResultJson j;
	int tag;
	if (c.operation == "select")
	{		
		if (node == NULL)
		{
			node = CCDirector::sharedDirector()->getRunningScene();
		}
		std::vector<std::string> v = singletion::split(c.tag, "-");
		for (int i = 1; i < v.size(); i++)
		{
			std::stringstream ss;
			ss << v[i];
			ss >> tag;
			int temp = node->getChildrenCount();
			node = node->getChildByTag(tag);
			if (node == NULL)
			{
				j.result = "False";
				break;
			}else
			{
				j.result = "True";
			}
		}
	}else if (c.operation == "click")
	{
		CCMenuItemImage* n = dynamic_cast<CCMenuItemImage*>(node);
		if (n == NULL)
		{
			j.result = "False";
		}else
		{
			n->activate();
			j.result = "True";
		}
	}else if (c.operation == "check")
	{
		CCNode* scene = CCDirector::sharedDirector()->getRunningScene();
		for (int i = 0; i < c.checkPoint.size(); i++)
		{
			if (c.checkPoint[i] == "selfNum")
			{
				std::stringstream ss;
				std::string str;
				PlayerLayer* temp = dynamic_cast<PlayerLayer*>(scene->getChildByTag(1)->getChildByTag(11));
				ss << (temp->getChildrenCount() - 3);
				ss >> str;
				j.checkResult.push_back(str);
			}else if (c.checkPoint[i] == "enemyNum")
			{
				std::stringstream ss;
				std::string str;
				PlayerLayer* temp = dynamic_cast<PlayerLayer*>(scene->getChildByTag(1)->getChildByTag(12));
				ss << (temp->getChildrenCount() - 3);
				ss >> str;
				j.checkResult.push_back(str);
			}else if (c.checkPoint[i] == "enemyNum")
			{
				std::stringstream ss;
				std::string str;
				PlayerLayer* temp = dynamic_cast<PlayerLayer*>(scene->getChildByTag(1)->getChildByTag(12));
				ss << (temp->getChildrenCount() - 3);
				ss >> str;
				j.checkResult.push_back(str);
			}else if (c.checkPoint[i] == "selfCalcFlag")
			{
				PlayerLayer* temp = dynamic_cast<PlayerLayer*>(scene->getChildByTag(1)->getChildByTag(11));
				std::stringstream ss;
				std::string str;
				ss << temp->getSum();
				ss >> str;
				if (((CCLabelTTF*)temp->getChildByTag(10))->getString() == str)
				{
					j.checkResult.push_back("True");
				}else
					j.checkResult.push_back("False");
			}else if (c.checkPoint[i] == "enemyCalcFlag")
			{
				PlayerLayer* temp = dynamic_cast<PlayerLayer*>(scene->getChildByTag(1)->getChildByTag(12));
				std::stringstream ss;
				std::string str;
				ss << temp->getSum();
				ss >> str;
				if (((CCLabelTTF*)temp->getChildByTag(10))->getString() == str)
				{
					j.checkResult.push_back("True");
				}else
					j.checkResult.push_back("False");
			}else if (c.checkPoint[i] == "winFlag")
			{
				CCLabelTTF* temp = dynamic_cast<CCLabelTTF*>(scene->getChildByTag(1)->getChildByTag(100));
				j.checkResult.push_back(temp->getString());
			}else if (c.checkPoint[i] == "selfSum")
			{
				PlayerLayer* temp = dynamic_cast<PlayerLayer*>(scene->getChildByTag(1)->getChildByTag(11));
				std::stringstream ss;
				std::string str;
				ss << temp->getSum();
				ss >> str;
				j.checkResult.push_back(str);
			}else if (c.checkPoint[i] == "enemySum")
			{
				PlayerLayer* temp = dynamic_cast<PlayerLayer*>(scene->getChildByTag(1)->getChildByTag(12));
				std::stringstream ss;
				std::string str;
				ss << temp->getSum();
				ss >> str;
				j.checkResult.push_back(str);
			}else if (c.checkPoint[i] == "selfAddCardNum")
			{
				j.checkResult.push_back("1");
			}else if (c.checkPoint[i] == "enemyAddCardNum")
			{
				j.checkResult.push_back("0");
			}
		}
	}
	return j;
}