#include<iostream>
#include"SM3_naive_attack.h"
//#include"SM3_function.h"
#include"markle_tree.h"
#include"SHA256_interface.h"
#include"SM3_extension_attack.h"
#include"SM3_interface.h"



using namespace std;
#define SM3_naive_attack 0//运行SM3生日攻击
//#define SM3_naive_attack 1

#define SM3_extension_attack 0//运行SM3长度扩展攻击
//#define SM3_extension_attack 1

#define SHA256_extension_attack 0//运行SHA256长度扩展攻击
//#define SHA256_extension_attack 1

#define SM3_optimize 0 //实行SM3优化算法
//#define SM3_optimize 1

//#define Markle_tree 0 //运行markle树
#define Markle_tree 1

#define SHA256 0//运行SHA256
//#define SHA256 1

#define Debug 0
//#define Debug 1 




#if SM3_naive_attack==1
int main(void)
{
	cout << "接下来运行针对SM3的生日攻击（这里只匹配16bits前缀)：\n";
	unsigned char hash_value2[64];
	unsigned char hash_value[64];
	unsigned int i, hash_len;
	char sample2[] = 
	{ 
										 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
										 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
										 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
										 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
										 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
										 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
										 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
										 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64 
	};
	unsigned int sample2_len = sizeof(sample2);
	sm3_hash(sample2, sample2_len, hash_value, &hash_len);
	printf("raw data:\n");
	for (i = 0; i < sample2_len; i++)
	{
		printf("0x%x  ", sample2[i]);
	}
	printf("\n");
	printf("hash length: %d bytes.\n", hash_len);
	printf("hash value:\n");
	for (i = 0; i < hash_len; i++)
	{
		printf("0x%x  ", hash_value[i]);
	}
	printf("\n");

	//构造消息，寻找一个碰撞
	char sample[10];
	srand((unsigned)time(NULL));
	int a = rand() * rand();
	for (int i = a; i < a + 0xfffff; i++) {
		int_to_hex(i, sample);
		unsigned int sample_len = strlen((char*)sample);
		sm3_hash(sample, sample_len, hash_value2, &hash_len);
		//摘要值的前16bit相同就算是一次碰撞
		if (hash_value2[0] == hash_value[0] && hash_value2[1] == hash_value[1])
		{
			printf("找到一个16bit前缀碰撞,共检索%d次\n", i - a);
			printf("raw data: %x\n", i);
			printf("hash length: %d bytes.\n", hash_len);
			printf("hash value:\n");
			for (int j = 0; j < hash_len; j++)
			{
				printf("0x%x  ", hash_value2[j]);
			}
			printf("\n\n");
			break;
		}
	}
	return 0;
}
#elif SM3_extension_attack==1
int main()
{
	unsigned char Hash[32] = { 0 };
	unsigned char Hash2[32] = { 0 };
	unsigned char Hash3[32] = { 0 };
	unsigned int* hash_len = NULL;
	unsigned char str[] = { 'a','b','c',0 };
	//66c7f0f4 62eeedd9 d1f2d46b dc10e4e2 4167c487 5cf2f7a2 297da02b 8f4ba8e0
	sm3(str, strlen((char*)str), Hash, hash_len);
	for (int i = 0; i < 32; i++)
	{
		printf("0x%x  ", Hash[i]);
	}
	printf("\n");
	//预计算hash(m1||m2),其中str2=m1||m2
	unsigned char str2[100] = { 0 };
	str2[0] = 'a';
	str2[1] = 'b';
	str2[2] = 'c';
	str2[3] = 0x80;
	str2[63] = 0x18;
	str2[64] = 'a';
	str2[65] = 'b';
	str2[66] = 'c';
	//66c7f0f4 62eeedd9 d1f2d46b dc10e4e2 4167c487 5cf2f7a2 297da02b 8f4ba8e0
	sm3(str2, 67, Hash2, hash_len);
	for (int i = 0; i < 32; i++)
	{
		printf("0x%x  ", Hash2[i]);
	}
	printf("\n");

	//长度扩展攻击，不知道原始数据m1，但知道后添加在尾部的数据m2及数据总长度NSrcLen
	unsigned int tempHash[8] = { 0 };     //经历完一轮压缩函数的哈希值
	unsigned int temp = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 4 * i + 3; j >= 4 * i; j--) {
			temp = Hash[j];
			temp = temp * pow(256, (4 * i + 3 - j));
			tempHash[i] += temp;
		}
	}
	//对m2进行消息填充
	int nSrcLen = strlen((char*)str);
	int NSrcLen = strlen((char*)str) + 64;
	unsigned int nGroupNum = (nSrcLen + 1 + 8 + 64) / 64;
	unsigned char* ucpMsgBuf = (unsigned char*)malloc(nGroupNum * 64);
	memset(ucpMsgBuf, 0, nGroupNum * 64);
	memcpy(ucpMsgBuf, str, nSrcLen);
	ucpMsgBuf[nSrcLen] = 0x80;
	int i = 0;
	for (i = 0; i < 8; i++)
	{
		ucpMsgBuf[nGroupNum * 64 - i - 1] = ((unsigned long long)(NSrcLen * 8) >> (i * 8)) & 0xFF;
	}
	//执行压缩函数，两个输入分别是上一轮压缩函数的哈希值和要添加到尾部的消息的填充值
	//for (int i = 0; i < 64; i++) {
	//	printf("%x", ucpMsgBuf[i]);
	//}
	_CF(ucpMsgBuf, tempHash);
	free(ucpMsgBuf);
	for (i = 0; i < 8; i++)
	{
		Hash3[i * 4 + 0] = (unsigned char)((tempHash[i] >> 24) & 0xFF);
		Hash3[i * 4 + 1] = (unsigned char)((tempHash[i] >> 16) & 0xFF);
		Hash3[i * 4 + 2] = (unsigned char)((tempHash[i] >> 8) & 0xFF);
		Hash3[i * 4 + 3] = (unsigned char)((tempHash[i] >> 0) & 0xFF);
	}
	for (int i = 0; i < 32; i++)
	{
		printf("0x%x  ", Hash3[i]);
	}
	printf("\n");
	for (int i = 0; i < 32; i++) {
		if (Hash2[i] == Hash3[i]) {
			if (i != 31)
				continue;
			else
				printf("伪造成功！");
		}
		else
			printf("伪造失败！");
	}
}

#elif SHA256_extension_attack==1
int main() 
{
	std::string x = "hello world";
	cout << sha256(x) << endl;
	return 0;
}
#elif SM3_optimize==1
int main()
{
	iterator_sm3();
}
#elif Markle_tree==1
int main()
{
	CreatDatalist();
	hash_root* root_1=creat_markle_tree(1,7);
	EVI_change_data(1);
	EVI_change_data(4);
	hash_root* root_2 = creat_markle_tree(1, 7);
	search(root_1->root, root_2->root);
	iteration(root_1->root);
	cout << "\n\n\n\n\n\n\n";
	iteration(root_2->root);


}
#elif SHA256==1


#elif Debug==1
int main()
{
	int i = 0;
	int j = 0;
	while (i < 4) {
		i++;
		j++;
	}
	cout << j;
}
#endif // SM3_naive_attack==1

