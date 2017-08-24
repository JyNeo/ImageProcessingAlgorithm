/**
  * @author:          yangjun
  * @date             2017/8/23 10:33
  * @description:     Highlighting a specific range of intensities in an image often is of interest.
  */ 

#include <core/core.hpp>
#include <highgui/highgui.hpp>

using namespace cv;

int main()
{
	//图像读取
	Mat image = imread("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0312(a)(kidney).tif");

	//判断图像读取是否有问题
	if (!image.data)
	{
		return 0;
	}

	//原始图像显示
	namedWindow("SrcImage");
	imshow("SrcImage", image);

	//处理图像
	int intensity_min = 150;

	int nl = image.rows;
	int nc = image.cols * image.channels();

	if (image.isContinuous())
	{
		nc = nc * nl;
		nl = 1;
	}

	for (int j = 0; j < nl; j++)
	{
		uchar *data = image.ptr<uchar>(j);
		for (int i = 0; i < nc; i++)
		{
			//二值化
			if (data[i] > intensity_min)
				data[i] = 255;
			else
				data[i] = 0;
		}
	}

	//显示图像
	namedWindow("DstImage");
	imshow("DstImage", image);

	waitKey();
	return 0;
}

/*********************************************************************************************************************************/

#include <highgui/highgui.hpp>
#include <core/core.hpp>

int main(int argc, char *argv[])
{
	IplImage* image = cvLoadImage("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0312(a)(kidney).tif");
	if (image == NULL)
	{
		return 0;
	}

	cvNamedWindow("SrcImage");
	cvShowImage("SrcImage", image);

	int intensity_min = 150;
	int width = image->width;
	int height = image->height;
	int channels = image->nChannels;
	int step = image->widthStep;
	uchar *data = (uchar *)image->imageData;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < channels; k++)
			{
				//二值化
				if (data[i*step + j*channels + k] > intensity_min)
					data[i*step + j*channels + k] = 255;
				else
					data[i*step + j*channels + k] = 0;
			}
		}
	}

	cvNamedWindow("DstImage");
	cvShowImage("DstImage", image);

	cvWaitKey();
	cvReleaseImage(&image);
	cvDestroyWindow("SrcImage");
	cvDestroyWindow("DstImage");
	return 0;
}