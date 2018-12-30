#include<opencv2/opencv.hpp>
#include<iostream>
#include "hyper_FCM.h"
using namespace std;

int main() {
	hyper_FCM hyperFCM("D:\\A\\imgproFCM\\file\\data.yml", 610,340,3, 2, 10, 1e-5, 10);
	cv::Mat result = hyperFCM.imageSegmentation();
	cv::Mat U_result = hyperFCM.getU();
	cv::Mat U;
	cv::transpose(U_result,U);
	/*cout<< U<<";" << endl;*/
	hyperFCM.writeMatToYML(result, "D:/A/imgproFCM/file/hyper_fcm.yml");
	hyperFCM.writeMatToYML(U_result, "D:/A/imgproFCM/file/U_result.yml");
	cv::namedWindow("hyper_FCM", 1);
	/*cout << result << ";" << endl;*/
	cv::imshow("hyper_FCM", result);
	cv::waitKey(0);

}