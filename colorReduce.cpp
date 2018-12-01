#pragma once
#include<opencv2/opencv.hpp>
void colorReduce1(cv::Mat &image, int div = 64) {
	int nl = image.rows;
	int nc = image.cols;
	for (int j = 0; j < nl; j++) {
		for (int i = 0; i < nc; i++) {
			//process each pixel
			image.at<cv::Vec3b>(j, i)[0] = image.at<cv::Vec3b>(j, i)[0] / div * div + div / 2;
			image.at<cv::Vec3b>(j, i)[1] = image.at<cv::Vec3b>(j, i)[1] / div * div + div / 2;
			image.at<cv::Vec3b>(j, i)[2] = image.at<cv::Vec3b>(j, i)[2] / div * div + div / 2;
		}//end of pixel processing--------------


	}//end of line
}
void colorReduce2(cv::Mat &image, int div = 64) {
	int nl = image.rows;//行数
	//每行的元素个数
	int nc = image.cols *image.channels();
	for (int j = 0; j < nl; j++) {
		//得到第j行的首地址
		uchar *data = image.ptr<uchar>(j);
		for (int i = 0; i < nc; i++) {
			//处理每一个像素，以达到量化的目的
			data[i] = data[i] / div * div + div / 2;
		}//像素处理完成
	}//行处理完成

}
void colorReduce3(cv::Mat &image, cv::Mat &redult, int div = 64) {
	int nl = image.rows;
	int ns = image.cols *image.channels();
	if (image.isContinuous()) {
		//没有额外的填补像素
		ns = ns * nl;
		nl = 1;
		//或用reshape()来改变图像的宽和高
	}
	for (int j = 0; j < nl; j++) {
		uchar *data = image.ptr<uchar>(j);
		for (int i = 0; i < ns; i++) {
			data[i] = data[i] / div * div + div / 2;
			//像素处理完成

		}//行处理完成
	}
}

void colorReduce4(cv::Mat &image, int div = 64) {
	//得到开始位置的迭代器
	cv::Mat_ <cv::Vec3b>::iterator it = image.begin <cv::Vec3b>();
	//得到终止位置的迭代器
	cv::Mat_ <cv::Vec3b>::iterator itend = image.end <cv::Vec3b>();
	//遍历所有像素
	for (; it != itend; ++it) {
		//处理每个像素
		(*it)[0] = (*it)[0] / div * div + div / 2;
		(*it)[1] = (*it)[1] / div * div + div / 2;
		(*it)[2] = (*it)[2] / div * div + div / 2;
		//处理像素完成-----------
	}


}
void colorReduce5(cv::Mat &image, int div = 64) {
	int n = 0;
	while (1)
		if (div >>= 1) n++;
		else break;

	//用来对像素 取整的掩模
	uchar mask = 0xFF << n;

	int nl = image.rows;//行数
	//每行的元素个数
	int nc = image.cols *image.channels();
	for (int j = 0; j < nl; j++) {
		//得到第j行的首地址
		uchar *data = image.ptr<uchar>(j);
		for (int i = 0; i < nc; i++) {
			//处理每一个像素，以达到量化的目的
			data[i] = (data[i] & mask) + div / 2;
		}//像素处理完成
	}//行处理完成


}
void salt(cv::Mat &image, int n) {
	for (int k = 0; k < n; k++) {
		//rand()是随机数生成函数
		int i = rand() % image.cols;
		int j = rand() % image.rows;
		if (image.channels() == 1) {
			//灰度图
			image.at<uchar>(j, i) = 255;
		}
		else if (image.channels() == 3) {
			//彩色图
			image.at<cv::Vec3b>(j, i)[0] = 255;
			image.at<cv::Vec3b>(j, i)[1] = 255;
			image.at<cv::Vec3b>(j, i)[2] = 255;
		}

	}
}
