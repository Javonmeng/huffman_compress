#include "huffman_compress.h"
void SelectNode(HuffmanTree *ht, int n, int &bt1, int &bt2)
//从1~x个结点选择parent结点为0,权重最小的两个结点 
{
	int i;
	huff_tree_node *ht1, *ht2, *t;
	ht1 = ht2 = NULL; //初始化两个结点为空 
	for (i = 1; i <= n; ++i) //循环处理1~n个结点（包括叶结点和非叶结点） 
	{
		if (!ht[i].parent) //父结点为空(结点的parent=0) 
		{
			if (ht1 == NULL) //结点指针1为空 
			{
				ht1 = ht + i; //指向第i个结点 
				continue; //继续循环 
			}
			if (ht2 == NULL) //结点指针2为空 
			{
				ht2 = ht + i; //指向第i个结点 
				if (ht1->weight>ht2->weight) //比较两个结点的权重，使ht1指向的结点权重小 
				{
					t = ht2;
					ht2 = ht1;
					ht1 = t;
				}
				continue; //继续循环 
			}
			if (ht1 && ht2) //若ht1、ht2两个指针都有效 
			{
				if (ht[i].weight <= ht1->weight) //第i个结点权重小于ht1指向的结点 
				{
					ht2 = ht1; //ht2保存ht1，因为这时ht1指向的结点成为第2小的 
					ht1 = ht + i; //ht1指向第i个结点 
				}
				else if (ht[i].weight<ht2->weight) { //若第i个结点权重小于ht2指向的结点 
					ht2 = ht + i; //ht2指向第i个结点 
				}
			}
		}
	}
	if (ht1>ht2) { //增加比较，使二叉树左侧为叶结点 
		bt2 = ht1 - ht;
		bt1 = ht2 - ht;
	}
	else {
		bt1 = ht1 - ht;
		bt2 = ht2 - ht;
	}
}
int CreateTree(HuffmanTree *&ht, int len_alphabet, int *weight)
{//n为叶子节点数，即字符表数目
	int i, len_node = 2 * len_alphabet - 1;//总的节点数
	int bt1, bt2; 
	if (len_alphabet <= 1)
		return HUFFTREE_ERROR; //只有一个结点，无法创建 
	ht = (HuffmanTree *)malloc((len_node + 1) * sizeof(HuffmanTree));
	for (i = 1; i <= len_alphabet; ++i) //初始化叶结点 
	{
		ht[i].weight = weight[i - 1];
		ht[i].parent = 0;
		ht[i].left = 0;
		ht[i].right = 0;
	}
	for (; i <= len_node; ++i)//初始化后续结点 
	{
		ht[i].weight = 0;
		ht[i].parent = 0;
		ht[i].left = 0;
		ht[i].right = 0;
	}
	for (i = len_alphabet + 1; i <= len_node; ++i) //逐个计算非叶结点，创建Huffman树 
	{
		SelectNode(ht, i - 1, bt1, bt2); //从1~i-1个结点选择parent结点为0,权重最小的两个结点 
		ht[bt1].parent = i;
		ht[bt2].parent = i;
		ht[i].left = bt1;
		ht[i].right = bt2;
		ht[i].weight = ht[bt1].weight + ht[bt2].weight;
	}
	return OK;
}
void HuffmanCoding(HuffmanTree *ht, int len_alphabet, HuffmanCode *&hc)
{
	HuffmanCode *cd = NULL;
	int start, i;
	int current, parent;
	hc = (HuffmanCode *)malloc(len_alphabet * sizeof(HuffmanCode));
	cd = (HuffmanCode*)malloc(sizeof(HuffmanCode)); 
	cd->code = (char*)malloc(sizeof(char)*(len_alphabet+1));//用来临时存放一个字符的编码结果 
	cd->code[len_alphabet] = '\0'; //设置字符串结束标志 
	cd->len = 0;
	for (i = 1; i <= len_alphabet; i++)
	{
		start = len_alphabet;
		current = i;
		parent = ht[current].parent;//获取当前结点的父结点 
		while (parent) //父结点不为空 
		{
			if (current == ht[parent].left)//若该结点是父结点的左子树  
				cd->code[--start] = '0'; //编码为0 
			else //若结点是父结点的右子树 
				cd->code[--start] = '1'; //编码为1 
			cd->len++;
			current = parent; //设置当前结点指向父结点 
			parent = ht[parent].parent; //获取当前结点的父结点序号    
		}
		hc[i - 1].code = (char*)malloc(sizeof(char)*(len_alphabet - start));//分配保存编码的内存 
		strcpy(hc[i - 1].code, &(cd->code[start])); //复制生成的编码  
		hc[i - 1].len = cd->len;
	}
	free(cd); //释放编码占用的内存 
}
void count(char *str, char* &alphabet, int* &weight, int &len_alphabet,int len_file)
{
	int i, j;
	int SIZE = 256;
	len_alphabet = 0;
	i = 0;
	alphabet = (char *)malloc(sizeof(char) * SIZE);
	weight = (int *)malloc(sizeof(int) * SIZE);
	alphabet[0] = '\0';
	while (i<len_file)
	{
		for (j = 0; j<len_alphabet; j++)
		{
			if (str[i] == alphabet[j])
			{
				weight[j]++;
				break;
			}
		}
		if (j==len_alphabet)
		{
			weight[j] = 1;
			alphabet[j] = str[i];
			j++;
			len_alphabet++;
			alphabet[j] = '\0';
		}
		i++;
	}
	alphabet[len_alphabet] = '\0';
}
int Encode(HuffmanCode *hc, char *alphabet, char *str, char *&code, int len_file)
//将一个字符串转换为Huffman编码
{
	int INCREMENT = 200000;
	unsigned int SIZE = 200000;
	code = (char*)malloc(sizeof(char)*(SIZE*5));
	long len = 0;
	unsigned int i = 0, j;
	code[0] = '\0';
	while (i<len_file)
	{
		j = 0;
		while (alphabet[j] != str[i])
			j++;
		while (len+strlen(hc[j].code) >= (SIZE - 1))
		{//越界开辟内存
			SIZE += INCREMENT;
			if (!(code = (char*)realloc(code, sizeof(char)*SIZE)))
			{
				return MALLOC_ERROR;
			}
		}
		strcpy(code + len, hc[j].code);
		len = len + strlen(hc[j].code); //累加字符串长度   	
		i++;
		if (i % 1000000 == 0)
		{
			cout.setf(ios::fixed);
			cout << "已完成"<<fixed<< setprecision(2) << 1.0 * i / len_file * 100 << "%" << endl;
		}
	}
	code[len] = '\0';
	return OK;
}

