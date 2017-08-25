/**
  * @author:          yangjun
  * @date             2017/8/25 17:24
  * @description:     局部直方图均衡
  */ 

#include <highgui/highgui.hpp>
#include <core/core.hpp>

using namespace cv;

//根据统计好的直方图求邻域内的CDF
void getCDF(float *s, const float *const c) {
	for (int i = 1; i < 256; i++) {
		s[i] = s[i - 1] + c[i];
	}
}

//局部变换和邻域中心移动后修改统计信息
void refresh(float *c, uchar sub, bool flag) {
	if (flag) {
		c[sub] += (1.0 / 49.0);
	}
	else {
		c[sub] -= (1.0 / 49.0);
	}
}

//应用局部直方图算法
void localhist(Mat image) {

	int cols = image.cols;
	int rows = image.rows;
	float count[256] = { 0.f }; //单个概率

	//统计第一邻域
	uchar sub = 0; //像素
	for (int m = 0; m < 7; m++) {
		for (int n = 0; n < 7; n++) {
			sub = image.at<uchar>(m, n);
			count[sub] += (1.0 / 49.0); //邻域内每个像素出现的概率
		}
	}

	float s[256]; //累积概率
	s[0] = count[0];
	getCDF(s, count); //邻域内的CDF

	sub = image.at<uchar>(3, 3);
	uchar newvalue = (uchar)((s[sub]) * 255 + 0.5);
	image.at<uchar>(3, 3) = newvalue;

	refresh(count, sub, false); //旧值减少一次
	refresh(count, newvalue, true); //新值增加一次
	
	//邻域位移，逐邻域修改直方图信息
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