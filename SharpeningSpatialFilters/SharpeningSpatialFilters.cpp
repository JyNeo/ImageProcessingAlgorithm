/**
  * @author:          yangjun
  * @date             2017/8/29 14:47
  * @description:     �񻯴������ҪĿ����ͻ���ҶȵĹ��ɲ��֡�
					  ͼ��΢����ǿ��Ե������ͻ�䣬�������Ҷȱ仯����������
  */ 

//���˲������������˲���ǿͼ���ϸ��  
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
	int tem[9] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 }; //��ģ��
	//int tem[9] = { 1, 1, 1, 1, -8, 1, 1, 1, 1 };

	//���˲�  
	for (int i = 0; i < srcImage->height; i++) {
		for (int j = 0; j < srcImage->width; j++) {

			//�߽紦��  
			if (i == 0 || i == srcImage->height || j == 0 || j == srcImage->width) {
				ptr = (uchar *)srcImage->imageData + i * srcImage->widthStep + j;
				dst = (uchar *)laplaceImage->imageData + i * laplaceImage->widthStep + j;
				*dst = *ptr; //�߽�ֵ����Դͼ���ֵ  
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
				*dst = (uchar)((sum + 4) / 9); //����ֵ����������  
			}
		}
	}

	cvNamedWindow("IaplaceImage");
	cvShowImage("IaplaceImage", laplaceImage);

	//��������˹�˲����ͼ����б궨(�궨����ûʲôЧ��: data_min=0, data_max=255)
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

	//�񻯵�ͼ����Դͼ�����, ���������֮ǰ��Ҫ�Ƚ��񻯵�ֵ��΢����һЩ  
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