#pragma once

#include <QDialog>
#include "ui_ThresholdDialog.h"
#include "core.hpp"

class ThresholdDialog : public QDialog
{
	Q_OBJECT

public:
	ThresholdDialog(QWidget *parent = Q_NULLPTR);
	~ThresholdDialog();

	void SetImage(cv::Mat);

private slots:
	void ThresholdReview();
	void UpdateImage();
	void CancelDone();

signals:
	void Finish();
	void ShowImage();

private:
	Ui::ThresholdDialog ui;

	void ProcessThreshold(cv::Mat, cv::Mat, double,double,int);

	cv::Mat		m_Mat;
	cv::Mat		m_DstMat;
};
