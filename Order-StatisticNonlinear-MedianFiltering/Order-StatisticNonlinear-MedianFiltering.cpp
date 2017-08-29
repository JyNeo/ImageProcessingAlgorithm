/**
* @author:          yangjun
* @date             2017/8/29 11:44
* @description:     ͳ�������˲�����һ�ַ����Կռ��˲�����
					�����˲�������Ӧ���˲�����Χ��ͼ�������������������ص�����Ϊ������Ȼ��ʹ��ͳ��������������ֵ�����������ص�ֵ
					��ֵ�˲����Դ������������ǳ���Ч����������Ҳ��Ϊ������������
*/

#include <core/core.hpp>
#include <highgui/highgui.hpp>  

int main() {
	IplImage *srcImage = cvLoadImage("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0335(a)(ckt_board_saltpep_prob_pt05).tif", 0);//�ԻҶ�ͼ�����ʽ����ͼƬ  
	if (srcImage == NULL)
	{
		return 0;
	}

	cvNamedWindow("SrcImage", CV_WINDOW_AUTOSIZE);
	cvShowImage("SrcImage", srcImage);

	uchar *ptr, *dst;
	int temp;
	IplImage *dstImage = cvCreateImage(cvGetSize(srcImage), srcImage->depth, 1);

	int tem[9] = { 0 }; //��ȡ��ֵʱ�������� 

	//��ֵ�˲�: ��ȥ��������ͬʱ��ͼ���ģ���̶ȱȽ�С���Ⱦ�ֵ�˲������ʺ�  
	for (int i = 0; i < srcImage->height; i++) {
		for (int j = 0; j < srcImage->width; j++) {

			//�߽紦��  
			if (i == 0 || i == srcImage->height || j == 0 || j == srcImage->width) {
				ptr = (uchar *)srcImage->imageData + i * srcImage->widthStep + j;
				dst = (uchar *)dstImage->imageData + i * dstImage->widthStep + j;
				*dst = *ptr; //�߽�ֵ����Դͼ���ֵ  
			}
			else {
				temp = 0;
				//��3 * 3ģ�帲�ǵ�ֵ���������飬���ڲ�����ֵ
				for (int m = -1; m <= 1; m++) {
					for (int n = -1; n <= 1; n++) {
						ptr = (uchar *)srcImage->imageData + (i + m)*srcImage->widthStep + j + n;
						tem[3 * (m + 1) + n + 1] = *ptr;
					}
				}
				//���������ð������
				for (int r = 0; r < 8; r++) {
					for (int s = 0; s < r - 1; s++) {
						if (tem[s] > tem[s + 1]) {
							temp = tem[s];
							tem[s] = tem[s + 1];
							tem[s + 1] = temp;
						}
					}
				}
				//����ͼ������ֵ
				dst = (uchar *)dstImage->imageData + i * dstImage->widthStep + j;
				*dst = (uchar)(tem[4]); //����ֵ  
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