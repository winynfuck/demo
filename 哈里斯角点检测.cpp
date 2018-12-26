#include<opencv2/opencv.hpp>
#include<iostream>
#include<assert.h>
#include<stdlib.h>
#include<math.h>
#include"lineFinder.h"
#include"calcbackProject.h"
#include"Histogram1D.h"


#define PI 3.1415926

using namespace std;


int main(int args, char** argv) {


	
	// Read input image
	cv::Mat image = cv::imread("D:/A/图像处理FCM/church01.jpg", 0);
	if (!image.data)
		return 0;

	// Display the image
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);

	// Detect Harris Corners
	cv::Mat cornerStrength;
	cv::cornerHarris(image, cornerStrength,
		3,     // neighborhood size
		3,     // aperture size
		0.01); // Harris parameter

// threshold the corner strengths
	cv::Mat harrisCorners;
	double threshold = 0.0001;
	cv::threshold(cornerStrength, harrisCorners,
		threshold, 255, cv::THRESH_BINARY_INV);
	//由于cornerStrength和harrisCorners都是cv_32f类型的图像，在imshow()中不能显示，因此需要把它转换为cv_8u的图像以便显示。
	harrisCorners.convertTo(harrisCorners, CV_8U);
	// Display the corners
	cv::namedWindow("Harris Corner Map");
	cv::imshow("Harris Corner Map", harrisCorners);
	cv::waitKey(0);





}