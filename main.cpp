#include "huffman_compress.h"
using namespace std;
int main()
{
	char *filename = "string1.txt";//此处为压缩文件名
	FILE *fp;
	if (!(fp = fopen(filename, "rb")))
		return FILE_ERROR;
	fseek(fp, 0, SEEK_END);
	int len_file = ftell(fp);
	char *string = (char *)malloc(sizeof(char) * (len_file+1));
	fseek(fp, 0, SEEK_SET);
	fread(string, len_file, sizeof(char),fp);
	string[len_file] = '\0';
	fclose(fp);

	std::cout << "读入文件：" << filename << std:: endl;
	cout << "文件大小为:" << len_file << "k" << endl;
	char *alphabet = NULL;//记得free 
	int *weight = NULL;//记得free
	int len_alphabet = 0;

	puts("统计字节及权重...");
	count(string, alphabet, weight, len_alphabet,len_file);
	puts("统计完成");

	HuffmanTree *ht = NULL;
	puts("构造Huffman树中...");
	CreateTree(ht, len_alphabet, weight);//构造Hufman树

	puts("生成字节表以及字节对应编码中...");
	HuffmanCode *hc = NULL;//记得free 
	HuffmanCoding(ht, len_alphabet, hc);//对字符表进行编码
	char *string_code = NULL;//记得free 
	Encode(hc, alphabet, string, string_code,len_file);//对字节流进行编码，得到01的字符串序列
	puts("编码已完成");

	unsigned char *string_compress; //记得free
	int len_byte=0;
	int len_zeros=0;
	std::string cmp_filename = std::string(filename);
	int l=cmp_filename.find_last_of(".");
	cmp_filename = cmp_filename.substr(0, l);
	cmp_filename += ".cmp";
	cout << "压缩文件至:" << cmp_filename << endl;
	char2bit(string_code, len_zeros,len_byte,string_compress);//将编码得到字符串转换为bit流
	FILE *fp1;
	fp1 = fopen(cmp_filename.c_str(), "wb+");
	fwrite(string_compress, len_byte, 1, fp1);
	fclose(fp1);
	cout.setf(ios::fixed);
	cout << "压缩为原文件的" << fixed << setprecision(2) << 1.0 * len_byte / len_file *100 << "%" << endl;


	std::string decmp_name = "decmp_" + std::string(filename);
	cout << "解压文件至：" << decmp_name << endl;
	FILE *fp2;
	if (!(fp2 = fopen(cmp_filename.c_str(), "rb")))
		return FILE_ERROR;
	fseek(fp2, 0, SEEK_END);
	int len_file_compress = ftell(fp2);
	unsigned char *string_from_compress = (unsigned char *)malloc(sizeof(unsigned char) * (len_file_compress + 1));
	fseek(fp2, 0, SEEK_SET);
	fread(string_from_compress, len_file_compress, sizeof(unsigned char), fp2);
	string_from_compress[len_file_compress] = '\0';
	fclose(fp2);
	char *bit2charstring=NULL;//记得free
	bit2char(bit2charstring, len_zeros, len_file_compress, string_from_compress);
	char *decode_string=NULL;//记得free
	int len_decode_string = 0;
	Decode(ht, bit2charstring, alphabet,len_alphabet, decode_string,len_decode_string);
	FILE *fp3=NULL;
	fp3 = fopen(decmp_name.c_str(), "wb+");
	fwrite(decode_string, len_decode_string, 1, fp1);
	puts("解压已完成");

	puts("按任意键结束...");
	getchar();
}