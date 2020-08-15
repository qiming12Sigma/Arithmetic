#pragma once
#include <windows.h>
#include <functional>

#include <QThread>
#include <QImage>
#include "stdafx.h"

class ThreadReadImage : public QThread
{
	Q_OBJECT

public:
	ThreadReadImage(QObject *parent);
	~ThreadReadImage();

	bool m_bRunState;
	void SetFolder(QString s);
	void SetFunImageInfo(std::function<void(s_ImageInfo)>);
	void SetFunQImage(std::function<void(QImage*)>);
	void ReadImgWinAPI(QString, s_ImageInfo &);

private:
	void run();
	void GetImgs(QString);
	QImage* ReadImgQtAPI(std::string);
	void ReadImgStd(std::string);

	QString		m_sFolder;
	QList<QString>	list_img;
	std::function<void(s_ImageInfo)> m_funImageInfo;
	std::function<void(QImage*)> m_funQImage;
};
