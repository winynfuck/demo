#include<opencv2/opencv.hpp>
#include<iostream>
#include<assert.h>
#include<stdlib.h>
#include<math.h>




#define PI 3.1415926

using namespace std;


int main(int args, char** argv) {
	// 打开另一幅图像
	cv::Mat image = cv::imread("D:/A/road.jpg");
	//轮廓图像和轮廓图像的阈值化处理结果
	cv::Mat contours;
	cv::Mat contoursInv;
	//用canny算子进行检测轮廓
	cv::Canny(image, contours, 125, 350);
	//对轮廓图像进行阈值化处理显示
	cv::threshold(contours, contoursInv, 128, 255, cv::THRESH_BINARY_INV);
	// 显示轮廓图像
	cv::namedWindow("Canny Contours",0);
	cv::imshow("Canny Contours", contoursInv);
	// 测试图像，可能是设置检测的线的方向的作用吧。
	cv::Mat test(200, 200, CV_8U, cv::Scalar(0));
	cv::line(test, cv::Point(100, 0), cv::Point(200, 200), cv::Scalar(255));
	cv::line(test, cv::Point(0, 50), cv::Point(200, 200), cv::Scalar(255));
	cv::line(test, cv::Point(0, 200), cv::Point(200, 0), cv::Scalar(255));
	cv::line(test, cv::Point(200, 0), cv::Point(0, 200), cv::Scalar(255));
	cv::line(test, cv::Point(100, 0), cv::Point(100, 200), cv::Scalar(255));
	cv::line(test, cv::Point(0, 100), cv::Point(200, 100), cv::Scalar(255));
	// 显示测试图像并保存测试图像
	cv::namedWindow("Test Image");
	cv::imshow("Test Image", test);
	cv::imwrite("test.bmp", test);

	// 进行霍夫变换
	std::vector<cv::Vec2f> lines;//霍夫变换的结果是(roh,theta)的二元组
	cv::HoughLines(contours, lines, 1, PI / 180, 60);

	// 在canny检测轮廓后的结果图像上显示霍夫变换检测得到的线
	cv::Mat result(contours.rows, contours.cols, CV_8U, cv::Scalar(255));
	image.copyTo(result);
	std::cout << "Lines detected: " << lines.size() << std::endl;

	std::vector<cv::Vec2f>::const_iterator it = lines.begin();
	while (it != lines.end()) {

		float rho = (*it)[0];   // first element is distance rho
		float theta = (*it)[1]; // second element is angle theta

		if (theta < PI / 4. || theta > 3.*PI / 4.) { // ~vertical line

			// point of intersection of the line with first row
			cv::Point pt1(rho / cos(theta), 0);
			// point of intersection of the line with last row
			cv::Point pt2((rho - result.rows*sin(theta)) / cos(theta), result.rows);
			// draw a white line
			cv::line(result, pt1, pt2, cv::Scalar(255), 1);

		}
		else { // ~horizontal line

		 // point of intersection of the line with first column
			cv::Point pt1(0, rho / sin(theta));
			// point of intersection of the line with last column
			cv::Point pt2(result.cols, (rho - result.cols*cos(theta)) / sin(theta));
			// draw a white line
			cv::line(result, pt1, pt2, cv::Scalar(255), 1);
		}

		std::cout << "line: (" << rho << "," << theta << ")\n";

		++it;
	}
	// 展示结果
	cv::namedWindow("Detected Lines with Hough");
	cv::imshow("Detected Lines with Hough", result);
	cv::waitKey(0);
	
	




}
