#pragma once
#include<iostream>
#include"SHA256.h"
//这里设置了关于SHA256的两个接口：
//接口1：从内存输入：
/*
@func:SHA256的填充函数
@param：消息分组msg，消息分组长度len,输出存储out，可选参数：消息总长（NUM），补充输出（out_add）
@return：none
*/
void padding(unsigned char msg[], int len, unsigned char out[],unsigned long long int NUM=0,unsigned char out_add[]=nullptr)
{
	int i = 0;
	int j = 0;
	for (; i < len; i++, j++)
	{
		out[j] = msg[i];
	}
	//下面使用j来操作
	if (len < 56) 
	{
		out[j] = 0x80;
		j++;
		while (j < 56)
		{
			out[j] = 0x00;
			j++;
		}
		int k = 63;
		while (k >= 56)
		{
			unsigned char a = NUM % 256;
			NUM = NUM >> 8;
			out[k] = a;
			k--;
		}
	}
	else if (len == 56)
	{
		int k = 64;
		while (k >= 56)
		{
			unsigned char a = NUM % 256;
			NUM = NUM >> 8;
			out[k] = a;
			k--;
		}
	}
	else if (len > 56)
	{
		if(len!=64)
			out[j] = 0x80;
		while (j < 64)
		{
			out[j] = 0x00;
			j++;
		}
		//接下来操作out_add
		j = 0;
		if (len == 64)
		{
			out_add[j] = 0x80;
			j++;
		}
		while (j < 56)
		{
			out_add[j] = 0x00;
			j++;
		}
		int k = 6;
		while (k >= 56)
		{
			unsigned char a = NUM % 256;
			NUM = NUM >> 8;
			out_add[k] = a;
			k--;
		}
	}
}

/*
@func：实现SHA256
@param：消息msg，输出存储(out_IV)
@return：none
*/
void SHA256_hash(unsigned char msg[], unsigned int out_IV[])
{
	int READ_NUM = 0;
	int len = strlen((char*)msg);
	unsigned int buf[8] = { 0x6a09e667ul, 0xbb67ae85ul, 0x3c6ef372ul, 0xa54ff53aul, 0x510e527ful, 0x9b05688cul, 0x1f83d9abul, 0x5be0cd19ul };
	if (!len)
	{
		unsigned char Padding[64] = { '\0' };//这里也许会出现越界问题
		padding(msg, 0, Padding);
		tr(buf, Padding);
	}
	while (READ_NUM < len)
	{
		unsigned char Data[64] = { '\0' };
		unsigned char Data_add[64] = { '\0' };
		unsigned char Padding[64] = {'\0'};//这里也许会出现越界问题
		int i = 0;
		for (; i < 64&&READ_NUM<len; i++,READ_NUM++)
		{
			Data[i] = msg[READ_NUM];
		}
		if (i < 56)
		{
			padding(Data, i, Padding,len*8);
			tr(buf, Padding);
		}
		else if (i >= 56)
		{

			padding(Data, i, Padding, len*8, Data_add);
			tr(buf, Padding);
			tr(buf, Data_add);
		}
		else//这里就是等于64的时候
		{
			padding(Data, i, Padding);
			tr(buf, Padding);
		}
		//接下来就是操作Padding

	}
	out_IV[0] = buf[0];
	out_IV[1] = buf[1];
	out_IV[2] = buf[2];
	out_IV[3] = buf[3];
	out_IV[4] = buf[4];
	out_IV[5] = buf[5];
	out_IV[6] = buf[6];
	out_IV[7] = buf[7];
}