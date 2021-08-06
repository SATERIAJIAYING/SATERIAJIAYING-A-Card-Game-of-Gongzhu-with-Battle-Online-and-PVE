#include "pch.h"
#include "Com.h"
#include "CDlgGame.h"
#include "CDlgLink.h"
#include "game.h"
#include "gongzhuDlg.h"
#include"cmath"

void Com::AcceptShowdownMsg(int order, bool sheep, bool trans, bool heartA, bool pig)//接收亮牌信息
{
	if (g_order - order == 2 || g_order - order == -2)//判断对家亮牌
	{
		if (sheep) desire1 += 3;//Case1
		if (pig) desire1 += 3;//Case2
		if (heartA) desire1 += 3;//Case3
	}
	else//判断非对家亮牌
	{
		if (sheep) desire1 -= 3;//Case4
		if (pig) desire1 -= 3;//Case5
		if (heartA) desire1 -= 3;//Case6
	}
	CardBright(order - 1, 0, sheep);
	CardBright(order - 1, 1, trans);
	CardBright(order - 1, 2, heartA);
	CardBright(order - 1, 3, pig);
}

void Com::CardBright(int order, int num,bool judge)
{
	cardbright[order - 1][num] = judge;
}
bool Com::Showdown_Sheep()
{
	//欲望值计算
	bool Bsheep;
	int y = 0, i;
	for (i = 0; i < 13; i++)//y赋值
		if (g_pGame->myCard[i].data == 1 || g_pGame->myCard[i].data == 12 || g_pGame->myCard[i].data == 13) y++;;
	if (ColourNum(1) >= 4) desire3++;//Case7
	desire3 += y;//Case8
	if (ColourNum(1) <= 2) desire3 -= (3 - ColourNum(1));//Case9
	if (Own(11) == false) Bsheep = false;
	else
	{
		if (desire3 <= 2) Bsheep = false;
		else Bsheep = IfBright(desire3, 6);//判断是否亮羊
	}

	return Bsheep;
}
bool Com::Showdown_Trans()
{
	//欲望值计算
	bool Btrans;
	if (ColourNum(2) >= 4) desire1++;//Case10
	if (ColourNum(2) <= 2) desire1 -= (3 - ColourNum(2));//Case11
	if (Own(23) == false) Btrans = false;
	else
	{
		if (desire1 <= 4) Btrans = false;
		else Btrans = IfBright(desire1, 11);//判断是否亮变压器
	}

	return Btrans;
}
bool Com::Showdown_HeartA()
{
	//欲望值计算
	int z = 0, i, m = Discard();
	bool BheartA;
	for (i = 0; i < 13; i++)//z赋值
		if (g_pGame->myCard[i].data == 28 || g_pGame->myCard[i].data == 29 || g_pGame->myCard[i].data == 30) z++;
	if (m >= 1) desire4 += m;//Case12
	desire4 += z;//Case13
	if (ColourNum(3) >= 5 && Own(39) == true) desire4++;//Case14
	if (ColourNum(3) >= 5 && Own(37) == true) desire4++;//Case15
	if (ColourNum(3) >= 5 && Own(38) == true) desire4++;//Case16
	if (ColourNum(3) >= 5) desire4++;//Case17
	if (Own(27) == false) BheartA = false;
	else
	{
		if (desire4 <= 4) BheartA = false;
		else BheartA = IfBright(desire4, 12);//判断是否亮红桃A
	}

	return BheartA;
}
bool Com::Showdown_Pig()
{
	//欲望值计算
	int x = 0, i, m = Discard();
	bool Bpig;
	for (i = 0; i < 13; i++)//x赋值
		if (g_pGame->myCard[i].data == 40 || g_pGame->myCard[i].data == 52) x++;;
	if (ColourNum(4) >= 4) desire2++;//Case18
	desire2 += (x + m);//Case19
	if (ColourNum(4) <= 2) desire2 -= (3 - ColourNum(4));//Cas20
	if (Own(51) == false) Bpig = false;
	else
	{
		if (desire2 <= 3) Bpig = false;
		else Bpig = IfBright(desire2, 8);//判断是否亮猪
	}

	return Bpig;
}



void Com::AcceptPlayMsg(int order, int card)//接收出牌信号
{//temp1保存第一名玩家的花色,1234分别对应一种花色
	Recorder(card);






}

bool Com::ComingOver()
{
	if (g_pGame->Round >= 8) return true;
	else return false;
}
int Com::Play()//出牌，返回值是0~12中要出的手牌对应的数组下标
{//temp1保存第一名玩家的花色，若是首个出牌玩家，-1则为梅花2, 0可出任意牌，1234分别对应一种花色
	int myPlay = 53, i, j=0,card = 0;
	bool retionality = false;//手牌合理性检测
	if (temp1 == -1)//只能出梅花2
	{
		for (i = 0; i < 13; i++)
		{
			if (g_pGame->myCard[i].data == 15)
			{
				myPlay = i;
				Recorder(15);
				return myPlay;
			}


		}
	}
	else if (g_Model == 3)//困难
	{
		if (g_order == g_pGame->CurrentOrder)//庄家
			if (Own(27) && Own(39) && Own(38) && Own(37) && ColourNum(3) >= 5) myPlay = Max(3);//追求满红
			else	myPlay = Play1();
		else if ((g_order - g_pGame->CurrentOrder + 4) % 4 == 1)//第二个出牌
			myPlay = Play2();
		else if ((g_order - g_pGame->CurrentOrder + 4) % 4 == 2)//第三个出牌
			myPlay = Play3();
		else myPlay = Play4();//末位出牌
	}
	else myPlay = Play5();//普通
	
	Recorder(myPlay);//记牌器
	for (i = 0;i < 13;i++)
		if (g_pGame->myCard[i].data == myPlay)
		{
			j = i;
			retionality = true;
		}
	//安全保护机制:若出现判断错误情况，进行合理性检测
	if (retionality) return j;//正常出牌
	else//检测有判断错误
	{
		if (temp1 == 0) return 0;
		else
		{
			if (Judge(temp1))
			{
				myPlay = Min(temp1);
				for (i = 0;i < 13;i++)
					if (g_pGame->myCard[i].data == myPlay) return i;
			}
			else return 0;
		}
	}
}








//----------------------------------------------------------------------------------------------------

