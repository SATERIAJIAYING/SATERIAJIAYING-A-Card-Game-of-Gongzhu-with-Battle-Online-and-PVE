#include "pch.h"
#include "Com.h"
#include "CDlgGame.h"
#include "CDlgLink.h"
#include "game.h"
#include "gongzhuDlg.h"
#include"cmath"

void Com::AcceptShowdownMsg(int order, bool sheep, bool trans, bool heartA, bool pig)//����������Ϣ
{
	if (g_order - order == 2 || g_order - order == -2)//�ж϶Լ�����
	{
		if (sheep) desire1 += 3;//Case1
		if (pig) desire1 += 3;//Case2
		if (heartA) desire1 += 3;//Case3
	}
	else//�жϷǶԼ�����
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
	//����ֵ����
	bool Bsheep;
	int y = 0, i;
	for (i = 0; i < 13; i++)//y��ֵ
		if (g_pGame->myCard[i].data == 1 || g_pGame->myCard[i].data == 12 || g_pGame->myCard[i].data == 13) y++;;
	if (ColourNum(1) >= 4) desire3++;//Case7
	desire3 += y;//Case8
	if (ColourNum(1) <= 2) desire3 -= (3 - ColourNum(1));//Case9
	if (Own(11) == false) Bsheep = false;
	else
	{
		if (desire3 <= 2) Bsheep = false;
		else Bsheep = IfBright(desire3, 6);//�ж��Ƿ�����
	}

	return Bsheep;
}
bool Com::Showdown_Trans()
{
	//����ֵ����
	bool Btrans;
	if (ColourNum(2) >= 4) desire1++;//Case10
	if (ColourNum(2) <= 2) desire1 -= (3 - ColourNum(2));//Case11
	if (Own(23) == false) Btrans = false;
	else
	{
		if (desire1 <= 4) Btrans = false;
		else Btrans = IfBright(desire1, 11);//�ж��Ƿ�����ѹ��
	}

	return Btrans;
}
bool Com::Showdown_HeartA()
{
	//����ֵ����
	int z = 0, i, m = Discard();
	bool BheartA;
	for (i = 0; i < 13; i++)//z��ֵ
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
		else BheartA = IfBright(desire4, 12);//�ж��Ƿ�������A
	}

	return BheartA;
}
bool Com::Showdown_Pig()
{
	//����ֵ����
	int x = 0, i, m = Discard();
	bool Bpig;
	for (i = 0; i < 13; i++)//x��ֵ
		if (g_pGame->myCard[i].data == 40 || g_pGame->myCard[i].data == 52) x++;;
	if (ColourNum(4) >= 4) desire2++;//Case18
	desire2 += (x + m);//Case19
	if (ColourNum(4) <= 2) desire2 -= (3 - ColourNum(4));//Cas20
	if (Own(51) == false) Bpig = false;
	else
	{
		if (desire2 <= 3) Bpig = false;
		else Bpig = IfBright(desire2, 8);//�ж��Ƿ�����
	}

	return Bpig;
}



void Com::AcceptPlayMsg(int order, int card)//���ճ����ź�
{//temp1�����һ����ҵĻ�ɫ,1234�ֱ��Ӧһ�ֻ�ɫ
	Recorder(card);






}

