lineFinder.cpp
#include "lineFinder.h"

void lineFinder:: setAccResolution(double dRho, double dTheta) {
	//设置累加器的分辨率
	deltaRho = dRho;
	deltaTheta = dTheta;
	
}
void lineFinder::setMinVote(int minv) {
	minVote = minv;
}
void lineFinder::setLineLengthAndGap(double length, double gap) {
	minLength = length;
	maxGap = gap;
}
std::vector<cv::Vec4i> lineFinder::findLines(cv::Mat &binary) {
	lines.clear();
	cv::HoughLinesP(binary, lines, deltaRho, deltaTheta, minVote, minLength, maxGap);
	return lines;
}
void lineFinder::drawDetectedLines(cv::Mat &result, cv::Scalar color) {
	//画线
	std::vector<cv::Vec4i>::const_iterator it2 = lines.begin();
	while (it2 != lines.end()) {
		cv::Point pt1((*it2)[0], (*it2)[1]);
		cv::Point pt2((*it2)[2], (*it2)[3]);
		cv::line(result, pt1, pt2, color);
		++it2;
	}
}
lineFinder.h
#pragma once
#include<opencv2/opencv.hpp>
#define PI 3.1415926;
using namespace std;
class lineFinder
{
private:
	//原图
	cv::Mat img;
	//向量中包含检测到的直线的端点
	std::vector<cv::Vec4i>lines;
	//累加器的分辨率
	double deltaRho;
	double deltaTheta;
	//直线被接受时所需的最小投票数
	int minVote;
	//直线的最小长度
	double minLength;
	//沿着直线方向的最大缺口
	double maxGap;
public:
	lineFinder() :deltaRho(1), deltaTheta(3.1415926 / 180), minVote(10), minLength(0.), maxGap(0.) {}
	void setAccResolution(double dRho, double dTheta);
	void setMinVote(int minv);
	void setLineLengthAndGap(double length, double gap);
	std::vector<cv::Vec4i>findLines(cv::Mat &binary);
	void drawDetectedLines(cv::Mat &image, cv::Scalar color = cv::Scalar(255, 255, 255));
};

main.cpp
#include<opencv2/opencv.hpp>
#include<iostream>
#include<assert.h>
#include<stdlib.h>
#include<math.h>
#include"lineFinder.h"


using namespace std;


int main(int args, char** argv) {

	// 打开另一幅图像
	cv::Mat image = cv::imread("D:/A/road.jpg");
	cv::Mat contours;
	cv::Canny(image, contours,125,250);
	//创建一个lineFinder实例
	lineFinder finder;
	//设置概率霍夫变换的参数
	finder.setLineLengthAndGap(100, 20);
	finder.setMinVote(80);
	//检测并绘制直线
	std::vector<cv::Vec4i>lines = finder.findLines(contours);
	finder.drawDetectedLines(image);
	cv::namedWindow("霍夫变换p进行直线检测", 0);
	cv::imshow("霍夫变换p进行直线检测", image);
	cv::waitKey(0);
	
	




}