int Com::Play1()
{
	int myPlay = 0;
	SizeSort();//记录各花色所剩手牌数
	int colour1 = MinColour();
	int colour2,i;
	for (i = 0;i < 13;i++)
	{
		if (MinCard(3)) colour2 = 3;
		else if (MinCard(2)) colour2 = 2;
		else if (MinCard(1)) colour2 = 1;
		else if (MinCard(4)) colour2 = 4;
		else colour2 = MinColour();
	}
	if (!ComingOver())//游戏未即将结束
	{
		if (!Node())//未出现比赛节点
		{
			if (colour1 == 1) myPlay = SafeMin(1, 11);
			else if (colour1 == 2) myPlay = SafeMin(2, 23);
			else if (colour1 == 3) myPlay = SafeMin(3, 33);
			else if (colour1 == 4) myPlay = SafeMin(4, 51);
			else
			{
				//程序错误
			}
		}
		else //出现比赛节点
		{
			if (colour1 == 1) myPlay = SafeMin(1, 8);
			else if (colour1 == 2) myPlay = SafeMin(2, 21);
			else if (colour1 == 3) myPlay =SafeMin(3, 33);
			else if (colour1 == 4) myPlay = SafeMin(4, 47);
			else
			{
				//程序错误
			}
		}
		
	}
	else//游戏即将结束
	{
		if (recorder[51] == true && Judge(4)) myPlay = SafeMin(4, 47);
		else if (recorder[1] == true && recorder[12] == true && recorder[13] == true && Own(11)) myPlay = 11;
		else if (Own(28) || Own(29) || Own(30)) myPlay = Min(3);
		else myPlay = Min(colour2);
	}
	return myPlay;
}









//--------------------------------------------------------------------------------------------------------

int Com::Play2()
{
	int myPlay = 0;
	if (!ComingOver())//游戏未即将结束
	{
		if (!Node())//未进入节点
		{
			if (Judge(temp1))//有手牌对应花色的牌
			{
				if (temp1 == 1) myPlay = SafeMin(1, 11);//方块
				else if (temp1 == 2) myPlay = SafeMin(2, 23);//梅花
				else if (temp1 == 3) myPlay = SafeMin(3, 33);//红桃
				else//黑桃
				{
					if (SafeMin(4, 51) != 51) myPlay = SafeMin(4, 51);
					else myPlay = Max(4);
				}
			}
			else//没有手牌对应花色的牌
			{
				if (g_pGame->Save[0] <= (13*temp1-3)&& g_pGame->Save[0] !=(13*temp1-12))
				{
					if (Judge(2)) myPlay = Max(2);//过滤大牌
					else if (Judge(1)) myPlay = Min(1);
					else if (Judge(4))
					{
						if (Max(4) != 51) myPlay = Max(4);
						else myPlay = SafeMin(4, 51);
					}
					else if (Judge(3)) myPlay = SafeMin(3, 36);
					else
					{
						myPlay = g_pGame->myCard[0].data;//出现错误
					}
				}
				else//认为敌家将会坐庄
				{
					if (Own(51) && temp1 != 4) myPlay = 51;//先出猪
					else if (Judge(3) && temp1 != 3) myPlay = Max(3);//再出红桃大牌
					else if (Judge(4) && temp1 != 4) myPlay = Max(4);
					else if (Judge(2) && temp1 != 2) myPlay = Max(2);
					else if (Judge(1) && temp1 != 1) myPlay = SafeMin(1, 11);
					else
					{
						myPlay = g_pGame->myCard[0].data;
					}
				}
			}
		}
		else//进入节点
		{
			if (Judge(temp1))//有手牌对应花色的牌
			{
				if (temp1 == 1) myPlay = SafeMin(1, 11);//方块
				else if (temp1 == 2) myPlay = SafeMin(2, 21);//梅花
				else if (temp1 == 3) myPlay = SafeMin(3, 33);//红桃
				else//黑桃
				{
					if (SafeMin(4, 47) != 51)
						myPlay = SafeMin(4, 47);
					else myPlay = Max(4);

				}
			}
			else//没有手牌对应花色的牌
			{
				if (g_pGame->Save[0] <= (13 * temp1 - 3) && g_pGame->Save[0] != (13 * temp1 - 12))//过滤大牌
				{
					if (Judge(2)) myPlay = Max(2);
					else if (Judge(1)) myPlay = Min(1);
					else if (Judge(4))
					{
						if (Max(4) != 51) myPlay = Max(4);
						else myPlay = SafeMin(4, 51);
					}
					else if (Judge(3)) myPlay = SafeMin(3, 36);
					else
					{
						myPlay = g_pGame->myCard[0].data;//出现错误
					}
				}
				else//认为敌家将会坐庄
				{
					if (Own(51) && temp1 != 4) myPlay = 51;//先出猪
					else if (Judge(3) && temp1 != 3) myPlay = Max(3);//再出红桃大牌
					else if (Judge(4) && temp1 != 4) myPlay = Max(4);
					else if (Judge(2) && temp1 != 2) myPlay = Max(2);
					else if (Judge(1) && temp1 != 1) myPlay = SafeMin(1, 11);
					else
					{
						myPlay = g_pGame->myCard[0].data;
					}
				}
			}
		}
	}
	else//游戏即将结束
	{
		if (Judge(temp1))//有手牌对应花色的牌
		{
			myPlay = Min(temp1);
		}
		else//没有手牌对应花色的牌
		{
			if (g_pGame->Save[0] <= (13 * temp1 - 3) && g_pGame->Save[0] != (13 * temp1 - 12))//过滤大牌
			{
				if (Judge(2)) myPlay = Max(2);
				else if (Judge(1)) myPlay = Min(1);
				else if (Judge(4))
				{
					if (Max(4) != 51) myPlay = Max(4);
					else myPlay = SafeMin(4, 51);
				}
				else if (Judge(3)) myPlay = SafeMin(3, 36);
				else
				{
					myPlay = g_pGame->myCard[0].data;//出现错误
				}
			}
			else//认为敌家将会坐庄
			{
				if (Own(51) && temp1 != 4) myPlay = 51;//先出猪
				else if (Judge(3) && temp1 != 3) myPlay = Max(3);//再出红桃大牌
				else if (Judge(4) && temp1 != 4) myPlay = Max(4);
				else if (Judge(2) && temp1 != 2) myPlay = Max(2);
				else if (Judge(1) && temp1 != 1) myPlay = SafeMin(1, 11);
				else
				{
					myPlay = g_pGame->myCard[0].data;
				}
			}
		}
	}
	return myPlay;
}

\








//----------------------------------------------------------------------------------------------------------


