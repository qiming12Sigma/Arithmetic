#include "mainwindow.h"
#include "core.hpp"        
#include "imgcodecs.hpp"
#include "highgui.hpp"
#include "opencv.hpp"
#include "log.h"
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <functional>
#include <QImageWriter>

#include "readimagethread.h"
#include "writeimagethread.h"
#include "pngwriter.h"
#include "thresholddlg.h"
#include "bilinear.h"

#include <QDebug>

#define TOPIMGNUM	36
#define LEFTIMGNUM	18
#define SCALEFIX 600.0
#define CALFIXSCALE(width) SCALEFIX/width<1?SCALEFIX/width:1

#define FAST_WRITE 128

#define IMGTESTPATH "D:/Image/follower.jpg"
int MAX_KERNEL_LENGTH = 31;
using namespace std;
using namespace cv;

void MainWindow::ConnectSignal()
{
	if (m_ActionList.size() > 0)
	{
		connect(m_ActionList.at(0), SIGNAL(triggered(bool)), this, SLOT(WatchImageMem(bool)));
	}
	if (m_ActionList.size() > 1)
	{
		connect(m_ActionList.at(1), SIGNAL(triggered(bool)), this, SLOT(ReadWriteImg(bool)));
	}
	if (m_ActionList.size() > 2)
	{
		connect(m_ActionList.at(2), SIGNAL(triggered(bool)), this, SLOT(BGR2RGB(bool)));
	}
	if (m_ActionList.size() > 3)
	{
		connect(m_ActionList.at(3), SIGNAL(triggered(bool)), this, SLOT(ShowRGBImage(bool)));
	}
	if (m_ActionList.size() > 4)
	{
		connect(m_ActionList.at(4), SIGNAL(triggered(bool)), this, SLOT(LinearBlend(bool)));
	}
	if (m_ActionList.size() > 5)
	{
		connect(m_ActionList.at(5), SIGNAL(triggered(bool)), this, SLOT(LinearBlur(bool)));
	}
	if (m_ActionList.size() > 6)
	{
		connect(m_ActionList.at(6), SIGNAL(triggered(bool)), this, SLOT(Gaussian(bool)));
	}
	if (m_ActionList.size() > 7)
	{
		connect(m_ActionList.at(7), SIGNAL(triggered(bool)), this, SLOT(MedianBlur(bool)));
	}
	if (m_ActionList.size() > 8)
	{
		connect(m_ActionList.at(8), SIGNAL(triggered(bool)), this, SLOT(BilateralBlur(bool)));
	}
	if (m_ActionList.size() > 9)
	{
		connect(m_ActionList.at(9), SIGNAL(triggered(bool)), this, SLOT(TurnInImg(bool)));
	}
	if (m_ActionList.size() > 10)
	{
		connect(m_ActionList.at(10), SIGNAL(triggered(bool)), this, SLOT(TurnOutImg(bool)));
	}
	if (m_ActionList.size() > 11)
	{
		connect(m_ActionList.at(11), SIGNAL(triggered(bool)), this, SLOT(Threshold(bool)));
	}
	if (m_ActionList.size() > 12)
	{
		connect(m_ActionList.at(12), SIGNAL(triggered(bool)), this, SLOT(LaplaceOperator(bool)));
	}
	if (m_ActionList.size() > 13)
	{
		connect(m_ActionList.at(13), SIGNAL(triggered(bool)), this, SLOT(SobelOperator(bool)));
	}
	if (m_ActionList.size() > 14)
	{
		connect(m_ActionList.at(14), SIGNAL(triggered(bool)), this, SLOT(SobelOperatorY(bool)));
	}
	if (m_ActionList.size() > 15)
	{
		connect(m_ActionList.at(15), SIGNAL(triggered(bool)), this, SLOT(CannyOperator(bool)));
	}
	if (m_ActionList.size() > 16)
	{
		connect(m_ActionList.at(16), SIGNAL(triggered(bool)), this, SLOT(AddBorder(bool)));
	}
	if (m_ActionList.size() > 17)
	{
		connect(m_ActionList.at(17), SIGNAL(triggered(bool)), this, SLOT(SavePngImage(bool)));
	}
	if (m_ActionList.size() > 18)
	{
		connect(m_ActionList.at(18), SIGNAL(triggered(bool)), this, SLOT(WritePngImage(bool)));
	}
	if (m_ActionList.size() > 19)
	{
		connect(m_ActionList.at(19), SIGNAL(triggered(bool)), this, SLOT(HalconThreadImage(bool)));
	}
	if (m_ActionList.size() > 20)
	{
		connect(m_ActionList.at(20), SIGNAL(triggered(bool)), this, SLOT(OpencvSaveImage(bool)));
	}
	if (m_ActionList.size() > 21)
	{
		connect(m_ActionList.at(21), SIGNAL(triggered(bool)), this, SLOT(SaveImageInQtOpenCV(bool)));
	}
	if (m_ActionList.size() > 22)
	{
		connect(m_ActionList.at(22), SIGNAL(triggered(bool)), this, SLOT(ThresholdImageValue(bool)));
	}
	if (m_ActionList.size() > 23)
	{
		connect(m_ActionList.at(23), SIGNAL(triggered(bool)), this, SLOT(HoughLineDone(bool)));
	}
	if (m_ActionList.size() > 24)
	{
		connect(m_ActionList.at(24), SIGNAL(triggered(bool)), this, SLOT(ThresholdPhory(bool)));
	}
	if (m_ActionList.size() > 25)
	{
		connect(m_ActionList.at(25), SIGNAL(triggered(bool)), this, SLOT(BilinearScaleImage(bool)));
	}
}

