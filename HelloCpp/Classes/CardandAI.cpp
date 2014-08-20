//////////////////////////////////////////////////////////////////////////
//纯逻辑计算，用于计算发牌以及AI
//与cocos无关
//牌序：
//		0-52：方片A-梅花A-红桃A-黑桃A-方片2-....-黑桃K；
//////////////////////////////////////////////////////////////////////////
#include "CardandAI.h"
#include "stdlib.h"
#include "time.h"
#include <iostream>

int RandomCard::initCard()
{
	int i,j,k;
	index = 0;
	for (i=0;i<MAX_CARD;i++)
	{
		CardPile[i]=i;
	}
	srand(time(NULL));
//	srand( 1 );				//便于测试，种子固定了
	for (i=0;i<100;i++)		//随机交换100次
	{
		j=rand()%MAX_CARD;
		k=rand()%MAX_CARD;
		if (j!=k)
		{
			CardPile[j]=CardPile[j]^CardPile[k];
			CardPile[k]=CardPile[j]^CardPile[k];
			CardPile[j]=CardPile[j]^CardPile[k];
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//获得一张牌
//num：点数（1-13：A-K）	kind：花色（1-4：方片-黑桃）
//////////////////////////////////////////////////////////////////////////
int RandomCard::getOneCard(int &num,int &kind)
{
	if (index>=MAX_CARD)
	{
		return -1;
	}
	num = CardPile[index]%13+1;
	kind = CardPile[index]%4+1;
	return CardPile[index++];
}

//////////////////////////////////////////////////////////////////////////
//初始化AI参数
//点数小于等于mustYes则一定要下一张	点数大于等于mustNo则一定不要下一张
//////////////////////////////////////////////////////////////////////////
void ComputerAI::config(int mustYes,int mustNo)
{
	must_Yes = mustYes;
	must_No = mustNo;

	if (mustYes > mustNo)
	{
		must_Yes = mustYes;
		must_No = mustNo;
	}
	if (mustYes == mustNo)
	{
		must_Yes = mustYes;
		must_No = mustNo+1;
	}
}

//////////////////////////////////////////////////////////////////////////
//根据当前输入决定下一步行动
//返回false表示不要，返回true表示要
//////////////////////////////////////////////////////////////////////////
bool ComputerAI::NextStep(int num)
{
	if ( num <= must_Yes)
	{
		std::cout<<"mustYes"<<std::endl;
		return true;
	} 
	else if (num >= must_No)
	{
		std::cout<<"mustNo"<<std::endl;
		return false;
	} 
	else
	{
		int th = (int)(100.0/(must_No-must_Yes)*(must_No-num)+0.5);
		int temp = (rand()%100);
		std::cout<<"th="<<th<<" temp="<<temp<<std::endl;
		return temp<=th;
	}

}
