//�����ǶԺ����ͱ����Ķ���
#define Lenth 32 
//������ָһ���Դ���ĳ���
//�������ǹ涨һ���Դ���ĳ���Ϊ16�ֽ�
#define zero 0
#define one 1




//���ļ������и���
//SM3�������壺
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
//����SM3���������壺




//SM4�������壺
//����


//SM2�������壺
