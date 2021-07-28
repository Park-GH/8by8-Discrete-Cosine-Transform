#define HEIGHT 512
#define WIDTH 512
#define BLK_SIZE 8
#define pi 3.14159265359
// height와 width도 변수로 받도록 수정하기
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

typedef unsigned char BYTE;
using namespace std;


//Prototype

void _2D_8x8_DCT();
void _2D_8x8_IDCT();
void _2D_8x8_Sep_DCT();
void _2D_8x8_Sep_IDCT();

//main

int main() {
	while (1) {
		cout << endl;
		cout << "1. 8x8 2D DCT" << endl;
		cout << "2. 8x8 2D IDCT" << endl;
		cout << "3. 8x8 Sep_DCT" << endl;
		cout << "4. 8x8 Sep_IDCT" << endl;
		cout << "5. Exit" << endl;
		cout << "Select mode : ";
		int mode;
		cin >> mode;
		switch (mode) {
		case 1:
			cout << "(8x8 2D DCT) Input file name : ";
			_2D_8x8_DCT();
			break;
		case 2:
			cout << "(8x8 2D IDCT) Input file name : ";
			_2D_8x8_IDCT();
			break;
		case 3:
			cout << "(8x8 Sep DCT) Input file name : ";
			_2D_8x8_Sep_DCT();
			break;
		case 4:
			cout << "(8x8 Sep IDCT) Input file name : ";
			_2D_8x8_Sep_IDCT();
			break;
		case 5:
			return 0;
		default:
			return 0;
		}
	}
}


void _2D_8x8_DCT() {
	//Read File	
	string str;
	cin >> str;

	ifstream ImgIn("Input/" + str, ios::in | ios::binary);
	ofstream ImgOut("DCT/" + str, ios::out | ios::binary);

	BYTE** img_memin = new BYTE * [HEIGHT];
	for (int h = 0; h < HEIGHT; h++)
		img_memin[h] = new BYTE[WIDTH];

	double** img_memout = new double * [HEIGHT];
	for (int h = 0; h < HEIGHT; h++) {
		img_memout[h] = new double[WIDTH];
	}

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			ImgIn.read((char*)&img_memin[h][w], sizeof(BYTE));
		}
	}

	//Processing
	double temp = 0;
	for (int i = 0; i < HEIGHT; i += BLK_SIZE) {
		for (int j = 0; j < WIDTH; j += BLK_SIZE) {
			//
			for (int y = 0; y < BLK_SIZE; y++) {
				for (int x = 0; x < BLK_SIZE; x++) {
					temp = 0;
					
					for (int y2 = 0; y2 < BLK_SIZE; y2++) {
						for (int x2 = 0; x2 < BLK_SIZE; x2++) {
							temp += img_memin[i + y2][j + x2] * cos((2 * (double)y2 + 1) * pi * y / (double)(2 * BLK_SIZE)) * cos((2 * (double)x2 + 1) * pi * x / (double)(2 * BLK_SIZE));
						}
					}

					temp /= (double)(BLK_SIZE / 2);
					if (y == 0 && x == 0)
						temp *= 0.5;
					else if (y == 0 || x == 0)
						temp *= (double)(1 / sqrt(2));
					img_memout[i + y][j + x] = temp;
				}
			}
			//
		}
	}
	//Write File
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++)
			ImgOut.write((char*)&img_memout[h][w], sizeof(double));
	}

	ImgIn.close();
	ImgOut.close();
}

void _2D_8x8_IDCT() {
	//Read File
	string str;
	cin >> str;
	ifstream ImgIn("DCT/" + str, ios::in | ios::binary);
	ofstream ImgOut("IDCT/" + str, ios::out | ios::binary);

	double** img_memin = new double * [HEIGHT];
	for (int h = 0; h < HEIGHT; h++)
		img_memin[h] = new double[WIDTH];

	BYTE** img_memout = new BYTE * [HEIGHT];
	for (int h = 0; h < HEIGHT; h++) {
		img_memout[h] = new BYTE[WIDTH];	}

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			ImgIn.read((char*)&img_memin[h][w], sizeof(double));
		}
	}

	//Processing
	double temp = 0;
	double temp2 = 0;
	for (int i = 0; i < HEIGHT; i += BLK_SIZE) {
		for (int j = 0; j < WIDTH; j += BLK_SIZE) {
			//
			for (int y = 0; y < BLK_SIZE; y++) {
				for (int x = 0; x < BLK_SIZE; x++) {
					temp = 0;
					
					for (int y2 = 0; y2 < BLK_SIZE; y2++) {
						for (int x2 = 0; x2 < BLK_SIZE; x2++) {
							temp2 = (double)img_memin[i + y2][j + x2] * cos((((2 * (double)y) + 1) * pi * y2 )/ (double)(2 * BLK_SIZE)) * cos((((2 * (double)x) + 1) * pi * x2 )/ (double)(2 * BLK_SIZE));
							if (y2 == 0 && x2 == 0)
								temp2 *= 0.5;
							else if (y2 == 0 || x2 == 0)
								temp2 *= (1 / sqrt(2));
							temp2 /= (double)(BLK_SIZE / 2);
							temp += temp2;
						}
					}
					//Clipping
					temp = floor(temp + 0.5);
					if (temp > 255)
						temp = 255;
					else if (temp < 0)
						temp = 0;
					img_memout[i + y][j + x] = (BYTE)temp;
				}
			}
			//
		}
	}
	//Write File
	for (int h = 0; h < HEIGHT; h++) {
			ImgOut.write((char*)img_memout[h], WIDTH);
	}

	ImgIn.close();
	ImgOut.close();
}

