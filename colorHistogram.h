#pragma once
#include<opencv2/opencv.hpp>

class colorHistogram {
	//使用掩码计算1D色调直方图
	//BGR图像应先转换到HSV色调空间
	//并去除低饱和度的像素
private:
	int histSize[3];//项的数量
	float hranges[2];//像素的最小最大值
	const float*ranges[3];
	int channels[3];//用到三个通道
public:
	colorHistogram(){
		histSize[0] = histSize[1] = histSize[2] = 256;
		hranges[0] = 0.0;    // BRG range
		hranges[1] = 255.0;
		ranges[0] = hranges;//三个通道使用相同的范围
		ranges[1] = hranges;
		ranges[2] = hranges;
		channels[0] = 0;		// the three channels 
		channels[1] = 1;
		channels[2] = 2;
	}
	cv::Mat getHueHistogram(const cv::Mat &image, int minSaturation = 0) {
		cv::Mat hist;
		//将图像转换到HSV色彩空间
		cv::Mat hsv;
		cv::Mat mask;
		cv::cvtColor(image, hsv, 40);
		//这一步预处理得到，去除低饱和度的mask，用于后面的计算直方图过程。
		if (minSaturation > 0) {
			//分割图像为三幅单通道的图像
			std::vector<cv::Mat>v;//定义一个向量
			cv::split(hsv, v);
			//去除低饱和度的像素
			cv::threshold(v[0], mask, minSaturation, 255, cv::THRESH_BINARY);
		}
		//色调通道图像直方图的参数
		hranges[0] = 0.0;
		hranges[1] = 180.0;
		channels[0] = 0;//色调通道
		//计算直方图,掩模是一个二值图像，只计算1的位置的直方图。
		cv::calcHist(&hsv, 1, channels, mask, hist, 1, histSize,ranges);
		return hist;
	}
	
};