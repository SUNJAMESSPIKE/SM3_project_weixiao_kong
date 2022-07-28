#pragma once
//这里是用于构造简化版本的markle_tree
//hash算法使用SHA256
#include<openssl/sha.h>
#include<iostream>
#include"define.h"
#include"SHA256_interface.h"

using namespace std;
struct hash_node;
struct DATA;
union inttochar;



union inttochar
{
	unsigned int hash_int[16];
	unsigned char hash_char[64];
};

struct DATA
{
	unsigned char msg[10];
	int len;
	hash_node* ptr_hash;
};

struct hash_node
{
	unsigned int IV[8];//hash值
	//int id;
	hash_node* left = nullptr;
	hash_node* right = nullptr;
	hash_node* parent = nullptr;
	DATA* ptr_data = nullptr;
};

struct hash_root
{
	int Record_node;//用于记录node栈使用
	int Record_data;//用于记录data栈使用情况
	int stack_start;//数据起始位置
	hash_node* root = nullptr;
};

DATA data_list[100];
hash_node* node_stack[100];

/****************************************/
void copy_IV(unsigned int IV_1, unsigned int IV_2);
void copy_msg(unsigned char msg[], unsigned char msg_1[]);
void cin_data(DATA* ptr);
void CreatHashnode(hash_node* out_node,hash_node* node_1 = nullptr, hash_node* node_2 = nullptr, DATA* node_3 = nullptr);

/****************************************/

void copy_IV(unsigned int* IV_1, unsigned int* IV_2)
{
	for (int i = 0; i < 8; i++)
	{
		IV_1[i] = IV_2[i];
	}
}
void copy_msg(unsigned char msg[], unsigned char msg_1[])
{
	int i = 0;
	char a = msg_1[i];
	while (a)
	{
		msg[i] = a;
		i++;
		a = msg_1[i];
	}
}
/*
@func：输入DATA数据
@param：
*/
void cin_data(DATA* ptr)
{
	cout << "please input the data: " << endl;
	cin >> ptr->msg;
	ptr->len = strlen((char*)ptr->msg);
}
void CreatHashnode(hash_node* out_node,hash_node* node_1, hash_node* node_2,DATA*node_3)
{
	if (node_3 && !(node_1 || node_2))
	{
		unsigned int IV[8];
		SHA256_hash(node_3->msg, IV);
		copy_IV(out_node->IV, IV);
		//指针变化
		out_node->ptr_data = node_3;
		node_3->ptr_hash = out_node;
	}
	else if (node_1 && node_2 && !node_3)
	{
		inttochar conv;
		copy_IV(conv.hash_int, node_1->IV);
		copy_IV(conv.hash_int+8, node_2->IV);
		unsigned int IV[8];
		SHA256_hash(conv.hash_char, IV);
		copy_IV(out_node->IV, IV);
		//指针变化：
		out_node->left = node_1;
		out_node->right = node_2;
		node_1->parent = out_node;
		node_2->parent = out_node;
	}
	else if (!node_1&&node_2&& !node_3)
	{
		//这里要遵从靠右原则
		inttochar conv;
		copy_IV(conv.hash_int, node_2->IV);
		copy_IV(conv.hash_int + 8, node_2->IV);
		unsigned int IV[8];
		SHA256_hash(conv.hash_char, IV);
		copy_IV(out_node->IV, IV);
		//指针变化：
		out_node->left = nullptr;
		out_node->right = node_2;
		node_2->parent = out_node;
	}
	else if (node_1 && !node_2 && !node_3)
	{
		//这里要遵从靠右原则
		inttochar conv;
		copy_IV(conv.hash_int, node_1->IV);
		copy_IV(conv.hash_int + 8, node_1->IV);
		unsigned int IV[8];
		SHA256_hash(conv.hash_char, IV);
		copy_IV(out_node->IV, IV);
		//指针变化：
		out_node->left = nullptr;
		out_node->right = node_1;
		node_1->parent = out_node;
	}
	else
	{
		cout << "error:wrong paramter!" << endl;
		return;
	}
	return;
}
void CreatDatalist()
{
	int i = 0;
	while (i<100)
	{
		DATA* ptr = &data_list[i];
		unsigned char msg[] = { 't','r','a','d','e','\0'};
		copy_msg(ptr->msg, msg);
		ptr->len = strlen((char*)msg);
		i++;
	}
}
void EVI_change_data(int i)
{
	DATA* ptr = &data_list[i];
	unsigned char msg[] = "spikecome";
	copy_msg(ptr->msg, msg);
	ptr->len = strlen((char*)msg);
	i++;
}
hash_root* creat_markle_tree(int start,int num_data) 
{
	int Record_node = 0;//用于记录node栈使用
	int Record_data = 0;//用于记录data栈使用情况
	//起始定义4个数据
	int i = start;//定义起始位置
	//while (i<num_data+start)
	//{
	//	DATA* ptr = &data_list[i];
	//	unsigned char msg[] = { 't','r','a','d','e',':',49+i-start,'\0'};
	//	copy_msg(ptr->msg, msg);
	//	/*strcpy_s((char*)ptr->msg,10,(char*) msg);*/
	//	ptr->len = strlen((char*)msg);
	//	Record_data++;
	//	i++;
	//}
	//i = start;
	while (i < num_data+start)
	{
		hash_node* mynode = new hash_node;
		node_stack[Record_node] = mynode;
		Record_node++;
		CreatHashnode(mynode, nullptr, nullptr, &data_list[i]);
		i++;
	}
	//底层创建完毕
	int cur = 0;
	int pale = Record_node;
	while (cur != Record_node)
	{
		if (cur + 1 < pale)
		{
			hash_node* mynode = new hash_node;
			node_stack[Record_node] = mynode;
			Record_node++;
			CreatHashnode(mynode, node_stack[cur], node_stack[cur + 1], nullptr);
			cur += 2;
			if (cur == pale)
				pale = Record_node;
		}
		else if (cur + 1 == pale && cur + 1 != Record_node)
		{
			hash_node* mynode = new hash_node;
			node_stack[Record_node] = mynode;
			Record_node++;
			CreatHashnode(mynode, node_stack[cur], nullptr, nullptr);
			cur += 1;
			pale = Record_node;
		}
		else
			cur++;
	}
	hash_root* root = new hash_root;
	root->Record_data = Record_data;
	root->Record_node = Record_node;
	root->root = node_stack[Record_node - 1];
	root->stack_start = 0;//这里默认为0
	return root;//返回根节点
}

