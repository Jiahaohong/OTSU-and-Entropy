#include "common.h"
#include "entropy.h"

int entropy(cv::Mat &input_img)
{
	int M = input_img.rows;
	int N = input_img.cols;
	int size = M * N;
	double log_size = std::log(M*N);

	double a, b, c; //公式里相加的三项
	double phi, phiMax = 0.0; //phi公式最大值
	int w0;    //小于阈值t的像素的个数
	double e0; //小于阈值t的像素一维熵（用像素个数代替概率）
	int w1;	   //大于阈值t的像素的个数
	double e1; //大于阈值t的像素一维熵（用像素个数代替概率）

	int gray_level, gray_num, gray_arr[256] = { 0 };
	int t, t_optm = 127;
	long int cnt1, cnt2;

	for (cnt1 = 0; cnt1 < M; ++cnt1)
	{
		for (cnt2 = 0; cnt2 < N; ++cnt2)
		{
			gray_level = input_img.at<uchar>(cnt1, cnt2);
			gray_arr[gray_level] += 1;
		}
	}

	for (t = 1; t < 254; ++t)
	{
		w0 = w1 = 0;
		e0 = e1 = 0.0;

		//小于阈值t的像素的参数
		for (cnt1 = 0; cnt1 < t; ++cnt1)
		{
			gray_num = gray_arr[cnt1];
			w0 += gray_num;
			if (gray_num != 0)
			{
				e0 += gray_num * std::log(gray_num);
			}
		}

		//大于阈值t像素的参数
		for (cnt2 = t; cnt2 < 256; ++cnt2)
		{
			gray_num = gray_arr[cnt2];
			//w1 += gray_arr[cnt2]; //（不用累加）
			if (gray_num != 0)
			{
				e1 += gray_num * std::log(gray_num);
			}
		}
		w1 = size - w0;

		a = std::log(w0) + std::log(size - w0) - 2 * log_size;
		b = log_size - e0 / w0;
		c = (log_size*w1 - e1) / w1;

		phi = a + b + c;
		if (phi > phiMax)
		{
			t_optm = t;
			phiMax = phi;
			//printf("%f\n", (double)w0 / size);
		}
	}

	return t_optm;
}