#include<opencv2/opencv.hpp>
#include<iostream>
#include<assert.h>
#include<stdlib.h>
#include<math.h>
#include"lineFinder.h"



#define PI 3.1415926

using namespace std;


int main(int args, char** argv) {
	// 打开另一幅图像
	cv::Mat image = cv::imread("D:/A/road.jpg");
	cv::Mat contours;
	cv::Canny(image, contours, 125, 250);
	//创建一个lineFinder实例
	lineFinder finder;
	//设置概率霍夫变换的参数
	finder.setLineLengthAndGap(100, 20);
	finder.setMinVote(80);
	//检测并绘制直线
	std::vector<cv::Vec4i>lines = finder.findLines(contours);


	int n = 0;//选择line 0
	//黑色图像
	cv::Mat oneline(contours.size(), CV_8U, cv::Scalar(0));
	//白色直线
	cv::line(oneline, cv::Point(lines[n][0], lines[n][1]),
		cv::Point(lines[n][2], lines[n][3]), cv::Scalar(255), 5
	);
	//轮廓与白线进行and操作
	cv::bitwise_and(contours, oneline, oneline);
	std::vector<cv::Point>points;
	//遍历像素得到所有点的位置
	for (int y = 0; y < oneline.rows; y++) {
		//y行
		uchar*rowPtr = oneline.ptr<uchar>(y);
		for (int x = 0; x < oneline.cols; x++) {
			//x列
			//如果位于轮廓上
			if (rowPtr[x]) {
				points.push_back(cv::Point(x, y));
			}
		}
	}
	//简单调用cv::fitLine函数便可以找到最合适的线:
	cv::Vec4f line;
	cv::fitLine(cv::Mat(points), line,2, 0, 0.01, 0.01);
	//画出直线
	int x0 = line[2];//直线上的点
	int y0 = line[3];
	int x1 = x0 - 200 * line[0];//使用单元向量
	int y1 = y0 - 200 * line[1];//添加长度为200的向量
	//拟合椭圆
	/*cv::RotatedRect rrect = cv::fitEllipse(cv::Mat(points));
	cv::ellipse(image, rrect, cv::Scalar(0));*/
	// 展示结果
	cv::line(image, cv::Point(x0, y0), cv::Point(x1, y1), cv::Scalar(0), 3);
	cv::namedWindow("Detected Lines with Hough");
	cv::imshow("Detected Lines with Hough", image);
	cv::waitKey(0);






}