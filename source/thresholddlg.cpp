#include "thresholddlg.h"
#include "imgproc.hpp"
#include <QtConcurrent>

ThresholdDialog::ThresholdDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(ThresholdReview()));
	connect(this, SIGNAL(ShowImage()), this, SLOT(UpdateImage()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(CancelDone()));
}

ThresholdDialog::~ThresholdDialog()
{
}

void ThresholdDialog::SetImage(cv::Mat image)
{
	QImage imaget(image.data, image.cols, image.rows, QImage::Format_RGB888);
	ui.label->setPixmap(QPixmap::fromImage(imaget));
	ui.label->setScaledContents(true);
	m_Mat = image;
	image.copyTo(m_DstMat);
}

void ThresholdDialog::ThresholdReview()
{
	double thVal = ui.doubleSpinBox_Threshold->value();
	double maxVal = ui.doubleSpinBox_MaxVal->value();

	int type = ui.comboBox->currentIndex();
	QtConcurrent::run(this, &ThresholdDialog::ProcessThreshold, m_Mat, m_DstMat, thVal, maxVal, type);
}

void ThresholdDialog::ProcessThreshold(cv::Mat src, cv::Mat dst, double thVal, double maxVal, int type)
{
	threshold(src, dst, thVal, maxVal, type);
	emit ShowImage();
}

void ThresholdDialog::UpdateImage()
{
	int rows = m_DstMat.rows;
	int columns = m_DstMat.cols;
	QImage imaget(m_DstMat.data, m_DstMat.cols, m_DstMat.rows, QImage::Format_RGB888);
	ui.label->setPixmap(QPixmap::fromImage(imaget));
	ui.label->setScaledContents(true);
}

void ThresholdDialog::CancelDone()
{
	emit Finish();
}
