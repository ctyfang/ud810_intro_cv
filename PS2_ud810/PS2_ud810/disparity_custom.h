#pragma once
#include <iostream>
#include <opencv\cv.hpp>
#include <opencv2\calib3d.hpp>
#include <map>

using namespace cv;
using namespace std;

Mat disparity_SSD(Mat img1, Mat img2);
Mat mod_disparity_img(Mat disparityImg, int scaleFactor, int shiftAmount);

struct window_dims
{
	int h_pos;
	int h_neg;
	int w_pos;
	int w_neg;
};

window_dims calc_window_dims(Size imgDims, int row, int col, int window_radius);