void MainWindow::SetToolTip()
{
	m_ListToolTip.push_back(tr("Open image"));
	m_ListToolTip.push_back(tr("Gray image"));
	m_ListToolTip.push_back(tr("Alpha change"));
	m_ListToolTip.push_back(tr("Origin image"));
	m_ListToolTip.push_back(tr("Linear blend image"));
	m_ListToolTip.push_back(tr("Linear Blur"));
	m_ListToolTip.push_back(tr("Gaussian Blur"));
	m_ListToolTip.push_back(tr("Median Blur"));
	m_ListToolTip.push_back(tr("Bilateral Blur"));
	m_ListToolTip.push_back(tr("PyrUp"));
	m_ListToolTip.push_back(tr("PyrDown"));
	m_ListToolTip.push_back(tr("Threshold"));
	m_ListToolTip.push_back(tr("LaplaceOperator"));
	m_ListToolTip.push_back(tr("SobelOperator-x"));
	m_ListToolTip.push_back(tr("SobelOperator-y"));
	m_ListToolTip.push_back(tr("Canny Operator"));
	m_ListToolTip.push_back(tr("Add Border"));
	m_ListToolTip.push_back(tr("Save png image"));
	m_ListToolTip.push_back(tr("Write png image with libpng"));
	m_ListToolTip.push_back(tr("HalconThreadImage"));
	m_ListToolTip.push_back(tr("OpenCV Save Image"));
	m_ListToolTip.push_back(tr("Save PNG Image with OPenCV and Qt"));
	m_ListToolTip.push_back(QString::fromLocal8Bit("阈值分割或二值化"));
	m_ListToolTip.push_back(QString::fromLocal8Bit("霍夫曼变换提取直线"));
	m_ListToolTip.push_back(QString::fromLocal8Bit("阈值分割和形态学操作"));
	m_ListToolTip.push_back(QString::fromLocal8Bit("双线性内插缩放图像"));
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);
	this->showMaximized();

	Log::SetAppPath(QCoreApplication::applicationDirPath().toStdString());
	Log::GetLog();


	SetToolTip();
	m_pReadImgThread = NULL;
	m_pWriteImgThread = NULL;
	m_pPngOperater = NULL;
	m_pThresholdDialog = NULL;

	for (int i = 1; i <= TOPIMGNUM; i++)
	{
		QString toolTip;
		if (m_ListToolTip.size() > i-1)
			toolTip = m_ListToolTip.at(i-1);

		QString imgPath = QString(":/ico/icon/t%0.png").arg(i);
		m_ActionList.push_back(new QAction(QIcon(imgPath), toolTip, this));
	}
	ui.toolBar->addActions(m_ActionList);

	for (int i = 1; i <= LEFTIMGNUM; i++)
	{
		QString imgPath = QString(":/ico/icon/0%0.png").arg(i);
		m_ActionListLeft.push_back(new QAction(QIcon(imgPath), "", this));
	}
	ui.toolBarLeft->addActions(m_ActionListLeft);

	this->ConnectSignal();

	m_pScene = new QGraphicsScene(this);
	m_pPixapItem = new QGraphicsPixmapItem(NULL);
	m_pScene->addItem(m_pPixapItem);
	ui.graphicsView->setScene(m_pScene);
}

MainWindow::~MainWindow()
{
	Log::destroy();
}

void MainWindow::WatchImageMem(bool state)
{
	/*QString fileName =  QFileDialog::getOpenFileName(this,"Select image", IMGTESTPATH, tr("Image Files (*.png *.jpg *.bmp)"));
	if (fileName.isEmpty())	return;*/

	QString fileName;
	QFileDialog file_dlg;
	file_dlg.setWindowTitle(tr("Open image"));
	file_dlg.setAcceptMode(QFileDialog::AcceptOpen);
	file_dlg.setFileMode(QFileDialog::AnyFile);
	file_dlg.setViewMode(QFileDialog::Detail);

	file_dlg.setGeometry(10, 30, 300, 200);
	file_dlg.setDirectory(".");
	if (file_dlg.exec() == QDialog::Accepted)
	{
		fileName = file_dlg.selectedFiles()[0];
	}
	else
		return;


	//std::string imageName = IMGTESTPATH;
	
	std::string textLog = "Read image cost time: ";
	double start, end;
	double cost;
	start = GetTickCount();

	std::string imageName = fileName.toStdString();
	cv::Mat image = imread(imageName, IMREAD_COLOR);

	end = GetTickCount();
	cost = end - start;
	textLog.append(QString::number(cost).toStdString());
	textLog.append("ms");
	Log::OUTPUTLOG(TYPE_INFO, textLog);

	m_ImageObj = image.clone();

	// get time in opencv.
	double t = (double)getTickCount();
	t = ((double)getTickCount()-t)/getTickFrequency();

	std::string strVersion = getVersionString();

	int cpuCount = getNumberOfCPUs();

	//std::cout << m_ImageObj << std::endl;
	
	if (!image.empty())                      
	{
		cv::Mat rgbImage;
		cvtColor(image, rgbImage,cv::COLOR_BGR2RGB);
		m_pImage = new QImage(rgbImage.data, rgbImage.cols, rgbImage.rows, QImage::Format_RGB888);
		m_pPixapItem->setPixmap(QPixmap::fromImage(*m_pImage));
		m_pPixapItem->setScale(CALFIXSCALE(m_pImage->width()));
		ui.graphicsView->update();
		
		int width = m_pImage->width();
		int height = m_pImage->height();
	}
}

