#pragma once

#define MAX_CARD 52				//52=������С��
class RandomCard
{
public:
	int initCard();				//��ʼ���ƶ�
	int getOneCard(int &num,int &kind);
protected:
private:
	int CardPile[MAX_CARD];		//�ƶ�
	int index;					//��ǰ�ѷ�����
};

class ComputerAI
{
public:
	bool NextStep(int num);		//���ݵ�ǰ���������һ���ж�
	void config(int mustYes,int mustNo);//AI����
protected:
private:
	int must_Yes;
	int must_No;
};


