/**
  * @author:          yangjun
  * @date             2017/8/21 19:47
  * @description:     Contrast enhancement using power-law（Gamma） transformations.
  */ 

#include <highgui/highgui.hpp>      
#include <imgproc/imgproc.hpp>  

using namespace cv;

int main(int argc, char *argv[])
{
	Mat image = imread("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0309(a)(washed_out_aerial_image).tif");
	Mat imageGamma(image.size(), CV_32FC3);
	
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			imageGamma.at<Vec3f>(i, j)[0] = (image.at<Vec3b>(i, j)[0])*(image.at<Vec3b>(i, j)[0])*(image.at<Vec3b>(i, j)[0]);
			imageGamma.at<Vec3f>(i, j)[1] = (image.at<Vec3b>(i, j)[1])*(image.at<Vec3b>(i, j)[1])*(image.at<Vec3b>(i, j)[1]);
			imageGamma.at<Vec3f>(i, j)[2] = (image.at<Vec3b>(i, j)[2])*(image.at<Vec3b>(i, j)[2])*(image.at<Vec3b>(i, j)[2]);
		}
	}

	// 归一化到 0 ~ 255    
	normalize(imageGamma, imageGamma, 0, 255, CV_MINMAX);

	// 转换成 8 bit图像显示    
	convertScaleAbs(imageGamma, imageGamma);
	
	imshow("SrcImage", image);
	imshow("DstImage", imageGamma);

	waitKey();
	return 0;
}

/*********************************************************************************************************************************/

#include <highgui/highgui.hpp>
#include <core/core.hpp>

int main(int argc, char *argv[])
{
	IplImage* srcImage = cvLoadImage("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0309(a)(washed_out_aerial_image).tif");
	if (srcImage == NULL)
	{
		return 0;
	}

	cvNamedWindow("SrcImage");
	cvShowImage("SrcImage", srcImage);

	int width = srcImage->width;
	int height = srcImage->height;
	int channels = srcImage->nChannels;
	int step = srcImage->widthStep;
	uchar *data = (uchar *)srcImage->imageData;

	double f;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			for (int k = 0; k < channels; k++)
			{
				//直接三次方然后再进行归一化为什么会出错？
				//data[i * step + j * channels + k] = data[i * step + j * channels + k] * data[i * step + j * channels + k] * data[i * step + j * channels + k];

				f = data[i * step + j * channels + k];
				f = (f + 0.5) / 256;
				f = powf(f, 3);
				data[i * step + j * channels + k] = (int)(f * 256 - 0.5);
			}
		}
	}

	// 归一化到 0 ~ 255
	//cvNormalize(srcImage, srcImage, 0, 255, CV_MINMAX);
	//cvConvertScaleAbs(srcImage, srcImage);

	cvNamedWindow("DstImage");
	cvShowImage("DstImage", srcImage);

	cvWaitKey();
	cvReleaseImage(&srcImage);
	cvDestroyWindow("SrcImage");
	cvDestroyWindow("DstImage");
	return 0;
}

/*********************************************************************************************************************************/

#include <highgui/highgui.hpp>
#include <core/core.hpp>

IplImage *srcImg = NULL;
float gGamma = 3;

void GammaCorrect(IplImage* src, IplImage* dst, float gamma);

int main(int argc, char* argv[])
{
	srcImg = cvLoadImage("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0309(a)(washed_out_aerial_image).tif");

	cvNamedWindow("SrcImage");
	cvShowImage("SrcImage", srcImg);

	GammaCorrect(srcImg, srcImg, gGamma);

	cvNamedWindow("DstImage");
	cvShowImage("DstImage", srcImg);
	cvWaitKey(0);

	cvReleaseImage(&srcImg);
	cvDestroyWindow("SrcImage");
	cvDestroyWindow("DstImage");

	return 0;
}

void GammaCorrect(IplImage* src, IplImage* dst, float gamma)
{
	int height = src->height;
	int width = src->width;
	int channels = src->nChannels;

	float f;
	for (int i = 0; i < height; i++)
	{
		//pImage为当前行首指针
		uchar* pImage = (uchar *)src->imageData + src->widthStep * i;

		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < channels; k++)
			{
				//预补偿，归一化，还原
				f = pImage[j * channels + k];
				f = (f + 0.5) / 256;
				f = powf(f, gamma);
				pImage[j * channels + k] = (int)(f * 256 - 0.5);
			}
		}
	}
}

/*********************************************************************************************************************************/

#include <highgui/highgui.hpp>
#include <core/core.hpp>

IplImage *srcImg = NULL;
float gGamma = 3;
int gGammaLUT[256];

//建立gamma校正查找表
void BuildTable(float gamma);

//用查表法做gamma变换
void GammaCorrect(IplImage* src, IplImage* dst);

int main(int argc, char* argv[])
{
	srcImg = cvLoadImage("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0309(a)(washed_out_aerial_image).tif");

	cvNamedWindow("SrcImage");
	cvShowImage("SrcImage", srcImg);

	BuildTable(gGamma);
	GammaCorrect(srcImg, srcImg);

	cvNamedWindow("DstImage");
	cvShowImage("DstImage", srcImg);
	cvWaitKey(0);

	cvReleaseImage(&srcImg);
	cvDestroyWindow("SrcImage");
	cvDestroyWindow("DstImage");

	return 0;
}

//根据gamma的值建立查找表
void BuildTable(float gamma)
{
	float f;
	for (int i = 0; i < 256; i++)
	{
		//预补偿，归一化，还原
		f = (i + 0.5) / 256;
		f = powf(f, gamma);
		gGammaLUT[i] = (int)(f * 256 - 0.5);
	}
}

void GammaCorrect(IplImage* src, IplImage* dst)
{
	int height = src->height;
	int width = src->width;
	int channels = src->nChannels;

	for (int i = 0; i < height; i++)
	{
		//pImage为当前行首指针
		uchar* pImage = (uchar*)src->imageData + src->widthStep * i;
		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < channels; k++)
			{
				pImage[j * channels + k] = gGammaLUT[(int)pImage[j * channels + k]];
			}
		}
	}
}