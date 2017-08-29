/**
  * @author:          yangjun
  * @date             2017/8/29 11:44
  * @description:     平滑滤波器用于模糊处理和降低噪声。
					  平滑线性空间滤波器的输出是包含在滤波器模板邻域内的像素的简单平均值
                      均值滤波器的主要应用是去除图像中的不相关细节，“不相关”是指与滤波器模板尺寸相比比较小的像素区域
					  均值滤波在去噪声的同时会有如下缺点：
					  1、边界模糊效应明显
					  2、细节丢失比较严重
  */ 
 
#include <highgui/highgui.hpp>
#include <core/core.hpp>

int main() {
	IplImage *srcImage = cvLoadImage("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0335(a)(ckt_board_saltpep_prob_pt05).tif", 0);
	if (srcImage == NULL)
	{
		return 0;
	}

	cvNamedWindow("SrcImage", CV_WINDOW_AUTOSIZE);
	cvShowImage("SrcImage", srcImage);

	IplImage *dstImage = cvCreateImage(cvGetSize(srcImage), srcImage->depth, 1);

	uchar *ptr, *dst;
	int sum;

	//模板系数  
	int tem[9] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };

	//均值滤波 3 * 3 模板的均值
	for (int i = 0; i < srcImage->height; i++) {
		for (int j = 0; j < srcImage->width; j++) {
			//边界处理  
			if (i == 0 || i == srcImage->height || j == 0 || j == srcImage->width) {
				ptr = (uchar *)srcImage->imageData + i * srcImage->widthStep + j;
				dst = (uchar *)dstImage->imageData + i * dstImage->widthStep + j;
				*dst = *ptr; //边界值赋予源图像的值  
			}
			else {
				sum = 0;
				for (int m = -1; m <= 1; m++) {
					for (int n = -1; n <= 1; n++) {
						ptr = (uchar *)srcImage->imageData + (i + m) * srcImage->widthStep + j + n;
						sum += (*ptr) * tem[3 * (m + 1) + n + 1]; //邻域内: 像素 * 模板系数
					}
				}

				dst = (uchar *)dstImage->imageData + i * dstImage->widthStep + j;
				*dst = (uchar)(sum / 9); //赋新值，四舍五入  
			}
		}
	}

	cvNamedWindow("DstImage");
	cvShowImage("DstImage", dstImage);

	cvWaitKey(0);
	cvReleaseImage(&srcImage);
	cvReleaseImage(&dstImage);
	cvDestroyAllWindows();
	return 0;
}