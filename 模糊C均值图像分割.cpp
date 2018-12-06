#pragma once
#include<opencv2/opencv.hpp>
#include<math.h>

using namespace std;
class FCM {
private:
	cv::Mat image;//图像
	cv::Mat result;//输出图像
	int m;//隶属度因子
	int maxIterationCount;//最大迭代次数
	double minError;//相邻两次迭代最小误差
	int classNumber;//分类类别数
	int dataNumber;//数据数

	double **U;//隶属度矩阵	
	double **old_U;//隶属度矩阵	
	int *data;//数据数组
	double  *center;//类别中心数组
	double  *value;//目标函数值数组
	double *colsum;
	


public:

	FCM(string filename, int m1 = 2, int maxIterationCount1 = 200, double  minError1 = 1e-5, int  classNumber1 = 4) {
		//构造函数
		//以灰度图的方式读取图片
		image = cv::imread(filename, 0);
		dataNumber = image.rows*image.cols;
		//设置FCM的几个初始参数，隶属度因子、最大迭代次数、目标函数的最小误差、分类类别数、隶属度矩阵
		m = m1;//隶属度因子
		maxIterationCount = maxIterationCount1;//最大迭代次数
		minError = minError1;//目标函数的最小误差
		classNumber = classNumber1;//分类类别数
		//要想把图像数据拉成一列进行计算，就得有一个数据数组。
		data = (int*)malloc(dataNumber * sizeof(int));//数据数组
		//聚类中心存放的也是像素值。要有一个聚类中心数组。
		center= new double[classNumber];//聚类中心数组
		//列和数组是初始化隶属度矩阵时候，用于归一化，保证隶属度矩阵列和为1.
		colsum = new double[dataNumber];//列和数组
		//函数值数组用于存放目标函数的值，用于停止迭代判断。
		value = new double[maxIterationCount];//函数值数组

		//隶属度矩阵初始化
		U = (double**)malloc(sizeof(double)*classNumber);
		for (int i = 0; i < classNumber; i++) {
			U[i] = (double*)malloc(sizeof(double)*dataNumber);
		}
		//旧的隶属度矩阵初始化，这一部分是调试需要
		old_U = (double**)malloc(sizeof(double)*classNumber);
		for (int i = 0; i < classNumber; i++) {
			old_U[i] = (double*)malloc(sizeof(double)*dataNumber);
		}
	
	}
	


