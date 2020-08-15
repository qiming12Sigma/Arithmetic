#include "bilinear.h"
#include <stdlib.h>
#include <string>

struct ImgData
{
	int width;
	int height;
	unsigned char* pBuffer;
	float fscale;
};

ImgData input_data,ouput_data;

BilinearScale::BilinearScale()
{
}

BilinearScale::~BilinearScale()
{
}

void BilinearScale::SetInputData(int width, int height, unsigned char* pBuffer, float ratio)
{
	input_data.width = width;
	input_data.height = height;
	input_data.fscale = ratio;
	input_data.pBuffer = pBuffer;

	for (int i = 0; i < input_data.width; i++)
	{
		for (int j = 0; j < input_data.height; j++)
		{
			int val_char = (int)*pBuffer;
			int mm = 0;
			pBuffer++;
		}
	}
}

void BilinearScale::Execute()
{
	int width = input_data.width*input_data.fscale;
	int height = input_data.height*input_data.fscale;
	int char_size = sizeof(unsigned char);
	unsigned char* pBuffer = (unsigned char*)malloc(width*height*char_size*4);
	memset(pBuffer,0,sizeof(unsigned char));

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			pBuffer[0] = 0;
			pBuffer[1] = 0;
			pBuffer[2] = 0;
			pBuffer+=4;
		}
	}
	ouput_data.width = width;
	ouput_data.height = height;
	ouput_data.pBuffer = pBuffer;
}

void BilinearScale::GetOutputData(int& width, int& height, unsigned char** pBuffer)
{
	width = ouput_data.width;
	height = ouput_data.height;
	*pBuffer = ouput_data.pBuffer;
}
