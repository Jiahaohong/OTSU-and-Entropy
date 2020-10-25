#include "common.h"
#include "otsu.h"

int otsu(cv::Mat &input_img)
{
	int M = input_img.rows;
	int N = input_img.cols;
	int size = M * N;

	double deltaB, etaMax = 0.0;
	long int w0; //灰度小于t的像素个数
	long int w1; //灰度大于t的像素个数
	double u0; //灰度小于t的像素的加权和，权重为灰度值（不是灰度值/灰度范围，方便后续计算）
	double u1; //灰度大于t的像素的加权和，权重为灰度值（不是灰度值/灰度范围，方便后续计算）
	double v0; //灰度大于t的像素的加权和，权重为灰度值的平方（不是灰度值/灰度范围，方便后续计算）
	double v1; //灰度大于t的像素的加权和，权重为灰度值的平方（不是灰度值/灰度范围，方便后续计算）
	double u0_2; //u0^2
	double u1_2; //u1^2
	
	int gray_level, gray_arr[256] = { 0 };
	int t, t_optm = 127;
	long int cnt1, cnt2;

	//统计灰度
	for (cnt1 = 0; cnt1 < M; ++cnt1)
	{
		for (cnt2 = 0; cnt2 < N; ++cnt2)
		{
			gray_level = input_img.at<uchar>(cnt1, cnt2);
			gray_arr[gray_level] += 1;
		}
	}

	//计算最佳阈值t
	for (t = 1; t < 254; ++t)
	{
		w0 = w1 = 0;
		u0 = u1 = v0 = v1 = 0.0;
		for (cnt1 = 0; cnt1 < t; ++cnt1)
		{
			w0 += gray_arr[cnt1];
			u0 += cnt1 * gray_arr[cnt1];
			v0 += cnt1^2 * gray_arr[cnt1]; 
		}
		for (cnt2 = t; cnt2 < 256; ++cnt2)
		{
			//w1 += gray_arr[cnt2]; //（不用累加）
			u1 += cnt2 * gray_arr[cnt2];
			v1 += cnt2^2 * gray_arr[cnt2];
		}
		w1 = size - w0;


		u0_2 = pow(u0, 2);
		u1_2 = pow(u1, 2);

		//deltaB计算公式
		//                     u0   u1            1
		//deltaB = w0 * w1 * ( —— - —— )^2 * ——————————
		//                     w0   w1       (M * N) ^2

		deltaB = w0 * w1 * pow( (u0 / w0 - u1 / w1), 2 ) / size / size;

		//delta0计算公式（不用计算） 
		//         v0     u0
		//delta0 = —— - ( —— )^2
		//         w0     w0 
		
		//deltaW计算公式（不用计算）
		//            1	                u0^2   u1^2
		//deltaW  = ————— * ( v0 + v1 - ———— - ———— )
		//          M * N                w0     w1
		//deltaW = (v0 + v1 - u0_2 / w0 - u1_2 / w1) / size;

		if (deltaB > etaMax)
		{
			t_optm = t;
			etaMax = deltaB;
			printf("%f\n", (double)w0 / size);
		}
	}

	return t_optm;
}