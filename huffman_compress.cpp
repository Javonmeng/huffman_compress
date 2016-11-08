#include "huffman_compress.h"
void SelectNode(HuffmanTree *ht, int n, int &bt1, int &bt2)
//��1~x�����ѡ��parent���Ϊ0,Ȩ����С��������� 
{
	int i;
	huff_tree_node *ht1, *ht2, *t;
	ht1 = ht2 = NULL; //��ʼ���������Ϊ�� 
	for (i = 1; i <= n; ++i) //ѭ������1~n����㣨����Ҷ���ͷ�Ҷ��㣩 
	{
		if (!ht[i].parent) //�����Ϊ��(����parent=0) 
		{
			if (ht1 == NULL) //���ָ��1Ϊ�� 
			{
				ht1 = ht + i; //ָ���i����� 
				continue; //����ѭ�� 
			}
			if (ht2 == NULL) //���ָ��2Ϊ�� 
			{
				ht2 = ht + i; //ָ���i����� 
				if (ht1->weight>ht2->weight) //�Ƚ���������Ȩ�أ�ʹht1ָ��Ľ��Ȩ��С 
				{
					t = ht2;
					ht2 = ht1;
					ht1 = t;
				}
				continue; //����ѭ�� 
			}
			if (ht1 && ht2) //��ht1��ht2����ָ�붼��Ч 
			{
				if (ht[i].weight <= ht1->weight) //��i�����Ȩ��С��ht1ָ��Ľ�� 
				{
					ht2 = ht1; //ht2����ht1����Ϊ��ʱht1ָ��Ľ���Ϊ��2С�� 
					ht1 = ht + i; //ht1ָ���i����� 
				}
				else if (ht[i].weight<ht2->weight) { //����i�����Ȩ��С��ht2ָ��Ľ�� 
					ht2 = ht + i; //ht2ָ���i����� 
				}
			}
		}
	}
	if (ht1>ht2) { //���ӱȽϣ�ʹ���������ΪҶ��� 
		bt2 = ht1 - ht;
		bt1 = ht2 - ht;
	}
	else {
		bt1 = ht1 - ht;
		bt2 = ht2 - ht;
	}
}
int CreateTree(HuffmanTree *&ht, int len_alphabet, int *weight)
{//nΪҶ�ӽڵ��������ַ�����Ŀ
	int i, len_node = 2 * len_alphabet - 1;//�ܵĽڵ���
	int bt1, bt2; 
	if (len_alphabet <= 1)
		return HUFFTREE_ERROR; //ֻ��һ����㣬�޷����� 
	ht = (HuffmanTree *)malloc((len_node + 1) * sizeof(HuffmanTree));
	for (i = 1; i <= len_alphabet; ++i) //��ʼ��Ҷ��� 
	{
		ht[i].weight = weight[i - 1];
		ht[i].parent = 0;
		ht[i].left = 0;
		ht[i].right = 0;
	}
	for (; i <= len_node; ++i)//��ʼ��������� 
	{
		ht[i].weight = 0;
		ht[i].parent = 0;
		ht[i].left = 0;
		ht[i].right = 0;
	}
	for (i = len_alphabet + 1; i <= len_node; ++i) //��������Ҷ��㣬����Huffman�� 
	{
		SelectNode(ht, i - 1, bt1, bt2); //��1~i-1�����ѡ��parent���Ϊ0,Ȩ����С��������� 
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
	cd->code = (char*)malloc(sizeof(char)*(len_alphabet+1));//������ʱ���һ���ַ��ı����� 
	cd->code[len_alphabet] = '\0'; //�����ַ���������־ 
	cd->len = 0;
	for (i = 1; i <= len_alphabet; i++)
	{
		start = len_alphabet;
		current = i;
		parent = ht[current].parent;//��ȡ��ǰ���ĸ���� 
		while (parent) //����㲻Ϊ�� 
		{
			if (current == ht[parent].left)//���ý���Ǹ�����������  
				cd->code[--start] = '0'; //����Ϊ0 
			else //������Ǹ����������� 
				cd->code[--start] = '1'; //����Ϊ1 
			cd->len++;
			current = parent; //���õ�ǰ���ָ�򸸽�� 
			parent = ht[parent].parent; //��ȡ��ǰ���ĸ�������    
		}
		hc[i - 1].code = (char*)malloc(sizeof(char)*(len_alphabet - start));//���䱣�������ڴ� 
		strcpy(hc[i - 1].code, &(cd->code[start])); //�������ɵı���  
		hc[i - 1].len = cd->len;
	}
	free(cd); //�ͷű���ռ�õ��ڴ� 
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
//��һ���ַ���ת��ΪHuffman����
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
		{//Խ�翪���ڴ�
			SIZE += INCREMENT;
			if (!(code = (char*)realloc(code, sizeof(char)*SIZE)))
			{
				return MALLOC_ERROR;
			}
		}
		strcpy(code + len, hc[j].code);
		len = len + strlen(hc[j].code); //�ۼ��ַ�������   	
		i++;
		if (i % 1000000 == 0)
		{
			cout.setf(ios::fixed);
			cout << "�����"<<fixed<< setprecision(2) << 1.0 * i / len_file * 100 << "%" << endl;
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
