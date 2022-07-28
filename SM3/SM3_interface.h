#pragma once
#include"SM3_function.h"
//�����ṩ�˹��ڵ���SM3�������ӿ�
//�ӿ�1��ֱ��������Ϣ
//�ӿ�2�����ļ��ж�����Ϣ



/*
@func���ӿ�1��ֱ������
@param��
@return��
*/
void SM3_hash(char msg[],unsigned int len,unsigned int hash_out[])
{
	int NUM = 0;//�Ѷ�������
	while(NUM<len)
	{
		int READ_NUM = 0;
		for (; READ_NUM < 64 && NUM < len; READ_NUM++, NUM++)
		{
			Data[READ_NUM] = msg[NUM];
		}
		Data[READ_NUM] = '\0';//������
		if (READ_NUM == 64)
		{
			message_extend();
			Compress_func();
		}
		else if (READ_NUM <= 56)
		{
			padding(NUM);
			message_extend();
			Compress_func();
		}
		else
		{
			padding(NUM);
			message_extend();
			Compress_func();
			for (int i = 0; i < 65; i++)
			{
				Data[i] = Data_add[i];
			}
			message_extend();
			Compress_func();
		}
	}
	for (int i = 0; i < 8; i++)
	{
		hash_out[i] = IV_1[i];
	}
	//end
}



/*
@func:�ӿ�2���ⲿ���������ļ���ȡ+��䣩
@param��msg(����)��len�����ĳ��ȣ���IV_hash(����洢 )
@return��none
*/
void iterator_sm3()
{
	int NUM = 0;//���ڼ�¼�ļ����ַ�
	int READ_NUM = 0;//���ļ��ж����ַ��ĸ�������512Ϊһ��
	//�˴����ļ���
	ifstream infile;
	infile.open("../data_cin.txt", ios::in);
	while (infile.peek() != EOF)
	{
		char a = '\0';
		for (READ_NUM = 0; READ_NUM < 64 && infile.peek() != EOF; READ_NUM++, NUM++)
		{
			Data[READ_NUM] = infile.get();
		}
		Data[READ_NUM] = '\0';//������
		if (READ_NUM == 64)
		{
			message_extend();
			Compress_func();
		}
		else if (READ_NUM <= 56)
		{
			padding(NUM);
			message_extend();
			Compress_func();
		}
		else
		{
			padding(NUM);
			message_extend();
			Compress_func();
			for (int i = 0; i < 65; i++)
			{
				Data[i] = Data_add[i];
			}
			message_extend();
			Compress_func();
		}
	}
	//�����
	cout << "THE HASH IS :" << endl;
	for (int i = 0; i < 8; i++)
	{
		cout << hex << IV_1[i] << "\t";
	}
	cout << endl;
}
