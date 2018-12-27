#include<opencv2/opencv.hpp>
#include<iostream>
#include<assert.h>
#include<stdlib.h>
#include<math.h>



using namespace std;


int main(int args, char** argv) {


	
	// Read input image
	cv::Mat image = cv::imread("D:/A/图像处理FCM/church01.jpg", 0);
	//Fast特征检测器，经过特别设计，能快速检测，依赖少数像素比较来确定是否接受一个特征点
	//特征点向量
	std::vector<cv::KeyPoint>keypoints;
	//使用opencv的通用接口函数来，构造Fast特征检测器。这里要注意书上的创建方法发生了变化。
	cv::Ptr<cv::FeatureDetector> fast =
		cv::FastFeatureDetector::create(10, true, cv::FastFeatureDetector::TYPE_9_16);
	//检测阈值
	fast->detect(image, keypoints);
	//特征点检测出来后，用一个通用的特征点绘制函数
	cv::drawKeypoints(image, keypoints, image, cv::Scalar(255, 255, 255), cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);//最后一个参数是绘制标记
	cv::imshow("Fast特征检测", image);
	cv::waitKey(0);

}