void MainWindow::ReadWriteImg(bool state)
{
	if (m_ImageObj.rows == 0) return;

	this->StartRunTime();
	cv::Mat image = m_ImageObj;
	if (image.data)
	{
		cv::Mat gray_image;
		cvtColor(image, gray_image, COLOR_BGR2GRAY);
		QImage image(gray_image.data, gray_image.cols, gray_image.rows, QImage::Format_Indexed8);
		m_pPixapItem->setPixmap(QPixmap::fromImage(image));
		m_pPixapItem->setScale(CALFIXSCALE(image.width()));
		ui.graphicsView->update();
	}
	this->ShowRunTime();
}

void MainWindow::StartRunTime()
{
	m_StartClock = clock();
}

void MainWindow::ShowRunTime()
{
	QString oldText = m_sLogText;/*ui.textEdit->toPlainText();*/
	oldText.append("\n");

	m_EndClock = clock();
	double totaltime = (double)(m_EndClock - m_StartClock) / CLOCKS_PER_SEC;
	QString text = "Run time in mimusec:";
	text.append(std::to_string((double)(m_EndClock - m_StartClock)).c_str());
	text.append("\n");
	text.append("Run time in second:");
	text.append(QString::number(totaltime));
	oldText.append(text);
	m_sLogText = oldText;
	ui.textEdit->setText(oldText);
}

void MainWindow::BGR2RGB(bool state)
{
	if (m_ImageObj.rows == 0) return;

	QImage image(m_ImageObj.data, m_ImageObj.cols, m_ImageObj.rows, QImage::Format_RGB888);
	m_pPixapItem->setPixmap(QPixmap::fromImage(image));
	m_pPixapItem->setScale(CALFIXSCALE(image.width()));
	ui.graphicsView->update();
}

void MainWindow::ShowRGBImage(bool state)
{
	if (m_ImageObj.rows == 0) return;

	cv::Mat rgbImage;
	cvtColor(m_ImageObj, rgbImage, cv::COLOR_BGR2RGB);
	QImage image(rgbImage.data, rgbImage.cols, rgbImage.rows, QImage::Format_RGB888);
	m_pPixapItem->setPixmap(QPixmap::fromImage(image));
	m_pPixapItem->setScale(CALFIXSCALE(image.width()));
	ui.graphicsView->update();
}

// 图像混合
void MainWindow::LinearBlend(bool state)
{
	if (m_ImageObj.rows == 0) return;

	QString fileName = QFileDialog::getOpenFileName(this, "Select image", IMGTESTPATH, tr("Image Files (*.png *.jpg *.bmp)"));
	if (fileName.isEmpty())	return;

	std::string imageName = fileName.toStdString();
	cv::Mat sec_image = imread(imageName, IMREAD_COLOR);
	float alpha = 0.5;
	cv::Mat dstImage,realImage;
	if (m_ImageObj.rows == sec_image.rows && m_ImageObj.cols == sec_image.cols)
	{
		addWeighted(m_ImageObj, alpha, sec_image, 1.0 - alpha, 0.0, dstImage);
		cvtColor(dstImage, realImage, cv::COLOR_BGR2RGB);
		QImage image(realImage.data, realImage.cols, realImage.rows, QImage::Format_RGB888);
		m_pPixapItem->setPixmap(QPixmap::fromImage(image));
		m_pPixapItem->setScale(CALFIXSCALE(image.width()));
		ui.graphicsView->update();
	}
}

// 线性滤波
void MainWindow::LinearBlur(bool state)
{
	if (m_ImageObj.rows == 0) return;

	this->StartRunTime();
	cv::Mat dst = m_ImageObj.clone();
	//for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	//{
		blur(m_ImageObj, dst, Size(19, 19), Point(-1, -1));
	//}

	cv::Mat rgbImage;
	cvtColor(dst, rgbImage, cv::COLOR_BGR2RGB);
	QImage image(rgbImage.data, rgbImage.cols, rgbImage.rows, QImage::Format_RGB888);
	m_pPixapItem->setPixmap(QPixmap::fromImage(image));
	m_pPixapItem->setScale(CALFIXSCALE(image.width()));
	ui.graphicsView->update();
	this->ShowRunTime();
}

