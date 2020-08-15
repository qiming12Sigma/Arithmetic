#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <windows.h>
#include <QMainWindow>
#include "ui_mainwindow.h"
#include <time.h>
#include "core.hpp"
#include "stdafx.h"

class pngwriter;
class QGraphicsScene;
class QGraphicsPixmapItem;
class ThreadReadImage;
class ThreadWriteImage;
class HalconCacl;
class ThresholdDialog;
class BilinearScale;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
	void ConnectSignal();
	void StartRunTime();
	void ShowRunTime();
	void SetToolTip();
	void OutputImageData(s_ImageInfo);
	void ReadPNGFile(std::string ,s_ImageInfo*);
	void WritePNGFile(std::string);
	bool SaveImageToPNGFile(QString, const char*, std::pair<long, long>, long, int);

private slots:
	void WatchImageMem(bool);
	void ReadWriteImg(bool);
	void BGR2RGB(bool);
	void ShowRGBImage(bool);
	void LinearBlend(bool);
	void LinearBlur(bool);
	void Gaussian(bool);
	void MedianBlur(bool);
	void BilateralBlur(bool);
	void TurnInImg(bool);
	void TurnOutImg(bool);
	void Threshold(bool);
	void LaplaceOperator(bool);
	void SobelOperator(bool);
	void SobelOperatorY(bool);
	void CannyOperator(bool);
	void AddBorder(bool);
	void SavePngImage(bool);
	void SaveBmpImage(bool);
	void AddToLogText(QString);
	void WritePngImage(bool);
	void HalconThreadImage(bool);
	void OpencvSaveImage(bool);
	void SaveImageInQtOpenCV(bool);
	void ThresholdImageValue(bool);
	void ThresholdFinish();
	void HoughLineDone(bool);
	void ThresholdPhory(bool);
	void BilinearScaleImage(bool);

private:
    Ui::MainWindow ui;

	clock_t		m_StartClock, m_EndClock;
	cv::Mat		m_ImageObj;
	QImage*		m_pImage;
	QString		m_sLogText;

	QList<QAction*>		m_ActionList;
	QList<QAction*>		m_ActionListLeft;
	QGraphicsScene*		m_pScene;
	QGraphicsPixmapItem* m_pPixapItem;
	QList<QString>		m_ListToolTip;

	ThreadReadImage*	m_pReadImgThread;
	ThreadWriteImage*	m_pWriteImgThread;
	pngwriter*		m_pPngOperater;
	ThresholdDialog*	m_pThresholdDialog;
	BilinearScale*		m_pBilinear;
};

#endif // MAINWINDOW_H
