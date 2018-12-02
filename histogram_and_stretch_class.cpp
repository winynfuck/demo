class Histogram1D {
	//计算单通道的灰度图的直方图
private:
	int histSize[1];//项的数量
	float hranges[2];//像素的最小最大值
	const float*ranges[1];
	int channels[1];//仅用到一个通道
public:
	Histogram1D() {
		//构造函数初始化参数
		histSize[0] = 256;
		hranges[0] = 0.0;
		hranges[1] = 255.0;
		ranges[0] = hranges;
		channels[0] = 0;//默认情况，我们考察0通道
	}
	cv::Mat getHistogram(const cv::Mat image) {
		cv::Mat hist;

		cv::calcHist(&image,
			1, //计算单张图像的直方图
			channels,  //通道数量
			cv::Mat(),//不使用图像作为掩码
			hist,//返回的直方图
			1,//这是1D的直方图
			histSize,//项的数量
			ranges //像素值的范围 
		);
		return hist;
	}

	cv::Mat getHistogramImage(const cv::Mat &image) {
		//首先计算直方图
		cv::Mat hist = getHistogram(image);
		//获取最大值和最小值
		double maxVal = 0;
		double minVal = 0;
		//minMaxLoc函数能获得数组或图像中的最大最小值。这里获得的是直方图中的项的数组的最大最小值。
		cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);
		//显示直方图的图像，新建一个直方图的图片，宽和高的大小为项的数量，数据类型为无符号8位整数，全部赋值为255.
		cv::Mat histImg(histSize[0], histSize[0], CV_8U, cv::Scalar(255));

		//设置最高点为nbins的90%
		int hpt = static_cast<int>(0.9*histSize[0]);
		//每个条目都绘制一条垂直线
		for (int h = 0; h < histSize[0]; h++) {
			//每一个项的数值，即属于这个灰度值的像素的个数。
			float binVal = hist.at<float>(h);
			//由属于这个灰度值的像素的个数求密度。
			int intensity = static_cast<int>(binVal*hpt / maxVal);
			//两点之间绘制一条线，第一个参数为要画线的图像，第二个第三个参数为画线的两点，第四个参数是线的颜色。注意图像的x轴y轴比较特别。向右是x轴，向下是y轴。
			cv::line(histImg, cv::Point(h, 0), cv::Point(h, histSize[0] - intensity), cv::Scalar::all(0));

		}
		return histImg;
	}
	cv::Mat applyLookup(const cv::Mat & image, const cv::Mat &lookup) {
		//查找表是1x256的uchar类型的matrix
		//输出图像
		cv::Mat dst;
		//应用查找标
		cv::LUT(image, lookup, dst);
		return dst;
	}
	cv::Mat stretch(const cv::Mat  &image, int minValue) {
		//首先计算直方图
		cv::Mat hist = getHistogram(image);
		//找出左端个数大于minValue的灰度值，0-255
		int imin=0;
		int min;
		for (; imin < histSize[0]; imin++) {
			int value = hist.at<float>(imin);
			if (value > minValue) {
				min = imin;
				break;
			}

		}
		//找出右端个数大于maxValue的灰度值，0-255
		int max;
		int imax = histSize[0] - 1;
		for (; imax >=0; imax--) {
			int value = hist.at<float>(imax);
			if (value > minValue) {
				max = imax;
				break;
			}

		}
		//构造一个查找表
		int dim(256);
		cv::Mat lookup(1, &dim, CV_8U);
		//给查找表赋值
		for (int i = 0; i < 256; i++) {
			//如果数值在min以下
			if (i < min) {
				lookup.at<uchar>(i) = 0;
			}
			//如果数值在max以上
			else if (i >max) {
				lookup.at<uchar>(i) = 0;
			}
			//如果数值在min和max之间
			else {
				lookup.at<uchar>(i) = static_cast<uchar>(255.0*(i - min) / (max - min) + 0.5);
			}
		}
		//应用查找表
		cv::Mat result;
		result = applyLookup(image, lookup);
		return result;

	}
};
