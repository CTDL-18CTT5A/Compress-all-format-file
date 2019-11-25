#pragma once
#include"CompressFile.h"

#include<iostream>
#include<vector>
#include<sstream>
#include<bitset>
using namespace std;



bool isLeaf(NODE* root)
{
	return !(root->pLeft) && !(root->pRight);

}

void ArrayOutput(HuffMap &map , int index , int a[], int n)
{
	int i = 0;
	for (i; i < n; ++i)
	{
		map.BitArray[index][i] = a[i];
	}
	map.BitArray[index][i] = 2;

}

//Duyệt cây lưu dữ liệu vào map
void CompressFile(HuffMap &map , NODE* root, int arr[], int top)
{
	static int index = 0;
	// Assign 0 to left edge and recur 
	if (root->pLeft) 
	{

		arr[top] = 0;
		CompressFile(map ,root->pLeft, arr, top + 1);
	}

	// Assign 1 to right edge and recur 
	if (root->pRight) 
	{

		arr[top] = 1;
		CompressFile(map,  root->pRight, arr, top + 1);
	}

	
	//Index là biến đếm những node lá ứng với các kí tự
	if (isLeaf(root)) 
	{ 
		map.charater[index] = root->_text;
		map.BitArray[index] = new int[top];
		ArrayOutput(map, index , arr, top);
		index++;
	}
}


int convertBinaryToDecimal(long long n)
{
	int decimalNumber = 0, i = 0, remainder;
	while (n != 0)
	{
		remainder = n % 10;
		n /= 10;
		decimalNumber += remainder * pow(2, i);
		++i;
	}
	return decimalNumber;
}


//Lưu đống dữ liệu vào map
void HuffmanCompress(FILE *fileInput , FILE *fileOut , HuffMap &map , HuffData  data, int size)
{


	// Construct Huffman Tree 
	NODE* root = builfHuffmanTree(data, size);
	// Print Huffman codes using 
	// the Huffman tree built above 
	int arr[500];
	long long top = 0;
	map.BitArray = new int* [size];

	map.charater = new char[size];
	

	CompressFile(map , root, arr, top);

	WriteHeaderFile(fileOut, data);

	rewind(fileInput);

	//Lưu vào file
	char ch;
	fread(&ch, sizeof(char), 1, fileInput);

	cout << "DEBUG" << endl;

	//Dem so luong cac bit da ma hoa
	long long countBit8 = 0;
	//Tong cac bit da ghi ra
	long long countSumBit = 0;


	long long countBit = 0;
	long long sumBitOut = 0;
	char bit8[9];
	while (!feof(fileInput))
	{
		int j = 0;
		for (int i = 0; i < size; i++)
		{

			if (ch == map.charater[i])
			{
				while (map.BitArray[i][j] != 2)
				{
					countSumBit++;
					bit8[countBit++] = char(map.BitArray[i][j] + 48);
					if (countBit == 8)
					{

						stringstream sstream(bit8);
						std::bitset<8> bits;
						sstream >> bits;
						char textOutput = char(bits.to_ulong());
						fwrite(&textOutput, sizeof(char), 1, fileOut);
						countBit = 0;
						countBit8 += 8;
						
					}

					j++;
				}

				break;
			}
			
		}
		fread(&ch, sizeof(char), 1, fileInput);
	}

	int soBitLe = 0;
	for (int z = 0; z < countBit; z++)
	{
		//fprintf(fileOut, "%c", bit8[z]);
		fwrite(&bit8[z], sizeof(char), 1, fileOut);
		soBitLe++;
	}
	char bl = char(soBitLe + 48);
	fwrite(&bl, sizeof(char), 1, fileOut);


	//fprintf(fileOut, "%d", soBitLe);


	
}



void WriteHeaderFile(FILE* Output, HuffData data)
{
	int size = strlen(data.s);
	//fwrite(&(size), sizeof(int), 1, Output);.
	//fprintf(Output, "%d", size);
	//fwrite("$", 1, 1, Output);
	fprintf(Output, "%d", size);
	fprintf(Output, "$");


	for (int i = 0; i < size; i++)
	{
		//fwrite(&(data.s[i]), sizeof(char), 1, Output);

		fprintf(Output, "%c", data.s[i]);
		fprintf(Output, "%d", data.wei[i]);
		if (i != size - 1)
			fprintf(Output, "$");
		else
			fprintf(Output, "~");
	}

}


