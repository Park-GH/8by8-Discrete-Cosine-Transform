#define BLK_SIZE 8
#define pi 3.14159265359

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>  
#include <filesystem>


typedef unsigned char BYTE;
using namespace std;

int main() {
	/*
	string str = "barbara(512x512).raw";
	int height, width;
	ifstream ImgIn( str, ios::in | ios::binary);
	ofstream ImgOut("fff" + str, ios::out | ios::binary);

	BYTE** img_memin = new BYTE * [512];
	for (int h = 0; h < 512; h++) {
		img_memin[h] = new BYTE[512];
		for (int w = 0; w < 512; w++)
			img_memin[h][w] = 0;
	}


	for (int h = 0; h < 512; h++) {
		for (int w = 0; w < 512; w++) {
			ImgIn.read((char*)&img_memin[h][w], sizeof(BYTE));
		}
	}

	for (int h = 0; h < 256; h++) {
		for (int w = 0; w < 512; w++)
			ImgOut.write((char*)&img_memin[h][w], sizeof(BYTE));
	}
	*/
	std::string path = "Input/";
	for (const auto& entry : std::filesystem:directory_iterator(path))
		cout << entry.path() << std::endl;

}