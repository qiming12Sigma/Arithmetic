#include "writeimagethread.h"
#include "core.hpp"  
#include "imgcodecs.hpp"
#include "opencv.hpp"
#include <QtConcurrent>
#include <QtConcurrentRun>
#include <time.h>
#include <QPixmap>
#include <QImageWriter>

ThreadWriteImage::ThreadWriteImage(QObject *parent):QThread(parent)
{
	m_bRun = false;
}

ThreadWriteImage::~ThreadWriteImage()
{

}

void ThreadWriteImage::WriteImgStdAPI(QString fileName, const char* pImageBuff, long width, long height, long bitCount)
{
	QString textLog = "Image path:";
	textLog.append(fileName);
	textLog.append(" image size:");
	textLog.append(QString::number((sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER))+width*height*4/1000000, 10, 0));
	textLog.append(" Save time:");

	double start, end;
	double cost;
	start = GetTickCount();

	BITMAPINFOHEADER bmiHdr; //定义信息头        
	bmiHdr.biSize = sizeof(BITMAPINFOHEADER);
	bmiHdr.biWidth = width;
	bmiHdr.biHeight = height;
	bmiHdr.biPlanes = 1;
	bmiHdr.biBitCount = 24;
	bmiHdr.biCompression = BI_RGB;
	bmiHdr.biSizeImage = width*height * 4;
	bmiHdr.biXPelsPerMeter = 0;
	bmiHdr.biYPelsPerMeter = 0;
	bmiHdr.biClrUsed = 0;
	bmiHdr.biClrImportant = 0;
	FILE* fp = fopen(fileName.toStdString().c_str(), "wb");
	if (fp)
	{
		BITMAPFILEHEADER fheader = { 0 };
		fheader.bfType = 'M' << 8 | 'B';
		fheader.bfSize = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) + bmiHdr.biSizeImage;
		fheader.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
		fwrite(&fheader, 1, sizeof(fheader), fp);
		fwrite(&bmiHdr, 1, sizeof(BITMAPINFOHEADER), fp);
		fwrite(pImageBuff, 1, bmiHdr.biSizeImage, fp);
		fclose(fp);
	}
	else
		fclose(fp);

	end = GetTickCount();
	cost = end - start;
	textLog.append(QString::number(cost));
	textLog.append("ms");
	OutputLog(textLog);
}

