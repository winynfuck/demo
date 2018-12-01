
第二节课
//加载图像，imread功能是加载图像文件成为一个mat对象。第二个参数是1或者0，是0则返回灰度图。
	cv::mat image = cv::imread("d:/a/cat.jpg",1);

	//convertto函数能将图像转换到另一种深度中去，第三个参数是缩放，第四个参数是平移。
	/*image.convertto(result,cv_32f, 1.0 / 255.0, 0);*/

	if (image.empty()) {
		printf("loading image failed");
	}


	//图像的深度是图像的颜色丰富程度，有cv_8u,表示8位无符号数，可以最多表示256种颜色。cv_8u前面不需要加cv::
	/*cv_assert(image.depth() == cv_8u);*/
	//获取图像的指针
	/*const uchar * p = image.ptr<uchar>(0);*/
	//通过指针取出数据需要转化为int类型。
	/*cout << (int)p[0] << endl;*/

	////定义一个掩模
	//cv::mat kernel = (cv::mat_<char>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	cv::mat dst;
	image.copyto(dst);
	////用掩模进行滤波
	//cv::filter2d(image, dst, image.depth(), kernel);
	int nchannels = image.channels();
	int rows = image.rows;
	int cols = image.cols*nchannels;
	for (int i = 1; i < rows-1; i++) {
		const uchar * current = image.ptr<uchar>(i);
		const uchar * up = image.ptr<uchar>(i-1);
		const uchar * down = image.ptr<uchar>(i+1);
		uchar *output = dst.ptr<uchar>(i);
		for (int j = nchannels; j < cols-nchannels; j++) {
			//*ptr表示指针所指向的值
			*output = cv::saturate_cast<uchar>(5 * current[j] - (up[j] + down[j] + current[j + nchannels] + current[j - nchannels]));
			output++;//指针所指向的内存移动
		}
	}
	cv::namedWindow("卷积前的图像", 0);
	cv::imshow("卷积前的图像", image);
	cv::namedWindow("卷积后的图像",0);
	cv::imshow("卷积后的图像", dst);
	cv::waitKey(0) 




/*
第一节课：加载保存修改
加载图像，imread功能是加载图像文件成为一个Mat对象。第二个参数是1或者0，是0则返回灰度图。
cv::Mat image = cv::imread("D:/A/cat.jpg",0);
namedwindow创建一个窗口。有两个参数，第二个参数默认为1，表示原图显示，不可更改大小。若为0，表示可以修改。
cv::namedwindow("origin image",0);
cv::imshow("origin image", image);
cv::mat cvtimage;
cvtcolor,色彩空间转换函数，三个参数，一二是原图以及输出图像，第三个是一个int类型的数字，代表着各种颜色空间的转换。具体可查函数文档。
cv::cvtcolor(image, cvtimage, 66);
cv::namedwindow("convert color image", 0);
cv::imshow("convert color image",cvtimage);
imwrite函数保存图像。但只有单通道和三通道的8位和16位的png,jpg,tif图像才能这么保存
cv::imwrite("d:/a/cat.tif", cvtimage);
cv::waitKey(0);
*/