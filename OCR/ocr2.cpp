
/* 数字识别 - by:tx
* 原理：
*   这是一个简单的识别数字的例子，局限性是只能识别规范字体
*   比如本程序以一个包含0~9数字(Times New Roman字体，其他字体准确度可能会略有降低)的图片为模板，
*   然后对提取资源图片中每个数字，进行模板匹配
*   优点在于理论上可以识别任何规范的图形，例如车牌识别等
*/

#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

Mat frame, roi, temp;

void numRecognizer();

int main()
{
	VideoCapture cap(0);
	temp = imread("E:\\OCR\\Project_1\\Project_1\\1.jpg", 0);  // 模板，原图0~9

	while (cap.isOpened())
	{
		cap >> frame;
		roi = frame(Rect(20, 80, 600, 100)).clone();
		blur(frame, frame, Size(15, 15));
		roi.copyTo(frame(Rect(20, 80, 600, 100)));
		rectangle(frame, Point(20, 80), Point(620, 180), Scalar(0, 200, 0), 2);
		imshow("数字识别", frame);
		int key = waitKey(20);
		if (frame.empty() || key == 27)
			break;
		if (key == (int)'0')
			numRecognizer();
	}

	return 0;
}

void numRecognizer()
{
	Mat src;
	cvtColor(roi, src, CV_BGR2GRAY);
	threshold(src, src, 100, 255, THRESH_BINARY_INV);
	Mat element = getStructuringElement(MORPH_RECT, Size(1, 1));
	morphologyEx(src, src, MORPH_OPEN, element);

	imwrite("D:\\src\\num123.jpg", src);

	int row_begin, row_end, col_begin, col_end;  // 需要去掉带有数字的图片上下左右的黑边/白边，这四个数用于表示去掉黑边，数字区域在src中的行列坐标

	for (int i = 0; i < src.rows - 1; i++)
	{
		Mat row = src.row(i);
		int sum = 0;
		for (int j = 0; j < src.cols; j++)
		{
			sum += row.data[j];
		}
		if (sum)
		{
			row_begin = i;  // 得到 row_begin
			break;
		}
	}

	for (int i = src.rows; i > 0; i--)
	{
		Mat row = src.row(i - 1);
		int sum = 0;
		for (int j = 0; j < src.cols; j++)
		{
			sum += row.data[j];
		}
		if (sum)
		{
			row_end = i;
			break;
		}
	}

	for (int i = 0; i < src.cols - 1; i++)
	{
		Mat col = src.col(i).clone();
		int sum = 0;
		for (int j = 0; j < src.rows; j++)
		{
			sum += col.data[j];
		}
		if (sum)
		{
			col_begin = i;
			break;
		}
	}

	for (int i = src.cols; i > 0; i--)
	{
		Mat col = src.col(i - 1).clone();
		int sum = 0;
		for (int j = 0; j < src.rows; j++)
		{
			sum += col.data[j];
		}
		if (sum)
		{
			col_end = i;
			break;
		}
	}

	/******* 开始提取每一个数字，进行匹配识别 ********/

	Mat dst = src(Range(row_begin, row_end), Range(col_begin, col_end));  // 去掉黑边的图片
	resize(temp, temp, Size(cvRound(7.1 * dst.rows), dst.rows));  // 改变temp的尺寸，与dst等高

	int col_count_begin = 0;  // 从左向右分析图片中的数字，这两个变量用于分析当前分析到图片的哪一列
	int col_count_end = 0;
	bool end = false;

	while (!end)  // ★★★★ 开始循环 ★★★★
	{
		for (int i = col_count_end + 1; i < dst.cols - 1; i++)
		{
			Mat col = dst.col(i).clone();
			int sum = 0;
			for (int j = 0; j < dst.rows; j++)
			{
				sum += col.data[j];
			}
			if (!sum)
			{
				col_count_end = i;
				break;
			}
		}

		Mat tempImage;
		if (col_count_begin == col_count_end)
		{
			tempImage = dst(Range::all(), Range(col_count_begin, dst.cols - 1));
			end = true;
		}
		else
		{
			tempImage = dst(Range::all(), Range(col_count_begin, col_count_end));  // 取出一个数字
		}

		//cout << "(" << tempImage.rows << ", " << tempImage.cols << ")\t";

		Mat resultImage;
		matchTemplate(temp, tempImage, resultImage, 1);

		double minValue, maxValue;
		Point minLocation;
		minMaxLoc(resultImage, &minValue, &maxValue, &minLocation);

		float multiple = 80.0 / dst.rows;
		int num1 = 70;
		int num2 = 115;
		int num3 = 173;
		int num4 = 228;
		int num5 = 290;
		int num6 = 348;
		int num7 = 405;
		int num8 = 465;
		int num9 = 522;

		//cout << "minLocation.x = " << minLocation.x << "\t";

		if (minLocation.x < cvRound(15 / multiple)) {
			cout << "0  ";
		}
		else if (minLocation.x > cvRound((num1 - 15) / multiple) && minLocation.x < cvRound((num1 + 15) / multiple)) {
			cout << "1  ";
		}
		else if (minLocation.x > cvRound((num2 - 15) / multiple) && minLocation.x < cvRound((num2 + 15) / multiple)) {
			cout << "2  ";
		}
		else if (minLocation.x > cvRound((num3 - 15) / multiple) && minLocation.x < cvRound((num3 + 15) / multiple)) {
			cout << "3  ";
		}
		else if (minLocation.x > cvRound((num4 - 15) / multiple) && minLocation.x < cvRound((num4 + 15) / multiple)) {
			cout << "4  ";
		}
		else if (minLocation.x > cvRound((num5 - 15) / multiple) && minLocation.x < cvRound((num5 + 15) / multiple)) {
			cout << "5  ";
		}
		else if (minLocation.x > cvRound((num6 - 15) / multiple) && minLocation.x < cvRound((num6 + 15) / multiple)) {
			cout << "6  ";
		}
		else if (minLocation.x > cvRound((num7 - 15) / multiple) && minLocation.x < cvRound((num7 + 15) / multiple)) {
			cout << "7  ";
		}
		else if (minLocation.x > cvRound((num8 - 15) / multiple) && minLocation.x < cvRound((num8 + 15) / multiple)) {
			cout << "8  ";
		}
		else if (minLocation.x > cvRound((num9 - 15) / multiple) && minLocation.x < cvRound((num9 + 15) / multiple)) {
			cout << "9  ";
		}
		else {
			cout << "ERROR  ";
		}

		for (int i = col_count_end + 1; i < dst.cols - 1; i++)
		{
			Mat col = dst.col(i).clone();
			int sum = 0;
			for (int j = 0; j < dst.rows; j++)
			{
				sum += col.data[j];
			}
			if (sum)
			{
				col_count_begin = i;
				col_count_end = i;
				break;
			}
		}

		if (end)
			cout << endl << "----------------------------------" << endl;
	}
}