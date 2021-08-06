#pragma once
#include <iostream>
#include <ctime>
#include <cstdio>
#include <string>

using namespace std;

/*
int������(�ֱ��A��K����)
����: 1~13 ��Ƭ	14~26 ÷��	27~39 ����	40~52 ����
��ɫ: 1��Ƭ 2÷�� 3���� 4����
������: 11,23(TRAN),51,27~39
ÿ����ҵ�OrderΪ1~4���߼�����ʱ�����

*/
struct Card
{
	int data = 0;
	bool Usable = false;
};
class Game
{
public:
	Card myCard[13];//�Լ����ƿ�
	int Collection[4][16] = { 0 };//�����ƿ⣬��ά�����±��Ӧ���order-1
	bool BTrans = false, BPig = false, BSheep = false, BHeartA = false;//�Ƿ�����
	int CurrentOrder = 0;//��ǰ�غϳ��Ƶ����ID
	int Save[4] = { 0 };//���α���һ�غ���ҵĳ���
	int PlayNum = 0;//��ǰ�غ��Ѿ����������
	int Round = 0;//��ǰ��Ϸ�Ѿ����еĻغ���
	int PlayedCollection = 0;//�Ѿ����˵�������

	void Deal();//�����˷��ƣ�������ʼ��4���ƶ���ַ��Ƹ�4�����
	int JudgeDecor(int card);//�������жϻ�ɫ
	int JudgeLargest();//����Save�ж�˭��󣬷��������Ƶ����order
	void ArrangeCard();//����myCard[13]
}; 
int GetGold(int id_user);//����ID��ȡgold
