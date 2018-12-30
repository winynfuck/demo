main.cpp
#include<opencv2/opencv.hpp>
#include<opencv2/features2d.hpp>
#include<iostream>

#include<math.h>
#include<opencv2/calib3d.hpp>
#include"CameraCalibrator.h"





using namespace std;


int main(int args, char** argv) {
	std::vector<std::string> filelist;
	cv::Mat image;
	// 生成文件夹列表
	for (int i = 1; i <= 20; i++) {

		std::stringstream str;
		str << "D:/A/imgproFCM/chessboards/chessboard" << std::setw(2) << std::setfill('0') << i << ".jpg";
		std::cout << str.str() << std::endl;
		image = cv::imread(str.str(), 0);
		filelist.push_back(str.str());
	
	}

	//实例化
	CameraCalibrator calib(0,true);
	cv::Size boardSize(6, 4);
	calib.addChessboardPoints(filelist, boardSize);
	calib.calibrate(image.size());
	//图像去畸变
	image = cv::imread(filelist[6]);
	cv::Mat uImage = calib.remp(image);
	// display camera matrix
	cv::Mat cameraMatrix = calib.getCameraMatrix();
	std::cout << " Camera intrinsic: " << cameraMatrix.rows << "x" << cameraMatrix.cols << std::endl;
	std::cout << cameraMatrix.at<double>(0, 0) << " " << cameraMatrix.at<double>(0, 1) << " " << cameraMatrix.at<double>(0, 2) << std::endl;
	std::cout << cameraMatrix.at<double>(1, 0) << " " << cameraMatrix.at<double>(1, 1) << " " << cameraMatrix.at<double>(1, 2) << std::endl;
	std::cout << cameraMatrix.at<double>(2, 0) << " " << cameraMatrix.at<double>(2, 1) << " " << cameraMatrix.at<double>(2, 2) << std::endl;

	imshow("Original Image", image);
	imshow("Undistorted Image", uImage);

	cv::waitKey(0);
	return 0;
	

	

}
CameraCalibrator.h
#pragma once
#include<opencv2/opencv.hpp>
using namespace std;
class CameraCalibrator
{
private:
	//输入点
	//位于世界坐标的点
	std::vector<std::vector<cv::Point3f>>objectPoints;
	//像素坐标的点
	std::vector<std::vector<cv::Point2f>>imagePoints;
	//输出矩阵
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;
	//标定的方式
	int flag;
	//用于图像去畸变
	cv::Mat map1, map2;
	bool mustInitUndistort;
public:
	CameraCalibrator(int flag=0,bool mustInitUndistort=true );
	int addChessboardPoints(const std::vector<std::string>&flielist, cv::Size&boardSize);
	void addPoints(const std::vector<cv::Point2f>&imageCorners, const std::vector<cv::Point3f>&objectCorners);
	double calibrate(const cv::Size&imageSize);
	cv::Mat remp(const cv::Mat &image);
	 // Getters
    cv::Mat getCameraMatrix() { return cameraMatrix; }
    cv::Mat getDistCoeffs()   { return distCoeffs; }
};

CameraCalibrator.cpp
#include "CameraCalibrator.h"



CameraCalibrator::CameraCalibrator(int flag1, bool mustInitUndistort1)
{
	flag = flag1;
	mustInitUndistort = mustInitUndistort1;
}

//打开棋盘图像并提取角点
int CameraCalibrator::addChessboardPoints(const std::vector<std::string> & filelist, cv::Size&boardSize) {
	//棋盘上的点的两种坐标
	std::vector<cv::Point2f>imageCorners;
	std::vector<cv::Point3f>objectCorners;
	//3D场景中的点
	//在棋盘坐标系中初始化棋盘角点
	//这些点位于（x,y,z）=(i,j,0)
	for (int i = 0; i < boardSize.height; i++) {
		for (int j = 0; j < boardSize.width; j++) {
			objectCorners.push_back(cv::Point3f(i, j, 0.0f));
		}
	}
	//2D图像中的点
	cv::Mat image;//为了保存棋盘图像
	int successes = 0;
	//for all viewpoints
	for (int i = 0; i < filelist.size(); i++) {
		//打开图像
		image = cv::imread(filelist[i], 0);
		bool found = cv::findChessboardCorners(image, boardSize, imageCorners);

		//获取亚像素角点的精度
		cv::cornerSubPix(image, imageCorners, cv::Size(5, 5), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 30, 0.1));
		//如果角点数目满足条件，那么将它加入数据
		if (imageCorners.size() == boardSize.area()) {
			//添加一个视角中的图像点及场景点
			addPoints(imageCorners, objectCorners);
			successes++;
		}

	}
	return 1;

}
void CameraCalibrator::addPoints(const std::vector<cv::Point2f>&imageCorners, const std::vector<cv::Point3f>&objectCorners) {
	//添加场景点和对应的图像点
	//2D图像点
	imagePoints.push_back(imageCorners);
	//对应3D场景中的点
	objectPoints.push_back(objectCorners);

}
//进行标定，返回重投影误差(re-projection error)
double CameraCalibrator::calibrate(const cv::Size&imageSize) {
	//必须重新进行去畸变
	mustInitUndistort = true;
	//输出旋转和平移
	std::vector<cv::Mat >rvecs, tvecs;
	//开始标定
	return calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs, flag);

}
cv::Mat CameraCalibrator::remp(const cv::Mat &image) {
	cv::Mat undistorted;
	if (mustInitUndistort) {
		//每次标定只需要初始化一次
		cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(), cv::Mat(),image.size(), CV_32FC1, map1, map2);
		mustInitUndistort = false;
	}
	//应用映射函数
	cv::remap(image, undistorted, map1, map2, cv::INTER_LINEAR);
	return undistorted;

}
