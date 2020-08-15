#pragma once
#include <minwindef.h>

struct s_ImageInfo
{
	bool bHaveImage;
	short iImageCount;
	BYTE* pImageBuff;	//图像缓冲区
	long lBuffSize;		//图像缓冲区大小

	long lImageWidth;	//图像宽度
	long lImageHeight;	//图像长度
	long lImageChannel;	//象素大小（单位：字节）
	long lImageLineSize;	//每行占内存大小line memory size
	long lImageSize;		//图像数据大小

	int bit_depth; 	   		//位深度
	int alpha_flag;			//是否有透明通道

	s_ImageInfo()
	{
		Reset();
	}

	void Reset()
	{
		bHaveImage = false;
		lBuffSize = 0;
		lImageWidth = 0;
		lImageHeight = 0;
		lImageChannel = 0;
		lImageLineSize = 0;
		lImageSize = 0;
		pImageBuff = nullptr;
	}
};
