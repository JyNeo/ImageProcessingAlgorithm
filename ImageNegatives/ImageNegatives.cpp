/**
  * @author:          yangjun
  * @date             2017/8/21 18:37
  * @description:     This type of processing is particularly suited for enhancing white or gray detail embedded in dark regions of an image,
  *                   especially when the black areas are dominant in size.
  */ 
#include <cv.h>
#include <highgui\highgui.hpp>  

using namespace cv;

//适用于增强嵌入在一幅图像的暗区域中的白色或灰色细节，特别是当黑色面积在尺寸上占主导地位时
int main(int argc, char *argv[])
{
	// 加载图片至内存  
	IplImage* srcImage = cvLoadImage("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0304(a)(breast_digital_Xray).tif");
	if (NULL == srcImage)
	{
		return 0;
	}

	cvNamedWindow("SrcImage");
	cvShowImage("SrcImage", srcImage);

	// 获取图片的一些属性  
	int height = srcImage->height;                // 图像高度  
	int width = srcImage->width;                  // 图像宽度（像素为单位）  
	int step = srcImage->widthStep;               // 相邻行的同列点之间的字节数  
	int channels = srcImage->nChannels;           // 颜色通道数目 (1,2,3,4)  
	uchar *data = (uchar *)srcImage->imageData;

	// 反色操作  
	for (int i = 0; i != height; ++i)
	{
		for (int j = 0; j != width; ++j)
		{
			for (int k = 0; k != channels; ++k)
			{
				data[i*step + j*channels + k] = 255 - data[i*step + j*channels + k];
			}
		}
	}

	// 将图片显示到对话框中 
	cvNamedWindow("DstImage");
	cvShowImage("DstImage", srcImage);

	cvWaitKey();

	// 释放内存与对话框销毁  
	cvReleaseImage(&srcImage);
	cvDestroyWindow("SrcImage");
	cvDestroyWindow("DstImage");

	return 0;
}