class ColorDetector {
private:
	//最小可接受的距离
	int minDist;
	//目标色
	cv::Vec3b target;
	//结果图像
	cv::Mat result;
	//包含转换后的图像
	cv::Mat converted;
public:
	ColorDetector() : minDist(100) {

		//构造函数
		target[0] = target[1] = target[2] = 0;
	}

	int getDistance(const cv::Vec3b& color) const {
		// return static_cast<int>(cv::norm<int,3>(cv::Vec3i(color[0]-target[0],color[1]-target[1],color[2]-target[2])));
		return abs(color[0] - target[0]) +
			abs(color[1] - target[1]) +
			abs(color[2] - target[2]);
	}
	void setColorDistanceThreshold(int distance) {
		if (distance < 0) {
			distance = 0;
		}
		minDist = distance;
	}
	int getColorDistanceThreshold() const {
		return minDist;
	}
	void setTargetColor(unsigned char red, unsigned char green, unsigned char blue) {
		//转换目标色到Lab颜色空间中去
		cv::Mat tmp(1, 1, CV_8UC3);
		tmp.at<cv::Vec3b>(0, 0)[0] = blue;
		tmp.at<cv::Vec3b>(0, 0)[1] = green;
		tmp.at<cv::Vec3b>(0, 0)[2] = red;
		cv::cvtColor(tmp, tmp, 44);
		target = tmp.at<cv::Vec3b>(0, 0);
	}
	
	cv::Vec3b getTargetColor() const {
		return target;
	}
	cv::Mat process(const cv::Mat &image) {
		//输出图像
		result.create(image.rows, image.cols, CV_8U);
		//按需重新分配中间图像

		converted.create(image.rows, image.cols, image.type());
		//转换到Lab颜色空间。Lab颜色空间是感知上均匀分布的色彩空间。
		cv::cvtColor(image, converted, 44);
		//得到转换后图像的迭代器
		cv::Mat_<cv::Vec3b>::const_iterator it = converted.begin<cv::Vec3b>();
		cv::Mat_<cv::Vec3b>::const_iterator end = converted.end<cv::Vec3b>();
		cv::Mat_<uchar>::iterator itout = result.begin<uchar>();
		for (; it != end; ++it, ++itout) {
			//处理每个像素
			//计算离目标颜色的距离
			//error:迭代器没有取出值
			if (getDistance(*it) < minDist) {
				*itout = 255;
			}
			else {
				*itout = 0;
			}//像素处理结束
		}
		return result;

	}
};