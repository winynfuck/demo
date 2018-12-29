#include<opencv2/opencv.hpp>
#include<opencv2/features2d.hpp>
#include<iostream>
#include<assert.h>
#include<stdlib.h>
#include<math.h>
#include<opencv2/xfeatures2d.hpp>




using namespace std;
using namespace cv::xfeatures2d;

int main(int args, char** argv) {

	// Read input image
	cv::Mat image = cv::imread("D:/A/图像处理FCM/church01.jpg", 0);
	//SURF特征又称为加速鲁棒特征，
	//特征点向量
	std::vector<cv::KeyPoint>keypoints;
	//使用opencv的通用接口函数来，构造Fast特征检测器。这里要注意书上的创建方法发生了变化。detector = SURF::create( )
	cv::Ptr<SURF>SURF = SURF::create(2500.0);
	//检测surf特征
	SURF->detect(image, keypoints);
	//使用通用的绘制特征点函数，绘制图像
	cv::drawKeypoints(image, keypoints, image, cv::Scalar(255, 255, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);//标志位
	cv::imshow("SURF特征点检测", image);
	cv::waitKey(0);
	
}