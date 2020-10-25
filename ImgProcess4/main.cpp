#include "common.h"
#include "otsu.h"
#include "entropy.h"

int main()
{
	//读入图像
	cv::Mat orig_img = cv::imread("lena.jpg");
	int M = orig_img.rows;
	int N = orig_img.cols;

	cv::imshow("orig img", orig_img);

	//转灰度图像
	cv::Mat gray_img;
	cv::cvtColor(orig_img, gray_img, CV_BGR2GRAY);
	cv::imshow("gray img", gray_img);

	cv::Mat res_img;

	//OTSU二值化
	int t1 = otsu(gray_img);
	printf("%d", t1);
	cv::threshold(gray_img, res_img, t1, 255, CV_THRESH_BINARY);
	cv::imshow("OTSU", res_img);

	//一维熵二值化
	int t2 = entropy(gray_img);
	printf("%d", t2);
	cv::threshold(gray_img, res_img, t2, 255, CV_THRESH_BINARY);
	cv::imshow("Entropy", res_img);

	cv::waitKey();

	return 0;
}