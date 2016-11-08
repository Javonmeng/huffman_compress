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
	int parent, left, right;//父亲，左孩子，右孩子
}huff_tree_node,HuffmanTree;
typedef struct huffman_code
{
	int len;//长度
	char *code;//字符的编码
}HuffmanCode;
#define OK 1
#define ERROR -1
#define ARGS_ERROR -2//参数错误
#define FILE_ERROR -3//文件错误
#define HEAD_ERROR -4//头标错误
#define MALLOC_ERROR -5//内存分配错误
#define HUFFTREE_ERROR -6//HUFFMANTREE错误
#define CHAR_BITS 8//一个char型中的位数
#define INT_BITS 32//一个int型中的位数
#define CODE_SIZE 256//霍夫曼编码个数
#define CACHE_SIZE 256//I/O缓存大小
#define ZIP_HEAD 0xFFFFFFFF//压缩头标文件
#define MAX_NAME_LEN 512
//////////////////////////////////////////
/////            函数声明           //////
/////////////////////////////////////////
//压缩
void SelectNode(HuffmanTree *ht, int n, int &bt1, int &bt2);//找到权值最小的两个节点
int CreateTree(HuffmanTree *&ht, int n, int *w);//创建HUFFMAN树
void HuffmanCoding(HuffmanTree *ht, int n, HuffmanCode *&hc);//生成字符对应的huffman编码,hc为含有各个字符对应编码的数组
void count(char *str, char *&alphabet, int *&weight, int &len_alphabet,int len_file);//返回字符串含有的字符表alphabet和与字符表相对应的一张权重表，以及字符表的长度n
int Encode(HuffmanCode *hc,char *alphabet,char *str, char *&code,int len_file);//将一个字符串转换为huffman编码
void Decode(HuffmanTree *ht, char *code,char *alphabet,int len_alphabet, char *&decode_string,int &len_decode_string);//将一个编码解码为字符串
void char2bit(char *string,int &len_zeros,int &len_bytes, unsigned char *&string_compress);//将以字符串存储的01转换为bit存储的01
void bit2char(char *&string, int len_zeros,int len_file,unsigned char *string_compress);