void Decode(HuffmanTree * ht, char * code, char * alphabet, int len_alphabet, char * &decode_string,int &len_decode_string)
{
	int position = 0;
	int i = 0, j = 0;
	int len_node = len_alphabet * 2 - 1;
	int INCREAMENT = 2000000;
	int SIZE = INCREAMENT;
	decode_string = (char*)malloc(sizeof(char)*SIZE);
	while (code[position])
	{
		for (i = len_node; ht[i].left&&ht[i].right; position++)
		{
			if (code[position] == '0')
			{
				i = ht[i].left;
			}
			else
			{
				i = ht[i].right;
			}
		}
		if (j == SIZE - 1)
		{
			SIZE += INCREAMENT;
			decode_string = (char*)realloc(decode_string,sizeof(char)*SIZE);
		}
			decode_string[j] = alphabet[i - 1];
			j++;
	}
	len_decode_string = j - 1;
	decode_string[j] = '\0';
}

void char2bit(char * string,int &len_zeros,int &len_byte,unsigned char *&string_compress)
{
	int len_bit = strlen(string);
	len_byte = len_bit % 8 == 0 ? len_bit / 8 : len_bit / 8 + 1;
	string_compress = (unsigned char*)malloc(sizeof(unsigned char)*len_byte);
	len_zeros = len_byte * 8-len_bit;
	int i = 0;
	unsigned char bit_char = 0;
	while (i<len_bit)
	{
		bit_char = 0;
		for (int j = 0; j < 8; j++)
		{
			bit_char |= string[i] - '0';
			if(j!=7)
				bit_char=bit_char << 1;
			i++;
		}
		if (i % 8 == 0)
			string_compress[i/8-1] = bit_char;
		
	}
	bit_char << len_zeros;
	string_compress[len_byte - 1] = bit_char;
}

void bit2char(char *& string, int len_zeros, int len_file, unsigned char * string_compress)
{
	int len_bit = len_file*8;
	string = (char*)malloc(sizeof(char)*len_bit);
	int i = 0;
	while (i < len_file)
	{
		unsigned int bit_char;
		for (int j = 0; j < 8; j++)
		{
			bit_char = string_compress[i] % 2;
			string_compress[i] = string_compress[i] / 2;
			string[i * 8 + 7 - j] = bit_char + '0';
		}
		i++;
	}
	string[len_bit - len_zeros] = '\0';
}
