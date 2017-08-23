/**
  * @author:          yangjun
  * @date             2017/8/21 19:52
  * @description:     Contrast stretching is a process that expands the range of intensity levels in an image
  */ 

#include <highgui/highgui.hpp>
#include <core/core.hpp>

//对比度拉伸
int main(int argc, char* argv[])
{
	IplImage* srcImage = cvLoadImage("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0310(b)(washed_out_pollen_image).tif");

	cvNamedWindow("SrcImage");
	cvShowImage("SrcImage", srcImage);

	int height = srcImage->height;
	int width = srcImage->width;
	int step = srcImage->widthStep;
	int channels = srcImage->nChannels;
	uchar* pImage = (uchar *)srcImage->imageData;

	//找到图像中像素的最大值和最小值
	int data_min = 255, data_max = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < channels; k++)
			{
				if (pImage[i * step + j * channels + k] < data_min)
					data_min = pImage[i * step + j * channels + k];
				if (pImage[i * step + j * channels + k] > data_max)
					data_max = pImage[i * step + j * channels + k];
			}
		}
	}

	//得到满标度的图像
	int temp = data_max - data_min;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < channels; k++)
			{
				pImage[i * step + j * channels + k] = (pImage[i * step + j * channels + k] - data_min) * 255 / temp;
			}
		}
	}

	cvNamedWindow("DstImage");
	cvShowImage("DstImage", srcImage);

	cvWaitKey();
	cvReleaseImage(&srcImage);
	cvDestroyWindow("SrcImage");
	cvDestroyWindow("DstImage");
	return 0;
}

/*********************************************************************************************************************************/

#include <core/core.hpp>
#include <highgui/highgui.hpp>

using namespace cv;

int main()
{
	//图像读取
	Mat srcImage = imread("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0310(b)(washed_out_pollen_image).tif");

	//判断图像读取是否有问题
	if (!srcImage.data)
	{
		return 0;
	}

	//原始图像显示
	namedWindow("SrcImage");
	imshow("SrcImage", srcImage);

	//处理图像
	int data_max = 0, data_min = 255;
	int nl = srcImage.rows;
	int nc = srcImage.cols * srcImage.channels();
	if (srcImage.isContinuous())
	{
		nc = nc * nl;
		nl = 1;
	}

	//找到图像中像素的最大值和最小值
	uchar *data;
	for (int j = 0; j < nl; j++)
	{
		data = srcImage.ptr<uchar>(j);
		for (int i = 0; i < nc; i++)
		{
			if (data[i] > data_max)
				data_max = data[i];
			if (data[i] < data_min)
				data_min = data[i];
		}
	}

	//得到满标度的图像
	int temp = data_max - data_min;
	for (int j = 0; j < nl; j++)
	{
		data = srcImage.ptr<uchar>(j);
		for (int i = 0; i < nc; i++)
		{
			data[i] = (data[i] - data_min) * 255 / temp;
		}
	}

	//显示图像
	namedWindow("DstImage");
	imshow("DstImage", srcImage);

	waitKey();
	return 0;
}