// 高斯滤波
void MainWindow::Gaussian(bool)
{
	if (m_ImageObj.rows == 0) return;

	this->StartRunTime();
	cv::Mat dst = m_ImageObj.clone();
	//for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	//{
		GaussianBlur(m_ImageObj, dst, Size(19, 19), 0, 0);
	//}

	cv::Mat rgbImage;
	cvtColor(dst, rgbImage, cv::COLOR_BGR2RGB);
	QImage image(rgbImage.data, rgbImage.cols, rgbImage.rows, QImage::Format_RGB888);
	m_pPixapItem->setPixmap(QPixmap::fromImage(image));
	m_pPixapItem->setScale(CALFIXSCALE(image.width()));
	ui.graphicsView->update();
	this->ShowRunTime();
}

// 中值滤波,速度超级慢，运行一次大概3.18秒
void MainWindow::MedianBlur(bool)
{
	if (m_ImageObj.rows == 0) return;

	this->StartRunTime();
	cv::Mat dst = m_ImageObj.clone();
	//for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 4)
	//{
		medianBlur(m_ImageObj, dst, 13);
	//}

	cv::Mat rgbImage;
	cvtColor(dst, rgbImage, cv::COLOR_BGR2RGB);
	QImage image(rgbImage.data, rgbImage.cols, rgbImage.rows, QImage::Format_RGB888);
	m_pPixapItem->setPixmap(QPixmap::fromImage(image));
	m_pPixapItem->setScale(CALFIXSCALE(image.width()));
	ui.graphicsView->update();
	this->ShowRunTime();
}

// 双边滤波器模糊,超级耗时.
void MainWindow::BilateralBlur(bool)
{
	if (m_ImageObj.rows == 0) return;

	this->StartRunTime();
	cv::Mat dst = m_ImageObj.clone();
	//for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	//{
		bilateralFilter(m_ImageObj, dst, 17, 17*2, 17/2);
	//}

	cv::Mat rgbImage;
	cvtColor(dst, rgbImage, cv::COLOR_BGR2RGB);
	QImage image(rgbImage.data, rgbImage.cols, rgbImage.rows, QImage::Format_RGB888);
	m_pPixapItem->setPixmap(QPixmap::fromImage(image));
	m_pPixapItem->setScale(CALFIXSCALE(image.width()));
	ui.graphicsView->update();
	this->ShowRunTime();
}

// 高斯金字塔缩小图像
void MainWindow::TurnInImg(bool)
{
	if (m_ImageObj.rows == 0) return;

	this->StartRunTime();
	Mat dst;
	pyrDown(m_ImageObj, dst, Size(m_ImageObj.cols / 2, m_ImageObj.rows / 2));

	cv::Mat rgbImage;
	cvtColor(dst, rgbImage, cv::COLOR_BGR2RGB);
	QImage image(rgbImage.data, rgbImage.cols, rgbImage.rows, QImage::Format_RGB888);
	m_pPixapItem->setPixmap(QPixmap::fromImage(image));
	m_pPixapItem->setScale(CALFIXSCALE(image.width()));
	ui.graphicsView->update();
	this->ShowRunTime();
}

// 高斯金字塔放大图像
void MainWindow::TurnOutImg(bool)
{
	if (m_ImageObj.rows == 0) return;

	this->StartRunTime();
	Mat dst;
	pyrUp(m_ImageObj, dst, Size(m_ImageObj.cols * 2, m_ImageObj.rows * 2));

	cv::Mat rgbImage;
	cvtColor(dst, rgbImage, cv::COLOR_BGR2RGB);
	QImage image(rgbImage.data, rgbImage.cols, rgbImage.rows, QImage::Format_RGB888);
	m_pPixapItem->setPixmap(QPixmap::fromImage(image));
	m_pPixapItem->setScale(CALFIXSCALE(image.width()));
	ui.graphicsView->update();
	this->ShowRunTime();
}

// 阈值分割
void MainWindow::Threshold(bool)
{
	if (m_ImageObj.rows == 0) return;

	this->StartRunTime();
	Mat dst;
	threshold(m_ImageObj, dst, 200, 255, 2);

	cv::Mat rgbImage;
	cvtColor(dst, rgbImage, cv::COLOR_BGR2RGB);
	QImage image(rgbImage.data, rgbImage.cols, rgbImage.rows, QImage::Format_RGB888);
	m_pPixapItem->setPixmap(QPixmap::fromImage(image));
	m_pPixapItem->setScale(CALFIXSCALE(image.width()));
	ui.graphicsView->update();
	this->ShowRunTime();
}

// 拉普拉斯算子
void MainWindow::LaplaceOperator(bool)
{
	if (m_ImageObj.rows == 0) return;

	this->StartRunTime();
	Mat src, src_gray, abs_dst, laplaci;

	//高斯模糊
	GaussianBlur(m_ImageObj, src, Size(5, 5), 0, 0);
	//转灰度图像
	cvtColor(src, src_gray, CV_BGR2GRAY);
	//拉普拉斯处理
	Laplacian(src_gray, laplaci, CV_16S, 3, 1, 0);
	//绝对值处理
	convertScaleAbs(laplaci, laplaci);

	QImage image(laplaci.data, laplaci.cols, laplaci.rows, QImage::Format_Indexed8);
	m_pPixapItem->setPixmap(QPixmap::fromImage(image));
	m_pPixapItem->setScale(CALFIXSCALE(image.width()));
	ui.graphicsView->update();
	this->ShowRunTime();
}

