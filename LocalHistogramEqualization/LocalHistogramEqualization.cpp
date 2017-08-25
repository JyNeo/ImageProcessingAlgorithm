/**
  * @author:          yangjun
  * @date             2017/8/25 17:24
  * @description:     �ֲ�ֱ��ͼ����
  */ 

#include <highgui/highgui.hpp>
#include <core/core.hpp>

using namespace cv;

//����ͳ�ƺõ�ֱ��ͼ�������ڵ�CDF
void getCDF(float *s, const float *const c) {
	for (int i = 1; i < 256; i++) {
		s[i] = s[i - 1] + c[i];
	}
}

//�ֲ��任�����������ƶ����޸�ͳ����Ϣ
void refresh(float *c, uchar sub, bool flag) {
	if (flag) {
		c[sub] += (1.0 / 49.0);
	}
	else {
		c[sub] -= (1.0 / 49.0);
	}
}

//Ӧ�þֲ�ֱ��ͼ�㷨
void localhist(Mat image) {

	int cols = image.cols;
	int rows = image.rows;
	float count[256] = { 0.f }; //��������

	//ͳ�Ƶ�һ����
	uchar sub = 0; //����
	for (int m = 0; m < 7; m++) {
		for (int n = 0; n < 7; n++) {
			sub = image.at<uchar>(m, n);
			count[sub] += (1.0 / 49.0); //������ÿ�����س��ֵĸ���
		}
	}

	float s[256]; //�ۻ�����
	s[0] = count[0];
	getCDF(s, count); //�����ڵ�CDF

	sub = image.at<uchar>(3, 3);
	uchar newvalue = (uchar)((s[sub]) * 255 + 0.5);
	image.at<uchar>(3, 3) = newvalue;

	refresh(count, sub, false); //��ֵ����һ��
	refresh(count, newvalue, true); //��ֵ����һ��
	
	//����λ�ƣ��������޸�ֱ��ͼ��Ϣ
	for (int i = 3; i < rows - 3; i++) {
		for (int j = 3; j < cols - 3; j++) {
			bool dirty = true;
			if (i == 3 && j == 3) {
				continue;
			}
			for (int p = 0; p < 7; p++) {
				//uchar old = image.at<uchar>(j - 4, i - 3 + p);
				//uchar add = image.at<uchar>(j + 3, i - 3 + p);
				uchar old = image.at<uchar>(i - 3 + p, j - 4);
				uchar add = image.at<uchar>(i - 3 + p, j + 3);
				if (old != add) {
					refresh(count, old, false);
					refresh(count, add, true);
					dirty = false;
				}
			}
			if (!dirty) {
				getCDF(s, count);
				sub = image.at<uchar>(i, j);
				refresh(count, sub, false);
				image.at<uchar>(i, j) = (uchar)(s[sub] * 255 + 0.5);
				refresh(count, image.at<uchar>(i, j), true);
			}
		}
	}
}

int main(int argc, char* argv[]) {

	Mat srcImage = imread("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0326(a)(embedded_square_noisy_512).tif", 0);
	if (!srcImage.data)
	{
		return 0;
	}
	
	namedWindow("SrcImage");
	imshow("SrcImage", srcImage);

	localhist(srcImage);

	namedWindow("DstImage");
	imshow("DstImage", srcImage);

	waitKey();
	return 0;
}