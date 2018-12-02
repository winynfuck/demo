#include<opencv2/opencv.hpp>
#include<iostream>
#include<assert.h>
#include<stdlib.h>
#include<math.h>


#include"ObjectFinder.h"
#include"Histogram1D.h"
#include"colorHistogram.h"



using namespace std;


int main(int args, char** argv) {
	//------------------------------对参考图像的处理
	//读取參考图像
	cv::Mat image = cv::imread("D:/A/baboon1.jpg");
	if (!image.data)
		return 0;

	//定义查找物体
	cv::Mat imageROI = image(cv::Rect(110, 260, 35, 40));
	//画出感兴趣区域的位置
	cv::rectangle(image, cv::Rect(110, 260, 35, 40), cv::Scalar(0, 0, 255));

	//显示參考图像
	cv::namedWindow("参考图片，感兴趣区域为红色");
	cv::imshow("参考图片，感兴趣区域为红色", image);

	//获得色度直方图
	colorHistogram hc;
	cv::Mat colorhist = hc.getHueHistogram(imageROI,65);

	//----------------------------------对目标图像的处理
	//####################读取、得到目标图像高饱和度模板

	//读入目标图像
	cv::Mat target_image = cv::imread("D:/A/baboon3.jpg");

	//将目标图像转换到HSV颜色空间
	cv::Mat target_hsv;
	cv::cvtColor(target_image, target_hsv, 40);

	//分离图像通道
	vector<cv::Mat> v;
	cv::split(target_hsv, v);

	//消除饱和度较低的像素点
	int minSat = 65;
	cv::Mat mask;
	cv::threshold(v[1], mask, minSat, 255, cv::THRESH_BINARY);
	cv::namedWindow("目标图像高饱和度模板");
	cv::imshow("目标图像高饱和度模板", mask);

	//####################直方图反投影、模板与投影的与运算

	//进行直方图反投影
	ObjectFinder finder;
	finder.setHistogram(colorhist);
	finder.setThreshold(-1.0f);
	int ch[1] = { 0 };
	cv::Mat back_project_result = finder.find(target_hsv, 0.0f, 180.0f, ch, 1);

	cv::namedWindow("目标图像直方图反投影");
	cv::imshow("目标图像直方图反投影", back_project_result);

	//利用位运算消除低饱和度像素
	cv::Mat result;
	cv::bitwise_and(back_project_result, mask, result);
	cv::namedWindow("目标图像直方图反投影后去除低饱和度");
	cv::imshow("目标图像直方图反投影后去除低饱和度", result);


	//在目标图像上画一个矩形表示参考图像中的位置
	cv::Rect rect(110, 260, 35, 40);
	cv::rectangle(target_image, rect, cv::Scalar(0, 0, 255));
	//均值漂移算法的输出是一个矩形
	cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER, 10, 0.01);
	cout << "均值漂移迭代次数 = " << cv::meanShift(result, rect, criteria) << endl;
	//在目标图像上画一个矩形
	cv::rectangle(target_image, rect, cv::Scalar(0, 255, 0));

	//展示结果图
	cv::namedWindow("查找结果");
	cv::imshow("查找结果", target_image);

	cv::waitKey();
	return 0;

}