// Sobel 算子-x
void MainWindow::SobelOperator(bool)
{
	if (m_ImageObj.rows == 0) return;

	this->StartRunTime();
	Mat dst,src_gray, x_grad;
	GaussianBlur(m_ImageObj, dst, Size(3, 3), 0, 0);
	cvtColor(dst, src_gray, CV_BGR2GRAY);
	Sobel(src_gray, x_grad, CV_16S, 1, 0, 3);
	convertScaleAbs(x_grad, x_grad);

	QImage image(x_grad.data, x_grad.cols, x_grad.rows, QImage::Format_Indexed8);
	m_pPixapItem->setPixmap(QPixmap::fromImage(image));
	m_pPixapItem->setScale(CALFIXSCALE(image.width()));
	ui.graphicsView->update();
	this->ShowRunTime();
}

// Sobel 算子-Y
void MainWindow::SobelOperatorY(bool)
{
	if (m_ImageObj.rows == 0) return;

	this->StartRunTime();
	Mat dst, src_gray, x_grad;
	GaussianBlur(m_ImageObj, dst, Size(3, 3), 0, 0);
	cvtColor(dst, src_gray, CV_BGR2GRAY);
	Sobel(src_gray, x_grad, CV_16S, 0, 1, 3);
	convertScaleAbs(x_grad, x_grad);

	QImage image(x_grad.data, x_grad.cols, x_grad.rows, QImage::Format_Indexed8);
	m_pPixapItem->setPixmap(QPixmap::fromImage(image));
	m_pPixapItem->setScale(CALFIXSCALE(image.width()));
	ui.graphicsView->update();
	this->ShowRunTime();
}

// canny 算子
void MainWindow::CannyOperator(bool)
{
	if (m_ImageObj.rows == 0) return;

	this->StartRunTime();
	Mat dst, src_gray, src_blur,src_canny;
	cvtColor(m_ImageObj, src_gray, CV_BGR2GRAY);
	blur(src_gray, src_blur, Size(3, 3));
	Canny(src_blur, src_canny, 3, 9, 3);
	convertScaleAbs(src_canny, src_canny);

	QImage image(src_canny.data, src_canny.cols, src_canny.rows, QImage::Format_Indexed8);
	m_pPixapItem->setPixmap(QPixmap::fromImage(image));
	m_pPixapItem->setScale(CALFIXSCALE(image.width()));
	ui.graphicsView->update();
	this->ShowRunTime();
}


void waterSegment(InputArray& _src, OutputArray& _dst, int& noOfSegment)
{
	Mat src = _src.getMat();
	Mat grayImage;
	cvtColor(src, grayImage, CV_BGR2GRAY);
	threshold(grayImage, grayImage, 0, 255, THRESH_BINARY | THRESH_OTSU);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(9, 9), Point(-1, -1));
	morphologyEx(grayImage, grayImage, MORPH_CLOSE, kernel);
	distanceTransform(grayImage, grayImage, DIST_L2, DIST_MASK_3, 5);
	normalize(grayImage, grayImage, 0, 1, NORM_MINMAX);
	grayImage.convertTo(grayImage, CV_8UC1);
	threshold(grayImage, grayImage, 0, 255, THRESH_BINARY | THRESH_OTSU);
	morphologyEx(grayImage, grayImage, MORPH_CLOSE, kernel);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Mat showImage = Mat::zeros(grayImage.size(), CV_32SC1);
	findContours(grayImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));
	for (size_t i = 0; i < contours.size(); i++)
	{
		//这里static_cast<int>(i+1)是为了分水岭的标记不同，区域1、2、3。。。。这样才能分割
		drawContours(showImage, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i + 1)), 2);
	}
	Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	morphologyEx(src, src, MORPH_ERODE, k);
	watershed(src, showImage);

	//随机分配颜色
	vector<Vec3b> colors;
	for (size_t i = 0; i < contours.size(); i++) {
		int r = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int b = theRNG().uniform(0, 255);
		colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}

	// 显示
	Mat dst = Mat::zeros(showImage.size(), CV_8UC3);
	int index = 0;
	for (int row = 0; row < showImage.rows; row++) {
		for (int col = 0; col < showImage.cols; col++) {
			index = showImage.at<int>(row, col);
			if (index > 0 && index <= contours.size()) {
				dst.at<Vec3b>(row, col) = colors[index - 1];
			}
			else if (index == -1)
			{
				dst.at<Vec3b>(row, col) = Vec3b(255, 255, 255);
			}
			else {
				dst.at<Vec3b>(row, col) = Vec3b(0, 0, 0);
			}
		}
	}
}