void insert_data(hash_root*root)
{
	int start = root->stack_start;//这里默认为0；
	int Record_node = root->Record_node;
	int Record_data = root->Record_data;
	//创建取出新的data空间
	DATA* ptr = &data_list[Record_data];
	unsigned char msg[] = { 'n','e','w','t','r','a','d','e','\0' };
	copy_msg(ptr->msg, msg);
	Record_data++;
	//更新hash值
	hash_node* ptr_node=root->root;
	hash_node* ptr_temp = nullptr;
	hash_node* ptr_parent = nullptr;
	while (ptr_node)
	{
		ptr_temp = ptr_node;
		ptr_node = ptr_node->right;
	}
	ptr_node=ptr_temp;
	ptr_parent = ptr_node->parent;
	//继承：
	if (!ptr_parent->left)
	{
		hash_node* mynode = new hash_node;
		CreatHashnode(mynode, nullptr, nullptr, ptr);
		ptr_parent->left = mynode;
	}
	else
	{
		hash_node* mynode = new hash_node;
		hash_node* Left = new hash_node;
		//指针变化：
		mynode->parent = ptr_node->parent;
		ptr_node->parent = mynode;
		mynode->left = Left;
		mynode->right = ptr_node;
		//left更新：
		CreatHashnode(Left, nullptr, nullptr, ptr);
		ptr_parent = mynode;
	}
	//hash更新：
	while (!ptr_parent)
	{
		CreatHashnode(ptr_parent, ptr_parent->left, ptr_parent->right, nullptr);
		ptr_parent = ptr_parent->parent;
	}
	//end
}
bool cmp_IV(unsigned int IV1[], unsigned int IV2[])
{
	for (int i = 0; i < 8; i++)
	{
		if (IV1[i] != IV2[i])
			return false;
	}
	return true;
}
void search(hash_node* node1, hash_node* node2)
{
	if (!node1 && !node2)
		return;
	if(!cmp_IV(node1->IV, node2->IV))
	{
		if (!node1->ptr_data && !node2->ptr_data)
		{
			//cout<<
			search(node1->left, node2->left);
			search(node1->right, node2->right);
		}
		else
		{
			cout << "error:\n";
			cout << "the data :  " << node2->ptr_data->msg << "  is wrong!" << endl;
		}
	}
	else 
	{
		return;
	}
}
void iteration(hash_node* node,int deep=0)
{
	if (!node)
		return;
	else if (!node->left && !node->right)
	{
		for (int i = 0; i < deep; i++)
		{
			cout << "                                    ";
		}
		cout << "-------->";
		for (int i = 0; i < 8; i++)
		{
			cout <<hex<<node->IV[i];
		}
		cout << endl;	
	}
	else
	{
		iteration(node->left, deep + 1);
		for (int i = 0; i < deep; i++)
		{
			cout << "                                    ";
		}
		cout << "-------->";
		for (int i = 0; i < 8; i++)
		{
			cout << hex << node->IV[i];
		}
		cout << endl;
		iteration(node->right, deep + 1);	
	}
}