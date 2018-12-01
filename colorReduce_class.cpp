class ColorDetector {
private:
	//最小可接受的距离
	int minDist;
	//目标色
	cv::Vec3b target;
	//结果图像
	cv::Mat result;
public:
	ColorDetector() : minDist(100) {

		// default parameter initialization here
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
		target[0] = blue;
		target[1] = green;
		target[2] = red;
	}
	void setTargetColor(cv::Vec3b color) {
		target = color;
	}
	cv::Vec3b getTargetColor() const {
		return target;
	}
	cv::Mat process(const cv::Mat &image) {
		//输出图像
		result.create(image.rows, image.cols, CV_8U);
		//得到迭代器
		cv::Mat_<cv::Vec3b>::const_iterator it = image.begin<cv::Vec3b>();
		cv::Mat_<cv::Vec3b>::const_iterator end = image.end<cv::Vec3b>();
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
