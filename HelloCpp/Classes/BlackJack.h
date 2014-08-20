#pragma once

#include "cocos2d.h"

class PlayerLayer : public cocos2d::CCLayer
{
public:
	virtual bool init();
	CREATE_FUNC(PlayerLayer);
	void addCard(CCObject* pSender);
	void restart();
	void useMenu(bool flag);
	void resetMenu(bool flag);
	void resetTheSum();
	void calc(CCObject* pSender);
	int getSum();
	void setAI(bool flag);
	int openCard();
protected:
private:
	std::list<int> card;
	bool isAI;			//false为玩家，true为电脑
	int sum;
};

class BlackJack : public cocos2d::CCLayer
{
public:
	virtual bool init();

	void initMenu();

	static cocos2d::CCScene* scene();

	void calcResult();

	virtual void keyBackClicked();

	CREATE_FUNC(BlackJack);

	void win();

	void over();

	void restart(CCObject* pSender);
protected:
private:


	PlayerLayer* pl1;
	PlayerLayer* pl2;

	cocos2d::CCLabelTTF* result;

	cocos2d::CCMenu* resultMenu;
};