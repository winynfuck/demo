void sharpen(const cv::Mat &image, cv::Mat &result) {
	result = cv::Mat::zeros(image.size(), image.type());
	for (int j = 1; j < image.rows - 1; j++) {
		//处理除了第一行和最后一行之外的所有行
		const uchar* previous = image.ptr<const uchar>(j - 1);//上一行
		const uchar* current = image.ptr<const uchar>(j);//当前行
		const uchar* next = image.ptr<const uchar>(j + 1);//下一行
		uchar* output = result.ptr<uchar>(j);//输出行
		int channel = image.channels();
		for (int i = 1; i < image.cols - 1; i++) {
			*output = cv::saturate_cast<uchar>(5 * current[i] - current[i - channel] - current[i + channel] - previous[i] - next[i]);
		}
		result.row(0).setTo(cv::Scalar(0));
		result.row(result.rows - 1).setTo(cv::Scalar(0));
		result.col(0).setTo(cv::Scalar(0));
		result.col(result.cols - 1).setTo(cv::Scalar(0));
	}
}
void sharpen2D(const cv::Mat &image, cv::Mat &result) {
	//构造核
	cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));
	//对核元素进行赋值
	kernel.at<float>(1, 1) = 5.0;
	kernel.at<float>(0, 1) = -1.0;
	kernel.at<float>(2, 1) = -1.0;
	kernel.at<float>(1, 2) = -1.0;
	kernel.at<float>(1, 0) = -1.0;
	//对图像进行滤波
	cv::filter2D(image, result, image.depth(), kernel);
}
