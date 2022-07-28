//这里是对函数和变量的定义
#define Lenth 32 
//这里是指一次性处理的长度
//这里我们规定一次性处理的长度为16字节
#define zero 0
#define one 1




//该文件最后进行更新
//SM3函数定义：
void left_move_cycle(int& cur, int num);
void right_move_cycle(int& cur, int num);
void bit_1_char(char& cur, int position);
void bit_0_char(char& cur, int position);
void zero_padding(char& cur);
void one_padding(char& cur);
void byte_merge_word(char array_ch[], int start, int& out);
int P1(int word);
int P0(int word);
int FF(int suq, int A, int B, int C);
int GG(int suq, int A, int B, int C);
void initial();
void group_and_padding_1();
void padding();
void message_extend();
void Compress_func();
//关于SM3的其他定义：




//SM4函数定义：
//……


//SM2函数定义：
