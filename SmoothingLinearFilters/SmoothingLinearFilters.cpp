/**
  * @author:          yangjun
  * @date             2017/8/29 11:44
  * @description:     ƽ���˲�������ģ������ͽ���������
					  ƽ�����Կռ��˲���������ǰ������˲���ģ�������ڵ����صļ�ƽ��ֵ
                      ��ֵ�˲�������ҪӦ����ȥ��ͼ���еĲ����ϸ�ڣ�������ء���ָ���˲���ģ��ߴ���ȱȽ�С����������
					  ��ֵ�˲���ȥ������ͬʱ��������ȱ�㣺
					  1���߽�ģ��ЧӦ����
					  2��ϸ�ڶ�ʧ�Ƚ�����
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

	//ģ��ϵ��  
	int tem[9] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };

	//��ֵ�˲� 3 * 3 ģ��ľ�ֵ
	for (int i = 0; i < srcImage->height; i++) {
		for (int j = 0; j < srcImage->width; j++) {
			//�߽紦��  
			if (i == 0 || i == srcImage->height || j == 0 || j == srcImage->width) {
				ptr = (uchar *)srcImage->imageData + i * srcImage->widthStep + j;
				dst = (uchar *)dstImage->imageData + i * dstImage->widthStep + j;
				*dst = *ptr; //�߽�ֵ����Դͼ���ֵ  
			}
			else {
				sum = 0;
				for (int m = -1; m <= 1; m++) {
					for (int n = -1; n <= 1; n++) {
						ptr = (uchar *)srcImage->imageData + (i + m) * srcImage->widthStep + j + n;
						sum += (*ptr) * tem[3 * (m + 1) + n + 1]; //������: ���� * ģ��ϵ��
					}
				}

				dst = (uchar *)dstImage->imageData + i * dstImage->widthStep + j;
				*dst = (uchar)(sum / 9); //����ֵ����������  
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