void ThreadWriteImage::WriteImgWinAPI(QString fileName, const char * pImageBuff, long lImageWidth, long lImageHeight, long lImageBitCount)
{
	QString textLog = "Image path:";
	textLog.append(fileName);
	textLog.append(" image size:");
	textLog.append(QString::number(lImageWidth*lImageHeight*4/1000000,10,0));
	textLog.append(" Save time:");

	double start, end;
	double cost;
	start = GetTickCount();

	BOOL bRet = TRUE;
	
	if (!(pImageBuff != NULL && lImageWidth >= 0 && lImageHeight >= 0 && lImageBitCount > 0))
	{
		Q_ASSERT(pImageBuff != NULL && lImageWidth >= 0 && lImageHeight >= 0 && lImageBitCount > 0);
	}

	//公共保存用的
	BITMAPINFO* m_pSaveBitmapInfo;
	m_pSaveBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	m_pSaveBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pSaveBitmapInfo->bmiHeader.biPlanes = 1;
	m_pSaveBitmapInfo->bmiHeader.biBitCount = 0;
	m_pSaveBitmapInfo->bmiHeader.biCompression = BI_RGB;
	m_pSaveBitmapInfo->bmiHeader.biSizeImage = 0;
	m_pSaveBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pSaveBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pSaveBitmapInfo->bmiHeader.biClrUsed = 0;
	m_pSaveBitmapInfo->bmiHeader.biClrImportant = 0;
	m_pSaveBitmapInfo->bmiHeader.biWidth = 0;
	m_pSaveBitmapInfo->bmiHeader.biHeight = 0;
	for (int i = 0; i < 256; i++)
	{
		m_pSaveBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
		m_pSaveBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
		m_pSaveBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
		m_pSaveBitmapInfo->bmiColors[i].rgbReserved = 0;
	}

	m_pSaveBitmapInfo->bmiHeader.biWidth = lImageWidth;
	m_pSaveBitmapInfo->bmiHeader.biHeight = lImageHeight;
	m_pSaveBitmapInfo->bmiHeader.biBitCount = (WORD)lImageBitCount;

	wchar_t * lpFileName = (wchar_t *)fileName.utf16();
	BOOL bRVal = TRUE;
	DWORD dwBytesRead = 0;
	DWORD dwSize = 0;
	BITMAPFILEHEADER bfh = { 0 };
	int nTable = 0;
	DWORD dwImageSize = 0;

	if (m_pSaveBitmapInfo->bmiHeader.biBitCount > 8)
	{
		nTable = 0;
	}
	else
	{
		nTable = 256;
	}

	dwImageSize = (m_pSaveBitmapInfo->bmiHeader.biWidth * m_pSaveBitmapInfo->bmiHeader.biHeight) * ((m_pSaveBitmapInfo->bmiHeader.biBitCount + 7) / 8);

	if (dwImageSize <= 0)
	{
		bRVal = FALSE;
	}
	else
	{
		bfh.bfType = (WORD)'M' << 8 | 'B';
		bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nTable * sizeof(RGBQUAD);
		bfh.bfSize = bfh.bfOffBits + dwImageSize;

		HANDLE hFile = ::CreateFile(lpFileName,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
		if (hFile == INVALID_HANDLE_VALUE) 
		{
			bRVal = FALSE;
		}
		else 
		{
			dwSize = sizeof(BITMAPFILEHEADER);
			::WriteFile(hFile, &bfh, dwSize, &dwBytesRead, NULL);

			dwSize = sizeof(BITMAPINFOHEADER) + nTable * sizeof(RGBQUAD);
			::WriteFile(hFile, m_pSaveBitmapInfo, dwSize, &dwBytesRead, NULL);

			dwSize = dwImageSize;
			::WriteFile(hFile, pImageBuff, dwSize, &dwBytesRead, NULL);

			CloseHandle(hFile);
		}
	}

	end = GetTickCount();
	cost = end - start;
	textLog.append(QString::number(cost));
	textLog.append("ms");
	OutputLog(textLog);
}

void ThreadWriteImage::SavePngWithMat(QString folder, char* pData, long width, long height, long bitCount)
{
	QString textLog = "Image path:";
	textLog.append(folder);
	textLog.append(" image size:");
	textLog.append(QString::number(width*height*4/1000000, 10, 0));
	textLog.append(" Save time:");

	double start, end;
	double cost;
	start = GetTickCount();

	uint lineSize = width * bitCount / 8;
	long dataSize = width*height*(bitCount/8);

	int type = CV_MAKETYPE(CV_8S,bitCount/8);

	cv::Mat image;
	image.create(height, width, bitCount);
	//image.create(height, width, CV_8S);
	image.data = new BYTE[dataSize];
	memset(image.data,0, dataSize);
	memcpy(image.data,pData, dataSize);

	// 最后一行尾地址
	pData += dataSize;
	for (int h = 0; h < height; ++h)
	{
		pData -= lineSize;
		memcpy(image.data, pData, lineSize);	// 复制整行
		image.data += lineSize;
	}

	// Mat数据指针移到最前面
	image.data -= dataSize;

	std::vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(0); //无压缩PNG
	compression_params.push_back(cv::IMWRITE_PNG_STRATEGY);
	compression_params.push_back(cv::IMWRITE_PNG_STRATEGY_DEFAULT);
	//bool code = cv::imwrite(folder.toStdString(), image/*, compression_params*/);

	m_OutputImage = image;

	end = GetTickCount();
	cost = end - start;
	textLog.append(QString::number(cost));
	textLog.append("ms");
	OutputLog(textLog);
}

void ThreadWriteImage::SaveBmpWithQImage(QString folder, char* pData, long width, long height, long bitCount)
{
	QString textLog = "Image path:";
	textLog.append(folder);
	textLog.append(" image size:");
	textLog.append(QString::number(width*height*4/1000000, 10, 0));
	textLog.append(" Save time:");

	double start, end;
	double cost;
	start = GetTickCount();

	QImage::Format format;
	if (bitCount == 32)
		format = QImage::Format_RGB32;
	else if (bitCount == 8)
		format = QImage::Format_Indexed8;
	else if (bitCount == 16)
		format = QImage::Format_RGB16;
	else if (bitCount == 24)
		format = QImage::Format_RGB888;

	QImage image((uchar*)pData, width, height, format);
	QImageWriter writer(folder, "bmp");

	//QList<QByteArray> imageFormats = QImageWriter::supportedImageFormats();
	//QList<QByteArray> mimeTypes = QImageWriter::supportedMimeTypes();

	//if (writer.supportsOption(QImageIOHandler::Description))
	//{
	//	// 设置描述信息
	//	writer.setText("Author", "Mr Wang");
	//	writer.setText("Description", "Qter");
	//}
	writer.setQuality(100);
	if (writer.canWrite())
	{
		// 写入图片至文件
		writer.write(image);
	}
	else
	{
		// 获取错误信息
		QImageWriter::ImageWriterError error = writer.error();
		QString strError = writer.errorString();
	}

	end = GetTickCount();
	cost = end - start;
	textLog.append(QString::number(cost));
	textLog.append("ms");
	OutputLog(textLog);
}

void ThreadWriteImage::SavePngWithQImage(QString folder, char* pData, long width, long height, long bitCount)
{
	QString textLog = "Image path:";
	textLog.append(folder);
	textLog.append(" image size:");
	textLog.append(QString::number(width*height*4/1000000, 10, 0));
	textLog.append(" Save time:");

	double start, end;
	double cost;
	start = GetTickCount();

	QImage::Format format;
	if (bitCount == 32)
		format = QImage::Format_RGB32;
	else if(bitCount == 8)
		format = QImage::Format_Indexed8;
	else if(bitCount == 16)
		format = QImage::Format_RGB16;
	else if (bitCount == 24)
		format = QImage::Format_RGB888;

	// bmp 到 png需要做垂直方向的翻转.
	uint lineSize = width * bitCount / 8;
	long dataSize = width*height*(bitCount / 8);

	uchar* pBuff = (uchar*)malloc(dataSize);
	memset(pBuff, 0, dataSize);

	// 最后一行尾地址
	pData += dataSize;
	for (int h = 0; h < height; ++h)
	{
		pData -= lineSize;
		memcpy(pBuff, pData, lineSize);
		pBuff += lineSize;
	}
	pBuff -= dataSize;

	QImage image(pBuff,width, height, format);

	QImageWriter writer(folder,"png");
	writer.setQuality(60);
	if (writer.canWrite())
	{
		// 写入图片至文件
		writer.write(image);
	}
	else
	{
		// 获取错误信息
		QImageWriter::ImageWriterError error = writer.error();
		QString strError = writer.errorString();
	}
	free(pBuff);

	end = GetTickCount();
	cost = end - start;
	textLog.append(QString::number(cost));
	textLog.append("ms");
	OutputLog(textLog);
}

void ThreadWriteImage::WriteImgQtApi(QString folder,QImage* image)
{
	QString textLog = "Image path:";
	textLog.append(folder);
	textLog.append(" image size:");
	textLog.append(QString::number(image->byteCount()));
	textLog.append(" Save time:");

	double start, end;
	double cost;
	start = GetTickCount();

	image->save(folder);
	delete image;
	image = nullptr;

	end = GetTickCount();
	cost = end - start;
	textLog.append(QString::number(cost));
	textLog.append("ms");
	OutputLog(textLog);
}

void ThreadWriteImage::WritePngQt(QString folder, char* data, long width, long height, long bitCount)
{
	QString textLog = "Image path:";
	textLog.append(folder);
	textLog.append(" image size:");
	textLog.append(QString::number(width*height * 4 / 1000000, 10, 4));
	textLog.append(" Save time:");

	double start, end;
	double cost;
	start = GetTickCount();

	QImage image((unsigned char*)data,width, height, QImage::Format_RGB888);
	image.save(folder,"png");

	//delete[] data;

	end = GetTickCount();
	cost = end - start;
	textLog.append(QString::number(cost));
	textLog.append("ms");
	OutputLog(textLog);
}

void ThreadWriteImage::SetFunLogOutput(std::function<void(QString)> fun)
{
	m_funLogOutput = fun;
}

void ThreadWriteImage::PushImageData(s_ImageInfo imgInfo)
{
	m_LockPush.lock();
	list_img.push_back(imgInfo);
	m_LockPush.unlock();
}

void ThreadWriteImage::PushQtImageData(QImage* image)
{
	m_LockPush.lock();
	list_image.push_back(image);
	m_LockPush.unlock();
}

void ThreadWriteImage::run()
{
	int i = 0;
	while (m_bRun)
	{
		m_LockPull.lock();
		if (!list_img.isEmpty())
		{
			QString fileName = m_sSavePath;
			fileName.append("\\image");
			fileName.append(QString::number(i));
			fileName.append(".png");
			i++;

			s_ImageInfo imgInfo = list_img.takeFirst();

			// 保存图像的操作放在线程里
			QFuture<void> future = QtConcurrent::run(this, &ThreadWriteImage::SavePngWithQImage,fileName, (char *)imgInfo.pImageBuff,
				imgInfo.lImageWidth,imgInfo.lImageHeight, imgInfo.iImageCount);
		}

		if (!list_image.isEmpty())
		{
			QString fileName = m_sSavePath;
			fileName.append("\\image");
			fileName.append(QString::number(i));
			fileName.append(".bmp");
			i++;

			QImage* image = list_image.takeFirst();
			QFuture<void> future = QtConcurrent::run(this, &ThreadWriteImage::WriteImgQtApi, fileName, image);
		}
		m_LockPull.unlock();
	}
}


void ThreadWriteImage::SavePngWithWinAPI(QString path, char* pData, long width, long height, long bitCount)
{
	/*CImage img;	CBitmap bm;
	bm.CreateBitmap(width, height, 0, bitCount, pData);
	HBITMAP hbmp = (HBITMAP)bm.GetSafeHandle();
	if (!img.IsNull())
		img.Destroy();
	img.Attach(hbmp);

	const char* fileName = path.toStdString().c_str();
	img.Save(_T("D:/IMAGE-OUTPUT/IMAGE99.PNG"), Gdiplus::ImageFormatBMP);*/
}