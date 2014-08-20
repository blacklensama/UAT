#include "BlackJack.h"
#include "SDK/socket.h"
#include "CardandAI.h"

USING_NS_CC;

ComputerAI myAI;
RandomCard myRandomCard;
int nocard_flag = 0;			//不继续要牌的人 0:都继续要 1：玩家不要 2：AI不要 3：都不要

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

	scene->addChild(layer, 0, 1);

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

	this->addChild(result, 0, 100);

	CCMenuItemImage* i = CCMenuItemImage::create("restart.png", "restart.png", 
		this, menu_selector(BlackJack::restart));

	resultMenu = CCMenu::create(i, NULL);

	resultMenu->setVisible(false);

	resultMenu->setPosition(ccp(visibleSize.width/8*3, visibleSize.width/8*3));

	this->addChild(resultMenu, 0, 200);
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

	std::cout << "BlackJack::init()" <<std::endl;

	myAI.config(10,20);
	std::cout << "RandomCard.initCard()" <<std::endl;
	myRandomCard.initCard();
	nocard_flag = 0;

	CCSprite* sprite = CCSprite::create("writebg.png");

	sprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	this->addChild(sprite, 0, 1);

	std::cout << "PlayerLayer1" <<std::endl;
	PlayerLayer* pl1 = PlayerLayer::create();
	pl1->setAI(false);
	pl1->setPosition(ccp(visibleSize.width/16, visibleSize.height/12.8));	
//	pl1->useMenu(false);

	std::cout << "PlayerLayer2" <<std::endl;
	PlayerLayer* pl2 = PlayerLayer::create();
	pl2->setAI(true);
	pl2->setPosition(ccp(visibleSize.width/2, visibleSize.height/1.6));
//	pl2->useMenu(false);

// 	pl1->addCard(NULL);
// 
// 	pl1->addCard(NULL);
// 
// 	pl2->addCard(NULL);
// 
// 	pl2->addCard(NULL);
	CCSprite* sprite_back = CCSprite::create("pukeImage1-new/back.jpg");
	sprite_back->setPosition(ccp(visibleSize.width/16 + visibleSize.width/12*1,visibleSize.width/16));
	pl2->addChild(sprite_back,1,1001);

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
	setAI(false);
	restart();	
	std::cout << this->getChildrenCount() << std::endl;
	return true;
}

void PlayerLayer::restart()
{
	std::cout << "PlayerLayer::restart()" <<std::endl;
	sum = 0;

	this->removeAllChildren();

	card.clear();

	useMenu(true);

	addCard(NULL);
	addCard(NULL);
}

void PlayerLayer::resetMenu(bool flag)
{
	this->getChildByTag(20)->setVisible(flag);
}