int Com::Play3()
{
	int myPlay = 0;
	if (g_pGame->JudgeDecor(g_pGame->Save[0]) == g_pGame->JudgeDecor(g_pGame->Save[1]))//第二位出指定花色牌
	{
		if (!ComingOver())//游戏未即将结束
		{
			if (Judge(temp1))//有此花色牌
			{
				int card = (g_pGame->Save[0] > g_pGame->Save[1]) ? g_pGame->Save[0] : g_pGame->Save[1];
				if (temp1 == 1)//方块
				{
					if (!recorder[11])//场上还有羊
					{
						if (!Own(11))//自己没有羊牌
						{
							if ((g_pGame->Save[0] > g_pGame->Save[1] && card > 11) || g_pGame->Save[0] == 1) myPlay = SafeMin(1, 11);//队友抢羊
							else   myPlay = Max(1);//自己抢羊
						}
						else//自己有羊牌
						{
							if (g_pGame->Save[0] == 1) myPlay = 11;//让队友吃羊
							else if ((g_pGame->Save[0] == 13 && Own(1)) || (g_pGame->Save[0] == 13 && recorder[1] == true)) myPlay = 11;//让队友吃羊
							else if (g_pGame->Save[0] == 12 && recorder[12] == true && recorder[13] == true) myPlay = 11;//让队友吃羊
							else myPlay = SafeMin(1, 11);//留着羊牌
						}
					}
					else//场上没有羊
					{
						if (g_pGame->Save[0] != 1 && g_pGame->Save[1] != 1) myPlay = SafeMin(1, card);
						else myPlay = Max(1);
					}
				}
				else if (temp1 == 4)//黑桃
				{
					if (SafeMin(4, 51) != 51)
						myPlay = SafeMin(4, 51);//避免得到猪牌
					else myPlay = Max(4);
				}
				else if (temp1 == 2)//梅花
				{
					if (g_pGame->Save[0] != 14 && g_pGame->Save[1] != 14)
					{
						if (SafeMin(2, card) == 23)
						{
							if (g_pGame->Save[0] > g_pGame->Save[1]) myPlay = SafeMin(2, 23);
							else myPlay = SafeMin(2, card);
						}
						else
						{
							myPlay = SafeMin(2, card);
						}
					}
					else myPlay = Max(2);
				}
				else if (temp1 == 3)
				{
					if (g_pGame->Save[0] == 27 || g_pGame->Save[0] == 39 || g_pGame->Save[0] == 38 || g_pGame->Save[0] == 37)
						myPlay = SafeMin(3, 31);//帮队友减少失分
					else
					{
						if (g_pGame->Save[0] != 27 && g_pGame->Save[1] != 27) myPlay = SafeMin(3, card);
						else myPlay = Max(3);
					}
				}
			}
			else//没有此花色牌
			{
				if (g_pGame->Save[0] != (13 * temp1 - 12) && g_pGame->Save[1] != (13 * temp1 - 12))//场上无A
				{
					if (g_pGame->Save[0] > g_pGame->Save[1])
					{
						if (Judge(4))
						{
							if (Max(4) == 51) myPlay = SafeMin(4, 51);
							else myPlay = Max(4);
						}
						else if (Judge(2))
						{
							if (Max(2) == 23) myPlay = SafeMin(2, 23);
							else myPlay = Max(2);
						}
						else if (Judge(1))
						{
							myPlay = SafeMin(1, 11);
						}
						else if (Judge(3))
						{
							myPlay = SafeMin(3, 31);
						}
						else
						{
							//程序出错
						}
					}
					else
					{
						if (Own(51) && temp1 != 4) myPlay = 51;//先出猪
						else if (Judge(3) && temp1 != 3) myPlay = Max(3);//再出红桃大牌
						else if (Judge(4) && temp1 != 4) myPlay = Max(4);
						else if (Judge(2) && temp1 != 2) myPlay = Max(2);
						else if (Judge(1) && temp1 != 1) myPlay = SafeMin(1, 11);
						else
						{
							myPlay = g_pGame->myCard[0].data;
						}
					}
				}
				else//场上有A
				{
					if (g_pGame->Save[0] == (13 * temp1 - 12))//对家出A
					{
						if (Max(4) != 51 || Min(4) != 51)//不出猪牌
						{
							if (Judge(4))
							{
								if (Max(4) == 51) myPlay = SafeMin(4, 51);
								else myPlay = Max(4);
							}
							else if (Judge(2))
							{
								if (Max(2) == 23) myPlay = SafeMin(2, 23);
								else myPlay = Max(2);
							}
							else if (Judge(1))
							{
								if (Own(11))
									myPlay = 11;
								else myPlay = SafeMin(1, 11);
							}
							else if (Judge(3))
							{
								myPlay = SafeMin(3, 31);
							}
							else
							{
								//程序出错
							}
						}
						else
						{
							if (Judge(2))
							{
								if (Max(2) == 23) myPlay = SafeMin(2, 23);
								else myPlay = Max(2);
							}
							else if (Judge(1))
							{
								if (SafeMin(1, 11) != 11)
									myPlay = SafeMin(1, 11);
								else myPlay = Max(1);
							}
							else if (Judge(3))
							{
								myPlay = SafeMin(3, 31);
							}
							else myPlay = Max(4);
						}
					}
					else//敌家出A
					{
						if (Own(51) && temp1 != 4) myPlay = 51;//先出猪
						else if (Judge(3) && temp1 != 3) myPlay = Max(3);//再出红桃大牌
						else if (Judge(4) && temp1 != 4) myPlay = Max(4);
						else if (Judge(2) && temp1 != 2) myPlay = Max(2);
						else if (Judge(1) && temp1 != 1) myPlay = SafeMin(1, 11);
						else
						{
							myPlay = g_pGame->myCard[0].data;
						}
					}
				}
			}
		}
		else//游戏即将结束
		{
			if (Judge(temp1))//有此花色
			{
				myPlay = Min(temp1);
			}
			else//没有此花色
			{
				if (g_pGame->Save[0] != 0 && g_pGame->Save[1] != 0)//场上无A
				{
					if (g_pGame->Save[0] > g_pGame->Save[1])
					{
						if (Judge(4))
						{
							if (Max(4) == 51) myPlay = SafeMin(4, 51);
							else myPlay = Max(4);
						}
						else if (Judge(2))
						{
							if (Max(2) == 23) myPlay = SafeMin(2, 23);
							else myPlay = Max(2);
						}
						else if (Judge(1))
						{
							myPlay = SafeMin(1, 11);
						}
						else if (Judge(3))
						{
							myPlay = SafeMin(3, 31);
						}
						else
						{
							//程序出错
						}
					}
					else
					{
						if (Own(51) && temp1 != 4) myPlay = 51;//先出猪
						else if (Judge(3) && temp1 != 3) myPlay = Max(3);//再出红桃大牌
						else if (Judge(4) && temp1 != 4) myPlay = Max(4);
						else if (Judge(2) && temp1 != 2) myPlay = Max(2);
						else if (Judge(1) && temp1 != 1) myPlay = SafeMin(1, 11);
						else
						{
							myPlay = g_pGame->myCard[0].data;
						}
					}
				}
				else//场上有A
				{
					if (g_pGame->Save[0] == (13 * temp1 - 12))//对家出A
					{
						if (Judge(4))
						{
							if (Max(4) == 51) myPlay = SafeMin(4, 51);
							else myPlay = Max(4);
						}
						else if (Judge(2))
						{
							if (Max(2) == 23) myPlay = SafeMin(2, 23);
							else myPlay = Max(2);
						}
						else if (Judge(1))
						{
							if (Own(11)) myPlay = 11;
							else myPlay = SafeMin(1, 11);
						}
						else if (Judge(3))
						{
							myPlay = SafeMin(3, 31);
						}
						else
						{
							//程序出错
						}
					}
					else//敌家出A
					{
						if (Own(51) && temp1 != 4) myPlay = 51;//先出猪
						else if (Judge(3) && temp1 != 3) myPlay = Max(3);//再出红桃大牌
						else if (Judge(4) && temp1 != 4) myPlay = Max(4);
						else if (Judge(2) && temp1 != 2) myPlay = Max(2);
						else if (Judge(1) && temp1 != 1) myPlay = SafeMin(1,11);
						else
						{
							myPlay = g_pGame->myCard[0].data;
						}
					}
				}
			}
		}
	}
	else//第二位未出指定花色的牌
	{
	  if (!Judge(temp1))//没有指定花色
	  {
		if (Judge(4))
		{
			if (Max(4) == 51) myPlay = SafeMin(4, 51);
			else myPlay = Max(4);
		}
		else if (Judge(2))
		{
			if (Max(2) == 23) myPlay = SafeMin(2, 23);
			else myPlay = Max(2);
		}
		else if (Judge(1))
		{
			myPlay = SafeMin(1, 11);
		}
		else if (Judge(3))
		{
			myPlay = SafeMin(3, 31);
		}
		else
		{
			//程序出错
		}
	  }
	  else//有指定花色
	  {	 
		  if (temp1 == 1)//方块
		  {
			  if (!recorder[11])//场上还有羊
			  {
				  if (!Own(11))//自己没有羊牌
				  {
					  if ((g_pGame->Save[0]  > 11) || g_pGame->Save[0] == 1) myPlay = SafeMin(1, 11);//队友抢羊
					  else   myPlay = Max(1);//自己抢羊
				  }
				  else//自己有羊牌
				  {
					  if (g_pGame->Save[0] == 1) myPlay = 11;//让队友吃羊
					  else if ((g_pGame->Save[0] == 13 && Own(1)) || (g_pGame->Save[0] == 13 && recorder[1] == true)) myPlay = 11;//让队友吃羊
					  else if (g_pGame->Save[0] == 12 && recorder[12] == true && recorder[13] == true) myPlay = 11;//让队友吃羊
					  else myPlay = SafeMin(1, 11);//留着羊牌
				  }
			  }
			  else//场上没有羊
			  {
				  if (g_pGame->Save[0] !=1) myPlay = SafeMin(1, g_pGame->Save[0]);
				  else myPlay = Max(1);
			  }
		  }
		  else if (temp1 == 4)//黑桃
		  {
			  if (SafeMin(4, 51) != 51)
				  myPlay = SafeMin(4, 51);//避免得到猪牌
			  else myPlay = Max(4);
		  }
		  else if (temp1 == 2)//梅花
		  {
			  if (g_pGame->Save[0] != 14)
			  {
				  if (SafeMin(2, g_pGame->Save[0]) == 23)
				  {
					  if (g_pGame->Save[0] > g_pGame->Save[1]) myPlay = SafeMin(2, 23);
					  else myPlay = SafeMin(2, g_pGame->Save[0]);
				  }
				  else
				  {
					  myPlay = SafeMin(2, g_pGame->Save[0]);
				  }
			  }
			  else myPlay = Max(2);
		  }
		  else if (temp1 == 3)
		  {
			  myPlay = SafeMin(3, 31);
		  }
	  }
    }
	   return myPlay;
}