void _2D_8x8_Sep_DCT() {
	//Read File
	string str;
	cin >> str;

	ifstream ImgIn("Input/" + str, ios::in | ios::binary);
	ofstream ImgOut("SepDCT/" + str, ios::out | ios::binary);

	BYTE** img_memin = new BYTE * [HEIGHT];
	for (int h = 0; h < HEIGHT; h++)
		img_memin[h] = new BYTE[WIDTH];


	double** img_temp = new double* [HEIGHT];
	for (int h = 0; h < HEIGHT; h++) {
		img_temp[h] = new double[WIDTH];
	}

	double** img_memout = new double* [HEIGHT];
	for (int h = 0; h < HEIGHT; h++) {
		img_memout[h] = new double[WIDTH];
	}

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			ImgIn.read((char*)&img_memin[h][w], sizeof(BYTE));
		}
	}

	//Processing
	double temp;
	for (int i = 0; i < HEIGHT; i += BLK_SIZE) {
		for (int j = 0; j < WIDTH; j += BLK_SIZE) {
			//
			for (int y = 0; y < BLK_SIZE; y++) {
				for (int x = 0; x < BLK_SIZE; x++) {
					temp = 0;
					for (int x2 = 0; x2 < BLK_SIZE; x2++) {

						temp += img_memin[i + y][j + x2] * cos((2 * (double)x2 + 1) * pi * x / (2 * (double)BLK_SIZE));
					}			
					temp /= sqrt(BLK_SIZE / 2);
					if (x == 0)
						temp *= (double)(1 / sqrt(2));
					img_temp[i + y][j + x] = temp;
				}
			}
			//
			for (int x = 0; x < BLK_SIZE; x++) {
				for (int y = 0; y < BLK_SIZE; y++) {
					temp = 0;
					for (int y2 = 0; y2 < BLK_SIZE; y2++) {
						temp += img_temp[i + y2][j + x] * cos((2 * (double)y2 + 1) * pi * y / (2 * (double)BLK_SIZE));
					}
					temp /= sqrt(BLK_SIZE / 2);
					if (y == 0)
						temp *= (double)(1 / sqrt(2));
					img_memout[i + y][j + x] = temp;
				}
			}
			//
		}
	}
	//Write File
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			ImgOut.write((char*)&img_memout[h][w], sizeof(double));
		}
	}

	ImgIn.close();
	ImgOut.close();
}

void _2D_8x8_Sep_IDCT() {
	//Read File
	string str;
	cin >> str;

	ifstream ImgIn("SepDCT/" + str, ios::in | ios::binary);
	ofstream ImgOut("SepIDCT/" + str, ios::out | ios::binary);

	double** img_memin = new double * [HEIGHT];
	for (int h = 0; h < HEIGHT; h++)
		img_memin[h] = new double[WIDTH];


	double** img_temp = new double* [HEIGHT];
	for (int h = 0; h < HEIGHT; h++) {
		img_temp[h] = new double[WIDTH];
	}

	BYTE** img_memout = new BYTE* [HEIGHT];
	for (int h = 0; h < HEIGHT; h++) {
		img_memout[h] = new BYTE[WIDTH];
	}

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			ImgIn.read((char*)&img_memin[h][w], sizeof(double));
		}
	}

	//Processing
	double temp;
	double temp2;
	for (int i = 0; i < HEIGHT; i += BLK_SIZE) {
		for (int j = 0; j < WIDTH; j += BLK_SIZE) {
			//
			for (int y = 0; y < BLK_SIZE; y++) {
				for (int x = 0; x < BLK_SIZE; x++) {
					temp = 0;
					for (int x2 = 0; x2 < BLK_SIZE; x2++) {

						temp2 = img_memin[i + y][j + x2] * cos((2 * (double)x + 1) * pi * x2 / (2 * (double)BLK_SIZE));
						if (x2 == 0)
							temp2 *= (double)(1 / sqrt(2));
						temp += temp2;
					}
					temp /= sqrt(BLK_SIZE / 2);

					img_temp[i + y][j + x] = temp;
				}
			}
			//
			for (int x = 0; x < BLK_SIZE; x++) {
				for (int y = 0; y < BLK_SIZE; y++) {
					temp = 0;
					for (int y2 = 0; y2 < BLK_SIZE; y2++) {
						temp2 = img_temp[i + y2][j + x] * cos((2 * (double)y + 1) * pi * y2 / (2 * (double)BLK_SIZE));
						if (y2 == 0)
							temp2 *= (double)(1 / sqrt(2));
						temp += temp2;
					}
					temp /= sqrt(BLK_SIZE / 2);

					//Clipping
					temp = floor(temp + 0.5);
					if (temp > 255)
						temp = 255;
					else if (temp < 0)
						temp = 0;
					img_memout[i + y][j + x] = (BYTE)temp;
				}
			}
			//
		}
	}
	//Write File
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			ImgOut.write((char*)&img_memout[h][w], sizeof(char));
		}
	}

	ImgIn.close();
	ImgOut.close();
}