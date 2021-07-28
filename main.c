//DSPE_2016706018_박건희_7
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 512
#define HEIGHT 512
#define DCT_BlockSize 8
#define PI 3.141592653689793238462
typedef unsigned char BYTE;

unsigned char** MemAlloc_2D(int width, int height);   // 메모리할당
void MemFree_2D(unsigned char** arr, int height);     // 메모리반환

void FileRead(char* filename, unsigned char** img_in, int width, int height);   // 데이터읽기
void FileWrite(char* filename, unsigned char** img_out, int width, int height); // 데이터쓰기

float GetPSNR(unsigned char** img_in, unsigned char** img_dist, int width, int height); //PSNR calculation

void FDCT(BYTE** img_in, double** img_coeffi, int blockSize, int height, int width);
void IDCT(double** img_coeffi, BYTE** img_recon, int blockSize, int lowFreqBlock, int highFreqBlock, int height, int width);

int main()
{
	BYTE** img_in, ** img_recon;
	double** img_coeffi;

	int i, j;
	double temp;
	BYTE data;                  

	FILE* fp_fdct_out = fopen("[8x8_DCT]Lena(512x512).raw", "wb");          // DCT파일 쓰기

	img_in = MemAlloc_2D(WIDTH, HEIGHT);
	img_recon = MemAlloc_2D(WIDTH, HEIGHT);

	img_coeffi = (double**)malloc(sizeof(double*) * HEIGHT);
	for (i = 0; i < HEIGHT; i++)
	{
		img_coeffi[i] = (double*)malloc(sizeof(double) * WIDTH);
	}

	FileRead("Lena(512x512).raw", img_in, WIDTH, HEIGHT);
	FDCT(img_in, img_coeffi, DCT_BlockSize, HEIGHT, WIDTH);            //Forward DCT

	for (i = 0; i < HEIGHT; i++)                      //DCT 데이터저장
	{
		for (j = 0; j < WIDTH; j++)
		{
			temp = img_coeffi[i][j];
			if (temp < 0)                    //Clipping
				temp = 0;
			else if (temp > 255)
				temp = 255;
			data = (BYTE)floor(temp + 0.5);
			fwrite(&data, 1, 1, fp_fdct_out);
		}
	}
	//IDCT
	IDCT(img_coeffi, img_recon, DCT_BlockSize, DCT_BlockSize, 0, HEIGHT, WIDTH);                            
	FileWrite("[8x8_IDCT]Lena(512x512).raw", img_recon, WIDTH, HEIGHT);
	printf("PSNR (Reconstruction by all coefficient) : %f dB \n\n", GetPSNR(img_in, img_recon, WIDTH, HEIGHT));       
	//IDCT Low6x6
	IDCT(img_coeffi, img_recon, DCT_BlockSize, 6, 0, HEIGHT, WIDTH);                                      
	FileWrite("[8x8_IDCT_Low6x6]Lena(512x512).raw", img_recon, WIDTH, HEIGHT);
	printf("PSNR (Reconstruction by low6x6 coefficient) : %f dB \n\n", GetPSNR(img_in, img_recon, WIDTH, HEIGHT));    
	//IDCT Low2x2
	IDCT(img_coeffi, img_recon, DCT_BlockSize, 2, 0, HEIGHT, WIDTH);                                       
	FileWrite("[8x8_IDCT_Low2x2]Lena(512x512).raw", img_recon, WIDTH, HEIGHT);
	printf("PSNR (Reconstruction by low2x2 coefficient) : %f dB \n\n", GetPSNR(img_in, img_recon, WIDTH, HEIGHT));  
	//IDCT High6x6
	IDCT(img_coeffi, img_recon, DCT_BlockSize, 0, 6, HEIGHT, WIDTH);                                       
	FileWrite("[8x8_IDCT_High6x6]Lena(512x512).raw", img_recon, WIDTH, HEIGHT);
	printf("PSNR (Reconstruction by high6x6 coefficient) : %f dB \n\n", GetPSNR(img_in, img_recon, WIDTH, HEIGHT));   

	MemFree_2D(img_in, HEIGHT);
	MemFree_2D(img_recon, HEIGHT);

	for (i = 0; i < HEIGHT; i++)
	{
		free(img_coeffi[i]);
	}
	free(img_coeffi);

	fclose(fp_fdct_out);

	return 0;
}