//-----------------------------------------------------------------------------------------------

int  Com::Play4()
{
	int myPlay = 0, count = 0;
	count = SafeMax(g_pGame->Save[0], g_pGame->Save[1], g_pGame->Save[2]);
	if (!ComingOver())//游戏未即将结束
	{
		if (Judge(temp1))//有相应花色
		{
			if (count == (temp1 * 13 - 12) && g_pGame->Save[1] == (temp1 * 13 - 12))//对家出A
			{
				if (temp1 == 1)
				{
					if (Own(11)) myPlay = 11;//对家吃羊
					else 	myPlay = SafeMin(1, 11);//过滤小牌
				}
				else if (temp1 == 4 && Max(4) == 51) myPlay = SafeMin(4, 51);
				else if (temp1 == 2 && Max(2) == 23) myPlay = SafeMin(2, 23);
				else myPlay = SafeMin(3, 31);
			}
			else if (count == (temp1 * 13 - 12) && g_pGame->Save[1] != (temp1 * 13 - 12))//敌家出A
			{
				if (temp1 == 1)
				{
					if (Max(1) == 11) myPlay = SafeMin(1, 11);
					else myPlay = Max(1);
				}
				else if (temp1 == 4 )
				{
					if (Own(51)) myPlay = 51;
					else myPlay = SafeMin(4, 51);
				}
				else if (temp1 == 2)
				{
					if (Own(23))
					{
						if (g_pCDlgGame->m_iScore_Left < 0 && g_pCDlgGame->m_iScore_Right < 0) myPlay = 23;//给敌家变压器
						else myPlay = Max(2);
					}
					else myPlay = Max(2);
				}
				else 
				{
					myPlay = Max(3);
				}
			}
			else//未出过A
			{
				if (temp1 == 1)
				{
					if (count != 1 && count < 11 && Own(11)) myPlay = 11;//吃羊
					else if (g_pGame->Save[0] == 11 || g_pGame->Save[1] == 11 || g_pGame->Save[2] == 11) myPlay = Max(1);//吃羊
					else if (g_pGame->Save[0] == 51 || g_pGame->Save[1] == 51 || g_pGame->Save[2] == 51)
						myPlay = SafeMin(1, count);//避免吃猪
					else if (g_pGame->JudgeDecor(g_pGame->Save[0]) == 3 || g_pGame->JudgeDecor(g_pGame->Save[1]) == 3 || g_pGame->JudgeDecor(g_pGame->Save[2]) == 3)
						myPlay = SafeMin(1, count);//避免得红桃牌
					else
					{
						if (SafeMin(1, 11) == 11) myPlay = Max(1);
						else myPlay = SafeMin(1, 11);
					}
				}
				else
				{
					if (g_pGame->Save[0] == 51 || g_pGame->Save[1] == 51 || g_pGame->Save[2] == 51)
						myPlay = SafeMin(temp1, count);//避免吃猪
					else if (g_pGame->JudgeDecor(g_pGame->Save[0]) == 3 || g_pGame->JudgeDecor(g_pGame->Save[1]) == 3 || g_pGame->JudgeDecor(g_pGame->Save[2]) == 3)
						myPlay = SafeMin(temp1, count);//避免得红桃牌
					else
					{
						if (Max(temp1) != 51 && Max(temp1) != 23)   myPlay = Max(temp1);//避免自己得猪或变压器
						else myPlay = SafeMin(temp1, count);
					}
				}
			}
		}
		else//没有相应花色
		{
			if (count == g_pGame->Save[1]) //对家目前牌最大
			{
			  if (temp1 != 1 && Own(11)) myPlay = 11;//对家吃羊
			  else if ((Judge(4)) && (Max(4) != 51 || Min(4) != 51))//避免对家拿猪
			  {
				 if (Judge(4))
				 {
					if (Max(4) == 51) myPlay = SafeMin(4, 51);
					else myPlay = Max(4);
				 }
				else if (Judge(2))
				{
					if (Max(2) == 23) myPlay = SafeMin(2, 23);
					else myPlay = Max(2);
				}
				else if (Judge(1))
				{
					myPlay = SafeMin(1, 11);
				}
				else if (Judge(3))
				{
					myPlay = SafeMin(3, 31);
				}
				else
				{
					//程序出错
				}
			  }
			  else
			  {
				  if (Judge(2))
				  {
					  if (Max(2) == 23) myPlay = SafeMin(2, 23);
					  else myPlay = Max(2);
				  }
				  else if (Judge(1))
				  {
					  myPlay = SafeMin(1, 11);
				  }
				  else if (Judge(3))
				  {
					  myPlay = SafeMin(3, 31);
				  }
				  else if (Judge(4)) myPlay = SafeMin(4, 51);
				  else
				  {
					  //程序出错
				  }
			  }
			}
			else//敌家目前牌最大
			{
				if (Own(51) && temp1 != 4) myPlay = 51;//先出猪
				else if (Judge(3) && temp1 != 3) myPlay = Max(3);//再出红桃大牌
				else if (Judge(4) && temp1 != 4) myPlay = Max(4);
				else if (Judge(2) && temp1 != 2)
				{
					if (g_pCDlgGame->m_iScore_Left < 0 && g_pCDlgGame->m_iScore_Right < 0 && Own(23)) myPlay = 23;
					else myPlay = Max(2);
				}
				else if (Judge(1) && temp1 != 1) myPlay = SafeMin(1, 11);
				else
				{
					myPlay = g_pGame->myCard[0].data;
				}
			}
		}
	}
	else//游戏即将结束
	{
		if (Judge(temp1))//有相应花色牌
		{
			if (SafeMin(temp1, count) == 51)//可能出猪牌
			{
				if (Min(temp1) != 51) myPlay = Min(temp1);
				else myPlay = Max(temp1);
			}
			else if (SafeMin(temp1, count) == 11)//可能出羊牌
			{
				if (count != 1 && count < 11) myPlay = 11;//我吃羊
				else if (count == g_pGame->Save[1]) myPlay = 11;//对家吃羊
				else if (SafeMin(temp1, 11) != 11) myPlay = SafeMin(temp1, 11);
				else  myPlay = Max(temp1);
			}
			else if (SafeMin(temp1, count) == 23)//可能出变压器
			{
				if (Min(temp1) != 23) myPlay = Min(temp1);
				else myPlay = Max(temp1);
			}
			else
			{
				myPlay = SafeMin(temp1, count);
			}
		}
		else//没有相应花色牌
		{
			if (count == g_pGame->Save[1]) //对家目前牌最大
			{
				if (temp1 != 1 && Own(11)) myPlay = 11;//对家吃羊
				else if ((Judge(4)) && (Max(4) != 51 || Min(4) != 51))//避免对家拿猪
				{
					if (Judge(4))
					{
						if (Max(4) == 51) myPlay = SafeMin(4, 51);
						else myPlay = Max(4);
					}
					else if (Judge(2))
					{
						if (Max(2) == 23) myPlay = SafeMin(2, 23);
						else myPlay = Max(2);
					}
					else if (Judge(1))
					{
						myPlay = SafeMin(1, 11);
					}
					else if (Judge(3))
					{
						myPlay = SafeMin(3, 31);
					}
					else
					{
						//程序出错
					}
				}
				else
				{
					if (Judge(2))
					{
						if (Max(2) == 23) myPlay = SafeMin(2, 23);
						else myPlay = Max(2);
					}
					else if (Judge(1))
					{
						myPlay = SafeMin(1, 11);
					}
					else if (Judge(3))
					{
						myPlay = SafeMin(3, 31);
					}
					else if (Judge(4)) myPlay = SafeMin(4, 51);
					else
					{
						//程序出错
					}
				}
			}
			else//敌家目前牌最大
			{
				if (Own(51) && temp1 != 4) myPlay = 51;//先出猪
				else if (Judge(3) && temp1 != 3) myPlay = Max(3);//再出红桃大牌
				else if (Judge(4) && temp1 != 4) myPlay = Max(4);
				else if (Judge(2) && temp1 != 2)
				{
					if (g_pCDlgGame->m_iScore_Left < 0 && g_pCDlgGame->m_iScore_Right < 0 && Own(23)) myPlay = 23;
					else myPlay = Max(2);
				}
				else if (Judge(1) && temp1 != 1) myPlay = SafeMin(1, 11);
				else
				{
					myPlay = g_pGame->myCard[0].data;
				}
			}
		}
	}
	return myPlay;
}








