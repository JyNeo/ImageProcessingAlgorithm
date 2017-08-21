/**
  * @author:          yangjun
  * @date             2017/8/21 19:16
  * @description:     This transformation maps a narrow range of low intensity values in the input into a wider range of output levels.
  */ 

#include <highgui/highgui.hpp>    
#include <imgproc/imgproc.hpp>    
#include <core/core.hpp>   

using namespace cv;

//使用对数变换来扩展图像中的暗像素的值，同时压缩更高灰度级的值
int main(int argc, char *argv[])
{
	Mat image = imread("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0305(a)(DFT_no_log).tif");
	Mat imageLog(image.size(), CV_32FC3);
	
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			imageLog.at<Vec3f>(i, j)[0] = log(1 + image.at<Vec3b>(i, j)[0]);
			imageLog.at<Vec3f>(i, j)[1] = log(1 + image.at<Vec3b>(i, j)[1]);
			imageLog.at<Vec3f>(i, j)[2] = log(1 + image.at<Vec3b>(i, j)[2]);
		}
	}

	// 归一化到 0 ~ 255  
	normalize(imageLog, imageLog, 0, 255, CV_MINMAX);

	// 转换成 8 bit图像显示  
	convertScaleAbs(imageLog, imageLog);
	
	imshow("SrcImage", image);
	imshow("DstImage", imageLog);

	waitKey();
	return 0;
}