bool Com::ComingOver()
{
	if (g_pGame->Round >= 8) return true;
	else return false;
}
int Com::Play()//���ƣ�����ֵ��0~12��Ҫ�������ƶ�Ӧ�������±�
{//temp1�����һ����ҵĻ�ɫ�������׸�������ң�-1��Ϊ÷��2, 0�ɳ������ƣ�1234�ֱ��Ӧһ�ֻ�ɫ
	int myPlay = 53, i, j=0,card = 0;
	bool retionality = false;//���ƺ����Լ��
	if (temp1 == -1)//ֻ�ܳ�÷��2
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
	else if (g_Model == 3)//����
	{
		if (g_order == g_pGame->CurrentOrder)//ׯ��
			if (Own(27) && Own(39) && Own(38) && Own(37) && ColourNum(3) >= 5) myPlay = Max(3);//׷������
			else	myPlay = Play1();
		else if ((g_order - g_pGame->CurrentOrder + 4) % 4 == 1)//�ڶ�������
			myPlay = Play2();
		else if ((g_order - g_pGame->CurrentOrder + 4) % 4 == 2)//����������
			myPlay = Play3();
		else myPlay = Play4();//ĩλ����
	}
	else myPlay = Play5();//��ͨ
	
	Recorder(myPlay);//������
	for (i = 0;i < 13;i++)
		if (g_pGame->myCard[i].data == myPlay)
		{
			j = i;
			retionality = true;
		}
	//��ȫ��������:�������жϴ�����������к����Լ��
	if (retionality) return j;//��������
	else//������жϴ���
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
	SizeSort();//��¼����ɫ��ʣ������
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
	if (!ComingOver())//��Ϸδ��������
	{
		if (!Node())//δ���ֱ����ڵ�
		{
			if (colour1 == 1) myPlay = SafeMin(1, 11);
			else if (colour1 == 2) myPlay = SafeMin(2, 23);
			else if (colour1 == 3) myPlay = SafeMin(3, 33);
			else if (colour1 == 4) myPlay = SafeMin(4, 51);
			else
			{
				//�������
			}
		}
		else //���ֱ����ڵ�
		{
			if (colour1 == 1) myPlay = SafeMin(1, 8);
			else if (colour1 == 2) myPlay = SafeMin(2, 21);
			else if (colour1 == 3) myPlay =SafeMin(3, 33);
			else if (colour1 == 4) myPlay = SafeMin(4, 47);
			else
			{
				//�������
			}
		}
		
	}
	else//��Ϸ��������
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
	if (!ComingOver())//��Ϸδ��������
	{
		if (!Node())//δ����ڵ�
		{
			if (Judge(temp1))//�����ƶ�Ӧ��ɫ����
			{
				if (temp1 == 1) myPlay = SafeMin(1, 11);//����
				else if (temp1 == 2) myPlay = SafeMin(2, 23);//÷��
				else if (temp1 == 3) myPlay = SafeMin(3, 33);//����
				else//����
				{
					if (SafeMin(4, 51) != 51) myPlay = SafeMin(4, 51);
					else myPlay = Max(4);
				}
			}
			else//û�����ƶ�Ӧ��ɫ����
			{
				if (g_pGame->Save[0] <= (13*temp1-3)&& g_pGame->Save[0] !=(13*temp1-12))
				{
					if (Judge(2)) myPlay = Max(2);//���˴���
					else if (Judge(1)) myPlay = Min(1);
					else if (Judge(4))
					{
						if (Max(4) != 51) myPlay = Max(4);
						else myPlay = SafeMin(4, 51);
					}
					else if (Judge(3)) myPlay = SafeMin(3, 36);
					else
					{
						myPlay = g_pGame->myCard[0].data;//���ִ���
					}
				}
				else//��Ϊ�мҽ�����ׯ
				{
					if (Own(51) && temp1 != 4) myPlay = 51;//�ȳ���
					else if (Judge(3) && temp1 != 3) myPlay = Max(3);//�ٳ����Ҵ���
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
		else//����ڵ�
		{
			if (Judge(temp1))//�����ƶ�Ӧ��ɫ����
			{
				if (temp1 == 1) myPlay = SafeMin(1, 11);//����
				else if (temp1 == 2) myPlay = SafeMin(2, 21);//÷��
				else if (temp1 == 3) myPlay = SafeMin(3, 33);//����
				else//����
				{
					if (SafeMin(4, 47) != 51)
						myPlay = SafeMin(4, 47);
					else myPlay = Max(4);

				}
			}
			else//û�����ƶ�Ӧ��ɫ����
			{
				if (g_pGame->Save[0] <= (13 * temp1 - 3) && g_pGame->Save[0] != (13 * temp1 - 12))//���˴���
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
						myPlay = g_pGame->myCard[0].data;//���ִ���
					}
				}
				else//��Ϊ�мҽ�����ׯ
				{
					if (Own(51) && temp1 != 4) myPlay = 51;//�ȳ���
					else if (Judge(3) && temp1 != 3) myPlay = Max(3);//�ٳ����Ҵ���
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
	else//��Ϸ��������
	{
		if (Judge(temp1))//�����ƶ�Ӧ��ɫ����
		{
			myPlay = Min(temp1);
		}
		else//û�����ƶ�Ӧ��ɫ����
		{
			if (g_pGame->Save[0] <= (13 * temp1 - 3) && g_pGame->Save[0] != (13 * temp1 - 12))//���˴���
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
					myPlay = g_pGame->myCard[0].data;//���ִ���
				}
			}
			else//��Ϊ�мҽ�����ׯ
			{
				if (Own(51) && temp1 != 4) myPlay = 51;//�ȳ���
				else if (Judge(3) && temp1 != 3) myPlay = Max(3);//�ٳ����Ҵ���
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
	if (g_pGame->JudgeDecor(g_pGame->Save[0]) == g_pGame->JudgeDecor(g_pGame->Save[1]))//�ڶ�λ��ָ����ɫ��
	{
		if (!ComingOver())//��Ϸδ��������
		{
			if (Judge(temp1))//�д˻�ɫ��
			{
				int card = (g_pGame->Save[0] > g_pGame->Save[1]) ? g_pGame->Save[0] : g_pGame->Save[1];
				if (temp1 == 1)//����
				{
					if (!recorder[11])//���ϻ�����
					{
						if (!Own(11))//�Լ�û������
						{
							if ((g_pGame->Save[0] > g_pGame->Save[1] && card > 11) || g_pGame->Save[0] == 1) myPlay = SafeMin(1, 11);//��������
							else   myPlay = Max(1);//�Լ�����
						}
						else//�Լ�������
						{
							if (g_pGame->Save[0] == 1) myPlay = 11;//�ö��ѳ���
							else if ((g_pGame->Save[0] == 13 && Own(1)) || (g_pGame->Save[0] == 13 && recorder[1] == true)) myPlay = 11;//�ö��ѳ���
							else if (g_pGame->Save[0] == 12 && recorder[12] == true && recorder[13] == true) myPlay = 11;//�ö��ѳ���
							else myPlay = SafeMin(1, 11);//��������
						}
					}
					else//����û����
					{
						if (g_pGame->Save[0] != 1 && g_pGame->Save[1] != 1) myPlay = SafeMin(1, card);
						else myPlay = Max(1);
					}
				}
				else if (temp1 == 4)//����
				{
					if (SafeMin(4, 51) != 51)
						myPlay = SafeMin(4, 51);//����õ�����
					else myPlay = Max(4);
				}
				else if (temp1 == 2)//÷��
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
						myPlay = SafeMin(3, 31);//����Ѽ���ʧ��
					else
					{
						if (g_pGame->Save[0] != 27 && g_pGame->Save[1] != 27) myPlay = SafeMin(3, card);
						else myPlay = Max(3);
					}
				}
			}
			else//û�д˻�ɫ��
			{
				if (g_pGame->Save[0] != (13 * temp1 - 12) && g_pGame->Save[1] != (13 * temp1 - 12))//������A
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
							//�������
						}
					}
					else
					{
						if (Own(51) && temp1 != 4) myPlay = 51;//�ȳ���
						else if (Judge(3) && temp1 != 3) myPlay = Max(3);//�ٳ����Ҵ���
						else if (Judge(4) && temp1 != 4) myPlay = Max(4);
						else if (Judge(2) && temp1 != 2) myPlay = Max(2);
						else if (Judge(1) && temp1 != 1) myPlay = SafeMin(1, 11);
						else
						{
							myPlay = g_pGame->myCard[0].data;
						}
					}
				}
				else//������A
				{
					if (g_pGame->Save[0] == (13 * temp1 - 12))//�Լҳ�A
					{
						if (Max(4) != 51 || Min(4) != 51)//��������
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
								//�������
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
					else//�мҳ�A
					{
						if (Own(51) && temp1 != 4) myPlay = 51;//�ȳ���
						else if (Judge(3) && temp1 != 3) myPlay = Max(3);//�ٳ����Ҵ���
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
		else//��Ϸ��������
		{
			if (Judge(temp1))//�д˻�ɫ
			{
				myPlay = Min(temp1);
			}
			else//û�д˻�ɫ
			{
				if (g_pGame->Save[0] != 0 && g_pGame->Save[1] != 0)//������A
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
							//�������
						}
					}
					else
					{
						if (Own(51) && temp1 != 4) myPlay = 51;//�ȳ���
						else if (Judge(3) && temp1 != 3) myPlay = Max(3);//�ٳ����Ҵ���
						else if (Judge(4) && temp1 != 4) myPlay = Max(4);
						else if (Judge(2) && temp1 != 2) myPlay = Max(2);
						else if (Judge(1) && temp1 != 1) myPlay = SafeMin(1, 11);
						else
						{
							myPlay = g_pGame->myCard[0].data;
						}
					}
				}
				else//������A
				{
					if (g_pGame->Save[0] == (13 * temp1 - 12))//�Լҳ�A
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
							//�������
						}
					}
					else//�мҳ�A
					{
						if (Own(51) && temp1 != 4) myPlay = 51;//�ȳ���
						else if (Judge(3) && temp1 != 3) myPlay = Max(3);//�ٳ����Ҵ���
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
	else//�ڶ�λδ��ָ����ɫ����
	{
	  if (!Judge(temp1))//û��ָ����ɫ
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
			//�������
		}
	  }
	  else//��ָ����ɫ
	  {	 
		  if (temp1 == 1)//����
		  {
			  if (!recorder[11])//���ϻ�����
			  {
				  if (!Own(11))//�Լ�û������
				  {
					  if ((g_pGame->Save[0]  > 11) || g_pGame->Save[0] == 1) myPlay = SafeMin(1, 11);//��������
					  else   myPlay = Max(1);//�Լ�����
				  }
				  else//�Լ�������
				  {
					  if (g_pGame->Save[0] == 1) myPlay = 11;//�ö��ѳ���
					  else if ((g_pGame->Save[0] == 13 && Own(1)) || (g_pGame->Save[0] == 13 && recorder[1] == true)) myPlay = 11;//�ö��ѳ���
					  else if (g_pGame->Save[0] == 12 && recorder[12] == true && recorder[13] == true) myPlay = 11;//�ö��ѳ���
					  else myPlay = SafeMin(1, 11);//��������
				  }
			  }
			  else//����û����
			  {
				  if (g_pGame->Save[0] !=1) myPlay = SafeMin(1, g_pGame->Save[0]);
				  else myPlay = Max(1);
			  }
		  }
		  else if (temp1 == 4)//����
		  {
			  if (SafeMin(4, 51) != 51)
				  myPlay = SafeMin(4, 51);//����õ�����
			  else myPlay = Max(4);
		  }
		  else if (temp1 == 2)//÷��
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
	if (!ComingOver())//��Ϸδ��������
	{
		if (Judge(temp1))//����Ӧ��ɫ
		{
			if (count == (temp1 * 13 - 12) && g_pGame->Save[1] == (temp1 * 13 - 12))//�Լҳ�A
			{
				if (temp1 == 1)
				{
					if (Own(11)) myPlay = 11;//�Լҳ���
					else 	myPlay = SafeMin(1, 11);//����С��
				}
				else if (temp1 == 4 && Max(4) == 51) myPlay = SafeMin(4, 51);
				else if (temp1 == 2 && Max(2) == 23) myPlay = SafeMin(2, 23);
				else myPlay = SafeMin(3, 31);
			}
			else if (count == (temp1 * 13 - 12) && g_pGame->Save[1] != (temp1 * 13 - 12))//�мҳ�A
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
						if (g_pCDlgGame->m_iScore_Left < 0 && g_pCDlgGame->m_iScore_Right < 0) myPlay = 23;//���мұ�ѹ��
						else myPlay = Max(2);
					}
					else myPlay = Max(2);
				}
				else 
				{
					myPlay = Max(3);
				}
			}
			else//δ����A
			{
				if (temp1 == 1)
				{
					if (count != 1 && count < 11 && Own(11)) myPlay = 11;//����
					else if (g_pGame->Save[0] == 11 || g_pGame->Save[1] == 11 || g_pGame->Save[2] == 11) myPlay = Max(1);//����
					else if (g_pGame->Save[0] == 51 || g_pGame->Save[1] == 51 || g_pGame->Save[2] == 51)
						myPlay = SafeMin(1, count);//�������
					else if (g_pGame->JudgeDecor(g_pGame->Save[0]) == 3 || g_pGame->JudgeDecor(g_pGame->Save[1]) == 3 || g_pGame->JudgeDecor(g_pGame->Save[2]) == 3)
						myPlay = SafeMin(1, count);//����ú�����
					else
					{
						if (SafeMin(1, 11) == 11) myPlay = Max(1);
						else myPlay = SafeMin(1, 11);
					}
				}
				else
				{
					if (g_pGame->Save[0] == 51 || g_pGame->Save[1] == 51 || g_pGame->Save[2] == 51)
						myPlay = SafeMin(temp1, count);//�������
					else if (g_pGame->JudgeDecor(g_pGame->Save[0]) == 3 || g_pGame->JudgeDecor(g_pGame->Save[1]) == 3 || g_pGame->JudgeDecor(g_pGame->Save[2]) == 3)
						myPlay = SafeMin(temp1, count);//����ú�����
					else
					{
						if (Max(temp1) != 51 && Max(temp1) != 23)   myPlay = Max(temp1);//�����Լ�������ѹ��
						else myPlay = SafeMin(temp1, count);
					}
				}
			}
		}
		else//û����Ӧ��ɫ
		{
			if (count == g_pGame->Save[1]) //�Լ�Ŀǰ�����
			{
			  if (temp1 != 1 && Own(11)) myPlay = 11;//�Լҳ���
			  else if ((Judge(4)) && (Max(4) != 51 || Min(4) != 51))//����Լ�����
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
					//�������
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
					  //�������
				  }
			  }
			}
			else//�м�Ŀǰ�����
			{
				if (Own(51) && temp1 != 4) myPlay = 51;//�ȳ���
				else if (Judge(3) && temp1 != 3) myPlay = Max(3);//�ٳ����Ҵ���
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
	else//��Ϸ��������
	{
		if (Judge(temp1))//����Ӧ��ɫ��
		{
			if (SafeMin(temp1, count) == 51)//���ܳ�����
			{
				if (Min(temp1) != 51) myPlay = Min(temp1);
				else myPlay = Max(temp1);
			}
			else if (SafeMin(temp1, count) == 11)//���ܳ�����
			{
				if (count != 1 && count < 11) myPlay = 11;//�ҳ���
				else if (count == g_pGame->Save[1]) myPlay = 11;//�Լҳ���
				else if (SafeMin(temp1, 11) != 11) myPlay = SafeMin(temp1, 11);
				else  myPlay = Max(temp1);
			}
			else if (SafeMin(temp1, count) == 23)//���ܳ���ѹ��
			{
				if (Min(temp1) != 23) myPlay = Min(temp1);
				else myPlay = Max(temp1);
			}
			else
			{
				myPlay = SafeMin(temp1, count);
			}
		}
		else//û����Ӧ��ɫ��
		{
			if (count == g_pGame->Save[1]) //�Լ�Ŀǰ�����
			{
				if (temp1 != 1 && Own(11)) myPlay = 11;//�Լҳ���
				else if ((Judge(4)) && (Max(4) != 51 || Min(4) != 51))//����Լ�����
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
						//�������
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
						//�������
					}
				}
			}
			else//�м�Ŀǰ�����
			{
				if (Own(51) && temp1 != 4) myPlay = 51;//�ȳ���
				else if (Judge(3) && temp1 != 3) myPlay = Max(3);//�ٳ����Ҵ���
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

    if(temp1 == 0)//ׯ��
	{	
		if (Judge(1)) myPlay = SafeMin(1,11);
		else if (Judge(2)) myPlay = SafeMin(2,23);
		else if (Judge(4)) myPlay = SafeMin(4,51);
		else if (Judge(3)) myPlay = SafeMin(3,33);
		else
		{
			myPlay = g_pGame->myCard[0].data;//��ʱ�������
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
					card = g_pGame->Save[j];//��AֹͣѰ��
					break;
				}
				else
				{
					if (g_pGame->Save[j] > card) card = g_pGame->Save[j];//Ѱ�������
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
				//��ʱ�������
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
bool Com::Node()//�ж��Ƿ���ֱ����ڵ�
{
	int i, k;//��¼��ɫ
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
					//δ���ֽڵ�
				}
			}
		}
		else
		{
			//δ���ֽڵ�
		}
	}
	return node;
}
int Com::MinColour()//���ز�Ϊ0��С��ɫ��
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
	for (k = 0; k < 4; k++)//�ҵ���һ����ɫ����Ϊ0�Ļ�ɫ
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
	if (j == -1)//ǰ��δ�и�j��ֵ�����
	{
		for (k = 0;k < 13;k++)
		{
			if (CardNum[k] != 0) j = k;//Ѱ�ҵ�һ����������Ϊ0�Ļ�ɫ
		}
	}
	else
	{
		//ǰ�����и�j��ֵ�����
	}
	if (colour1 == true || colour2 == true || colour3 == true || colour4 == true)//�ų����ջ�ɫ
	{
		if (NoDifficultHeartA())
		{
			if (NoDifficultPig())
			{
				for (i = k; i < 4; i++)//����>����>÷��>����
				{
					if (i == 0 && colour1 == false) continue;
					if (i == 1 && colour2 == false) continue;
					if (i == 2 && colour3 == false) continue;
					if (i == 3 && colour4 == false) continue;
					if (CardNum[i] == 0) continue;//��ɫ������0ֱ������
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
							//�������ı�
						}
					}
				}
			}
			else
			{
				for (i = k; i < 3; i++)//����>÷��>����>����
				{
					if (i == 0 && colour1 == false) continue;
					if (i == 1 && colour2 == false) continue;
					if (i == 2 && colour3 == false) continue;
					if (i == 3 && colour4 == false) continue;
					if (CardNum[i] == 0) continue;//��ɫ������0ֱ������
					else
					{
						if (CardNum[i] < CardNum[j]) j = i;
						else if (CardNum[i] == CardNum[j])
						{
							//�������ı�
						}
						else
						{
							//�������ı�
						}
					}
				}
			}
		}
		else
		{
			if (NoDifficultPig())//����>����>÷��(�����ǳ�����)
			{
				for (i = k; i < 4; i++)
				{
					if (i == 0 && colour1 == false) continue;
					if (i == 1 && colour2 == false) continue;
					if (i == 2 && colour3 == false) continue;
					if (i == 3 && colour4 == false) continue;
					if (CardNum[i] == 0) continue;//��ɫ������0ֱ������
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
									//�������ı�
								}
							}
							else
							{
								//�������ı�
							}
						}
					}
				}
			}
			else//����>÷���������Ǻ��Һ��ң�
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
					//�������������÷����ɫ����Ϊ0
					if (CardNum[2] != 0) j = 2;//���Ⱥ���
					else j = 3;//ֻʣ����
				}
			}
		}
	}
	else//���л�ɫ�����ڷ��ջ�ɫ
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
	if(0<=j&&3>=j) return j + 1;//���к������ж�
	else
	{
		if (Judge(1)) return 1;
		else if (Judge(4)) return 4;
		else if (Judge(2)) return 2;
		else return 3;
	}

}
void Com::SizeSort()//��¼����ɫ��ʣ������
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
			judge = true;//�ҵ���ֹͣѰ��
			break;
		}
		else
		{
			//�ޱ仯
		}
	}
	return judge;
}
int Com::SafeMax(int card1,int card2,int card3)//���ذ�ȫ�ƴ�ֵ(������Ӧ������,��һ��int������ɫ)
{
	int colour1, colour2, colour3,count=card1,i;
	colour1 = g_pGame->JudgeDecor(card1);
	colour2 = g_pGame->JudgeDecor(card2);
	colour3 = g_pGame->JudgeDecor(card3);
	if (colour1 == colour2)//�жϻ�ɫ�Ƿ���������
	{
		if (card1 == (13 * colour1 - 12)) return card1;
		else if (card2 == 13 * colour1 - 12) return card2;
		else if (card2 > card1) count = card2;//��¼����
		else
		{
			//�������仯
		}
	}
	else
	{
		//��ͬ��ɫ�������ж�
	}
	//������һ���ж�
	if (colour1 == colour3)//�жϻ�ɫ�Ƿ���������
	{
		if (card3 == (13 * colour1 - 12)) return card3;
		else if (card3 > count) count= card3;//��¼����
		else
		{
			//�������ı�
		}
	}
	else
	{
		//��ͬ��ɫ�������ж�
	}
	return count;
}
int Com::Min(int colour)//���ض�Ӧ��ɫ�Ƶ�����Сֵ
{
	int count = 13 * (colour - 1) + 1, i;
	for (i = 0; i < 13; i++)
	{
		if (g_pGame->JudgeDecor(g_pGame->myCard[i].data) == colour)
		{
			if (count == 13 * (colour - 1) + 1)
			{
				if (g_pGame->myCard[i].data > count) count = g_pGame->myCard[i].data;//A����������
			}
			else
			{
				if (g_pGame->myCard[i].data < count) count = g_pGame->myCard[i].data;//2-K����������
			}
		}
	}
	//����count=13 * (colour - 1) + 1�����
	return count;//ʹ��ǰע�������colour��Ӧ�Ʋſ���
}
int Com::Max(int colour)//���ض�Ӧ��ɫ�Ƶ������ֵ
{
	int count = 13 * (colour - 1) + 2, i;
	for (i = 0; i < 13; i++)
	{
		if (g_pGame->JudgeDecor(g_pGame->myCard[i].data) == colour)
		{
			if (g_pGame->myCard[i].data == 13 * (colour - 1) + 1)//�ҵ�A��ֹͣѰ��
			{
				count = g_pGame->myCard[i].data;
				break;
			}
			else
			{
				if (g_pGame->myCard[i].data > count) count = g_pGame->myCard[i].data;//2-K����������
			}
		}
	}
	//����count=13 * (colour - 1) + 2�����
	return count;//ʹ��ǰע�������colour��Ӧ�Ʋſ���
}
int Com::SafeMin(int colour, int card)//���ذ�ȫ��Сֵ���ѳ������ֵ���£�
{
	int count = 13 * (colour - 1) + 1;
	if (card % 13 == 1) count = Max(colour);//ǰ������ѳ�A���ɳ�������
	else
	{
		for (int i = 0; i < 13; i++)
			if (g_pGame->JudgeDecor(g_pGame->myCard[i].data) == colour)
			{
				if (g_pGame->myCard[i].data > count && g_pGame->myCard[i].data < card) count = g_pGame->myCard[i].data;//ѡ����Լ������е����ѳ�����Ƶ����������
			}
	}
	if (count == 13 * (colour - 1) + 1) count = Min(colour);//�Լ���������С��Ҳ���ڳ����ѳ��Ƶ����ֵ
	return count;//ʹ��ǰע�������colour��Ӧ�Ʋſ���
}
void Com::Recorder(int card)
{
	recorder[card] = true;
}
bool Com::IfBright(int m, int n)//mʼ��С��n
{
	srand((unsigned)time(NULL));
	bool judge;
	int x = rand() % (n - 1 + 1) + 1;
	if (1 <= x && m >= x) judge = true;
	else judge = false;
	return judge;
}
int Com::Discard()//�������������ֵĻ�ɫ����
{
	int i, count1 = 0, count2 = 0, count3 = 0, count4 = 0, count = 0;//countֵ����2ʱ�Ͳ����������ֵ�����
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
bool Com::Own(int data1)//�ж��Ƿ���ĳ��,���������ж���
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
int Com::ColourNum(int colour)//�����Լ�������ĳ��ɫ������,��������ɫ
{
	int i, count = 0;
	for (i = 0; i < 13; i++)
	{
		if (g_pGame->myCard[i].data != 0)
		{
			if (colour == 1 && (g_pGame->myCard[i].data >= 1 && g_pGame->myCard[i].data <= 13))//����
				count++;
			else if (colour == 2 && (g_pGame->myCard[i].data >= 14 && g_pGame->myCard[i].data <= 26))//÷��
				count++;
			else if (colour == 3 && (g_pGame->myCard[i].data >= 27 && g_pGame->myCard[i].data <= 39))//����
				count++;
			else if (colour == 4 && (g_pGame->myCard[i].data >= 40 && g_pGame->myCard[i].data <= 52))//����
				count++;
		}
		else break;
	}
	return count++;
}



void Com::JudgeChaseHeart() {//27~39 ����
	if (ChaseHeart >= 0) {//ֻ�зǷ�ֹ�з�׷������״̬�Ż��ж�
		int i = 0, j = 0, count = 0;//count��¼�����ƿ����к��ҵĸ����ܺ�
		int heartcount[4] = { 0 };//��¼ÿ����������ƿ��к��Ҹ���
		int heartonly = -1;//��¼ֻ��һ����������ƿ��к������±�
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
				if (CardNum[2] >= 5) {//�������������ƿ��к��ң��Լ����Ҷ����д���
					int maxcount=0;//��¼������
					for (j = 0; j <= 12; j++)
						if (g_pGame->myCard[j].data <= 39 && g_pGame->myCard[j].data >= 35)maxcount++;
					if (maxcount >= 3)ChaseHeart = 1;
					else ChaseHeart = 0;
				}
				else ChaseHeart = 0;
				break;
			}
			case 1: {
				if (heartonly == g_pGame->CurrentOrder - 1 || ChaseHeart == 1) {//Ψһ�����ƿ��к��ҵľ����Լ�
					int maxcount=0;//��¼������
					for (j = 0; j <= 12; j++)
						if (g_pGame->myCard[j].data <= 39 && g_pGame->myCard[j].data >= 35)maxcount++;
					int leftheart = 13 - heartcount[0] - heartcount[1] - heartcount[2] - heartcount[3];//����ʣ�������
					if (maxcount < leftheart % 4 + 2)ChaseHeart = 0;
				}
				else {//Ψһ�����ƿ��к��ҵĲ����Լ�
					for (i = 0; i <= 3; i++) {
						if (heartcount[i] > 8) {
							ChaseHeart = -i - 1;
								if (abs(-ChaseHeart - g_pGame->CurrentOrder) == 2)ChaseHeart = 0;//�ǶԼ���׷������
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