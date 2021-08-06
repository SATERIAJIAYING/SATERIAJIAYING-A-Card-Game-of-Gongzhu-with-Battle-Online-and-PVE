#pragma once




class Com
{
public:
	void AcceptShowdownMsg(int order, bool sheep, bool trans, bool heartA, bool pig);//接收亮牌信息
	bool Showdown_Sheep();
	bool Showdown_Trans();
	bool Showdown_HeartA();
	bool Showdown_Pig();
	void AcceptPlayMsg(int order, int card);//接收出牌信号
	int Play();//出牌


	bool recorder[53] = { 0 }; //记牌器
	int CardNum[4] = { 0 };//记录各花色所剩手牌数
	int desire1 = 2, desire2 = 2, desire3 = 2, desire4 = 2;//欲望值,分别对应变压器、猪、羊、红桃A
	int ChaseHeart=0;//追求满红模式，1为开启，0为关闭负数为防止负数绝对值order玩家满红
	bool cardbright[4][4] = { 0 };//记录其他玩家亮牌情况
	//cardbrigjt[][0]对应羊，1、2、3依次对应变压器、红桃A、猪                                

	int ColourNum(int);//返回自己手牌中某花色的牌数,参数代表花色
	bool Own(int);//判断是否含有某牌,参数代表判断牌
	int Discard();//参数返回易脱手的花色数量
	bool IfBright(int m, int n);//m始终小于n
	void Recorder(int);//记牌器
	int SafeMin(int, int);//返回安全牌小值（指定参数最大值以下）
	int SafeMax(int,int,int);//返回安全牌大值(参数对应三张牌,第一个int决定花色)	
	bool Judge(int);//判断是否有对应花色的手牌
	void SizeSort();//记录各花色所剩手牌数
	bool Node();//判断是否出现比赛节点
	bool ComingOver();//判断游戏是否即将结束
	int MinColour();//返回不为0最小花色数
	int GameMaxColour();//记录场上所剩最大花色数
	void CardBright(int,int,bool);//记录其他玩家亮牌情况
	bool MinCard(int);//判断某种花色是否有小牌	
	int Play2();//（困难）第二个出牌
	int Play3();//（困难）第三个出牌
	int Play4();//（困难）末位出牌
	int Play5();//中等难度，一个函数决定所有出牌判断

	int Play1();//（困难）庄家
	int Min(int);//返回对应花色牌点数最小值
	int Max(int);//返回对应花色牌点数最大值
	bool NoDifficultPig();//猪刀(含黑桃Q、K、A一张)
	bool NoDifficultHeartA();//红桃刀（含有红桃A，7~~13中的一张）

	void JudgeChaseHeart();//判断是否启动追求满红模式，最高优先级
};