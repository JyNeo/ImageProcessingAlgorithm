/**
  * @author:          yangjun
  * @date             2017/8/25 17:25
  * @description:     使用直方图统计的局部增强
  */ 

#include <highgui/highgui.hpp>
#include <core/core.hpp>

using namespace cv;

float getMean(const float *c) {
	float m = 0;
	for (size_t x = 0; x < 256; x++) {
		m += x * c[x];
	}
	return m;
}

float getVariance(const float *c, float mean) {
	float delta = 0;
	for (size_t y = 0; y < 256; y++) {
		if (c[y] != 0) {
			delta += (pow((y - mean), 2) * c[y]);
		}
	}
	delta = sqrt(delta);
	return delta;
}

void getCount(float *c, int originx, int originy, int wid, int hei, Mat img) {
	for (size_t t = 0; t < 256; t++) {
		c[t] = 0;
	}
	for (int i = originx; i < wid; i++) {
		for (int j = originy; j < hei; j++) {
			unsigned char sub = img.at<uchar>(i, j);
			c[sub] += 1.0 / 256.0;
		}
	}
}

void init(float * count) {
	for (size_t t = 0; t < 256; t++) {
		count[t] = 0;
	}
}
int histStatistic(Mat img, int dim, float k0, float k1, float k2, float E) {
	if (dim % 2 == 0) {
		return -1;
	}

	int width = img.cols;
	int height = img.rows;
	int sum = width * height;
	float count[256];
	
	for (size_t t = 0; t < 256; t++) {
		count[t] = 0;
	}
	unsigned char sub = 0;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			sub = img.at<uchar>(i, j);
			count[sub] += 1.0 / sum;
		}
	}
	
	float mean = getMean(count);
	float delta = getVariance(count, mean);
	float mxy = 0;
	float deltaxy = 0;
	float local = dim * dim;
	for (int i = dim / 2; i < height - dim / 2 - 1; i++) {
		for (int j = dim / 2; j < width - dim / 2 - 1; j++) {
			//重新初始化统计数组
			init(count);
			//统计局部直方图
			//getCount(count,j-dim/2,i-dim/2,j+dim/2+1,i+dim/2+1,img);
			for (int p = j - dim / 2; p < j + dim / 2 + 1; p++) {
				for (int q = i - dim / 2; q < i + dim / 2 + 1; q++) {
					sub = img.at<uchar>(p, q);
					count[sub] += 1.0 / local;
				}
			}
			mxy = getMean(count);
			deltaxy = getVariance(count, mxy);
			//增强判断
			if (mxy <= mean*k0 && deltaxy <= k2*delta && deltaxy >= k1*delta) {
				img.at<uchar>(j, i) = img.at<uchar>(j, i)*E;
			}
		}
	}
	return 0;
}

int main(int argc, char ** argv) {

	Mat srcImage = imread("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0326(a)(embedded_square_noisy_512).tif", 0);
	if (!srcImage.data)
	{
		return 0;
	}

	namedWindow("SrcImage");
	imshow("SrcImage", srcImage);

	histStatistic(srcImage, 9, 0.4, 0, 0.4, 15);

	namedWindow("DstImage");
	imshow("DstImage", srcImage);

	waitKey();
	return 0;
}