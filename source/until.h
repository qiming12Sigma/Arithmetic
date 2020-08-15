#pragma once
#define _CRT_SECURE_NO_WARNINGS#include <stdio.h>
#include <windows.h>
#include <string>
#include <time.h>

// windows 操作系统获取可用CPU数量
int GetCpuAvailableCount()
{
	SYSTEM_INFO sysinfo;
	GetNativeSystemInfo(&sysinfo);
	return (int)sysinfo.dwNumberOfProcessors;
}
unsigned char* ReadBmpFile(const char* file_name){	FILE* file = fopen(file_name, "rb");	if (file)	{		unsigned char* imageBuffer = nullptr;		fseek(file, 0, SEEK_END);		int length = ftell(file);		if (length > 0)		{			rewind(file);			imageBuffer = new unsigned char[length + 1];			fread(imageBuffer, sizeof(unsigned char), length, file);			imageBuffer[length] = '\0';		}		fclose(file);		if (0x4D42 == *((unsigned short*)imageBuffer))		{			int pixelDataOffset = *((int*)(imageBuffer + 10));			int width = *((int*)(imageBuffer + 18));			int height = *((int*)(imageBuffer + 22));			unsigned char* pData = imageBuffer + pixelDataOffset;			for (int i = 0; i < width*height * 3; i += 3)			{				unsigned char temp = pData[i];				pData[i] = pData[i + 2];				pData[i + 2] = temp;			}			return pData;		}	}	return nullptr;}std::string GetTimeStr(){	time_t timep; time(&timep); 	char tmp[64];	strftime(tmp, sizeof(tmp), "%Y-%m-%d-%H:%M:%S", localtime(&timep));	return tmp;}