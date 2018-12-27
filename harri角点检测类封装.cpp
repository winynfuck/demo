main.cpp
#include<opencv2/opencv.hpp>
#include<iostream>
#include<assert.h>
#include<stdlib.h>
#include<math.h>
#include"HarriDetector.h"


#define PI 3.1415926

using namespace std;


int main(int args, char** argv) {


	
	// Read input image
	cv::Mat image = cv::imread("D:/A/图像处理FCM/church01.jpg", 0);
	//创建Harris检测器的对象
	HarriDetector harris;
	//计算哈里斯值
	harris.detect(image);
	//检测角点
	std::vector<cv::Point>pts;
	harris.getCorners(pts,0.01);
	//绘制哈里斯角点
	harris.drawOnImage(image, pts);

}
HarriDetector.h
#pragma once
#include<opencv2/opencv.hpp>
class HarriDetector
{
	private:
	//表示角点强度的32位浮点图像
	cv::Mat cornerStrength;
	//表示阈值化后角点的32位浮点图像
	cv::Mat cornerTh;
	//局部极大值图像（内部）
	cv::Mat localMax;
	//导数平滑的相邻像素的尺寸
	int neighbourhood;
	//梯度计算的孔径大小
	int aperture;
	//哈里斯参数
	double k;
	//阈值计算的最大强度
	double maxStrength;
	//计算得到的阈值(内部)
	double threshold;
	//非极大值抑制的相邻像素的尺寸
	int nonMaxSize;
	//非极大值抑制的核
	cv::Mat kernel;


public:

	HarriDetector(int neighbourhood = 3, int  aperture = 3, double k = 0.01, double  maxStrength = 0.0, double  threshold = 0.01, int nonMaxSize = 3);
	void setLocalMaxWindowSize(int size);
	void detect(const cv::Mat &image);
	void getCorners(std::vector<cv::Point> &points, double qualityLevel);
	cv::Mat getCornerMap(double quanlityLevel);
	void drawOnImage(cv::Mat &image, const std::vector<cv::Point>&points, cv::Scalar color = cv::Scalar(255, 255, 255), int radius = 3, int thickness = 2);
};
HarriDetector.cpp
#include"HarriDetector.h"



HarriDetector::HarriDetector(int neighbourhood1, int  aperture1 , double k1 , double  maxStrength1, double  threshold1 , int nonMaxSize1 ){
	//创建非极大值抑制的核
	setLocalMaxWindowSize(nonMaxSize);
	neighbourhood = neighbourhood1;
	aperture = aperture1,
	 k = k1,
	maxStrength = maxStrength1,
	threshold = threshold1,
	nonMaxSize = nonMaxSize1;
}
void HarriDetector::setLocalMaxWindowSize(int size) {

	nonMaxSize = size;
	kernel.create(nonMaxSize, nonMaxSize, CV_8U);
}

void HarriDetector::detect(const cv::Mat &image)
{
	//Harris计算
	//角点检测的参数解析：原图像，输出的角点强度图，导致平滑的相邻像素尺寸，梯度计算的孔径大小，哈里斯参数
	cv::cornerHarris(image, cornerStrength, neighbourhood, aperture, k);
	//内部阈值计算
	double minStrength;//未使用
	//cv::minMaxLoc()函数用于找到最小值最大值和单通道图像的最小值最大值位置
	cv::minMaxLoc(cornerStrength, &minStrength, &maxStrength);
	//局部最大值检测
	cv::Mat dilated;//临时图像
	cv::dilate(cornerStrength, dilated, cv::Mat());//膨胀操作
	cv::compare(cornerStrength, dilated, localMax, cv::CMP_EQ);
}
cv::Mat HarriDetector::getCornerMap(double quanlityLevel) {
	//由Harris值获取角点图
	cv::Mat cornerMap;
	//对角点强度图进行阈值化
	threshold = quanlityLevel * maxStrength;
	cv::threshold(cornerStrength, cornerTh, threshold, 255, cv::THRESH_BINARY);
	//转换为8位图像
	cornerTh.convertTo(cornerMap, CV_8U);
	//非极大值抑制
	cv::bitwise_and(cornerMap, localMax, cornerMap);
	return cornerMap;

}
void HarriDetector::getCorners(std::vector<cv::Point> &points, double qualityLevel) {

	// Get the corner map
	cv::Mat cornerMap = getCornerMap(qualityLevel);
	//遍历像素得到所有特征点
	for (int y = 0; y < cornerMap.rows; y++) {
		const uchar*rowPtr = cornerMap.ptr<uchar>(y);
		for (int x = 0; x < cornerMap.cols; x++) {
			//如果是特征点
			if (rowPtr[x]){
				points.push_back(cv::Point(x, y));
				}
		}
	}
}

void HarriDetector::drawOnImage(cv::Mat &image, const std::vector<cv::Point>&points, cv::Scalar color, int radius, int thickness) {
	std::vector<cv::Point>::const_iterator it = points.begin();
	//对于所有角点
	while (it != points.end()) {
		//绘制一个圆
		cv::circle(image, *it, radius, color, thickness);
		++it;
	}
	cv::imshow("角点检测图", image);
	cv::waitKey(0);

}