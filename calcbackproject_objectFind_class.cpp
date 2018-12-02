class ObjectFinder {
	 //使用反投影直方图法检测特定的图像内容

private:

	float hranges[2];//最大最小值
	const float* ranges[3];//每个通道的范围，由最大最小值求得
	int channels[3];

	float threshold;//设置的阈值，用于最后结果的二值化
	cv::MatND histogram;//感兴趣区域的直方图数组
	cv::SparseMat shistogram;//稀疏矩阵
	bool isSparse;//判断是否为稀疏矩阵

public:

	ObjectFinder() : threshold(0.1f), isSparse(false) {

		ranges[0] = hranges; // 所有通道设置为相同的最大最小值
		ranges[1] = hranges;
		ranges[2] = hranges;
	}

	// 设置阈值为[0,1]
	void setThreshold(float t) {

		threshold = t;
	}

	//获取阈值
	float getThreshold() {

		return threshold;
	}

	//设置感兴趣区域的直方图并均一化
	void setHistogram(const cv::MatND& h) {

		isSparse = false;
		histogram = h;
		cv::normalize(histogram, histogram, 1.0);
	}

	//稀疏矩阵的情况
	void setHistogram(const cv::SparseMat& h) {

		isSparse = true;
		shistogram = h;
		cv::normalize(shistogram, shistogram, 1.0, cv::NORM_L2);
	}

	// 识别的主要方法一
	cv::Mat find(const cv::Mat& image) {

		cv::Mat result;

		hranges[0] = 0.0;	// range [0,255]
		hranges[1] = 255.0;
		channels[0] = 0;		// 三通道
		channels[1] = 1;
		channels[2] = 2;
		 //稀疏矩阵使用反投影直方图得到结果图像
		if (isSparse) { // call the right function based on histogram type

			cv::calcBackProject(&image,
				1,            // one image
				channels,     // vector specifying what histogram dimensions belong to what image channels
				shistogram,   // the histogram we are using
				result,       // the resulting back projection image
				ranges,       // the range of values, for each dimension
				255.0         // the scaling factor is chosen such that a histogram value of 1 maps to 255
			);

		}
		else {
			//非稀疏矩阵使用反投影直方图得到结果图像
			cv::calcBackProject(&image,
				1,            // one image
				channels,     // vector specifying what histogram dimensions belong to what image channels
				histogram,    // the histogram we are using
				result,       // the resulting back projection image
				ranges,       // the range of values, for each dimension
				255.0         // the scaling factor is chosen such that a histogram value of 1 maps to 255
			);
		}


		// 二值化
		if (threshold > 0.0)
			cv::threshold(result, result, 255 * threshold, 255, cv::THRESH_BINARY);

		return result;
	}
	 //主要识别方法二

	cv::Mat find(const cv::Mat& image, float minValue, float maxValue, int *channels, int dim) {

		cv::Mat result;

		hranges[0] = minValue;
		hranges[1] = maxValue;

		for (int i = 0; i < dim; i++)
			this->channels[i] = channels[i];

		if (isSparse) { // call the right function based on histogram type

			cv::calcBackProject(&image,
				1,            // we only use one image at a time
				channels,     // vector specifying what histogram dimensions belong to what image channels
				shistogram,   // the histogram we are using
				result,       // the resulting back projection image
				ranges,       // the range of values, for each dimension
				255.0         // the scaling factor is chosen such that a histogram value of 1 maps to 255
			);

		}
		else {

			cv::calcBackProject(&image,
				1,            // we only use one image at a time
				channels,     // vector specifying what histogram dimensions belong to what image channels
				histogram,    // the histogram we are using
				result,       // the resulting back projection image
				ranges,       // the range of values, for each dimension
				255.0         // the scaling factor is chosen such that a histogram value of 1 maps to 255
			);
		}

		// Threshold back projection to obtain a binary image
		if (threshold > 0.0)
			cv::threshold(result, result, 255 * threshold, 255, cv::THRESH_BINARY);

		return result;
	}

};
