#include "pch.h"
#include "game.h"
#include "CDlgGame.h"
#include "CDlgLink.h"
#include "gongzhuDlg.h"
#include "net.h"


void Game::Deal() //主机发牌
{
	
	int Card[4][13] = { 0 };//存放分好的牌堆
	int CardNum[4] = { 0 };//四个牌堆的牌数
	for (int i = 1; i <= 52;)
	{
		int pick = rand() % 4;//当前发牌对象对应的order
		if (CardNum[pick] >= 13)
			continue;//当前发牌对象手牌已满13张，跳过本次发牌；
		else
		{
			Card[pick][CardNum[pick]] = i;//把牌放入牌堆
			CardNum[pick]++;
			i++;
		}
	}
	//主机的牌堆直接保存
	for (int i = 0; i < 13; i++)
		myCard[i].data = Card[0][i];

	//客户端牌堆化成信号发送给客户端
	string Transmission[3] = { "*","*","*" };//数组下标为order-1
	//编码规则：  “*”+“牌编号”+'!'+“牌编号”+'!'+...+“!”如："*1!2!3!4!5!6!7!8!9!10!11!12!13!"
	for (int i = 1; i <= 3; i++) {
		for (int j = 0; j <= 12; j++)
		{
			Transmission[i - 1] += to_string(Card[i][j]) + "!";
		}
	}

	//分别传递Transmission[3]给其余三个客户端
	for (int i = 1; i < 4; i++)
		g_pServer->Send_Message(Transmission[i - 1], g_pUser[i]->receiver);

}



int Game::JudgeDecor(int card)//根据牌判断花色
{
	if (card >= 1 && card <= 13)
		return 1;
	if (card >= 14 && card <= 26)
		return 2;
	if (card >= 27 && card <= 39)
		return 3;
	if (card >= 40 && card <= 52)
		return 4;
	return -1;
}

int Game::JudgeLargest()//根据Save判断谁最大，返回最大出牌的玩家order
{
	int max = 0, min = 0;

	switch (JudgeDecor(Save[0]))
	{
	case 1:max = 13; min = 1; break;
	case 2:max = 26; min = 14; break;
	case 3:max = 39; min = 27; break;
	case 4:max = 52; min = 40;
	}

	int maxSave = 0;//保存最大出牌的下标
	for (int i = 0; i < 4; i++)
		if (Save[i] >= min && Save[i] <= max)
		{
			if (Save[i] == min)//A在同花色中最大
			{
				maxSave = i;
				break;
			}
			else
			{
				if (Save[i] > Save[maxSave])
					maxSave = i;
			}			
		}
			

	return (CurrentOrder + maxSave - 1) % 4 + 1;

}

void Game::ArrangeCard()//整理myCard[13]
{
	for (int i = 0; i < 12; i++)
	{
		if (myCard[i].data == 0 && myCard[i + 1].data != 0)
		{
			for (int j = i; j < 12; j++)
			{
				myCard[j].data = myCard[j + 1].data;
				myCard[j + 1].data = 0;
			}
			break;
		}
			
	}
}

int GetGold(int id_user)//根据ID读取gold
{
	int IDtemp = 0, Goldtemp = 0;
	fstream mystream("Sav.dat", ios::in | ios::binary);//判断文件是否存在
	if (mystream.good())//文件存在
	{
		while (!mystream.eof())
		{
			mystream.read((char*)&IDtemp, sizeof(int));
			mystream.read((char*)&Goldtemp, sizeof(int));

			if (IDtemp == id_user)//读取到的ID匹配
			{
				mystream.close();
				return Goldtemp;
			}
		}

		mystream.clear();//读取不到
		mystream.close();
		return 1000;

	}
	else//文件不存在
		return 1000;

}