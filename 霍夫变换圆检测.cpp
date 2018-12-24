#include<opencv2/opencv.hpp>
#include<iostream>
#include<assert.h>
#include<stdlib.h>
#include<math.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "linefinder.h"

#include"lineFinder.h"


using namespace std;


int main(int args, char** argv) {

	// 打开另一幅图像
	cv::Mat image = cv::imread("D:/A/chariot.jpg",0);
	//霍夫变换圆检测
	//在进行霍夫算法圆检测之前，最好先进行图像平滑，可以减少可能引起误检测的图像噪声
	cv::GaussianBlur(image, image, cv::Size(5, 5), 1.5);
	std::vector<cv::Vec3f> circles;
	//第三个参数是累加器的分辨率，第四个是两个圆之间的最小距离，第五个是canny高阈值，第六个是最小投票数，最后俩个是极小极大半径
	cv::HoughCircles(image, circles, CV_HOUGH_GRADIENT,
		2,   // accumulator resolution (size of the image / 2) 
		50,  // minimum distance between two circles
		200, // Canny high threshold 
		100, // minimum number of votes 
		25, 100); // min and max radius

	std::cout << "Circles: " << circles.size() << std::endl;

	// Draw the circles
	image = cv::imread("../chariot.jpg", 0);
	std::vector<cv::Vec3f>::const_iterator itc = circles.begin();

	while (itc != circles.end()) {

		cv::circle(image,
			cv::Point((*itc)[0], (*itc)[1]), // circle centre
			(*itc)[2], // circle radius
			cv::Scalar(255), // color 
			2); // thickness

		++itc;
	}

	cv::namedWindow("Detected Circles");
	cv::imshow("Detected Circles", image);

	cv::waitKey();

	
	




}
