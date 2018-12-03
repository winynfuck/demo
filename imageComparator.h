#pragma once
#include<opencv2/opencv.hpp>
#include"colorHistogram.h"
class imageComparator {
private:
	cv::Mat reference;
	cv::Mat input;
	cv::Mat refH;
	cv::Mat inputH;
	colorHistogram hc;
	int div;
public:
	imageComparator() :div(32) {
		//减色因子
		//比较是指减色后的图像之间进行比较
		//色彩空间中的每个维度都将按照该变量进行减色


	}
	void setColorReduction(int factor) {
		div = factor;
	}
	int getColorReduction() {
		return div;
	}
	void setReferenceImage(const cv::Mat & image) {
		reference = hc.colorReduce(image,div);
		refH = hc.getHueHistogram(reference);
	}
	double compare(const cv::Mat &image) {
		input = hc.colorReduce(image, div);
		inputH = hc.getHueHistogram(input);
		return cv::compareHist(refH, inputH,2);
	}


};