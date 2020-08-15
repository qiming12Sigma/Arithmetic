#pragma once
#include <minwindef.h>

struct s_ImageInfo
{
	bool bHaveImage;
	short iImageCount;
	BYTE* pImageBuff;	//ͼ�񻺳���
	long lBuffSize;		//ͼ�񻺳�����С

	long lImageWidth;	//ͼ����
	long lImageHeight;	//ͼ�񳤶�
	long lImageChannel;	//���ش�С����λ���ֽڣ�
	long lImageLineSize;	//ÿ��ռ�ڴ��Сline memory size
	long lImageSize;		//ͼ�����ݴ�С

	int bit_depth; 	   		//λ���
	int alpha_flag;			//�Ƿ���͸��ͨ��

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
