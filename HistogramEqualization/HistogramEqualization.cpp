/**
  * @author:          yangjun
  * @date             2017/8/23 16:28
  * @description:     An image whose pixels tend to occupy the entire range of possible intensity levels and, in addition, tend to be distributed uniformly,
  *                   will have an appearance of high contrast and will exhibit a large variety of gray tones.
  */ 

#include <highgui/highgui.hpp>
#include <core/core.hpp>

//ֱ��ͼ���⻯: ���Ƕ�ͼ����з��������죬���·���ͼ�������ֵ��ʹһ���Ҷȷ�Χ�ڵ���������������ͬ  
//�ܹ�����Աȶȣ��Ӷ��ﵽͼ����ǿ��Ŀ�ģ���ͼ��������������ͼ��ֱ��ͼ�ԶԱȶȽ��е����ķ���
int main(int argc, char* argv[])
{
	int grayLevel[256] = { 0 };
	float CDFAraay[256] = { 0.f };

	IplImage* srcImage = cvLoadImage("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0320(1)(top_left).tif", 0);

	if (srcImage == NULL)
	{
		return 0;
	}

	cvNamedWindow("SrcImage");
	cvShowImage("SrcImage", srcImage);

	int width = srcImage->width;
	int height = srcImage->height;
	int step = srcImage->widthStep;

	//ͳ��ֱ��ͼ�ĻҶȼ��ֲ�
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			int tag = ((uchar*)(srcImage->imageData + i * step))[j];
			grayLevel[tag]++;
		}

	//����ֱ��ͼ���ۼƷֲ����̣����ڶ�ֱ��ͼ���о��⻯
	double temp = 0;
	for (int i = 0; i < 256; i++)
	{
		temp += grayLevel[i];
		CDFAraay[i] = temp * 255 / (width * height);
	}

	IplImage* dstImage = cvCreateImage(cvSize(width, height), 8, 1);

	//��һ��ֱ��ͼ
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			((uchar*)(dstImage->imageData + i * step))[j] = CDFAraay[((uchar*)(srcImage->imageData + i * step))[j]];
		}

	cvNamedWindow("DstImage");
	cvShowImage("DstImage", dstImage);
	cvWaitKey();

	cvDestroyWindow("SrcImage");
	cvDestroyWindow("DstImage");
	cvReleaseImage(&srcImage);
	cvReleaseImage(&dstImage);

	return 0;
}

/*********************************************************************************************************************************/

#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>
#include <core/core.hpp>

using namespace cv;

MatND getHistogram(const Mat image);
Mat getHistogramImage(const Mat image);

int main(int argc, char* argv[])
{
	Mat srcImage = imread("F:\\DIP3E_Original_Images\\DIP3E_Original_Images_CH03\\Fig0320(4)(bottom_left).tif", 0);
	if (!srcImage.data)
	{
		return 0;
	}

	MatND histogram = getHistogram(srcImage);

	namedWindow("SrcImage");
	imshow("SrcImage", srcImage);

	namedWindow("SrcImage Histogram");
	imshow("SrcImage Histogram", getHistogramImage(srcImage));

	//ֱ��ͼ���⻯
	uchar *data;
	int nl = srcImage.rows;
	int nc = srcImage.cols * srcImage.channels();

	int result = 0;//�µĻҶ�ֵ
	int total = srcImage.size().height * srcImage.size().width;//������

	//���ÿ���Ҷȸ��ʵ��ۼ�,���������¼���Ҷ�ֵʱ�ԻҶȸ����ظ������ۼӣ���Լʱ��
	for (int i = 1; i < 256; i++)
	{
		histogram.at<float>(i) = histogram.at<float>(i) + histogram.at<float>(i - 1);
	}

	//����ͼ�����¼���Ҷ�ֵ
	if (srcImage.isContinuous())
	{
		nc = nc * nl;
		nl = 1;
	}

	for (int j = 0; j < nl; j++)
	{
		data = srcImage.ptr<uchar>(j);
		for (int i = 0; i < nc; i++)
		{
			result = (int)(histogram.at<float>(data[i]) * 255 / total);
			if (result > 255)
				data[i] = 255;
			else if (result < 0)
				data[i] = 0;
			else
				data[i] = result;
		}
	}

	namedWindow("DstImage");
	imshow("DstImage", srcImage);

	namedWindow("DstImage Histogram");
	imshow("DstImage Histogram", getHistogramImage(srcImage));

	waitKey(0);
	return 0;
}

MatND getHistogram(const Mat image)
{
	MatND hist;
	int histSize[1] = { 256 };
	int channels[1] = { 0 };
	float hranges[2] = { 0.0, 255.0 };
	const float *ranges[1] = { hranges };
	
	//����ֱ��ͼ
	calcHist(
		&image,
		1,
		channels,
		Mat(),
		hist,
		1,
		histSize,
		ranges
	);
	return hist;
}

Mat getHistogramImage(const Mat image)
{
	int histSize[1] = { 256 };
	MatND hist = getHistogram(image);

	double maxVal = 0;
	double minVal = 0;

	//����ֱ��ͼ
	minMaxLoc(hist, &minVal, &maxVal, 0, 0);
	Mat histImg(histSize[0], histSize[0], CV_8U, Scalar(255));
	int hpt = static_cast<int>(0.9 * histSize[0]);

	for (int h = 0; h < histSize[0]; h++)
	{
		float binVal = hist.at<float>(h);
		int intensity = static_cast<int>(binVal * hpt / maxVal);
		line(histImg, Point(h, histSize[0]), Point(h, histSize[0] - intensity), Scalar::all(0));
	}
	return histImg;
}