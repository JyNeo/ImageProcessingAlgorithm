/**
* @author:          yangjun
* @date             2017/8/29 11:44
* @description:     统计排序滤波器是一种非线性空间滤波器。
					这种滤波器的响应以滤波器包围的图像区域中所包含的像素的排序为基础，然后使用统计排序结果决定的值代替中心像素的值
					中值滤波器对处理脉冲噪声非常有效，该种噪声也称为“椒盐噪声”
*/

#include <core/core.hpp>
#include <highgui/highgui.hpp>  

int main() {
	IplImage *srcImage = cvLoadImage("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0335(a)(ckt_board_saltpep_prob_pt05).tif", 0);//以灰度图像的形式读入图片  
	if (srcImage == NULL)
	{
		return 0;
	}

	cvNamedWindow("SrcImage", CV_WINDOW_AUTOSIZE);
	cvShowImage("SrcImage", srcImage);

	uchar *ptr, *dst;
	int temp;
	IplImage *dstImage = cvCreateImage(cvGetSize(srcImage), srcImage->depth, 1);

	int tem[9] = { 0 }; //获取中值时用于排序 

	//中值滤波: 在去除噪声的同时，图像的模糊程度比较小，比均值滤波更加适合  
	for (int i = 0; i < srcImage->height; i++) {
		for (int j = 0; j < srcImage->width; j++) {

			//边界处理  
			if (i == 0 || i == srcImage->height || j == 0 || j == srcImage->width) {
				ptr = (uchar *)srcImage->imageData + i * srcImage->widthStep + j;
				dst = (uchar *)dstImage->imageData + i * dstImage->widthStep + j;
				*dst = *ptr; //边界值赋予源图像的值  
			}
			else {
				temp = 0;
				//将3 * 3模板覆盖的值拷贝进数组，便于查找中值
				for (int m = -1; m <= 1; m++) {
					for (int n = -1; n <= 1; n++) {
						ptr = (uchar *)srcImage->imageData + (i + m)*srcImage->widthStep + j + n;
						tem[3 * (m + 1) + n + 1] = *ptr;
					}
				}
				//对数组进行冒泡排序
				for (int r = 0; r < 8; r++) {
					for (int s = 0; s < r - 1; s++) {
						if (tem[s] > tem[s + 1]) {
							temp = tem[s];
							tem[s] = tem[s + 1];
							tem[s + 1] = temp;
						}
					}
				}
				//对新图赋予新值
				dst = (uchar *)dstImage->imageData + i * dstImage->widthStep + j;
				*dst = (uchar)(tem[4]); //赋新值  
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