// 添加边框
void MainWindow::AddBorder(bool)
{
	if (m_ImageObj.rows == 0) return;

	this->StartRunTime();
	int top, bottom, left, right;
	top = (int)(0.05*m_ImageObj.rows);
	bottom = m_ImageObj.rows;
	left = (int)(0.05*m_ImageObj.cols); 
	right = m_ImageObj.cols;
	Mat dst;
	RNG rng(12345);

	Scalar value(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	copyMakeBorder(m_ImageObj, dst, top, bottom, left, right, BORDER_CONSTANT, value);

	QImage image(dst.data, dst.cols, dst.rows, QImage::Format_Indexed8);
	m_pPixapItem->setPixmap(QPixmap::fromImage(image));
	m_pPixapItem->setScale(CALFIXSCALE(image.width()));
	ui.graphicsView->update();
	this->ShowRunTime();
}

// 测试opencv写PNG图像效率.
void MainWindow::SavePngImage(bool)
{	
	if (m_pReadImgThread == nullptr)
	{
		m_pReadImgThread = new ThreadReadImage(this);
		m_pReadImgThread->m_bRunState = true;
		m_pReadImgThread->SetFolder("D://IMAGE-INPUT");

		auto funImgInfo = std::bind(&MainWindow::OutputImageData, this, std::placeholders::_1);
		m_pReadImgThread->SetFunImageInfo(funImgInfo);

		m_pReadImgThread->start();
	}

	if (m_pWriteImgThread == nullptr)
	{
		m_pWriteImgThread = new ThreadWriteImage(this);
		m_pWriteImgThread->m_bRun = true;
		m_pWriteImgThread->SetFolderPath("D://IMAGE-OUTPUT");

		connect(m_pWriteImgThread, SIGNAL(OutputLog(QString)), this, SLOT(AddToLogText(QString)));
		m_pWriteImgThread->start();
	}
}

void MainWindow::SaveBmpImage(bool)
{
	if (m_pReadImgThread == nullptr)
	{
		m_pReadImgThread = new ThreadReadImage(this);
		m_pReadImgThread->m_bRunState = true;
		m_pReadImgThread->SetFolder("D://IMAGE-INPUT");

		auto funImgInfo = std::bind(&MainWindow::OutputImageData, this, std::placeholders::_1);
		m_pReadImgThread->SetFunImageInfo(funImgInfo);

		m_pReadImgThread->start();
	}

	if (m_pWriteImgThread == nullptr)
	{
		m_pWriteImgThread = new ThreadWriteImage(this);
		m_pWriteImgThread->m_bRun = true;
		m_pWriteImgThread->SetFolderPath("D://IMAGE-OUTPUT");

		connect(m_pWriteImgThread, SIGNAL(OutputLog(QString)), this, SLOT(AddToLogText(QString)));
		m_pWriteImgThread->start();
	}
}

void MainWindow::OutputImageData(s_ImageInfo info)
{
	if (m_pWriteImgThread != nullptr)
	{
		m_pWriteImgThread->PushImageData(info);
	}
}

void MainWindow::AddToLogText(QString text)
{
	m_sLogText.append("\n");
	m_sLogText.append(text);
	this->ShowRunTime();
}

void MainWindow::WritePngImage(bool)
{
	QImageWriter writer("D:/IMAGE-OUTPUT/00.png", "png");
	writer.setQuality(100);
	if (writer.canWrite())
	{
		int width = m_pImage->width();
		int height = m_pImage->height();

		QImage image(m_pImage->bits(), m_pImage->width(), m_pImage->height(), QImage::Format_RGB888);
		// 写入图片至文件
		writer.write(image);
	}
	else
	{
		// 获取错误信息
		QImageWriter::ImageWriterError error = writer.error();
		QString strError = writer.errorString();
	}
}

void MainWindow::HalconThreadImage(bool)
{

}

void MainWindow::ReadPNGFile(std::string filename, s_ImageInfo *info)
{
	if (m_pPngOperater == NULL)
	{
		m_pPngOperater = new pngwriter();
		m_pPngOperater->readfromfile(filename.c_str());
		info->lImageWidth = m_pPngOperater->getwidth();
		info->lImageHeight = m_pPngOperater->getheight();
		info->bit_depth = m_pPngOperater->getbitdepth();
		info->iImageCount = m_pPngOperater->getcolortype();
		long bufSize = info->lImageWidth*info->lImageHeight*sizeof(unsigned char)*3;
		info->pImageBuff = (unsigned char*)malloc(bufSize);
		unsigned char* pBuf = m_pPngOperater->getgraph();
		memcpy(info->pImageBuff, pBuf, bufSize);
	}
}

void MainWindow::WritePNGFile(std::string filename)
{
	
}

void MainWindow::OpencvSaveImage(bool)
{
	s_ImageInfo imgInfo;
	ThreadReadImage tr(this);
	tr.ReadImgWinAPI("D:/IMAGE-INPUT/iPhone X Black/10_10_3_1_002011571.bmp", imgInfo);

	this->StartRunTime();
	QString textLog = "Save time:";
	double start, end;
	double cost;
	start = GetTickCount();


	QImage::Format format;
	int type;
	if (imgInfo.iImageCount == 32)
	{
		format = QImage::Format_RGB32;
		type = CV_8UC4;
	}
	else if (imgInfo.iImageCount == 8)
	{
		format = QImage::Format_Indexed8;
		type = CV_8UC1;
	}
	else if (imgInfo.iImageCount == 16)
	{
		format = QImage::Format_RGB16;
		type = CV_8UC2;
	}
	else if (imgInfo.iImageCount == 24)
	{
		format = QImage::Format_RGB888;
		type = CV_8UC3;
	}

	QImage imageQt((uchar*)imgInfo.pImageBuff, imgInfo.lImageWidth, imgInfo.lImageHeight, format);

	QImage rgbImage;
	if (format = QImage::Format_RGB32)
	{
		QImage tempImage = imageQt.convertToFormat(QImage::Format_RGB888);
		rgbImage = tempImage.mirrored(false,true);
		format = QImage::Format_RGB888;
		type = CV_8UC3;
	}
	
	int perLine = rgbImage.bytesPerLine();
	cv::Mat matImg = cv::Mat(rgbImage.height(), rgbImage.width(), type, (void*)rgbImage.constBits(), perLine);

	QImage imaget(matImg.data, matImg.cols, matImg.rows, QImage::Format_RGB888);
	m_pPixapItem->setPixmap(QPixmap::fromImage(imaget));
	m_pPixapItem->setScale(CALFIXSCALE(imaget.width()));
	ui.graphicsView->update();

	std::vector<int> compressionParam;
	compressionParam.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compressionParam.push_back(3);
	imwrite("D:/IMAGE-OUTPUT/02.png", matImg);

	end = GetTickCount();
	cost = end - start;
	textLog.append(QString::number(cost));
	textLog.append("ms");
	this->AddToLogText(textLog);
	qDebug() << "++++++++++++++++++" << textLog;
}

bool MainWindow::SaveImageToPNGFile(QString folder, const char* pData, std::pair<long, long> imageSize, long bitCount, int quality)
{
	long width = imageSize.first;
	long height = imageSize.second;

	QImage::Format format;

	if (bitCount == 8)
		format = QImage::Format_Indexed8;
	else if (bitCount == 16)
		format = QImage::Format_RGB16;
	else if (bitCount == 24)
		format = QImage::Format_RGB888;
	else if (bitCount == 32)
		format = QImage::Format_RGB32;

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

	QImage image((uchar*)pData, width, height, format);
	//QImage mirrImage = image.mirrored(false,true);

	if (format == QImage::Format_RGB32)
	{
		QImage rgb24Image = image.convertToFormat(QImage::Format_RGB888);
		QImage&& bgrImage = rgb24Image.rgbSwapped();
		cv::Mat matImg = cv::Mat(bgrImage.height(), bgrImage.width(), CV_8UC3, (void*)bgrImage.constBits(), bgrImage.bytesPerLine());
		imwrite(folder.toStdString(), matImg);
	}
	else if(format == QImage::Format_RGB888)
	{
		cv::Mat matImg = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
		imwrite(folder.toStdString(), matImg);
	}
	else
	{
		QImageWriter writer(folder, "PNG");
		// 无损保存图片
		writer.setQuality(quality);
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

			//todotest
			qDebug() << QString("[Grab Image Test] Save Image Error %1").arg(strError);
			return false;
		}
	}
	return true;
}

