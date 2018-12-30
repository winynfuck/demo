#include<opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include<vector>
#include<math.h>
#include <iostream>
#include <string>
#include "hyper_FCM.h"
using namespace std;
using namespace cv;

hyper_FCM::hyper_FCM(string filename, int imageRow1, int imageCol1, int imageDim1, int m1, int maxIterationCount1, double  minError1, int  classNumber1) {
	//构造函数
	//读取数据,初始数据格式：行方向代表数据数，列方向代表光谱值等信息。
	FileStorage fsDemo(filename, FileStorage::READ);
	fsDemo["data"] >> data;
	//转置一下。行方向代表光谱值等信息，列方向代表数据数。
	cv::transpose(data, data);
	/*cout << data<<";"<< endl;*/

	//读取数据信息
	imageRow = imageRow1;
	imageCol = imageCol1;
	imageDim = imageDim1;

	//获取数据数和聚类中心数
	dataNumber = data.rows;
	classNumber = classNumber1;//分类类别数

	//设置FCM的几个初始参数，隶属度因子、最大迭代次数、目标函数的最小误差、分类类别数、隶属度矩阵
	m = m1;//隶属度因子
	maxIterationCount = maxIterationCount1;//最大迭代次数
	minError = minError1;//目标函数的最小误差
	
	//分配空间
	//聚类中心存放的是光谱值等信息
	center.create(classNumber1, imageDim, CV_32F);//聚类中心矩阵
	old_center.create(classNumber1, imageDim, CV_32F);//未更新的聚类中心矩阵

	//列和数组是初始化隶属度矩阵时候，用于归一化，保证隶属度矩阵列和为1.
	colsum = new double[dataNumber];//列和数组
	//函数值数组用于存放目标函数的值，用于停止迭代判断。
	value = new double[maxIterationCount];//函数值数组
	//隶属度矩阵初始化
	U = (double**)malloc(sizeof(double)*classNumber);
	for (int i = 0; i < classNumber; i++) {
		U[i] = (double*)malloc(sizeof(double)*dataNumber);
	}


}
void hyper_FCM::writeMatToYML(const cv::Mat &mat, const std::string ymlName) {
	cv::FileStorage fs(ymlName, cv::FileStorage::WRITE);
	fs << "Mat" << mat;
	fs.release();
}
void hyper_FCM::U_init() {
	//随机初始化隶属度矩阵
	for (int j = 0; j < dataNumber; j++) {
		colsum[j] = 0;//列和
		for (int i = 0; i < classNumber; i++) {
			U[i][j] = rand();
			colsum[j] += U[i][j];
		}
	}
	//隶属度矩阵归一化，使得一列的和为1，符合模糊C均值隶属度矩阵的要求：每个像素对于各个类的隶属度之和为1.
	for (int j = 0; j < dataNumber; j++) {
		for (int i = 0; i < classNumber; i++) {
			U[i][j] = U[i][j] / colsum[j];
			/*cout <<"初始化,"<<U[i][j] << endl;*/
		}
	}

}


double hyper_FCM::getDistance(cv::Mat m1, cv::Mat m2) {
	//获取两个一维矩阵之间的距离
	//判断两个矩阵的维度是否相同
	if (m1.rows != m2.rows || m1.cols != m2.cols) {
		cout << "计算距离的矩阵行列数不同" << endl;
	}
	double dist = 0;
	for (int i = 0; i < m1.rows; i++) {
		dist += pow(m1.at<float>(i, 0) - m2.at<float>(i, 0), 2);
	}
	return sqrt(dist);

}


void hyper_FCM::updateU() {
	//更新隶属度矩阵
	for (int i = 0; i < classNumber; i++) {
		for (int j = 0; j < dataNumber; j++) {
			//获取第i个和第j个数据
			cv::Mat mj = data.rowRange(j, j + 1).clone();
			cv::transpose(mj, mj);
			cv::Mat mi = data.rowRange(i, i + 1).clone();
			cv::transpose(mi, mi);
			//计算第i个数据和第j个数据的距离
			double dji = getDistance(mi, mj);
			double mole2 = 0;
			for (int k = 0; k < classNumber; k++) {
				//找出k类的聚类中心
				cv::Mat centerk = center.rowRange(k, k + 1).clone();
				//转置
				cv::Mat centerk_trans = centerk.t();

				double djk = getDistance(mj, centerk_trans);
			    mole2 += pow(dji/djk, 2.0/(m - 1));
				
			}
			U[i][j] = 1/mole2;
			if (U[i][j] > 1) {
				U[i][j] = 1;
			}

		}
	}
}


void hyper_FCM::updateC() {
	//求聚类中心

	for (int i = 0; i < classNumber; i++) {
		cv::Mat molecule,temp;//初始化分子
		molecule.create(imageDim, 1, CV_32F);
		molecule = Scalar(0);
		double denominator = 0;//分母

		for (int j = 0; j < dataNumber; j++) {
			//找出第j个数据
			cv::Mat mj = data.rowRange(j, j + 1).clone();
			cv::transpose(mj, mj);

			denominator += pow(U[i][j], m);
			multiply(mj, pow(U[i][j], m), temp);
			molecule += temp;

		}
	
		cv::multiply(molecule, 1/denominator, molecule);
		for (int k = 0; k < imageDim; k++) {
			center.at<float>(i, k) = molecule.at<float>(k, 0);
		}
		
		
	}
}

