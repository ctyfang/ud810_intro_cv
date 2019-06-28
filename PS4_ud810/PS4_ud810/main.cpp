// PS4ud810.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cv_helpers.h"
#include "feature_detectors.h"
#include <iostream>

Mat harris_response(Mat I_x, Mat I_y, Size window_size, double alpha)
{
	I_x /= 8;
	I_y /= 8;
	// R = det(M) - alpha * trace(M)^2
	// Use uniform unity window function

	Mat I_x2, I_xy, I_y2;
	multiply(I_x, I_x, I_x2);
	multiply(I_y, I_y, I_y2);
	multiply(I_x, I_y, I_xy);

	//Mat kernel = Mat::ones(window_size, CV_8U);
	Mat kernel = getGaussianKernel(3, 1.5);
	filter2D(I_x2, I_x2, -1, kernel);
	filter2D(I_y2, I_y2, -1, kernel);
	filter2D(I_xy, I_xy, -1, kernel);

	// Calculate harris value at each pixel
	Mat C;
	pow(I_xy, 2, C);

	Mat det, term1, term2;
	multiply(I_x2, I_y2, term1);
	pow(I_xy, 2, term2);
	subtract(term1, term2, det);

	Mat trace, term3, term4;
	add(I_x2, I_y2, term3);
	pow(term3, 2, term4);
	trace = alpha * term4;

	Mat H;
	subtract(det, trace, H);

 	imshow("HarrisValues", H);
	waitKey(0);

	

	// Use gaussian to sum the window
	//GaussianBlur(H, H, window_size, 1.5, 1.5);

	return H;
}

int main()
{
	/* Q1 - HARRIS CORNERS */
	// load images
	Mat simA = imread("./input/simA.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	//Mat simA = imread("./input/check.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat transA = imread("./input/transA.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	imshow("SimA Original", simA);
	imshow("TransA Original", transA);
	//waitKey(0);

	// smooth noise
	imshow("SimA Blur", simA);
	imshow("TransA Blur", transA);
	//waitKey(0);
	//destroyAllWindows();

	// apply sobel to get gradients (includes smoothing
	Mat simA_dx, simA_dy;
	Sobel(simA, simA_dx, CV_64F, 1, 0, 3);
	Sobel(simA, simA_dy, CV_64F, 0, 1, 3);

	Mat simA_collage;
	vector<Mat> simA_parts;
	//simA_parts.push_back(simA);
	simA_parts.push_back(simA_dx);
	simA_parts.push_back(simA_dy);
	hconcat(simA_parts, simA_collage);

	Mat transA_dx, transA_dy;
	Sobel(transA, transA_dx, CV_64F, 1, 0, 1);
	Sobel(transA, transA_dy, CV_64F, 0, 1, 1);

	Mat transA_collage;
	vector<Mat> transA_parts;
	//transA_parts.push_back(transA);
	transA_parts.push_back(transA_dx);
	transA_parts.push_back(transA_dy);
	hconcat(transA_parts, transA_collage);

	imshow("SimA Gradients", simA_collage);
	imshow("TransA Gradients", transA_collage);
	waitKey(0);
	destroyAllWindows();

	// Compute and Analyze Harris Values
	Mat R = harris_response(simA_dx, simA_dy, Size(3,3), 0.04);
	double minVal, maxVal;
	Point minLoc, maxLoc;
	minMaxLoc(R, &minVal, &maxVal, &minLoc, &maxLoc);

	imshow("Harris Response", R);
	waitKey(0);
	destroyAllWindows();

	// Threshold, apply as binary mask
	Mat R_thresh;
	threshold(R, R_thresh, 1000000, maxVal, CV_THRESH_BINARY);
	bitwise_and(R_thresh, R, R_thresh);
	imshow("Original", simA);
	imshow("Thresholded Harris", R_thresh);
	waitKey(0);

	Image_NMS(R_thresh, 3);
	imshow("NMS Harris", R_thresh);
	waitKey(0);
	return 0;
}