void MainWindow::SaveImageInQtOpenCV(bool)
{
	s_ImageInfo imgInfo;
	ThreadReadImage tr(this);
	tr.ReadImgWinAPI("D:/IMAGE-INPUT/iPhone X Black/10_10_3_1_002011571.bmp", imgInfo);

	this->StartRunTime();
	QString textLog = "Save time:";
	double start, end;
	double cost;
	start = GetTickCount();

	std::pair<long, long> size_length;
	size_length.first = imgInfo.lImageWidth;
	size_length.second = imgInfo.lImageHeight;
	this->SaveImageToPNGFile("D:/IMAGE-OUTPUT/02.png", (char*)imgInfo.pImageBuff, size_length, imgInfo.iImageCount,60);

	end = GetTickCount();
	cost = end - start;
	textLog.append(QString::number(cost));
	textLog.append("ms");
	this->AddToLogText(textLog);
	qDebug() << "++++++++++++++++++" << textLog;
}

void MainWindow::ThresholdImageValue(bool)
{
	if (m_ImageObj.rows == 0) return;
	if (m_pThresholdDialog == NULL)
	{
		m_pThresholdDialog = new ThresholdDialog(this);
		m_pThresholdDialog->SetImage(m_ImageObj);
		connect(m_pThresholdDialog,SIGNAL(Finish()),this,SLOT(ThresholdFinish()));
		m_pThresholdDialog->show();
	}
}

void MainWindow::ThresholdFinish()
{
	if (m_pThresholdDialog != NULL)
	{
		delete m_pThresholdDialog;
		m_pThresholdDialog = NULL;
	}
}

void MainWindow::HoughLineDone(bool)
{
	if (m_ImageObj.empty())
		return;

	cv::Mat img_rgb, img_gry, img_cny, img_Gaussian, img_edge;
	cv::cvtColor(m_ImageObj, img_gry, cv::COLOR_BGR2GRAY);

	std::vector<cv::Vec2f> vecLines;
	cv::HoughLines(img_gry, vecLines,2,3,120);
	const int alpha = 2000;

	for (int i = 0; i < vecLines.size(); i++)
	{
		cv::Vec2f p = vecLines[i];

		float rho = vecLines[i][0], theta = vecLines[i][1];
		double cs = cos(theta), sn = sin(theta);
		double x = rho * cs, y = rho * sn;

		cv::Point pt1(cvRound(x + alpha * (-sn)), cvRound(y + alpha * cs));
		cv::Point pt2(cvRound(x - alpha * (-sn)), cvRound(y - alpha * cs));
		cv::line(img_gry, pt1, pt2, cv::Scalar(0, 0, 255));
	}

	QImage image(img_gry.data, img_gry.cols, img_gry.rows, QImage::Format_Indexed8);
	m_pPixapItem->setPixmap(QPixmap::fromImage(image));
	m_pPixapItem->setScale(CALFIXSCALE(image.width()));
	ui.graphicsView->update();
}

