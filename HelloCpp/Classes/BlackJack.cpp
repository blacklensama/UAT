#include "BlackJack.h"
#include "socket.h"

USING_NS_CC;


void BlackJack::keyBackClicked()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}

CCScene* BlackJack::scene()
{
	

	CCScene* scene = CCScene::create();

	BlackJack* layer = BlackJack::create();

	scene->addChild(layer);

	return scene;
}

void BlackJack::initMenu()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	result = CCLabelTTF::create("0", "Arial", 24);
	
	result->setColor(ccc3(0, 0, 0));

	result->setPosition(ccp(visibleSize.width/8*3, visibleSize.width/2));

	result->setVisible(false);

	this->addChild(result);

	CCMenuItemImage* i = CCMenuItemImage::create("restart.png", "restart.png", 
		this, menu_selector(BlackJack::restart));

	resultMenu = CCMenu::create(i, NULL);

	resultMenu->setVisible(false);

	resultMenu->setPosition(ccp(visibleSize.width/8*3, visibleSize.width/8*3));

	this->addChild(resultMenu);
}

bool BlackJack::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	this->setKeypadEnabled(true);

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	std::cout << visibleSize.width << " " << visibleSize.height << std::endl;

	std::cout << origin.x << " " << origin.y << std::endl;


	CCSprite* sprite = CCSprite::create("writebg.png");

	sprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	this->addChild(sprite);

	PlayerLayer* pl1 = PlayerLayer::create();

	pl1->setPosition(ccp(visibleSize.width/16, visibleSize.height/12.8));

	pl1->useMenu(false);

	PlayerLayer* pl2 = PlayerLayer::create();

	pl2->setPosition(ccp(visibleSize.width/2, visibleSize.height/1.6));

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
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	std::cout << visibleSize.width;

	CCLabelTTF* ttf = CCLabelTTF::create("player", "Arial", 12);

	CCLabelTTF* ttf1 = CCLabelTTF::create("0", "Arial", 12);

	ttf->setPosition(ccp(origin.x, visibleSize.height/3));

	ttf->setColor(ccc3(0,0,0));

	ttf1->setPosition(ccp(visibleSize.width/8, visibleSize.height/3));

	ttf1->setColor(ccc3(0,0,0));

	ttf->setVisible(flag);

	ttf1->setVisible(flag);

	this->addChild(ttf);

	this->addChild(ttf1, 0, 10);

	CCMenuItemImage* addCard = CCMenuItemImage::create("addCard.png", "addCard.png", 
							this, menu_selector(PlayerLayer::addCard));

	addCard->setPosition(ccp(visibleSize.width/4,visibleSize.height/3.2));

	CCMenuItemImage* noCard = CCMenuItemImage::create("noCard.png", "noCard.png",
							this, menu_selector(PlayerLayer::calc));

	noCard->setPosition(ccp(visibleSize.width/8*3, visibleSize.height/3.2));

	CCMenu* menu = CCMenu::create(addCard, noCard, NULL);

	menu->setVisible(flag);

	menu->setPosition(ccp(0, 0));

	this->addChild(menu, 20, 20);
}

void PlayerLayer::addCard(CCObject* pSender)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

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

	sprite->setPosition(ccp(visibleSize.width/16 + visibleSize.width/8*(card.size()-1),visibleSize.width/16));

	this->addChild(sprite);
	

	resetTheSum();

	if (sum > 21)
	{
		((BlackJack*)this->getParent())->over();
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

	result->setString("win");

	result->setVisible(true);

	resultMenu->setVisible(true);
}

void BlackJack::over()
{
	
	result->setString("over");

	result->setVisible(true);

	resultMenu->setVisible(true);
}

void BlackJack::restart(CCObject* pSender)
{
	((PlayerLayer*)this->getChildByTag(11))->restart();

	((PlayerLayer*)this->getChildByTag(12))->restart();

	result->setVisible(false);

	resultMenu->setVisible(false);
}