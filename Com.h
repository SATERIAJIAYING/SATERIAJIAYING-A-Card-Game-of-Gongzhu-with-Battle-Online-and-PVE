#pragma once




class Com
{
public:
	void AcceptShowdownMsg(int order, bool sheep, bool trans, bool heartA, bool pig);//����������Ϣ
	bool Showdown_Sheep();
	bool Showdown_Trans();
	bool Showdown_HeartA();
	bool Showdown_Pig();
	void AcceptPlayMsg(int order, int card);//���ճ����ź�
	int Play();//����


	bool recorder[53] = { 0 }; //������
	int CardNum[4] = { 0 };//��¼����ɫ��ʣ������
	int desire1 = 2, desire2 = 2, desire3 = 2, desire4 = 2;//����ֵ,�ֱ��Ӧ��ѹ�������򡢺���A
	int ChaseHeart=0;//׷������ģʽ��1Ϊ������0Ϊ�رո���Ϊ��ֹ��������ֵorder�������
	bool cardbright[4][4] = { 0 };//��¼��������������
	//cardbrigjt[][0]��Ӧ��1��2��3���ζ�Ӧ��ѹ��������A����                                

	int ColourNum(int);//�����Լ�������ĳ��ɫ������,��������ɫ
	bool Own(int);//�ж��Ƿ���ĳ��,���������ж���
	int Discard();//�������������ֵĻ�ɫ����
	bool IfBright(int m, int n);//mʼ��С��n
	void Recorder(int);//������
	int SafeMin(int, int);//���ذ�ȫ��Сֵ��ָ���������ֵ���£�
	int SafeMax(int,int,int);//���ذ�ȫ�ƴ�ֵ(������Ӧ������,��һ��int������ɫ)	
	bool Judge(int);//�ж��Ƿ��ж�Ӧ��ɫ������
	void SizeSort();//��¼����ɫ��ʣ������
	bool Node();//�ж��Ƿ���ֱ����ڵ�
	bool ComingOver();//�ж���Ϸ�Ƿ񼴽�����
	int MinColour();//���ز�Ϊ0��С��ɫ��
	int GameMaxColour();//��¼������ʣ���ɫ��
	void CardBright(int,int,bool);//��¼��������������
	bool MinCard(int);//�ж�ĳ�ֻ�ɫ�Ƿ���С��	
	int Play2();//�����ѣ��ڶ�������
	int Play3();//�����ѣ�����������
	int Play4();//�����ѣ�ĩλ����
	int Play5();//�е��Ѷȣ�һ�������������г����ж�

	int Play1();//�����ѣ�ׯ��
	int Min(int);//���ض�Ӧ��ɫ�Ƶ�����Сֵ
	int Max(int);//���ض�Ӧ��ɫ�Ƶ������ֵ
	bool NoDifficultPig();//��(������Q��K��Aһ��)
	bool NoDifficultHeartA();//���ҵ������к���A��7~~13�е�һ�ţ�

	void JudgeChaseHeart();//�ж��Ƿ�����׷������ģʽ��������ȼ�
};