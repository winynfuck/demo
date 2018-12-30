#include<opencv2/opencv.hpp>
#include<opencv2/features2d.hpp>
#include<iostream>

#include<math.h>
#include<opencv2/calib3d.hpp>





using namespace std;


int main(int args, char** argv) {
	// Read input images
	cv::Mat image= cv::imread("D:/A/imgproFCM/chessboards/chessboard01.jpg", 0);
	//输出图像角点的vector
	std::vector<cv::Point2f>imageCorners;
	//棋牌中角点数目
	cv::Size boardSize(6, 4);
	//获取棋牌的角点
	bool found = cv::findChessboardCorners(image, boardSize, imageCorners);
	//绘制角点
	cv::drawChessboardCorners(image, boardSize, imageCorners, found);//已经找到的角点
	cv::imshow("找到的角点", image);
	cv::waitKey(0);

	

}