/**
  * @author:          yangjun
  * @date             2017/8/23 14:06
  * @description:     Storing the four highest-order bit planes would allow us to reconstruct the original image in acceptable detail.
  *                   Storing these four planes instead of the original image requires 50% less storage
  */ 

#include <core/core.hpp>
#include <highgui/highgui.hpp>

using namespace std;
using namespace cv;

//代替突出灰度级范围，利用突出特定比特平面来为整个图像外观作出贡献，突出分层的最好做法就是二值化
int main()
{
	//图像读取
	Mat image = imread("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0314(a)(100-dollars).tif");

	//判断图像读取是否有问题
	if (!image.data)
	{
		return 0;
	}

	//原始图像显示
	namedWindow("ScrImage");
	imshow("ScrImage", image);

	//处理图像
	Mat image_tmp = image.clone();
	int nl = image.rows;
	int nc = image.cols * image.channels();
	if (image.isContinuous())
	{
		nc = nc * nl;
		nl = 1;
	}

	uchar *data;
	uchar *data_tmp;
	int rang_min, rang_max;
	for (int n = 1; n <= 8; n++)
	{
		rang_min = pow(2, n - 1);
		rang_max = pow(2, n);

		for (int j = 0; j < nl; j++)
		{
			data = image.ptr<uchar>(j);
			data_tmp = image_tmp.ptr<uchar>(j);

			for (int i = 0; i < nc; i++)
			{
				//二值化
				if (data[i] >= rang_min && data[i] < rang_max)
					data_tmp[i] = 255;
				else
					data_tmp[i] = 0;
			}
		}

		switch (n)
		{
			case 1:
				namedWindow("Bit Level 1");
				imshow("Bit Level 1", image_tmp);
				break;
			case 2:
				namedWindow("Bit Level 2");
				imshow("Bit Level 2", image_tmp);
				break;
			case 3:
				namedWindow("Bit Level 3");
				imshow("Bit Level 3", image_tmp);
				break;
			case 4:
				namedWindow("Bit Level 4");
				imshow("Bit Level 4", image_tmp);
				break;
			case 5:
				namedWindow("Bit Level 5");
				imshow("Bit Level 5", image_tmp);
				break;
			case 6:
				namedWindow("Bit Level 6");
				imshow("Bit Level 6", image_tmp);
				break;
			case 7:
				namedWindow("Bit Level 7");
				imshow("Bit Level 7", image_tmp);
				break;
			case 8:
				namedWindow("Bit Level 8");
				imshow("Bit Level 8", image_tmp);
				break;
			default:
				break;
		}
	}

	waitKey();
	return 0;
}

/*********************************************************************************************************************************/

#include <core/core.hpp>
#include <highgui/highgui.hpp>

int main()
{
	//图像读取
	IplImage* srcImage = cvLoadImage("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0314(a)(100-dollars).tif");
	IplImage* dstImage = cvCreateImage(cvSize(srcImage->width, srcImage->height), srcImage->depth, srcImage->nChannels);

	//判断图像读取是否有问题
	if (srcImage == NULL)
	{
		return 0;
	}

	//原始图像显示
	cvNamedWindow("ScrImage");
	cvShowImage("ScrImage", srcImage);

	int rang_min, rang_max;
	int width = srcImage->width;
	int height = srcImage->height;
	int channels = srcImage->nChannels;
	int step = srcImage->widthStep;

	uchar *data = (uchar *)srcImage->imageData;
	uchar *data_temp = (uchar *)dstImage->imageData;

	for (int n = 1; n <= 8; n++)
	{
		rang_min = pow(2, n - 1);
		rang_max = pow(2, n);

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				for (int k = 0; k < channels; k++)
				{
					//二值化
					if (data[i*step + j*channels + k] >= rang_min && data[i*step + j*channels + k] < rang_max)
						data_temp[i*step + j*channels + k] = 255;
					else
						data_temp[i*step + j*channels + k] = 0;
				}
			}
		}

		switch (n)
		{
			case 1:
				cvNamedWindow("Bit Level 1");
				cvShowImage("Bit Level 1", dstImage);
				break;
			case 2:
				cvNamedWindow("Bit Level 2");
				cvShowImage("Bit Level 2", dstImage);
				break;
			case 3:
				cvNamedWindow("Bit Level 3");
				cvShowImage("Bit Level 3", dstImage);
				break;
			case 4:
				cvNamedWindow("Bit Level 4");
				cvShowImage("Bit Level 4", dstImage);
				break;
			case 5:
				cvNamedWindow("Bit Level 5");
				cvShowImage("Bit Level 5", dstImage);
				break;
			case 6:
				cvNamedWindow("Bit Level 6");
				cvShowImage("Bit Level 6", dstImage);
				break;
			case 7:
				cvNamedWindow("Bit Level 7");
				cvShowImage("Bit Level 7", dstImage);
				break;
			case 8:
				cvNamedWindow("Bit Level 8");
				cvShowImage("Bit Level 8", dstImage);
				break;
			default:
				break;
		}
	}

	cvWaitKey();
	cvReleaseImage(&srcImage);
	cvReleaseImage(&dstImage);
	cvDestroyWindow("SrcImage");
	cvDestroyWindow("Bit Level 1");
	cvDestroyWindow("Bit Level 2");
	cvDestroyWindow("Bit Level 3");
	cvDestroyWindow("Bit Level 4");
	cvDestroyWindow("Bit Level 5");
	cvDestroyWindow("Bit Level 6");
	cvDestroyWindow("Bit Level 7");
	cvDestroyWindow("Bit Level 8");
	return 0;
}