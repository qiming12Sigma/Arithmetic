#pragma once
#include <windows.h>
#include <functional>
#include <QThread>
#include <QMutex>
#include <QImage>
#include "stdafx.h"
#include "core.hpp"  

class ThreadWriteImage : public QThread
{
	Q_OBJECT

public:
	ThreadWriteImage(QObject *parent);
	~ThreadWriteImage();
	void PushImageData(s_ImageInfo);
	void PushQtImageData(QImage*);
	void SetFolderPath(QString s) { m_sSavePath = s; }
	void SetFunLogOutput(std::function<void(QString)>);
	void WriteImgWinAPI(QString, const char *, long, long, long);
	void WriteImgStdAPI(QString, const char*,long,long,long);
	void WritePngQt(QString, char*, long, long, long);
	void SavePngWithMat(QString, char*, long, long, long);
	void SavePngWithQImage(QString, char*, long, long, long);
	void SaveBmpWithQImage(QString, char*, long, long, long);
	void SavePngWithWinAPI(QString, char*, long, long, long);
	cv::Mat& GetImage() {return m_OutputImage;}
	
	bool m_bRun;

signals:
	void OutputLog(QString);

private:
	void WriteImgQtApi(QString,QImage*);
	void run();

	QMutex	m_LockPush;
	QMutex	m_LockPull;
	QString m_sSavePath;
	QList<s_ImageInfo>	list_img;
	QList<QImage*>	list_image;

	cv::Mat		m_OutputImage;

	std::function<void(QString)>	m_funLogOutput;
};
