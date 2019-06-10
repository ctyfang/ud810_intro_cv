#pragma once
#include <algorithm>
#include <iostream>
#include <opencv\cv.hpp>

using namespace cv;
using namespace std;

const double PI = acos(-1.0);

Mat hough_lines_acc(Mat &inputImg, Mat &originalImage);
Mat hough_circles_acc(Mat &inputImg, Mat &originalImage, int radiusPx);
Mat find_circles(Mat &inputImg, Mat &originalImage, int radiusMin, int radiusMax);
