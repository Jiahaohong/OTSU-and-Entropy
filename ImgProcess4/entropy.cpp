#include "common.h"
#include "entropy.h"

int entropy(cv::Mat &input_img)
{
	int M = input_img.rows;
	int N = input_img.cols;
	int size = M * N;
	double log_size = std::log(M*N);

	double a, b, c; //��ʽ����ӵ�����
	double phi, phiMax = 0.0; //phi��ʽ���ֵ
	int w0;    //С����ֵt�����صĸ���
	double e0; //С����ֵt������һά�أ������ظ���������ʣ�
	int w1;	   //������ֵt�����صĸ���
	double e1; //������ֵt������һά�أ������ظ���������ʣ�

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

		//С����ֵt�����صĲ���
		for (cnt1 = 0; cnt1 < t; ++cnt1)
		{
			gray_num = gray_arr[cnt1];
			w0 += gray_num;
			if (gray_num != 0)
			{
				e0 += gray_num * std::log(gray_num);
			}
		}

		//������ֵt���صĲ���
		for (cnt2 = t; cnt2 < 256; ++cnt2)
		{
			gray_num = gray_arr[cnt2];
			//w1 += gray_arr[cnt2]; //�������ۼӣ�
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