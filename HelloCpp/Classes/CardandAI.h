#pragma once

#define MAX_CARD 52				//52=不含大小王
class RandomCard
{
public:
	int initCard();				//初始化牌堆
	int getOneCard(int &num,int &kind);
protected:
private:
	int CardPile[MAX_CARD];		//牌堆
	int index;					//当前已发牌数
};

class ComputerAI
{
public:
	bool NextStep(int num);		//根据当前输入决定下一步行动
	void config(int mustYes,int mustNo);//AI配置
protected:
private:
	int must_Yes;
	int must_No;
};


