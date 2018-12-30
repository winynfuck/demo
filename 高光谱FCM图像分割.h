#pragma once
#include<opencv2/opencv.hpp>
using namespace std;
class hyper_FCM
{
private:

	cv::Mat data;//输入的多维矩阵
	cv::Mat result;//输出的分割图像
	int imageRow;//输入矩阵的行数
	int imageCol;//输入矩阵的列数
	int imageDim;
	int m;//隶属度因子

	int maxIterationCount;//最大迭代次数
	double minError;//相邻两次迭代最小误差
	int classNumber;//分类类别数
	int dataNumber;//数据数
	double *centersum;
	double **U;//隶属度矩阵	
	cv::Mat  center;//类别中心数组
	cv::Mat  old_center;//存储为更新的聚类中心数组
	double  *value;//目标函数值数组
	double *colsum;//隶属度矩阵列和数组

public:
	hyper_FCM(string filename, int imageRow, int imageCol, int imageDim, int m = 2, int maxIterationCount = 200, double  minError = 1e-5, int  classNumber = 10);
	void U_init();
	//获取数列矩阵中的一行，将数据放到向量中去。
	double getDistance(cv::Mat m1, cv::Mat m2);
	void updateU();
	void updateC();
	double calcTargetValue();
	void process();
	cv::Mat deBlur();
	cv::Mat imageSegmentation();
	void writeMatToYML(const cv::Mat &mat, const std::string xmlName);
	cv::Mat getU();
};
