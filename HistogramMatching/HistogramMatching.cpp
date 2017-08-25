/**
* @author:          yangjun
* @date             2017/8/24 15:11
* @description:     Histogram Matching:
*					1�����ԴͼƬ���ۻ�ֱ��ͼ�����ʣ��ֲ�
*					2�����Ŀ��ͼƬ���ۻ�ֱ��ͼ�����ʣ��ֲ�
*					3��ֱ��ͼƥ��: ����ӳ��任
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

	//1�����ԴͼƬ���ۻ�ֱ��ͼ�����ʣ��ֲ�
	memset(grayArray, 0, sizeof(grayArray));
	for (int i = 0; i < srcImg->height; i++)
	{
		for (int j = 0; j < srcImg->width; j++)
		{
			int tag = srcData[i * srcImg->widthStep + j];
			grayArray[tag]++; //ͳ��ÿ������ֵ���ֵĴ���
		}
	}

	temp = 0;
	for (int i = 0; i < size; i++)
	{
		temp = temp + grayArray[i];
		srcCDFArray[i] = temp * (size - 1) / (srcImg->width * srcImg->height); //�����ۼƸ��ʷֲ�
	}

	//2�����Ŀ��ͼƬ���ۻ�ֱ��ͼ�����ʣ��ֲ�
	memset(grayArray, 0, sizeof(grayArray));
	for (int i = 0; i < dstImg->height; i++)
	{
		for (int j = 0; j < dstImg->width; j++)
		{
			int tag = dstData[i * dstImg->widthStep + j];
			grayArray[tag]++; //ͳ��ÿ������ֵ���ֵĴ���
		}
	}

	temp = 0;
	for (int i = 0; i < size; i++)
	{
		temp = temp + grayArray[i];
		dstCDFArray[i] = temp * (size - 1) / (dstImg->width * dstImg->height); //�����ۼƸ��ʷֲ�
	}

	//3��ֱ��ͼƥ�䣨�涨�����㷨
	int histogramMap[size]; //ֱ��ͼӳ��
	int minTag; //��ӳ�䲻Ψһʱ��������ѡ����С��ֵ

	for (int i = 0; i < size; i++) //Դͼ��ĻҶȼ�
	{
		float minMap = 255.f;
		for (int j = 0; j < size; j++) //Ŀ��ͼ��ĻҶȼ�
		{
			if (abs(srcCDFArray[i] - dstCDFArray[j]) < minMap)
			{
				minMap = abs(srcCDFArray[i] - dstCDFArray[j]);
				minTag = j;
			}
		}
		histogramMap[i] = minTag; //Դͼ���Ŀ��ͼ��֮��Ҷȼ���ӳ���ϵ
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

	//�ۻ��ֲ���������
	float srcCDFArray[size] = { 0.f };
	float dstCDFArray[size] = { 0.f };

	//1�����ԴͼƬ���ۻ�ֱ��ͼ�����ʣ��ֲ�  
	memset(grayArray, 0, sizeof(grayArray));
	for (int row = 0; row < srcRows; row++)
		for (int col = 0; col < srcCols; col++)
		{
			int tag = srcImg.at<uchar>(row, col);
			grayArray[tag]++; //ͳ��ÿ������ֵ���ֵĴ���
		}

	temp = 0;
	for (int i = 0; i < size; i++)
	{
		temp += grayArray[i];
		srcCDFArray[i] = temp * (size - 1) / (srcRows * srcCols); //�����ۼƸ��ʷֲ�
	}

	//2�����Ŀ��ͼƬ���ۻ�ֱ��ͼ�����ʣ��ֲ�
	memset(grayArray, 0, sizeof(grayArray));
	for (int row = 0; row < dstRows; row++)
		for (int col = 0; col < dstCols; col++)
		{
			int tag = dstImg.at<uchar>(row, col);
			grayArray[tag]++; //ͳ��ÿ������ֵ���ֵĴ���
		}

	temp = 0;
	for (int i = 0; i < size; i++)
	{
		temp += grayArray[i];
		dstCDFArray[i] = temp * (size - 1) / (dstRows * dstCols); //�����ۼƸ��ʷֲ�
	}

	//3��ֱ��ͼƥ�䣨�涨�����㷨
	int histogramMap[size]; //ֱ��ͼӳ��
	int minTag; //��ӳ�䲻Ψһʱ��������ѡ����С��ֵ
	for (int i = 0; i < size; i++) //Դͼ��ĻҶȼ�
	{
		float minMap = 255.f;
		for (int j = 0; j < size; j++) //Ŀ��ͼ��ĻҶȼ�
		{
			if (minMap > abs(srcCDFArray[i] - dstCDFArray[j]))
			{
				minMap = abs(srcCDFArray[i] - dstCDFArray[j]);
				minTag = j;
			}
		}
		histogramMap[i] = minTag; //Դͼ���Ŀ��ͼ��֮��Ҷȼ���ӳ���ϵ
	}

	Mat resultImg = srcImg; //��������Mat��ͷ���֣�srcImg��resultImg����������
							//Mat resultImg = srcImg.clone(); //��ȫ��������srcImg��������Ϣ������resultImg��
	for (int row = 0; row < resultImg.rows; row++)
		for (int col = 0; col < resultImg.cols; col++)
		{
			int tag = srcImg.at<uchar>(row, col);
			resultImg.at<uchar>(row, col) = histogramMap[tag];
		}

	return resultImg;
}