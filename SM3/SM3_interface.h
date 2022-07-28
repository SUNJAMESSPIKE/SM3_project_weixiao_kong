#pragma once
#include"SM3_function.h"
//这里提供了关于调用SM3的两个接口
//接口1：直接输入信息
//接口2：从文件中读入信息



/*
@func：接口1：直接输入
@param：
@return：
*/
void SM3_hash(char msg[],unsigned int len,unsigned int hash_out[])
{
	int NUM = 0;//已读数据量
	while(NUM<len)
	{
		int READ_NUM = 0;
		for (; READ_NUM < 64 && NUM < len; READ_NUM++, NUM++)
		{
			Data[READ_NUM] = msg[NUM];
		}
		Data[READ_NUM] = '\0';//结束符
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
@func:接口2：外部迭代器（文件读取+填充）
@param：msg(明文)，len（明文长度），IV_hash(输出存储 )
@return：none
*/
void iterator_sm3()
{
	int NUM = 0;//用于记录文件总字符
	int READ_NUM = 0;//从文件中读入字符的个数，以512为一组
	//此处打开文件：
	ifstream infile;
	infile.open("../data_cin.txt", ios::in);
	while (infile.peek() != EOF)
	{
		char a = '\0';
		for (READ_NUM = 0; READ_NUM < 64 && infile.peek() != EOF; READ_NUM++, NUM++)
		{
			Data[READ_NUM] = infile.get();
		}
		Data[READ_NUM] = '\0';//结束符
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
	//输出：
	cout << "THE HASH IS :" << endl;
	for (int i = 0; i < 8; i++)
	{
		cout << hex << IV_1[i] << "\t";
	}
	cout << endl;
}