void MainWindow::ThresholdPhory(bool)
{
	if (m_ImageObj.empty())
		return;
}

void MainWindow::BilinearScaleImage(bool)
{
	if (m_ImageObj.empty())
		return;

	int cols = m_ImageObj.cols;
	int rows = m_ImageObj.rows;
	int size = sizeof(unsigned char);

	m_pBilinear = new BilinearScale();
	m_pBilinear->SetInputData(m_ImageObj.cols, m_ImageObj.rows, m_ImageObj.data,0.6f);
	m_pBilinear->Execute();

	int width, height;
	unsigned char* pBuffer;
	m_pBilinear->GetOutputData(width, height, &pBuffer);

	delete m_pBilinear;
	m_pBilinear = nullptr;

	//pBuffer = (unsigned char*)malloc(width*height*size);
	//memset(pBuffer, 20, sizeof(unsigned char));

	QImage image(pBuffer, width, height, QImage::Format_RGB888);
	m_pPixapItem->setPixmap(QPixmap::fromImage(image));
	//m_pPixapItem->setScale(CALFIXSCALE(image.width()));
	ui.graphicsView->update();
}

// scan every pixel in Mat.C语言风格的像素遍历.
Mat& ScanImageAndReduceC(Mat& I, const uchar* const table)
{
	// accept only char type matrices
	CV_Assert(I.depth() != sizeof(uchar));

	int channels = I.channels();

	int nRows = I.rows * channels;
	int nCols = I.cols;

	if (I.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	int i, j;
	uchar* p;
	for (i = 0; i < nRows; ++i)
	{
		p = I.ptr<uchar>(i);
		for (j = 0; j < nCols; ++j)
		{
			p[j] = table[p[j]];
		}
	}
	return I;
}

// C++语言风格的像素遍历.
void ScanImageAndReduce(Mat& I, const uchar* const table)
{
	int channels = I.channels();

	int nRows = I.rows * channels;
	int nCols = I.cols;

	if (I.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	uchar* p = I.data;
	for (unsigned int i = 0; i < nCols*nRows; ++i)
		*p++ = table[*p];
}

// 迭代器方式遍历图像.
Mat& ScanImageAndReduceIterator(Mat& I, const uchar* const table)
{
	// accept only char type matrices
	CV_Assert(I.depth() != sizeof(uchar));

	const int channels = I.channels();
	switch (channels)
	{
		case 1:
		{
			MatIterator_<uchar> it, end;
			for (it = I.begin<uchar>(), end = I.end<uchar>(); it != end; ++it)
				*it = table[*it];
			break;
		}
		case 3:
		{
			MatIterator_<Vec3b> it, end;
			for (it = I.begin<Vec3b>(), end = I.end<Vec3b>(); it != end; ++it)
			{
				(*it)[0] = table[(*it)[0]];
				(*it)[1] = table[(*it)[1]];
				(*it)[2] = table[(*it)[2]];
			}
		}
	}

	return I;
}

// 将像素中给定的值替换成其他值.
void ReplacePixel(Mat& I, const uchar* const table)
{
	Mat lookUpTable(1, 256, CV_8U);
	uchar* p = lookUpTable.data;
	for (int i = 0; i < 256; ++i)
		p[i] = table[i];

	Mat J;
	LUT(I, lookUpTable, J);
}

// 基本的像素访问方法实现掩码操作.
void Sharpen(const Mat& myImage, Mat& Result)
{
	CV_Assert(myImage.depth() == CV_8U);  // 仅接受uchar图像

	Result.create(myImage.size(), myImage.type());
	const int nChannels = myImage.channels();

	for (int j = 1; j < myImage.rows - 1; ++j)
	{
		const uchar* previous = myImage.ptr<uchar>(j - 1);
		const uchar* current = myImage.ptr<uchar>(j);
		const uchar* next = myImage.ptr<uchar>(j + 1);

		uchar* output = Result.ptr<uchar>(j);

		for (int i = nChannels; i < nChannels*(myImage.cols - 1); ++i)
		{
			*output++ = saturate_cast<uchar>(5 * current[i]
				- current[i - nChannels] - current[i + nChannels] - previous[i] - next[i]);
		}
	}

	Result.row(0).setTo(Scalar(0));
	Result.row(Result.rows - 1).setTo(Scalar(0));
	Result.col(0).setTo(Scalar(0));
	Result.col(Result.cols - 1).setTo(Scalar(0));
}

// 增强图像亮度,增加每个像素的灰度值.
void IncreasePixelValue(const Mat& image,float alpha,int beta)
{
	Mat res_mat = Mat::zeros(image.size(), image.type());

	for (int y = 0; y < image.rows; y++)
	{
		for (int x = 0; x < image.cols; x++)
		{
			for (int c = 0; c < 3; c++)
			{
				res_mat.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha*(image.at<Vec3b>(y, x)[c]) + beta);
			}
		}
	}
}