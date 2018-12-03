#pragma once
#include<opencv2/opencv.hpp>

class morphoFeatures {
private:
	//用于生成二值图像的阈值
	int threshold;
	//角点检测的结构元素

	cv::Mat cross;
	cv::Mat diamond;
	cv::Mat square;
	cv::Mat x;
public:
	morphoFeatures():threshold(-1),cross(5,5,CV_8U,cv::Scalar(1)),diamond(5,5,CV_8U,cv::Scalar(1)),square(5,5,CV_8U,cv::Scalar(1)),x(5,5,CV_8U,cv::Scalar(0)){
		//创建十字形元素
		for (int i = 0; i < 5; i++) {
			cross.at<uchar>(2, i) = 1;
			cross.at<uchar>(i, 2) = 1;
		}
		//创建菱形元素
		diamond.at<uchar>(0, 0) = 0;
		diamond.at<uchar>(0, 1) = 0;
		diamond.at<uchar>(1, 0) = 0;
		diamond.at<uchar>(4, 4) = 0;
		diamond.at<uchar>(3, 4) = 0;
		diamond.at<uchar>(4, 3) = 0;
		diamond.at<uchar>(4, 3) = 0;
		diamond.at<uchar>(4, 0) = 0;
		diamond.at<uchar>(4, 1) = 0;
		diamond.at<uchar>(3, 0) = 0;
		diamond.at<uchar>(0, 4) = 0;
		diamond.at<uchar>(0, 3) = 0;
		diamond.at<uchar>(1, 4) = 0;
		//创建x型元素
		for (int i = 0; i < 5; i++) {
			x.at<uchar>(i, i) = 1;
			x.at<uchar>(4 - i, i) = 1;
		}
		


	}
	void setThreshold(int  t) {
		threshold = t;
	}
	int getThreshold() {
		return threshold;
	}
	void drawOnImage(const cv::Mat binary, cv::Mat &image) {
		cv::Mat_<uchar>::const_iterator it = binary.begin<uchar>();
		cv::Mat_<uchar>::const_iterator itend = binary.end<uchar>();
		//遍历每个像素
		for (int i = 0; it != itend; ++it, ++i) {
			if (!*it) {
				cv::circle(image, cv::Point(i%image.step, i / image.step), 5, cv::Scalar(255, 0, 0));
			}
		}
		

	}
	cv::Mat getEdges(const cv::Mat &image) {
		//得到梯度图
		cv::Mat result;
		cv::morphologyEx(image, result, cv::MORPH_GRADIENT, cv::Mat());
		//阈值化
		cv::threshold(result, result, threshold, 255, 1);
		return result;
	}
	cv::Mat getCorners(const cv::Mat &image) {
		//角点检测要接连使用一系列的结构元素
		cv::Mat result;
		//十字形膨胀
		cv::dilate(image, result, cross);
		//菱形腐蚀
		cv::erode(result, result, diamond);
		cv::Mat result2;
		//x形膨胀
		cv::dilate(result,result2, x);
		//方形腐蚀
		cv::erode(result2, result2, square);
		//通过对两张图像做差得到角点图像
		cv::absdiff(result2, result, result);
		//阈值化得到二值图像
		cv::threshold(result, result, 10, 255, 1);
		return result;

	}
};
int main(int args, char** argv) {
	//------------------------------对参考图像的处理
	//读取參考图像
	cv::Mat image = cv::imread("D:/A/building.jpg",0);
	cv::Mat corners;
	morphoFeatures m;
	corners = m.getCorners(image);
	//在图像中显示角点
	m.drawOnImage(corners, image);
	cv::imshow("corners on image", image);
	cv::waitKey(0);

}
