#include<opencv2/opencv.hpp>
#include<opencv2/features2d.hpp>
#include<iostream>
#include<assert.h>
#include<stdlib.h>
#include<math.h>
#include<opencv2/xfeatures2d.hpp>




using namespace std;
using namespace cv::xfeatures2d;

int main(int args, char** argv) {
	// Read input images
	cv::Mat image1 = cv::imread("D:/A/imgproFCM/church01.jpg", 0);
	cv::Mat image2 = cv::imread("D:/A/imgproFCM/church02.jpg", 0);

	if (!image1.data || !image2.data) {
		printf("图像读取失败");
		return -1;
	}
		

	// 提取左右影像的特征点
	std::vector<cv::KeyPoint> keypoints1;
	std::vector<cv::KeyPoint> keypoints2;

	//构造一个SURF特征检测器
	cv::Ptr<SURF>surf = SURF::create(2500.0);;

	//特征检测
	surf->detect(image1, keypoints1);
	surf->detect(image2, keypoints2);

	std::cout << "Number of SURF points (1): " << keypoints1.size() << std::endl;
	std::cout << "Number of SURF points (2): " << keypoints2.size() << std::endl;



	//构造SURF描述提取器
	cv::Ptr<SURF> surfDesc = SURF::create();

	// 提取特征属性到特征描述子
	cv::Mat descriptors1, descriptors2;
	surfDesc->compute(image1, keypoints1, descriptors1);
	surfDesc->compute(image2, keypoints2, descriptors2);


	// 构造特征匹配器
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce");

	//匹配
	std::vector<cv::DMatch> matches;
	matcher->match(descriptors1, descriptors2, matches);

	std::cout << "Number of matched points: " << matches.size() << std::endl;

	std::nth_element(matches.begin(),    // initial position
		matches.begin() + 24, // position of the sorted element
		matches.end());     // end position
// 去除不想显示的匹配点。
	matches.erase(matches.begin() + 25, matches.end());

	cv::Mat imageMatches;
	cv::drawMatches(image1, keypoints1,  // 1st image and its keypoints
		image2, keypoints2,  // 2nd image and its keypoints
		matches,			// the matches
		imageMatches,		// the image produced
		cv::Scalar(255, 255, 255)); // color of the lines
	cv::namedWindow("Matches");
	cv::imshow("Matches", imageMatches);
	cv::waitKey(0);
	return 0;


}