//FDCT 함수
void FDCT(BYTE** img_in, double** img_coeffi, int blockSize, int height, int width)
{
	int x, y, u, v, i, j;
	double coeffi, cn, cm;
	for (x = 0; x < height; x += blockSize)     
	{
		for (y = 0; y < width; y += blockSize)
		{
			for (u = 0; u < blockSize; u++)    
			{
				for (v = 0; v < blockSize; v++)
				{
					coeffi = 0;
					for (i = 0; i < blockSize; i++)
					{
						for (j = 0; j < blockSize; j++)	//연산 진행
						{
							cn = (u == 0 ? 1.0 / sqrt(2.0) : 1);     
							cm = (v == 0 ? 1.0 / sqrt(2.0) : 1);

							coeffi += cn * cm * (2 / (double)blockSize) * (double)img_in[x + i][y + j] * cos(((2 * i + 1) * u * PI) / (double)(2 * blockSize)) * cos(((2 * j + 1) * v * PI) / (double)(2 * blockSize));
						}
					}
					img_coeffi[x + u][y + v] = coeffi;      //계수저장
				}
			}
		}
	}
}

void IDCT(double** img_coeffi, BYTE** img_recon, int blockSize, int lowFreqBlock, int highFreqBlock, int height, int width)
{
	int x, y, u, v, i, j;
	double coeffi, cn, cm;

	for (x = 0; x < height; x += blockSize)     
	{
		for (y = 0; y < width; y += blockSize)
		{
			for (u = 0; u < blockSize; u++)    
			{
				for (v = 0; v < blockSize; v++)
				{
					coeffi = 0;
					if (highFreqBlock == 0)
					{
						for (i = 0; i < lowFreqBlock; i++)
						{
							for (j = 0; j < lowFreqBlock; j++)
							{
								cn = (i == 0 ? 1.0 / sqrt(2.0) : 1);     //IDCT 진행
								cm = (j == 0 ? 1.0 / sqrt(2.0) : 1);

								coeffi += cn * cm * (2 / (double)blockSize) * img_coeffi[x + i][y + j] * cos(((2 * u + 1) * i * PI) / (double)(2 * blockSize)) * cos(((2 * v + 1) * j * PI) / (double)(2 * blockSize));
							}
						}
					}
					else if (lowFreqBlock == 0)
					{
						for (i = blockSize - highFreqBlock; i < blockSize; i++)
						{
							for (j = blockSize - highFreqBlock; j < blockSize; j++)
							{
								cn = (i == 0 ? 1.0 / sqrt(2.0) : 1);     //IDCT 진행
								cm = (j == 0 ? 1.0 / sqrt(2.0) : 1);

								coeffi += cn * cm * (2 / (double)blockSize) * img_coeffi[x + i][y + j] * cos(((2 * u + 1) * i * PI) / (double)(2 * blockSize)) * cos(((2 * v + 1) * j * PI) / (double)(2 * blockSize));
							}
						}
					}

					img_recon[x + u][y + v] = coeffi;      //result save
				}
			}
		}
	}
}

// 2D 메모리할당
unsigned char** MemAlloc_2D(int width, int height) 
{
	unsigned char** arr;
	int i = 0;

	arr = (unsigned char**)malloc(sizeof(unsigned char*) * height);
	for (i = 0; i < height; i++)
		arr[i] = (unsigned char*)malloc(sizeof(unsigned char) * width);

	return arr;
}

// 2D 
void MemFree_2D(unsigned char** arr, int height) 
{
	int i = 0;

	for (i = 0; i < height; i++)
		free(arr[i]);
	free(arr);
}

//파일읽기
void FileRead(const char* filename, unsigned char** img_in, int width, int height)  
{
	FILE* fp_in = 0;
	int i = 0;

	fp_in = fopen(filename, "rb");
	for (i = 0; i < height; i++)
		fread(img_in[i], sizeof(unsigned char), width, fp_in);
	fclose(fp_in);
}
//파일쓰기
void FileWrite(const char* filename, unsigned char** img_out, int width, int height)  
{
	FILE* fp_out;
	int i = 0;

	fp_out = fopen(filename, "wb");
	for (i = 0; i < height; i++)
		fwrite(img_out[i], sizeof(unsigned char), width, fp_out);
	fclose(fp_out);
}
//PSNR calculation

float GetPSNR(unsigned char** img_ori, unsigned char** img_dist, int width, int height) 
{




	float MSE = 0.0;
	float PSNR = 0.0;

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			MSE += powf((float)(img_ori[j][i] - img_dist[j][i]), 2);
		}
	}

	MSE /= (width * height);
	PSNR = 10.0 * (float)log10(255.0 * 255.0 / MSE);
	return PSNR;
}