int Com::Play5()
{
	int myPlay = 0,i, j;

    if(temp1 == 0)//庄家
	{	
		if (Judge(1)) myPlay = SafeMin(1,11);
		else if (Judge(2)) myPlay = SafeMin(2,23);
		else if (Judge(4)) myPlay = SafeMin(4,51);
		else if (Judge(3)) myPlay = SafeMin(3,33);
		else
		{
			myPlay = g_pGame->myCard[0].data;//此时程序错误
		}
	}
	else
	{
		int card = g_pGame->Save[0];
		for (j = 0;j < (g_order - g_pGame->CurrentOrder + 4) % 4;j++)
		{
			if (g_pGame->JudgeDecor(g_pGame->Save[j]) == temp1)
			{
				if (g_pGame->Save[j] == 13 * (temp1 - 1) + 1)
				{
					card = g_pGame->Save[j];//有A停止寻找
					break;
				}
				else
				{
					if (g_pGame->Save[j] > card) card = g_pGame->Save[j];//寻找最大牌
				}
			}
		}
		if (Judge(temp1))
			myPlay = SafeMin(temp1, card);
		else
		{
			if (Judge(3)) myPlay = Max(3);
			else if (Judge(4)) myPlay = Max(4);
			else if (Judge(2)) myPlay = Max(2);
			else if (Judge(1)) myPlay = Min(1);
			else
			{
				//此时程序错误
			}
		}
	}
	return myPlay;
}




