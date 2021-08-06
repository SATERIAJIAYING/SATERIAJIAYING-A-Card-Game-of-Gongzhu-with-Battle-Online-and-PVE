#include "pch.h"
#include "game.h"
#include "CDlgGame.h"
#include "CDlgLink.h"
#include "gongzhuDlg.h"
#include "net.h"


void Game::Deal() //��������
{
	
	int Card[4][13] = { 0 };//��ŷֺõ��ƶ�
	int CardNum[4] = { 0 };//�ĸ��ƶѵ�����
	for (int i = 1; i <= 52;)
	{
		int pick = rand() % 4;//��ǰ���ƶ����Ӧ��order
		if (CardNum[pick] >= 13)
			continue;//��ǰ���ƶ�����������13�ţ��������η��ƣ�
		else
		{
			Card[pick][CardNum[pick]] = i;//���Ʒ����ƶ�
			CardNum[pick]++;
			i++;
		}
	}
	//�������ƶ�ֱ�ӱ���
	for (int i = 0; i < 13; i++)
		myCard[i].data = Card[0][i];

	//�ͻ����ƶѻ����źŷ��͸��ͻ���
	string Transmission[3] = { "*","*","*" };//�����±�Ϊorder-1
	//�������  ��*��+���Ʊ�š�+'!'+���Ʊ�š�+'!'+...+��!���磺"*1!2!3!4!5!6!7!8!9!10!11!12!13!"
	for (int i = 1; i <= 3; i++) {
		for (int j = 0; j <= 12; j++)
		{
			Transmission[i - 1] += to_string(Card[i][j]) + "!";
		}
	}

	//�ֱ𴫵�Transmission[3]�����������ͻ���
	for (int i = 1; i < 4; i++)
		g_pServer->Send_Message(Transmission[i - 1], g_pUser[i]->receiver);

}



int Game::JudgeDecor(int card)//�������жϻ�ɫ
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

int Game::JudgeLargest()//����Save�ж�˭��󣬷��������Ƶ����order
{
	int max = 0, min = 0;

	switch (JudgeDecor(Save[0]))
	{
	case 1:max = 13; min = 1; break;
	case 2:max = 26; min = 14; break;
	case 3:max = 39; min = 27; break;
	case 4:max = 52; min = 40;
	}

	int maxSave = 0;//���������Ƶ��±�
	for (int i = 0; i < 4; i++)
		if (Save[i] >= min && Save[i] <= max)
		{
			if (Save[i] == min)//A��ͬ��ɫ�����
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

void Game::ArrangeCard()//����myCard[13]
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

int GetGold(int id_user)//����ID��ȡgold
{
	int IDtemp = 0, Goldtemp = 0;
	fstream mystream("Sav.dat", ios::in | ios::binary);//�ж��ļ��Ƿ����
	if (mystream.good())//�ļ�����
	{
		while (!mystream.eof())
		{
			mystream.read((char*)&IDtemp, sizeof(int));
			mystream.read((char*)&Goldtemp, sizeof(int));

			if (IDtemp == id_user)//��ȡ����IDƥ��
			{
				mystream.close();
				return Goldtemp;
			}
		}

		mystream.clear();//��ȡ����
		mystream.close();
		return 1000;

	}
	else//�ļ�������
		return 1000;

}