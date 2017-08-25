/**
* @author:          yangjun
* @date             2017/8/24 15:11
* @description:     Histogram Matching:
*					1、求解源图片的累积直方图（概率）分布
*					2、求解目标图片的累积直方图（概率）分布
*					3、直方图匹配: 建立映射变换
*/

#include <highgui/highgui.hpp>
#include <core/core.hpp>

IplImage* getHistogramMatching(IplImage* srcImg, IplImage* dstImg);

int main()
{
	IplImage* srcImage = cvLoadImage("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0323(a)(mars_moon_phobos).tif", 0);
	if (srcImage == NULL)
	{
		return 0;
	}

	cvNamedWindow("SrcImage");
	cvShowImage("SrcImage", srcImage);

	IplImage* dstImage = cvLoadImage("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0323(b)(mars_moon_phobos).tif", 0);
	if (dstImage == NULL)
	{
		return 0;
	}

	cvNamedWindow("DstImage");
	cvShowImage("DstImage", dstImage);

	IplImage* resultImg = getHistogramMatching(srcImage, dstImage);

	cvNamedWindow("ResultImage");
	cvShowImage("ResultImage", resultImg);

	cvWaitKey();
	cvReleaseImage(&srcImage);
	cvReleaseImage(&dstImage);
	cvReleaseImage(&resultImg);
	cvDestroyAllWindows();
	return 0;
}

IplImage* getHistogramMatching(IplImage* srcImg, IplImage* dstImg)
{
	float temp;
	const int size = 256;
	int grayArray[size];

	uchar* srcData = (uchar*)srcImg->imageData;
	uchar* dstData = (uchar*)dstImg->imageData;

	float srcCDFArray[size] = { 0.f };
	float dstCDFArray[size] = { 0.f };

	//1、求解源图片的累积直方图（概率）分布
	memset(grayArray, 0, sizeof(grayArray));
	for (int i = 0; i < srcImg->height; i++)
	{
		for (int j = 0; j < srcImg->width; j++)
		{
			int tag = srcData[i * srcImg->widthStep + j];
			grayArray[tag]++; //统计每个像素值出现的次数
		}
	}

	temp = 0;
	for (int i = 0; i < size; i++)
	{
		temp = temp + grayArray[i];
		srcCDFArray[i] = temp * (size - 1) / (srcImg->width * srcImg->height); //计算累计概率分布
	}

	//2、求解目标图片的累积直方图（概率）分布
	memset(grayArray, 0, sizeof(grayArray));
	for (int i = 0; i < dstImg->height; i++)
	{
		for (int j = 0; j < dstImg->width; j++)
		{
			int tag = dstData[i * dstImg->widthStep + j];
			grayArray[tag]++; //统计每个像素值出现的次数
		}
	}

	temp = 0;
	for (int i = 0; i < size; i++)
	{
		temp = temp + grayArray[i];
		dstCDFArray[i] = temp * (size - 1) / (dstImg->width * dstImg->height); //计算累计概率分布
	}

	//3、直方图匹配（规定化）算法
	int histogramMap[size]; //直方图映射
	int minTag; //当映射不唯一时，按惯例选择最小的值

	for (int i = 0; i < size; i++) //源图像的灰度级
	{
		float minMap = 255.f;
		for (int j = 0; j < size; j++) //目标图像的灰度级
		{
			if (abs(srcCDFArray[i] - dstCDFArray[j]) < minMap)
			{
				minMap = abs(srcCDFArray[i] - dstCDFArray[j]);
				minTag = j;
			}
		}
		histogramMap[i] = minTag; //源图像和目标图像之间灰度级的映射关系
	}

	IplImage* resultImg = cvCloneImage(srcImg);
	for (int i = 0; i < resultImg->height; i++)
	{
		for (int j = 0; j < resultImg->width; j++)
		{
			int tag = ((uchar *)srcImg->imageData)[i * srcImg->widthStep + j];
			((uchar *)resultImg->imageData)[i * resultImg->widthStep + j] = histogramMap[tag];
		}
	}
	return resultImg;
}

/*********************************************************************************************************************************/

#include <cv.h>
#include <highgui/highgui.hpp>
#include <core/core.hpp>

using namespace cv;

Mat HistogramMatching(Mat srcImg, Mat dstImg);

int main()
{
	Mat srcImage = imread("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0323(a)(mars_moon_phobos).tif", 0);
	if (!srcImage.data)
	{
		return 0;
	}

	namedWindow("SrcImage");
	imshow("SrcImage", srcImage);

	Mat dstImage = imread("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0323(b)(mars_moon_phobos).tif", 0);
	if (!dstImage.data)
	{
		return 0;
	}

	namedWindow("DstImage");
	imshow("DstImage", dstImage);

	Mat resultImage = HistogramMatching(srcImage, dstImage);

	namedWindow("ResultImage");
	imshow("ResultImage", resultImage);

	waitKey();
	return 0;
}

Mat HistogramMatching(Mat srcImg, Mat dstImg)
{
	float temp;
	const int size = 256;

	int grayArray[size];
	int srcRows = srcImg.rows;
	int srcCols = srcImg.cols;
	int dstRows = dstImg.rows;
	int dstCols = dstImg.cols;

	//累积分布函数数组
	float srcCDFArray[size] = { 0.f };
	float dstCDFArray[size] = { 0.f };

	//1、求解源图片的累积直方图（概率）分布  
	memset(grayArray, 0, sizeof(grayArray));
	for (int row = 0; row < srcRows; row++)
		for (int col = 0; col < srcCols; col++)
		{
			int tag = srcImg.at<uchar>(row, col);
			grayArray[tag]++; //统计每个像素值出现的次数
		}

	temp = 0;
	for (int i = 0; i < size; i++)
	{
		temp += grayArray[i];
		srcCDFArray[i] = temp * (size - 1) / (srcRows * srcCols); //计算累计概率分布
	}

	//2、求解目标图片的累积直方图（概率）分布
	memset(grayArray, 0, sizeof(grayArray));
	for (int row = 0; row < dstRows; row++)
		for (int col = 0; col < dstCols; col++)
		{
			int tag = dstImg.at<uchar>(row, col);
			grayArray[tag]++; //统计每个像素值出现的次数
		}

	temp = 0;
	for (int i = 0; i < size; i++)
	{
		temp += grayArray[i];
		dstCDFArray[i] = temp * (size - 1) / (dstRows * dstCols); //计算累计概率分布
	}

	//3、直方图匹配（规定化）算法
	int histogramMap[size]; //直方图映射
	int minTag; //当映射不唯一时，按惯例选择最小的值
	for (int i = 0; i < size; i++) //源图像的灰度级
	{
		float minMap = 255.f;
		for (int j = 0; j < size; j++) //目标图像的灰度级
		{
			if (minMap > abs(srcCDFArray[i] - dstCDFArray[j]))
			{
				minMap = abs(srcCDFArray[i] - dstCDFArray[j]);
				minTag = j;
			}
		}
		histogramMap[i] = minTag; //源图像和目标图像之间灰度级的映射关系
	}

	Mat resultImg = srcImg; //仅创建了Mat的头部分，srcImg和resultImg共享数据区
							//Mat resultImg = srcImg.clone(); //完全拷贝，将srcImg的所有信息拷贝到resultImg中
	for (int row = 0; row < resultImg.rows; row++)
		for (int col = 0; col < resultImg.cols; col++)
		{
			int tag = srcImg.at<uchar>(row, col);
			resultImg.at<uchar>(row, col) = histogramMap[tag];
		}

	return resultImg;
}