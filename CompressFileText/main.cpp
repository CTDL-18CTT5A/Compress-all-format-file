
//
#include"CompressFile.h"

#include<bitset>



int main()
{
	
	//FILE* file = fopen("main.cpp", "rb");
	//FILE* fileCompresss = fopen("InputCompress.huf", "wb");
	//HuffData data = ReadFileBin(file);
	//cout << "read complete" << endl;
	//HuffMap map;
	////int size = sizeof(arr) / sizeof(arr[0]);
	//int size = strlen(data.s);
	//HuffmanCompress(file  , fileCompresss,  map , data, size);

	//fclose(fileCompresss);
	//fclose(file);



	FILE* header = fopen("InputCompress.huf", "rb");

	FILE* out = fopen("ec.txt", "wb");
	Decode(header, out);
	
	
	//________________BAI 1 ____________________
	//FILE* file = fopen("Input.txt", "r");
	//FILE* fileCompresss = fopen("InputCompress.txt", "w");
	//HuffData data = ReadFileText(file);
	//HuffMap map;
	////int size = sizeof(arr) / sizeof(arr[0]);
	//int size = strlen(data.s);
	//HuffmanCompress(file  , fileCompresss,  map , data, size);

	//fclose(fileCompresss);
	//fclose(file);

		

	/*FILE* header = fopen("InputCompress.txt", "r");
	FILE* out = fopen("ec.txt", "w");
	Decode(header, out);*/

	//FILE* temp = fopen("Huffman.h", "rb");
	//char* s = new char[1];
	//while (!feof(temp))
	//{
	//	fread(s, sizeof(char), 1, temp);
	//	puts(s);
	//}
	


	return 0;
}


