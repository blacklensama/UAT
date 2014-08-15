#include "BlackJack.h"
#include "socket.h"

USING_NS_CC;

CCScene* BlackJack::scene()
{
	

	CCScene* scene = CCScene::create();

	BlackJack* layer = BlackJack::create();

	scene->addChild(layer);

	return scene;
}

void BlackJack::initMenu()
{
	result = CCLabelTTF::create("0", "Arial", 24);
	
	result->setColor(ccc3(0, 0, 0));

	result->setPosition(ccp(300, 400));

	result->setVisible(false);

	this->addChild(result);

	CCMenuItemImage* i = CCMenuItemImage::create("restart.png", "restart.png", 
		this, menu_selector(BlackJack::restart));

	resultMenu = CCMenu::create(i, NULL);

	resultMenu->setVisible(false);

	resultMenu->setPosition(ccp(300, 330));

	this->addChild(resultMenu);
}

bool BlackJack::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCSprite* sprite = CCSprite::create("writebg.png");

	sprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	this->addChild(sprite);

	int x = 0;
	int y = 350;
	for (int i = 0; i < 50; i++)
	{
		CCSprite* temp = CCSprite::create("pukeImage1/3.jpg");
		temp->setPosition(ccp(x, y));
		x += 10;
		//this->addChild(temp);
	}

	PlayerLayer* pl1 = PlayerLayer::create();

	pl1->setPosition(ccp(50, 50));

	pl1->useMenu(true);

	PlayerLayer* pl2 = PlayerLayer::create();

	pl2->setPosition(ccp(400, 400));

	pl2->useMenu(false);

	pl1->addCard(NULL);

	pl1->addCard(NULL);

	pl2->addCard(NULL);

	pl2->addCard(NULL);

	this->addChild(pl1, 11, 11);

	this->addChild(pl2, 12, 12);

	initMenu();

	return true;
}

bool PlayerLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	restart();

	//useMenu();

	

	return true;
}

void PlayerLayer::restart()
{
	sum = 0;

	this->removeAllChildren();

	card.clear();

	useMenu(true);
}

void PlayerLayer::useMenu(bool flag = true)
{
	CCLabelTTF* ttf = CCLabelTTF::create("player", "Arial", 24);

	CCLabelTTF* ttf1 = CCLabelTTF::create("0", "Arial", 24);

	ttf->setPosition(ccp(0,150));

	ttf->setColor(ccc3(0,0,0));

	ttf1->setPosition(ccp(100, 150));

	ttf1->setColor(ccc3(0,0,0));

	ttf->setVisible(flag);

	ttf1->setVisible(flag);

	this->addChild(ttf);

	this->addChild(ttf1, 0, 10);

	CCMenuItemImage* addCard = CCMenuItemImage::create("addCard.png", "addCard.png", 
							this, menu_selector(PlayerLayer::addCard));

	addCard->setPosition(ccp(200, 200));

	CCMenuItemImage* noCard = CCMenuItemImage::create("noCard.png", "noCard.png",
							this, menu_selector(PlayerLayer::calc));

	noCard->setPosition(ccp(300, 200));

	CCMenu* menu = CCMenu::create(addCard, noCard, NULL);

	menu->setVisible(flag);

	menu->setPosition(ccp(0, 0));

	this->addChild(menu, 20, 20);
}

void PlayerLayer::addCard(CCObject* pSender)
{
	int num = rand()%13 + 1;
	sum += num;

	card.push_back(num);

	std::stringstream ss;
	std::string str;

	ss << num;
	ss >> str;
	str = "pukeImage1/" + str;
	str += ".jpg";

	CCSprite* sprite = CCSprite::create(str.c_str());

	sprite->setPosition(ccp(50 + 100*(card.size()-1),50));

	this->addChild(sprite);
	

	resetTheSum();

	if (sum > 21)
	{
		
	}
}

void PlayerLayer::resetTheSum()
{
	CCLabelTTF* ttf1 = (CCLabelTTF*)this->getChildByTag(10);
	std::string str;
	std::stringstream ss;
	ss << sum;
	ss >> str;
	ttf1->setString(str.c_str());
}

void PlayerLayer::calc(CCObject* pSender)
{
	BlackJack* b = (BlackJack*)this->getParent();

	b->calcResult();
}

int PlayerLayer::getSum()
{
	return sum;
}

void BlackJack::calcResult()
{
	int num1 = ((PlayerLayer*)this->getChildByTag(11))->getSum();

	int num2 = ((PlayerLayer*)this->getChildByTag(12))->getSum();

	if (num1 > num2)
	{
		win();	
	}else
	{
		over();
	}

}

void BlackJack::win()
{
	((PlayerLayer*)this->getChildByTag(11))->restart();

	((PlayerLayer*)this->getChildByTag(12))->restart();

	result->setString("win");

	result->setVisible(true);

	resultMenu->setVisible(true);
}

void BlackJack::over()
{
	((PlayerLayer*)this->getChildByTag(11))->restart();

	((PlayerLayer*)this->getChildByTag(12))->restart();

	result->setString("over");

	result->setVisible(true);

	resultMenu->setVisible(true);
}

void BlackJack::restart(CCObject* pSender)
{
	result->setVisible(false);

	resultMenu->setVisible(false);
}