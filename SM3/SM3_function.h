//�����ǻ����������壺
//���ε��ԣ�����ֻ����һ����������ݣ�32bits��
#include<iostream>
#include<fstream>
using namespace std;
//�������壺
static char Data[65] = { '\0' };//����Ԥ����һ���ո����
static char Data_add[65] = { '\0' };//����һЩ�������
static unsigned int words_0[68] = { 0 };
static unsigned int words_1[64] = { 0 };
static int T[64] = { 0 };
static int IV_1[8] = { 0x7380166f,0x4914b2b9,0x172442d7,0xda8a0600,0xa96f30bc,0x163138aa,0xe38dee4d,0xb0fb0e4e };


/*
@func�������С��ת��
@param����ַ����
@return��none
*/
uint32_t reversebytes_uint32t(uint32_t value) 
{
	return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 |
		(value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24;
}

/*
@func��ѭ�����ƺ���
@param���ֱ�����ַ��int��������λ����int��
@return��none
*/
void left_move_cycle(int& cur, int num)
{
	//ע�⣺�������߼��ϵ����ƺ����ƣ�����ʵ�ʴ洢���
	int temp1 = cur;
	int temp2 = cur;
	temp1=temp1 << num;
	temp2 = temp2 >> (31 - num);
	temp2 = temp2 >> 1;
	cur = temp1 ^ temp2;
}


/*
@func��ѭ�����ƺ���
@param���ֱ�����ַ��int��������λ����int��
@return��none
*/
void left_move_cycle(unsigned int& cur, int num)
{
	unsigned int temp1 = cur;
	unsigned int temp2 = cur;
	temp1 = temp1 << num;
	temp2 = temp2 >> (31 - num);
	temp2 = temp2 >> 1;
	cur = temp1 ^ temp2;
}

/*
@func��ѭ�����ƺ���
@param���ֱ�����ַ��int��������λ����int��
@return��none
*/
void right_move_cycle(int& cur, int num)
{
	//ע�⣺�������߼��ϵ����ƺ����ƣ�����ʵ�ʴ洢���
	int temp1 = cur;
	int temp2 = cur;
	temp1 = temp1 >> num;
	temp2 = temp2 << (31 - num);
	temp2 = temp2 << 1;
	cur = temp1 ^ temp2;
}
/*
@func��ѭ�����ƺ���
@param���ֱ�����ַ��int��������λ����int��
@return��none
*/
void right_move_cycle(unsigned int& cur, int num)
{
	//ע�⣺�������߼��ϵ����ƺ����ƣ�����ʵ�ʴ洢���
	unsigned int temp1 = cur;
	unsigned int temp2 = cur;
	temp1 = temp1 >> num;
	temp2 = temp2 << (31 - num);
	temp2 = temp2 << 1;
	cur = temp1 ^ temp2;
}

/*
@func���޸�intָ��λΪ1(Ĭ�ϴ��ģʽ)
@param:�޸ĵĵ�ַ���޸ĵ�λ�ã�0������
@return��none
*/
void bit_1_char(char& cur,int position)
{
	char a = 1<<position;
	cur = cur | a;
}
/*
@func���޸�intָ��λΪ0(Ĭ�ϴ��ģʽ)
@param:�޸ĵĵ�ַ���޸ĵ�λ�ã�0������
@return��none
*/
void bit_0_char(char& cur, int position)
{
	char a = 1 << position;
	a = ~a;
	cur = cur & a;
}
/*
@func��ȫ0���
@param��char��ַ����
@return��none
*/
void zero_padding(char& cur)
{
	cur = cur << 8;
}
/*
@func��ȫ1���
@param��char��ַ����
@return��none
*/
void one_padding(char& cur)
{
	cur = cur << 8;
	cur = ~cur;
}

/*
@func�����ĸ��ֽںϲ�Ϊһ����,������ΪС��ģʽ
@param��������ʼ��ַ��addr������ʼ����
@return:none
*/
void byte_merge_word(char array_ch[], int start,unsigned int& out)
{
	unsigned int temp1 = array_ch[start];
	unsigned int temp2 = array_ch[start + 1];
	unsigned int temp3 = array_ch[start + 2];
	unsigned int temp4 = array_ch[start + 3];
	out = temp1 + (temp2 << 8) + (temp3 << 16) + (temp4 << 24);
	out=reversebytes_uint32t(out);
}

/*
@func��P1�û�
@param���֣�int��
@return���֣�int��
*/
unsigned int P1(unsigned int word)
{
	unsigned int temp1 = word;
	unsigned int temp2 = word;
	left_move_cycle(temp1, 15);
	left_move_cycle(temp2, 23);
	return word ^ temp1 ^ temp2;
}
/*
@func:P2�û�
@param���֣�int��
@return���֣�int��
*/
unsigned int P0(unsigned int word)
{
	unsigned int temp1 = word;
	unsigned int temp2 = word;
	left_move_cycle(temp1, 9);
	left_move_cycle(temp2, 17);
	return word ^ temp1 ^ temp2;
}

/*
@func��FF����
@param��A,B,C��int��,�Լ�������ţ�int��
@return��int
*/
unsigned int FF(int suq, unsigned int A, unsigned int B, unsigned int C)
{
	if (suq < 16) 
	{
		return A ^ B ^ C;
	}
	else
	{
		return (A & B) | (A & C) | (B & C);
	}
}
/*
@func��GG����
@param��ABC��int������ţ�int��suq
@return��int
*/
unsigned int GG(int suq,unsigned int A,unsigned int B,unsigned int C)
{
	if (suq < 16)
	{
		return A ^ B ^ C;
	}
	else
		return (A & B) | (~A & C);
}
/*
@func��ֱ�Ӷ�������
@param��none
@return��none
*/
void padding(int NUM)
{
	int i = strlen(Data);
	unsigned long long int Len = NUM * 8;//bit lenth��mod2^64��
	int L = 64 - i % 64;
	//padding 1:
	if (L > 8)//(i<64)
	{
		char a = 1 << 7;
		Data[i] = a;
		i++;
		L--;
		while (L > 8)
		{
			zero_padding(Data[i]);
			i++;
			L--;
		}//padding end:�����ƺ���̫��Ҫ����Ϊ�������ʱ����ȫ0
		//���������ǳ�����䣺
		char temp = '\0';
		for (int i = 63; L > 0; i--, L--)
		{
			temp = Len % 256;
			Len = Len >> 8;
			Data[i] = temp;
		}
	}
	else
	{
		char a = 1 << 7;
		Data[i] = a;
		i++;
		L--;
		while (L)
		{
			zero_padding(Data[i]);
			i++;
			L--;
		}
		L += 64;
		while (L>8)
		{
			zero_padding(Data_add[i]);
			i++;
			L--;
		}
		char temp = '\0';
		for (int i = 63; L > 0; i--, L--)
		{
			temp = Len % 256;
			Len = Len >> 8;
			Data_add[i] = temp;
		}
	}
	//padding 2:lenth padding
	
}
/*
@func����Ϣ��չ�㷨(ע�⣺�����Ƕ�һ���������ݽ�����չ)
@param��none
@return��none
*/
void message_extend()
{
	//ǰ16���ֶ���
	for (int i = 0; i < 16; i++)
	{
		byte_merge_word(Data, i * 4, words_0[i]);
	}
	for (int i = 16; i < 68; i++)
	{
		unsigned int temp1 = words_0[i - 3];
		unsigned int temp2 = words_0[i - 13];
		left_move_cycle(temp1, 15);
		left_move_cycle(temp2, 7);
		words_0[i] = P1(words_0[i - 16] ^ words_0[i - 9] ^ temp1) ^ temp2 ^ words_0[i - 6];
	}
	//words_0����
	for (int i = 0; i < 64; i++)
	{
		words_1[i] = (words_0[i] ^ words_0[i + 4]);
	}
}
/*
@func��ѹ������
@param��none
@return��none 
*/
void Compress_func()
{
	//����ĵ�������Ҫ�����һ��
	unsigned int A = (IV_1[0]), B = (IV_1[1]), C = (IV_1[2]), D = (IV_1[3]), E = (IV_1[4]), F = (IV_1[5]), G =(IV_1[6]), H = (IV_1[7]);
	//cout << hex << A;
	
	
	unsigned int A1 = A, B1 = B, C1 = C, D1 = D, E1 = E, F1 = F, G1 = G, H1 = H;
	unsigned int SS1 = 0, SS2 = 0, TT1 = 0, TT2 = 0;
	unsigned int temp_1 = 0x79cc4519;
	unsigned int temp_2 = 0x7a879d8a;
	//����T��ʼ����
	for (int i = 0; i < 16; i++)
	{
		T[i] =temp_1;
	}
	for (int i = 16; i < 64; i++)
	{
		T[i] = temp_2;
	}
	for (int i = 0; i < 64; i++)
	{
		//SS1������
	    unsigned int tempA_1 = A;
		left_move_cycle(tempA_1, 12);
		unsigned int tempT_1 = T[i];
		left_move_cycle(tempT_1, i%32);
		tempT_1 = tempA_1 + E + tempT_1;
		left_move_cycle(tempT_1, 7);
		SS1 = tempT_1;
		//SS2����
		tempA_1 = A;
		left_move_cycle(tempA_1, 12);
		SS2 = SS1 ^ tempA_1;
		//TT1������
		TT1 = FF(i, A, B, C) + D + SS2 + words_1[i];
		//TT2������
		TT2 = GG(i, E, F, G) + H + SS1 + words_0[i];
		//��������
		D = C;
		unsigned int temp_B = B;
		left_move_cycle(temp_B, 9);
		C = temp_B;
		B = A;
		A = TT1;
		H = G;
		unsigned int temp_F = F;
		left_move_cycle(temp_F, 19);
		G = temp_F;
		F = E;
		E = P0(TT2);
	}
	//IV���£�
	IV_1[0] = (A ^ A1); IV_1[1] = (B ^ B1); IV_1[2] = (C ^ C1); IV_1[3] = (D ^ D1); IV_1[4] = (E ^ E1); IV_1[5] = (F ^ F1); IV_1[6] = (G ^ G1); IV_1[7] = (H ^ H1);
} 