//---------------------------------------------------------------------------------------------
int Com::GameMaxColour()
{
	int i,count1 = 0,count2 = 0,count3 = 0,count4=0,count=0;
	for (i = 1;i < 14;i++)
		if (recorder[i] == false) count1++;
	for (i = 14;i < 27;i++)
		if (recorder[i] == false) count2++;
	for (i = 27;i < 40;i++)
		if (recorder[i] == false) count3++;
	for (i = 40;i < 53;i++)
		if (recorder[i] == false) count4++;
	if (count4 >= count && NoDifficultPig()) count = count4;
	if (count3 >= count && NoDifficultHeartA()) count = count3;
	if (count2 >= count) count = count2;
	if (count1 >= count) count = count1;
	return count;
}
bool Com::Node()//判断是否出现比赛节点
{
	int i, k;//记录花色
	bool node = false;
	if (g_pGame->Round >= 5) return true;
	else
	{
		if (g_order != g_pGame->CurrentOrder)
		{
			k = g_pGame->JudgeDecor(g_pGame->Save[0]);
			for (i = 0; i < (g_order - g_pGame->CurrentOrder + 4) % 4; i++)
			{
				if (g_pGame->JudgeDecor(g_pGame->Save[i]) != k)
				{
					node = true;
				}
				else
				{
					//未出现节点
				}
			}
		}
		else
		{
			//未出现节点
		}
	}
	return node;
}
int Com::MinColour()//返回不为0最小花色数
{
	int i=0, j=-1, k=0;
	bool colour1=true, colour2=true, colour3=true, colour4=true;
	for (i = 0;i < 13;i++)
	{
		if (g_pGame->JudgeDecor(g_pGame->myCard[i].data) == 1)
		{
			if (g_pGame->myCard[i].data >= 10 || g_pGame->myCard[i].data == 1) colour1 = false;
		}
		else if (g_pGame->JudgeDecor(g_pGame->myCard[i].data) == 2)
		{
			if (g_pGame->myCard[i].data >= 23 || g_pGame->myCard[i].data == 14) colour2 = false;
		}
		else if (g_pGame->JudgeDecor(g_pGame->myCard[i].data) == 3)
		{
			if (g_pGame->myCard[i].data >= 36 || g_pGame->myCard[i].data == 27) colour3 = false;
		}
		else if (g_pGame->JudgeDecor(g_pGame->myCard[i].data) == 4)
		{
			if (g_pGame->myCard[i].data >= 49 || g_pGame->myCard[i].data == 40) colour4 = false;
		}
	}
	for (k = 0; k < 4; k++)//找到第一个花色数不为0的花色
	{
		if(k == 0 && CardNum[0]!=0 && colour1==true)
		{
			j = k;
			break;
		}
		else if (k == 1 && CardNum[1] != 0 && colour2 == true)
		{
			j = k;
			break;
		}
		else if (k == 2 && CardNum[2] != 0 && colour3 == true)
		{
			j = k;
			break;
		}
		else if (k == 3 && CardNum[3] != 0 && colour4 == true)
		{
			j = k;
			break;
		}
	}
	if (j == -1)//前面未有给j赋值的情况
	{
		for (k = 0;k < 13;k++)
		{
			if (CardNum[k] != 0) j = k;//寻找第一个手牌数不为0的花色
		}
	}
	else
	{
		//前面已有给j赋值的情况
	}
	if (colour1 == true || colour2 == true || colour3 == true || colour4 == true)//排除风险花色
	{
		if (NoDifficultHeartA())
		{
			if (NoDifficultPig())
			{
				for (i = k; i < 4; i++)//黑桃>方块>梅花>红桃
				{
					if (i == 0 && colour1 == false) continue;
					if (i == 1 && colour2 == false) continue;
					if (i == 2 && colour3 == false) continue;
					if (i == 3 && colour4 == false) continue;
					if (CardNum[i] == 0) continue;//花色数等于0直接跳过
					else
					{
						if (CardNum[i] < CardNum[j]) j = i;
						else if (CardNum[i] == CardNum[j])
						{
							if (i == 3) j = 3;
							else if (i == 2 && j == 0) j = 0;
							else if (i == 2 && j != 0) j = 1;
							else j = 0;
						}
						else
						{
							//不发生改变
						}
					}
				}
			}
			else
			{
				for (i = k; i < 3; i++)//方块>梅花>红桃>黑桃
				{
					if (i == 0 && colour1 == false) continue;
					if (i == 1 && colour2 == false) continue;
					if (i == 2 && colour3 == false) continue;
					if (i == 3 && colour4 == false) continue;
					if (CardNum[i] == 0) continue;//花色数等于0直接跳过
					else
					{
						if (CardNum[i] < CardNum[j]) j = i;
						else if (CardNum[i] == CardNum[j])
						{
							//不发生改变
						}
						else
						{
							//不发生改变
						}
					}
				}
			}
		}
		else
		{
			if (NoDifficultPig())//黑桃>方块>梅花(不考虑出红桃)
			{
				for (i = k; i < 4; i++)
				{
					if (i == 0 && colour1 == false) continue;
					if (i == 1 && colour2 == false) continue;
					if (i == 2 && colour3 == false) continue;
					if (i == 3 && colour4 == false) continue;
					if (CardNum[i] == 0) continue;//花色数等于0直接跳过
					else
					{
						if (i == 2) continue;
						else
						{
							if (CardNum[i] < CardNum[j]) j = i;
							else if (CardNum[i] == CardNum[j])
							{
								if (i == 3) j = 3;
								else
								{
									//不发生改变
								}
							}
							else
							{
								//不发生改变
							}
						}
					}
				}
			}
			else//方块>梅花（不考虑黑桃红桃）
			{
				if (CardNum[0] != 0 && CardNum[1] != 0)
				{
					if (CardNum[0] <= CardNum[1]) j = 0;
					else j = 1;
				}
				else if (CardNum[0] != 0 && CardNum[1] == 0) j = 0;
				else if (CardNum[0] == 0 && CardNum[1] != 0) j = 1;
				else
				{
					//特殊情况：方块梅花花色数均为0
					if (CardNum[2] != 0) j = 2;//优先红桃
					else j = 3;//只剩黑桃
				}
			}
		}
	}
	else//所有花色均存在风险花色
	{
		if (g_order == 1)
		{
			if (cardbright[2][0] == true && Judge(1) && recorder[11] == false) j = 0;
			else if ((cardbright[1][3] == true || cardbright[3][3] == true) && Judge(4) && recorder[51] == false) j = 3;
			else
			{
				if (Judge(1)) j = 0;
				else if (Judge(4)) j = 3;
				else if (Judge(2)) j = 1;
				else j = 2;
			}
		}
		else if (g_order == 2)
		{
			if (cardbright[3][0] == true && Judge(1) && recorder[11] == false) j = 0;
			else if ((cardbright[0][3] == true || cardbright[2][3] == true) && Judge(4) && recorder[51] == false) j = 3;
			else
			{
				if (Judge(1)) j = 0;
				else if (Judge(4)) j = 3;
				else if (Judge(2)) j = 1;
				else j = 2;
			}
		}
		else if (g_order == 3)
		{
			if (cardbright[0][0] == true && Judge(1) && recorder[11] == false) j = 0;
			else if ((cardbright[1][3] == true || cardbright[3][3] == true) && Judge(4) && recorder[51] == false) j = 3;
			else
			{
				if (Judge(1)) j = 0;
				else if (Judge(4)) j = 3;
				else if (Judge(2)) j = 1;
				else j = 2;
			}
		}
		else 
		{
			if (cardbright[1][0] == true && Judge(1) && recorder[11] == false) j = 0;
			else if ((cardbright[0][3] == true || cardbright[2][3] == true) && Judge(4) && recorder[51] == false) j = 3;
			else
			{
				if (Judge(1)) j = 0;
				else if (Judge(4)) j = 3;
				else if (Judge(2)) j = 1;
				else j = 2;
			}
		}
	}
	if(0<=j&&3>=j) return j + 1;//进行合理性判断
	else
	{
		if (Judge(1)) return 1;
		else if (Judge(4)) return 4;
		else if (Judge(2)) return 2;
		else return 3;
	}

}
void Com::SizeSort()//记录各花色所剩手牌数
{
	int i, k = 0, count[4] = { 0 };
	for (i = 0; i < 13; i++)
	{
		k = g_pGame->JudgeDecor(g_pGame->myCard[i].data);
		count[k - 1]++;
	}
	for (i = 0; i < 4; i++)
		CardNum[i] = count[i];
}
bool Com::NoDifficultPig()
{
	int i;
	for (i = 0; i < 13; i++)
		if (g_pGame->myCard[i].data == 51 || g_pGame->myCard[i].data == 52 || g_pGame->myCard[i].data == 40) return false;
	return true;
}
bool Com::NoDifficultHeartA()
{
	int i;
	for (i = 0; i < 13; i++)
		if ((g_pGame->myCard[i].data >= 33 && g_pGame->myCard[i].data <= 39) || g_pGame->myCard[i].data == 27) return false;
	return true;
}
bool Com::Judge(int colour)
{
	bool judge = false;
	int i;
	for (i = 0; i < 13; i++)
	{
		if (g_pGame->JudgeDecor(g_pGame->myCard[i].data) == colour)
		{
			judge = true;//找到就停止寻找
			break;
		}
		else
		{
			//无变化
		}
	}
	return judge;
}
int Com::SafeMax(int card1,int card2,int card3)//返回安全牌大值(参数对应三张牌,第一个int决定花色)
{
	int colour1, colour2, colour3,count=card1,i;
	colour1 = g_pGame->JudgeDecor(card1);
	colour2 = g_pGame->JudgeDecor(card2);
	colour3 = g_pGame->JudgeDecor(card3);
	if (colour1 == colour2)//判断花色是否满足条件
	{
		if (card1 == (13 * colour1 - 12)) return card1;
		else if (card2 == 13 * colour1 - 12) return card2;
		else if (card2 > card1) count = card2;//记录大牌
		else
		{
			//不发生变化
		}
	}
	else
	{
		//不同花色不进行判断
	}
	//进行下一轮判断
	if (colour1 == colour3)//判断花色是否满足条件
	{
		if (card3 == (13 * colour1 - 12)) return card3;
		else if (card3 > count) count= card3;//记录大牌
		else
		{
			//不发生改变
		}
	}
	else
	{
		//不同花色不进行判断
	}
	return count;
}
int Com::Min(int colour)//返回对应花色牌点数最小值
{
	int count = 13 * (colour - 1) + 1, i;
	for (i = 0; i < 13; i++)
	{
		if (g_pGame->JudgeDecor(g_pGame->myCard[i].data) == colour)
		{
			if (count == 13 * (colour - 1) + 1)
			{
				if (g_pGame->myCard[i].data > count) count = g_pGame->myCard[i].data;//A大于其他数
			}
			else
			{
				if (g_pGame->myCard[i].data < count) count = g_pGame->myCard[i].data;//2-K的正常排序
			}
		}
	}
	//包含count=13 * (colour - 1) + 1的情况
	return count;//使用前注意必须有colour对应牌才可用
}
int Com::Max(int colour)//返回对应花色牌点数最大值
{
	int count = 13 * (colour - 1) + 2, i;
	for (i = 0; i < 13; i++)
	{
		if (g_pGame->JudgeDecor(g_pGame->myCard[i].data) == colour)
		{
			if (g_pGame->myCard[i].data == 13 * (colour - 1) + 1)//找到A就停止寻找
			{
				count = g_pGame->myCard[i].data;
				break;
			}
			else
			{
				if (g_pGame->myCard[i].data > count) count = g_pGame->myCard[i].data;//2-K的正常排序
			}
		}
	}
	//包含count=13 * (colour - 1) + 2的情况
	return count;//使用前注意必须有colour对应牌才可用
}
int Com::SafeMin(int colour, int card)//返回安全牌小值（已出牌最大值以下）
{
	int count = 13 * (colour - 1) + 1;
	if (card % 13 == 1) count = Max(colour);//前面玩家已出A，可出任意牌
	else
	{
		for (int i = 0; i < 13; i++)
			if (g_pGame->JudgeDecor(g_pGame->myCard[i].data) == colour)
			{
				if (g_pGame->myCard[i].data > count && g_pGame->myCard[i].data < card) count = g_pGame->myCard[i].data;//选择出自己手牌中低于已出最大牌点数的最大牌
			}
	}
	if (count == 13 * (colour - 1) + 1) count = Min(colour);//自己手牌中最小牌也大于场上已出牌的最大值
	return count;//使用前注意必须有colour对应牌才可用
}
void Com::Recorder(int card)
{
	recorder[card] = true;
}
bool Com::IfBright(int m, int n)//m始终小于n
{
	srand((unsigned)time(NULL));
	bool judge;
	int x = rand() % (n - 1 + 1) + 1;
	if (1 <= x && m >= x) judge = true;
	else judge = false;
	return judge;
}
int Com::Discard()//参数返回易脱手的花色数量
{
	int i, count1 = 0, count2 = 0, count3 = 0, count4 = 0, count = 0;//count值大于2时就不满足易脱手的条件
	for (i = 0; i < 13; i++)
	{
		if (g_pGame->myCard[i].data >= 1 && g_pGame->myCard[i].data <= 13)
			count1++;
		if (g_pGame->myCard[i].data >= 14 && g_pGame->myCard[i].data <= 26&& g_pGame->myCard[i].data!=23)
		{
			if ((g_pGame->myCard[i].data >= 23 && g_pGame->myCard[i].data <= 26) || g_pGame->myCard[i].data == 14) count2 += 3;
			else count2++;
		}
		if (g_pGame->myCard[i].data >= 27 && g_pGame->myCard[i].data <= 39)
		{		
			if ((g_pGame->myCard[i].data >= 36 && g_pGame->myCard[i].data <= 39)|| g_pGame->myCard[i].data==27) count3 += 3;
			else count3++;
		}
		if (g_pGame->myCard[i].data >= 40 && g_pGame->myCard[i].data <= 52)
		{			
			if ((g_pGame->myCard[i].data >= 51 && g_pGame->myCard[i].data <= 52)|| g_pGame->myCard[i].data == 40) count4 += 3;
			else count4++;
		}	
	}
	if (count1 <= 2) count++;
	if (count2 <= 2) count++;
	if (count3 <= 2) count++;
	if (count4 <= 2) count++;
	return count;
}
bool Com::Own(int data1)//判断是否含有某牌,参数代表判断牌
{
	int i;
	bool card = false;
	for (i = 0; i < 13; i++)
	{
		if (g_pGame->myCard[i].data == data1)
		{
			card = true;
			break;
		}
	}
	return card;
}
int Com::ColourNum(int colour)//返回自己手牌中某花色的牌数,参数代表花色
{
	int i, count = 0;
	for (i = 0; i < 13; i++)
	{
		if (g_pGame->myCard[i].data != 0)
		{
			if (colour == 1 && (g_pGame->myCard[i].data >= 1 && g_pGame->myCard[i].data <= 13))//方块
				count++;
			else if (colour == 2 && (g_pGame->myCard[i].data >= 14 && g_pGame->myCard[i].data <= 26))//梅花
				count++;
			else if (colour == 3 && (g_pGame->myCard[i].data >= 27 && g_pGame->myCard[i].data <= 39))//红桃
				count++;
			else if (colour == 4 && (g_pGame->myCard[i].data >= 40 && g_pGame->myCard[i].data <= 52))//黑桃
				count++;
		}
		else break;
	}
	return count++;
}



