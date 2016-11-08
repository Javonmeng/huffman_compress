#include "huffman_compress.h"
using namespace std;
int main()
{
	char *filename = "string1.txt";//�˴�Ϊѹ���ļ���
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

	std::cout << "�����ļ���" << filename << std:: endl;
	cout << "�ļ���СΪ:" << len_file << "k" << endl;
	char *alphabet = NULL;//�ǵ�free 
	int *weight = NULL;//�ǵ�free
	int len_alphabet = 0;

	puts("ͳ���ֽڼ�Ȩ��...");
	count(string, alphabet, weight, len_alphabet,len_file);
	puts("ͳ�����");

	HuffmanTree *ht = NULL;
	puts("����Huffman����...");
	CreateTree(ht, len_alphabet, weight);//����Hufman��

	puts("�����ֽڱ��Լ��ֽڶ�Ӧ������...");
	HuffmanCode *hc = NULL;//�ǵ�free 
	HuffmanCoding(ht, len_alphabet, hc);//���ַ�����б���
	char *string_code = NULL;//�ǵ�free 
	Encode(hc, alphabet, string, string_code,len_file);//���ֽ������б��룬�õ�01���ַ�������
	puts("���������");

	unsigned char *string_compress; //�ǵ�free
	int len_byte=0;
	int len_zeros=0;
	std::string cmp_filename = std::string(filename);
	int l=cmp_filename.find_last_of(".");
	cmp_filename = cmp_filename.substr(0, l);
	cmp_filename += ".cmp";
	cout << "ѹ���ļ���:" << cmp_filename << endl;
	char2bit(string_code, len_zeros,len_byte,string_compress);//������õ��ַ���ת��Ϊbit��
	FILE *fp1;
	fp1 = fopen(cmp_filename.c_str(), "wb+");
	fwrite(string_compress, len_byte, 1, fp1);
	fclose(fp1);
	cout.setf(ios::fixed);
	cout << "ѹ��Ϊԭ�ļ���" << fixed << setprecision(2) << 1.0 * len_byte / len_file *100 << "%" << endl;


	std::string decmp_name = "decmp_" + std::string(filename);
	cout << "��ѹ�ļ�����" << decmp_name << endl;
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
	char *bit2charstring=NULL;//�ǵ�free
	bit2char(bit2charstring, len_zeros, len_file_compress, string_from_compress);
	char *decode_string=NULL;//�ǵ�free
	int len_decode_string = 0;
	Decode(ht, bit2charstring, alphabet,len_alphabet, decode_string,len_decode_string);
	FILE *fp3=NULL;
	fp3 = fopen(decmp_name.c_str(), "wb+");
	fwrite(decode_string, len_decode_string, 1, fp1);
	puts("��ѹ�����");

	puts("�����������...");
	getchar();
}