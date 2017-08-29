/**
  * @author:          yangjun
  * @date             2017/8/29 14:47
  * @description:     锐化处理的主要目的是突出灰度的过渡部分。
					  图像微分增强边缘和其他突变，而削弱灰度变化缓慢的区域。
  */ 

//锐化滤波，并利用锐化滤波增强图像的细节  
#include <core/core.hpp>
#include <highgui/highgui.hpp>  

int main() {
	IplImage *srcImage = cvLoadImage("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0338(a)(blurry_moon).tif", 0);
	if (srcImage==NULL)
	{
		return 0;
	}
	
	cvNamedWindow("SrcImage", CV_WINDOW_AUTOSIZE);
	cvShowImage("SrcImage", srcImage);

	uchar *ptr, *dst;
	IplImage *laplaceImage = cvCreateImage(cvGetSize(srcImage), srcImage->depth, 1);
	IplImage *dstImage = cvCreateImage(cvGetSize(srcImage), srcImage->depth, 1);

	int sum;    
	int tem[9] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 }; //锐化模板
	//int tem[9] = { 1, 1, 1, 1, -8, 1, 1, 1, 1 };

	//锐化滤波  
	for (int i = 0; i < srcImage->height; i++) {
		for (int j = 0; j < srcImage->width; j++) {

			//边界处理  
			if (i == 0 || i == srcImage->height || j == 0 || j == srcImage->width) {
				ptr = (uchar *)srcImage->imageData + i * srcImage->widthStep + j;
				dst = (uchar *)laplaceImage->imageData + i * laplaceImage->widthStep + j;
				*dst = *ptr; //边界值赋予源图像的值  
			}
			else {
				sum = 0;
				for (int m = -1; m <= 1; m++) {
					for (int n = -1; n <= 1; n++) {
						ptr = (uchar *)srcImage->imageData + (i + m) * srcImage->widthStep + j + n;
						sum += (*ptr) * tem[3 * (m + 1) + n + 1];
					}
				}
				dst = (uchar *)laplaceImage->imageData + i * laplaceImage->widthStep + j;
				*dst = (uchar)((sum + 4) / 9); //赋新值，四舍五入  
			}
		}
	}

	cvNamedWindow("IaplaceImage");
	cvShowImage("IaplaceImage", laplaceImage);

	//对拉普拉斯滤波后的图像进行标定(标定好像没什么效果: data_min=0, data_max=255)
	//uchar data_max = 0, data_min = 255;
	//for (int i = 0; i < laplaceImage->height; i++)
	//{
	//	for (int j = 0; j < laplaceImage->width; j++)
	//	{
	//		ptr = (uchar *)laplaceImage->imageData + i * laplaceImage->widthStep + j;
	//		if (*ptr > data_max)
	//			data_max = *ptr;
	//		if (*ptr < data_min)
	//			data_min = *ptr;
	//	}
	//}

	//for (int i = 0; i < laplaceImage->height; i++)
	//{
	//	for (int j = 0; j < laplaceImage->width; j++)
	//	{
	//		ptr = (uchar *)laplaceImage->imageData + i * laplaceImage->widthStep + j;
	//		*ptr = ((*ptr) - data_min) * 255 / (data_max - data_min);
	//	}
	//}

	//锐化的图像与源图像相加, 但是在相加之前需要先将锐化的值稍微降低一些  
	for (int i = 0; i < laplaceImage->height; i++) {
		for (int j = 0; j < laplaceImage->width; j++) {
			ptr = (uchar *)laplaceImage->imageData + i * laplaceImage->widthStep + j;
			*ptr = ((*ptr) + 4) / 5;
		}
	}

	cvNamedWindow("After IaplaceImage");
	cvShowImage("After IaplaceImage", laplaceImage);

	cvAdd(srcImage, laplaceImage, dstImage, 0);
	cvNamedWindow("DstImage");
	cvShowImage("DstImage", dstImage);

	cvWaitKey(0);
	cvReleaseImage(&srcImage);
	cvReleaseImage(&dstImage);
	cvReleaseImage(&laplaceImage);
	cvDestroyAllWindows();
	return 0;
}