double hyper_FCM::calcTargetValue() {
	//计算目标函数值
	double sumValue = 0;
	for (int j = 0; j < dataNumber; j++) {
		for (int i = 0; i < classNumber; i++) {
			//找出第j个数据
			cv::Mat mj = data.rowRange(j, j + 1).clone();
			cv::transpose(mj, mj);
			//找出第i个聚类中心
			cv::Mat centeri = center.rowRange(i, i + 1).clone();
			cv::transpose(centeri, centeri);
			
			sumValue += pow(U[i][j], m)*getDistance(mj, centeri);

		}
	}
	return sumValue;





}
void hyper_FCM::process() {
	for (int t = 1; t < maxIterationCount; t++) {

		if (t == 1)
		{	//第一次迭代，初始化隶属度。常规方法初始化，需要迭代次数没有优化。
			U_init();

		}
		else
		{	//将上一次隶属度矩阵保存下来。
			for (int i = 0; i < center.rows; i++) {
				for (int j = 0; j < center.cols; j++) {
					old_center.at<float>(i, j) = center.at<float>(i, j);
				}
			}
			//非第一次迭代，更新隶属度矩阵
			updateU();//如果不是第一次迭代，则根据上一次得到的聚类中心，更新隶属度矩阵
		}
		
	/*	if (t == 2) {
			for (int i = 0; i < classNumber; i++) {
				for (int j = 0; j < dataNumber; j++) {
					cout << U[i][j] << endl;
				}
			}

		}*/
		//更新聚类中心
		updateC();


		for (int i = 0; i < classNumber; i++) {
			cv::Mat c = center.rowRange(i, i + 1).clone();
			cout << "第" << i + 1 << "个聚类中心：" << c << ";" << "\n" << endl;
		}

		//根据当前的隶属度矩阵U和当前的聚类中心C,计算目标函数的值
		double valueSum = calcTargetValue();
		std::cout << "第" << t << "次迭代" << ",目标函数值:" << valueSum << "\n" << endl;
		value[t] = valueSum;
		if (t >= 2 && (abs(value[t] - value[t - 1]) < minError)) {
			cout << "迭代结束\n" << endl;
			break;
		}

		//用聚类中心来判断迭代停止
		bool breaking = false;
		for (int i = 0; i < classNumber; i++) {
			cv::Mat centeri = center.rowRange(i, i + 1).clone();
			cv::Mat old_centeri = old_center.rowRange(i, i + 1).clone();
			double d = getDistance(centeri, old_centeri);
			if (d > minError) {
				break;//不改变breaking的值，继续迭代。
			}
			else {
				breaking = true;

			}
		}
		//如果符合停止条件，则停止迭代
		if (breaking == true) {
			cout << "迭代结束\n" << endl;
			break;
		}



	}

}
cv::Mat hyper_FCM::deBlur() {
	//去模糊，将图像中每一个像素点的灰度值赋予相应聚类中心的灰度值。
	//定义一个colormap
	int colormap1[10] = { 128,0,128,255,255,0,0,128,255,255 };
	int colormap2[10] = { 128,0,255,128,255,64,255,255,255,0 };
	int colormap3[10] = { 255,0,255,0,128,0,255,0,255,255 };
	for (int j = 0; j < dataNumber; j++) {
		double colmax = 0.0;
		int colmax_i = 0;
		//对于每一个数据，找出隶属度矩阵中一列最大的隶属度所属于的类。
		for (int i = 0; i < classNumber; i++) {
			if (U[i][j] > colmax) {
				colmax = U[i][j];
				colmax_i = i;

			}


		}
		//找到数据真实的行数和列数。
		if (j%imageCol == 0) {
			int real_row = j / imageCol - 1;
		}
		int real_row = j / imageCol;
		int real_col = (j - real_row * imageCol);
		//对于图像，每个数据，不同的类赋予不同的颜色
		/*result.create(imageRow, imageCol, CV_8U);*/
		result.create(imageRow,imageCol, CV_8UC3);
	
		//输出标签图像
		/*result.at<uchar>(real_row, real_col) = colmax_i;*/
		/*if (colmax_i == 5) {
			result.at<uchar>(real_row, real_col) = 255;
		}
		else
		{
			result.at<uchar>(real_row, real_col) = 0;
		}*/
			result.at<Vec3b>(real_row, real_col)[0] = colormap1[colmax_i];
			result.at<Vec3b>(real_row, real_col)[1] = colormap2[colmax_i];
			result.at<Vec3b>(real_row, real_col)[2]= colormap3[colmax_i];

	}


	return	result;

}
cv::Mat hyper_FCM::imageSegmentation() {
	//模糊c均值聚类处理
	process();
	//去模糊
	cv::Mat result = deBlur();
	return result;
}
cv::Mat hyper_FCM::getU() {
	cv::Mat U1(classNumber, dataNumber, CV_32F);
	for (int j = 0; j < dataNumber; j++) {
		for (int i = 0; i < classNumber; i++) {
			U1.at<uchar>(i,j) = U[i][j];
		}
	}
	return	U1;
}