void ReadHeaderFile(FILE* fileIN, HuffData& data)
{
	char ch;
	//ch = fgetc(fileIN);
	//Get size
	int size = 0;

	fread(&ch, 1, 1, fileIN);



	while (ch!= '$')
	{
		int t = int(ch - 48);
		size = size * 10 + t;
		fread(&ch, 1, 1, fileIN);
	}
	data.s = new char[size];
	data.wei = new int[size];

	fread(&ch, 1, 1, fileIN);
	//Save vào data
	int i = 0;
	int maxBit = 0;
	while (ch != '~')
	{
		/*if (ch == '\n')
			ch = getc(fileIN);*/
		data.s[i] = ch;
		
		//Save wei
		int wei = 0;
		fread(&ch, 1, 1, fileIN);
		while (ch != '$')
		{
			int t = int(ch - 48);
			wei = wei * 10 + t;
			fread(&ch, 1, 1, fileIN);
			if (ch == '~')
				break;
		}
		data.wei[i] = wei;


		i++;
		if (ch == '~')
			break;	
		fread(&ch, 1, 1, fileIN);
	}
	data.s[i] = '\0';
}




long long getNumberOfFileAtIndex(FILE* in, int index)
{
	fseek(in, index, SEEK_SET);
	int thisPoint = ftell(in);
	fseek(in, 0, SEEK_END);
	long long maxsize = ftell(in);
	return maxsize - thisPoint;

}


//Hàm trả về vị trí cuối bit lưu số bit lẻ trả vệ  vị trí con trỏ ở bit lẻ
long long VitriLe(FILE* fileIN)
{

	fseek(fileIN, 0, SEEK_END);
	return ftell(fileIN) - 1;

}

void ConvertToBinArray(FILE* in, int indexStart, int* a , long long &k , long long viTriLe)
{
	long long EndFile = VitriLe(in);
	fseek(in, indexStart, SEEK_SET);
	int count = ftell(in);
	/////////////DANH DAU
	char ch;
	fread(&ch, sizeof(char), 1, in);
	while (count != viTriLe)
	{

		for (int i = 7; i >= 0; --i)
		{
			char s = ((ch & (1 << i)) ? '1' : '0');
			s -= 48;
			a[k] = s;
			k++;
		}
		fread(&ch, sizeof(char), 1, in);

		count++;
	}

	while (count !=EndFile)
	{
	
		a[k] = int(ch - 48);
		k++;
		fread(&ch, sizeof(char), 1, in);
		count++;
	}
}



void Decode(FILE* in, FILE* out)
{
	//Lay Bit Le
	
	//______________Gán vtLe để lấy ra số bit lẻ_______________
	long long vtLe = VitriLe(in);
	fseek(in, vtLe, SEEK_SET);
	char ch[2];
	ch[1] = '\0';
	fread(ch, sizeof(char), 1, in);

	int SoLuongBitLe = int(ch[0] - 48);
	//Sau khi lưu số lượng bit lẻ vì vtLe đang nằm ở cuối file , ta trừ đi số bit lẻ => ra đc vị trí số lẻ đầu tiên.
	for(int z = 0; z < SoLuongBitLe; z++)
		vtLe -= 1;

	rewind(in);


	HuffData data;
	ReadHeaderFile(in, data);
	


	//____________Define max size of bit array_____________
	int StartIndex = ftell(in);
	long long sizeOfBit = getNumberOfFileAtIndex(in, StartIndex)*8;


	int* bit = new int[sizeOfBit];
	long long n = 0;
	rewind(in);
	fseek(in, StartIndex, SEEK_SET);



	//__________Chuyển kí tự về bit và lưu hết vào trong mảng bit____________
	ConvertToBinArray(in, StartIndex, bit , n , vtLe);
	//Tiếp tục lưu các bia thừa.

	int size = strlen(data.s);
	//_________Tạo ra map dữ liệu____________

	NODE* root = builfHuffmanTree(data, size);	
	HuffMap map;
	map.BitArray = new int* [size]; //SS
	map.charater = new char [size]; //SS
	int arr[500];
	long long top = 0;

	CompressFile(map, root, arr, top);

	//Nếu cây huffman không tồn tại(Tức chỉ có 1 phần tử trong file)
	if (size == 1)
	{
		int temp = 0;
		while (temp < data.wei[0])
		{
			fprintf(out, "%c",data.s[0]);
			temp++;
		}
		return;
	}
	cout << "____________________________" << endl;

	//________Chuyển từ dãy bit sang string để dễ so sánh_________
	string* BitArray = new string[size];
	for (int i = 0; i < size; i++)
	{
		int j = 0;
		while (map.BitArray[i][j]!=2)
		{
			char test = char(map.BitArray[i][j]+48);
			BitArray[i] += test;

			j++;
		}
	}

	
	//Bắt đầu so sánh và ghi ra file
	string compare;
	int byte = 0;
	for (int i = 0; i < n; i++)
	{

		compare += char(bit[i]+48);
		for (int k = 0; k < size; k++)
		{
			if (compare == BitArray[k])
			{

				char op[2];
				op[1] = '\0';
				op[0] = map.charater[k];
				fwrite(op, sizeof(char), 1, out);
				byte++;
				compare.clear();
				break;
			}
		}
	}


}