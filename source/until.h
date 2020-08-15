#pragma once
#define _CRT_SECURE_NO_WARNINGS
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
