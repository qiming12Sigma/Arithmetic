#include <windows.h>
#include "readimagethread.h"
#include <QDirIterator>
#include <QDir>
#include <QFileInfo>
#include <QImage>

ThreadReadImage::ThreadReadImage(QObject *parent) : QThread(parent)
{
	m_bRunState = false;
}

ThreadReadImage::~ThreadReadImage()
{

}

void ThreadReadImage::run()
{
	for (size_t i = 0; i < list_img.size(); i++)
	{
		std::string path = list_img.at(i).toStdString();

		s_ImageInfo imgInfo;
		ReadImgWinAPI(path.c_str(), imgInfo);
		m_funImageInfo(imgInfo);

		//QImage* image = this->ReadImgQtAPI(path);
		//m_funQImage(image);
	}
}

void ThreadReadImage::SetFolder(QString s)
{
	m_sFolder = s;
	this->GetImgs(m_sFolder);
	int size = this->list_img.size();
}

void ThreadReadImage::SetFunImageInfo(std::function<void(s_ImageInfo)> fun)
{
	m_funImageInfo = fun;
}

void ThreadReadImage::SetFunQImage(std::function<void(QImage*)> fun)
{
	m_funQImage = fun;
}

void ThreadReadImage::GetImgs(QString _dir)
{
	QDirIterator it(_dir, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);	//遍历所有目录和文件
	while (it.hasNext())//存在
	{
		QString name = it.next();//读取		
		QFileInfo info(name);
		if (info.isDir())//判断是目录
		{
			this->GetImgs(name);//递归
		}
		else
		{
			if (info.suffix() == "jpg" || info.suffix() == "bmp" || info.suffix() == "png")
			{
				this->list_img.push_back(name);//符合添加
			}
		}
	}
}

void ThreadReadImage::ReadImgWinAPI(QString path, s_ImageInfo &sImageInfo)
{
	wchar_t * lpFileName = (wchar_t *)path.utf16();

	HANDLE hFile = ::CreateFile(lpFileName,GENERIC_READ,0,nullptr,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,nullptr);

	DWORD dwSize = 0, dwBytesRead = 0;
	if (hFile != INVALID_HANDLE_VALUE)
	{
		BITMAPFILEHEADER FileHeader; //文件头;
		BITMAPINFOHEADER InfoHeader; //信息头;

		dwSize = sizeof(BITMAPFILEHEADER);
		::ReadFile(hFile, &FileHeader, dwSize, &dwBytesRead, nullptr);

		dwSize = sizeof(BITMAPINFOHEADER);
		::ReadFile(hFile, &InfoHeader, dwSize, &dwBytesRead, nullptr);

		sImageInfo.lImageWidth = InfoHeader.biWidth;
		sImageInfo.lImageHeight = InfoHeader.biHeight;
		sImageInfo.lImageSize = FileHeader.bfSize;
		sImageInfo.pImageBuff = new BYTE[sImageInfo.lImageSize];
		sImageInfo.iImageCount = InfoHeader.biBitCount;

		if (InfoHeader.biBitCount == 8)
		{
			dwSize = 256 * sizeof(RGBQUAD);
			::ReadFile(hFile, sImageInfo.pImageBuff, dwSize, &dwBytesRead, nullptr);
		}

		dwSize = sImageInfo.lImageSize;
		::ReadFile(hFile, sImageInfo.pImageBuff, dwSize, &dwBytesRead, nullptr);

		sImageInfo.bHaveImage = true;
	}
	CloseHandle(hFile);
}

QImage* ThreadReadImage::ReadImgQtAPI(std::string path)
{
	QImage* img = new QImage();
	img->load(path.c_str());
	return img;
}

void ThreadReadImage::ReadImgStd(std::string path)
{

}