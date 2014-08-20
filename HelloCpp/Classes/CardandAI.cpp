//////////////////////////////////////////////////////////////////////////
//���߼����㣬���ڼ��㷢���Լ�AI
//��cocos�޹�
//����
//		0-52����ƬA-÷��A-����A-����A-��Ƭ2-....-����K��
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
//	srand( 1 );				//���ڲ��ԣ����ӹ̶���
	for (i=0;i<100;i++)		//�������100��
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
//���һ����
//num��������1-13��A-K��	kind����ɫ��1-4����Ƭ-���ң�
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
//��ʼ��AI����
//����С�ڵ���mustYes��һ��Ҫ��һ��	�������ڵ���mustNo��һ����Ҫ��һ��
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
//���ݵ�ǰ���������һ���ж�
//����false��ʾ��Ҫ������true��ʾҪ
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