void Com::JudgeChaseHeart() {//27~39 红桃
	if (ChaseHeart >= 0) {//只有非防止敌方追求满红状态才会判断
		int i = 0, j = 0, count = 0;//count记录特征牌库里有红桃的个数总和
		int heartcount[4] = { 0 };//记录每个玩家特征牌库中红桃个数
		int heartonly = -1;//记录只有一个玩家特征牌库有红的玩家下标
		for (i = 0; i <= 3; i++) {
			for (j = 0; j <= 12; j++) {
				if (g_pGame->Collection[i][j] <= 39 && g_pGame->Collection[i][j] >= 27) heartcount[i] += 1;
			}
		}
		for (i = 0; i <= 3; i++) {
			if (heartcount[i] > 0) {
				count++; heartonly = i;
			}
		}
		if (count > 1)
			ChaseHeart = 0;
		else {
			switch (count) {
			case 0: {
				if (CardNum[2] >= 5) {//场上无人特征牌库有红桃，自己红桃多且有大牌
					int maxcount=0;//记录大牌数
					for (j = 0; j <= 12; j++)
						if (g_pGame->myCard[j].data <= 39 && g_pGame->myCard[j].data >= 35)maxcount++;
					if (maxcount >= 3)ChaseHeart = 1;
					else ChaseHeart = 0;
				}
				else ChaseHeart = 0;
				break;
			}
			case 1: {
				if (heartonly == g_pGame->CurrentOrder - 1 || ChaseHeart == 1) {//唯一特征牌库有红桃的就是自己
					int maxcount=0;//记录大牌数
					for (j = 0; j <= 12; j++)
						if (g_pGame->myCard[j].data <= 39 && g_pGame->myCard[j].data >= 35)maxcount++;
					int leftheart = 13 - heartcount[0] - heartcount[1] - heartcount[2] - heartcount[3];//场上剩余红桃数
					if (maxcount < leftheart % 4 + 2)ChaseHeart = 0;
				}
				else {//唯一特征牌库有红桃的不是自己
					for (i = 0; i <= 3; i++) {
						if (heartcount[i] > 8) {
							ChaseHeart = -i - 1;
								if (abs(-ChaseHeart - g_pGame->CurrentOrder) == 2)ChaseHeart = 0;//是对家在追求满红
							break;
						}
					}
					if (ChaseHeart > 0)ChaseHeart = 0;
				}
				break;
			}
			}
		}
	}
}

bool Com::MinCard(int colour)
{
	int i,judge=false;
	for (i = 0;i < 13;i++)
	{
		if (g_pGame->myCard[i].data >= (13 * colour - 11) && g_pGame->myCard[i].data <= (13 * colour - 4)) judge = true;
	}
	return judge;
}