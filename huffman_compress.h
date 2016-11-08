#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
using namespace std;
typedef struct huff_tree_node
{
	long weight;
	int parent, left, right;//���ף����ӣ��Һ���
}huff_tree_node,HuffmanTree;
typedef struct huffman_code
{
	int len;//����
	char *code;//�ַ��ı���
}HuffmanCode;
#define OK 1
#define ERROR -1
#define ARGS_ERROR -2//��������
#define FILE_ERROR -3//�ļ�����
#define HEAD_ERROR -4//ͷ�����
#define MALLOC_ERROR -5//�ڴ�������
#define HUFFTREE_ERROR -6//HUFFMANTREE����
#define CHAR_BITS 8//һ��char���е�λ��
#define INT_BITS 32//һ��int���е�λ��
#define CODE_SIZE 256//�������������
#define CACHE_SIZE 256//I/O�����С
#define ZIP_HEAD 0xFFFFFFFF//ѹ��ͷ���ļ�
#define MAX_NAME_LEN 512
//////////////////////////////////////////
/////            ��������           //////
/////////////////////////////////////////
//ѹ��
void SelectNode(HuffmanTree *ht, int n, int &bt1, int &bt2);//�ҵ�Ȩֵ��С�������ڵ�
int CreateTree(HuffmanTree *&ht, int n, int *w);//����HUFFMAN��
void HuffmanCoding(HuffmanTree *ht, int n, HuffmanCode *&hc);//�����ַ���Ӧ��huffman����,hcΪ���и����ַ���Ӧ���������
void count(char *str, char *&alphabet, int *&weight, int &len_alphabet,int len_file);//�����ַ������е��ַ���alphabet�����ַ������Ӧ��һ��Ȩ�ر��Լ��ַ���ĳ���n
int Encode(HuffmanCode *hc,char *alphabet,char *str, char *&code,int len_file);//��һ���ַ���ת��Ϊhuffman����
void Decode(HuffmanTree *ht, char *code,char *alphabet,int len_alphabet, char *&decode_string,int &len_decode_string);//��һ���������Ϊ�ַ���
void char2bit(char *string,int &len_zeros,int &len_bytes, unsigned char *&string_compress);//�����ַ����洢��01ת��Ϊbit�洢��01
void bit2char(char *&string, int len_zeros,int len_file,unsigned char *string_compress);