void PlayerLayer::useMenu(bool flag = true)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	std::cout << "PlayerLayer::useMenu" <<  flag << std::endl;

	CCLabelTTF* ttf = CCLabelTTF::create("player", "Arial", 12);

	CCLabelTTF* ttf1 = CCLabelTTF::create("0", "Arial", 12);

	ttf->setPosition(ccp(origin.x, visibleSize.height/3));

	ttf->setColor(ccc3(0,0,0));

	ttf1->setPosition(ccp(visibleSize.width/8, visibleSize.height/3));

	ttf1->setColor(ccc3(0,0,0));

	ttf->setVisible(flag);

	ttf1->setVisible(flag);

	this->addChild(ttf, 0, 1);

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

	//int num = rand()%13 + 1;
	//sum += num;
	int num,kind;
	int org_num = myRandomCard.getOneCard(num,kind);
	//num = 1;		//便于调试AI行为
	if (num>=10)
	{
		sum += 10;
	}
	else
	{
		sum += num;
	}
	
	card.push_back(org_num);

	std::stringstream ss;
	std::string str;

	//ss << num;
	//ss >> str;
	
	//str = "pukeImage1/" + str;
	//str += ".jpg";

	ss << num << "_" << kind <<".jpg";
	ss >> str;

	std::cout << "AddCard-"<<isAI<<":" << str << std::endl;

	str = "pukeImage1-new/" + str;

	CCSprite* sprite = CCSprite::create(str.c_str());

	sprite->setPosition(ccp(visibleSize.width/16 + visibleSize.width/12*(card.size()-1),visibleSize.width/16));

	this->addChild(sprite,0,100+card.size()-1);

	if (card.size() >2 && isAI)
	{
		CCSprite* sprite_back = CCSprite::create("pukeImage1-new/back.jpg");
		sprite_back->setPosition(ccp(visibleSize.width/16 + visibleSize.width/12*(card.size()-1),visibleSize.width/16));
		this->addChild(sprite_back,1,1000+card.size()-1);
	}
		
	resetTheSum();

	if (sum > 21)
	{
		//((BlackJack*)this->getParent())->over();
		((BlackJack*)this->getParent())->calcResult();
	}
	else
	{
		if ( card.size() >2 && !isAI && nocard_flag < 2)
		{
			PlayerLayer* ai;
			ai = ((PlayerLayer*)this->getParent()->getChildByTag(12));
			if ( myAI.NextStep(ai->getSum()))
			{
				ai->addCard(NULL);
			}
			else
			{
				ai->calc(NULL);
			}		
		}
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
	resetMenu(false);
	
	if (isAI)
	{
		nocard_flag += 2;
	} 
	else
	{
		nocard_flag += 1;
		if (nocard_flag < 2)
		{
			PlayerLayer* ai;
			ai = ((PlayerLayer*)this->getParent()->getChildByTag(12));
			while(myAI.NextStep(ai->getSum()))
			{
				ai->addCard(NULL);
			}
			ai->calc(NULL);
		}		
	}

	if (nocard_flag>=3)
	{
		BlackJack* b = (BlackJack*)this->getParent();

		b->calcResult();
	}
	
}

int PlayerLayer::getSum()
{
	return sum;
}

void PlayerLayer::setAI(bool flag = true)
{
	isAI = flag;
}

int PlayerLayer::openCard()
{
	if (!isAI)
	{
		return -1;
	}
	for (int i=1001;i<=1000+card.size()-1;i++)
	{
		getChildByTag(i)->setVisible(false);
	}
	return 0;
}

void BlackJack::calcResult()
{
	((PlayerLayer*)this->getChildByTag(12))->openCard();	
	
	int num1 = ((PlayerLayer*)this->getChildByTag(11))->getSum();

	int num2 = ((PlayerLayer*)this->getChildByTag(12))->getSum();

	if (( num1 <= 21 && num1 > num2 )||( num2 > 21 ))
	{
		win();	
	}
	else
	{
		over();
	}

}

void BlackJack::win()
{
	((PlayerLayer*)this->getChildByTag(11))->resetMenu(false);

	((PlayerLayer*)this->getChildByTag(12))->resetMenu(false);

	result->setString("You Win!^_^");

	result->setVisible(true);

	resultMenu->setVisible(true);
}

void BlackJack::over()
{
	((PlayerLayer*)this->getChildByTag(11))->resetMenu(false);

	((PlayerLayer*)this->getChildByTag(12))->resetMenu(false);

	result->setString("You Lost!T_T");

	result->setVisible(true);

	resultMenu->setVisible(true);
}

void BlackJack::restart(CCObject* pSender)
{
	std::cout << "RandomCard.initCard()_restart" <<std::endl;
	myRandomCard.initCard();
	nocard_flag = 0;	

	((PlayerLayer*)this->getChildByTag(11))->restart();

	((PlayerLayer*)this->getChildByTag(12))->restart();

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSprite* sprite_back = CCSprite::create("pukeImage1-new/back.jpg");
	sprite_back->setPosition(ccp(visibleSize.width/16 + visibleSize.width/12*1,visibleSize.width/16));
	((PlayerLayer*)this->getChildByTag(12))->addChild(sprite_back,1,1001);

	result->setVisible(false);

	resultMenu->setVisible(false);
}
