#include <iostream>
#include "openssl/evp.h"
#include <time.h> 
using namespace std;




/*
@func:SM3��opensslʵ��
@param����Ϣ��message������Ϣ���ȣ�len��������洢��hash����������ȣ�hash_len��
@return:none
*/
int sm3_hash(char* message, size_t len, unsigned char* hash, unsigned int* hash_len)
{
	EVP_MD_CTX* md_ctx;
	const EVP_MD* md;

	md = EVP_sm3();
	md_ctx = EVP_MD_CTX_new();
	EVP_DigestInit_ex(md_ctx, md, NULL);
	EVP_DigestUpdate(md_ctx, message, len);
	EVP_DigestFinal_ex(md_ctx, hash, hash_len);
	EVP_MD_CTX_free(md_ctx);
	return 0;
}



/*
@func��int��ʮ�������ַ�ת��
@param�����ݣ�int��a������洢ch��char��
@return��none
*/
void int_to_hex(int a, char ch[10]) {
	char mid_char;
	int temp = 0, i = 0;
	while (a / 16 > 0)
	{
		temp = a % 16;
		if (temp > 9)
			ch[i] = temp + 97;
		else
			ch[i] = temp + 48;
		i++;
		a = a / 16;

		if (a < 16)
		{
			ch[i] = a + 48;
			ch[i + 1] = 0;
			break;
		}
	}
	temp = strlen(ch);
	for (i = 0; i < strlen(ch) / 2; i++)
	{
		mid_char = ch[i];
		ch[i] = ch[temp - i - 1];
		ch[temp - i - 1] = mid_char;
	}
}