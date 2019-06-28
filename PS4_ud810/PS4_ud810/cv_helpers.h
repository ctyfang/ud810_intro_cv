#pragma once
#include <opencv\cv.hpp>

using namespace cv;

void Image_NMS(Mat &src, int radius)
{
	Mat return_mat = src.clone();
	double minVal, maxVal;
	Point minLoc, maxLoc;

	// Using a square window of size (2*radius+1, 2*radius+1) about each pixel, leave the maximum, set all other values to 0
	for (int u = radius; u < src.rows; u += (2*radius+1))
	{
		for (int v = radius; v < src.cols; v += (2 * radius + 1))
		{
			int r_max = u + radius;
			int c_max = v + radius;

			if (u + radius >= src.rows)
				r_max = src.rows - 1;
			
			if (v + radius >= src.cols)
				c_max = src.cols - 1;

			Range rows(u - radius, r_max);
			Range cols(v - radius, c_max);
			Mat current_window = src(rows, cols);
			Mat kernel = Mat::zeros(current_window.size(), CV_8U);

			minMaxLoc(current_window, &minVal, &maxVal, &minLoc, &maxLoc);
			kernel.at<uchar>(maxLoc) = 1;
			multiply(kernel, current_window, current_window, 1.0, CV_64F);
		}
	}
}