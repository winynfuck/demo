#pragma once
#include<opencv2/opencv.hpp>
class laplacianZC {
private:
	//原图
	cv::Mat img;
	//包含laplacian的32位浮点图像
	cv::Mat laplace;
	//laplacian卷积核的大小
	int aperture;
public:
	laplacianZC() :aperture(3) {

	}
	//设置卷积核的大小
	void setAperture(int a) {
		aperture = a;
	}
	int getAperture() {
		return aperture;
	}
	//计算浮点数laplacian
	cv::Mat computerLaplacian(const cv::Mat &image) {
		//计算laplacian
		cv::Laplacian(image, laplace, CV_32F, aperture);
		//保留图像的局部备份
		img = image.clone();
		return laplace;
	}
	cv::Mat getLaplacianImage(double scale = -1.0) {
		if (scale < 0) {
			double lapmin, lapmax;
			cv::minMaxLoc(laplace, &lapmin, &lapmax);
			scale = 127 / std::max(-lapmin, lapmax);
		}
		cv::Mat laplaceImage;
		laplace.convertTo(laplaceImage, CV_8U, scale, 128);
		return laplaceImage;
	}
	//得到零点交叉的二值图像
	//如果相邻像素的乘积小于threshold
	//那么零点交叉将被忽略
	cv::Mat getZeroCrossings(float threshold = 1.0) {

		// Create the iterators
		cv::Mat_<float>::const_iterator it = laplace.begin<float>() + laplace.step1();
		cv::Mat_<float>::const_iterator itend = laplace.end<float>();
		cv::Mat_<float>::const_iterator itup = laplace.begin<float>();

		// Binary image initialize to white
		cv::Mat binary(laplace.size(), CV_8U, cv::Scalar(255));
		cv::Mat_<uchar>::iterator itout = binary.begin<uchar>() + binary.step1();

		// negate the input threshold value
		threshold *= -1.0;

		for (; it != itend; ++it, ++itup, ++itout) {

			// if the product of two adjascent pixel is negative
			// then there is a sign change
			if (*it * *(it - 1) < threshold)
				*itout = 0; // horizontal zero-crossing
			else if (*it * *itup < threshold)
				*itout = 0; // vertical zero-crossing
		}

		return binary;
	}
	


};