	void preprocess() {
		//把图像数据拉成一列
		for (int i = 0; i < image.rows; i++) {
			for (int j = 0; j < image.cols; j++) {
				int a = image.ptr<uchar>(i)[j];
				data[i*image.cols + j] = image.ptr<uchar>(i)[j];
				/*cout << data[i*image.cols + j] << "\n" << endl;*/
				
			}
		}
		/*for (int i = 0; i < dataNumber; i++) {
			cout << data[i] <<"\n"<<endl;
		}*/
		
	}
	cv::Mat deBlur() {
		//去模糊，将图像中每一个像素点的灰度值赋予相应聚类中心的灰度值。
		for (int j = 0; j < dataNumber; j++) {
			float colmax = 0.0;
			int colmax_i=0;
			//对于每一个数据，找出隶属度矩阵中一列最大的隶属度所属于的类。
			for (int i = 0; i < classNumber; i++) {
				if (U[i][j]> colmax) {
					colmax = U[i][j];
					colmax_i = i;

				}


			}
			//找到数据真实的行数和列数。
			if (j%image.cols == 0) {
				int real_row = j / image.cols - 1;
			}
			int real_row = j / image.cols;
			int real_col = (j-real_row*image.cols);
			//对于图像，每个数据，不同的类赋予不同的颜色
			result.create(image.size(), image.type());
			result.at<uchar>(real_row, real_col) = int(center[colmax_i]);
		
		}
		return result;

	}
	float calcTargetValue() {
		//计算目标函数值
		float  valueSum = 0;
		/*for (int i = 0; i < classNumber; i++) {
			for (int j = 0; j < dataNumber; j++) {
				cout << U[i][j] << "\n" << endl;
			}
		}*/
		
		for (int i = 0; i < classNumber; i++) {

			for (int j = 0; j < dataNumber; j++) {
				//这里的距离用的是欧式距离
				valueSum += pow(U[i][j],m) * pow(abs(data[j] - center[i]), 2);

			}

		}
		return valueSum;

	}
	void U_init() {
		//随机初始化隶属度矩阵
		for (int j = 0; j < dataNumber; j++) {
			colsum[j] = 0;//列和
			for (int i = 0; i < classNumber; i++) {
				U[i][j] = rand();
				colsum[j] += U[i][j];
			}
		}
		//归一化
		for (int j = 0; j < dataNumber; j++) {
			for (int i = 0; i < classNumber; i++) {
				U[i][j] = U[i][j] / colsum[j];
				/*cout <<"初始化,"<<U[i][j] << endl;*/
			}
		}
		//检验列和是否为1，事实上不为1，可能因为除法的误差，每一项的误差为e-16，所以可以近似看作列和为1
		/*for (int j = 0; j < dataNumber; j++) {
			double colsum2 = 0;
			for (int i = 0; i < classNumber; i++) {
				colsum2 += U[i][j];

			}
			if (colsum2 != 1) {
				cout << colsum2-1 << "\n" << endl;
			}
		}*/
	}
	void updateU() {
		//更新隶属度矩阵
		for (int i = 0; i < classNumber; i++) {

			for (int j = 0; j < dataNumber; j++) {
				old_U[i][j] = U[i][j];
				double  dij = 0;//数据j与聚类中心i之间的距离
				double  dkj = 0;//数据j与聚类中心k之间的距离
				double sum = 0;//方便计算公式中的倒数
				dij = abs(data[j] - center[i]);
				/*cout << j << "个数据" << dij << "\n" << endl;*/
				if (dij == 0) {
					//如果j个数据与i个聚类中心距离为0，则不改变隶属度。
					U[i][j] = old_U[i][j];
					break;
				}
				for (int k = 0; k < classNumber; k++) {
					dkj = sqrt(pow((data[j] - center[k]), 2));
					/*cout << j << "个数据" << dkj << "\n" << endl;*/
					if (dkj == 0) {
						sum += 0;
					}
					else {
						sum += pow((dij / dkj), 2 * (m - 1));
					}/*cout << j << "个数据" << sum << "\n" << endl;*/
				}
				U[i][j] = 1 / sum;
				/*cout <<","<<U[i][j]-old_U[i][j] << endl;*/
			}
		}
	}
	void updateC() {
		//求聚类中心
		for (int i = 0; i < classNumber; i++) {
			double sum_uijm = 0;//分母
			double sum_uijmxj = 0;//分子
			for (int j = 0; j < dataNumber; j++) {
				sum_uijm += pow(U[i][j], m);
				sum_uijmxj += pow(U[i][j], m)*data[j];
			}
			center[i] = sum_uijmxj / sum_uijm;
		}

	}
	
	void process() {
		for (int t = 1; t < maxIterationCount; t++) {
			//第一次迭代，
			if(t == 1) 
			{
				U_init();
				
			}
			else 
			{
				updateU();//如果不是第一次迭代，则根据上一次得到的聚类中心，更新隶属度矩阵
			}
			//更新聚类中心
			updateC();
			/*for (int i = 0; i < classNumber; i++) {
				cout << "第"<<t<<"次聚类中心："<<center[i] << "\n" << endl;
			}*/

			//根据当前的隶属度矩阵U和当前的聚类中心C,计算目标函数的值
			double valueSum = calcTargetValue();
			std::cout << "第" << t << "次迭代" << ",目标函数值:" << valueSum<<"\n"<<endl;
			value[t] = valueSum;
			if(t>=2 && (abs(value[t] - value[t - 1]) < minError)) {
				cout << "迭代结束\n" << endl;
				break;
			}
			

		}
		
	}
	cv::Mat imageSegmentation() {
		//预处理,得到数据数组
		preprocess();
		//模糊c均值聚类
		process();
		//去模糊
		result = deBlur();
		return result;
	}
};
#include<opencv2/opencv.hpp>
#include<iostream>
#include"fcm.h"

using namespace std;
int main() {
	FCM f("D:\\A\\car.jpg",2,200,1e-5,4);
	cv::Mat result =f.imageSegmentation();
	cv::namedWindow("fcm", 0);
	cv::imshow("fcm", result);
	cv::waitKey(0);
	

}