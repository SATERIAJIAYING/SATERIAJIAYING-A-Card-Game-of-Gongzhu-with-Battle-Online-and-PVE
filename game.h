#pragma once
#include <iostream>
#include <ctime>
#include <cstdio>
#include <string>

using namespace std;

/*
int索引：(分别从A到K排序)
手牌: 1~13 方片	14~26 梅花	27~39 红桃	40~52 黑桃
花色: 1方片 2梅花 3红桃 4黑桃
特征牌: 11,23(TRAN),51,27~39
每个玩家的Order为1~4，逻辑上逆时针递增

*/
struct Card
{
	int data = 0;
	bool Usable = false;
};
class Game
{
public:
	Card myCard[13];//自己的牌库
	int Collection[4][16] = { 0 };//特征牌库，二维数组下标对应玩家order-1
	bool BTrans = false, BPig = false, BSheep = false, BHeartA = false;//是否亮牌
	int CurrentOrder = 0;//当前回合出牌的玩家ID
	int Save[4] = { 0 };//依次保留一回合玩家的出牌
	int PlayNum = 0;//当前回合已经出牌玩家数
	int Round = 0;//当前游戏已经进行的回合数
	int PlayedCollection = 0;//已经出了的特征牌

	void Deal();//主机端发牌，包含初始化4个牌堆与分发牌给4个玩家
	int JudgeDecor(int card);//根据牌判断花色
	int JudgeLargest();//根据Save判断谁最大，返回最大出牌的玩家order
	void ArrangeCard();//整理myCard[13]
}; 
int GetGold(int id_user);//根据ID读取gold
