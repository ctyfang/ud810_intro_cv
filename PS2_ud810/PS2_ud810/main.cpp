#include "disparity_custom.h"



int main()
{
	// -------------------- SSD Matching -----------------------
	// Load Image 1
	Mat img0_L = imread("./input/pair0-L.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img0_R = imread("./input/pair0-R.png", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("Left", img0_L);
	imshow("Right", img0_R);
	waitKey(0);

	Mat img0_D_LR = mod_disparity_img(disparity_SSD(img0_L, img0_R), 64, 127);
	Mat img0_D_RL = mod_disparity_img(disparity_SSD(img0_R, img0_L), 64, 127);

	imshow("Disparity LR", img0_D_LR);
	imshow("Disparity RL", img0_D_RL);
	waitKey(0);

	// Load Image 2
	Mat img1_L = imread("./input/pair1-L.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img1_R = imread("./input/pair1-R.png", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("Left", img1_L);
	imshow("Right", img1_R);
	waitKey(0);

	Mat img1_D_LR = mod_disparity_img(disparity_SSD(img1_L, img1_R), 25, 127);
	Mat img1_D_RL = mod_disparity_img(disparity_SSD(img1_R, img1_L), 25, 127);
	imshow("Disparity LR", img1_D_LR);
	imshow("Disparity RL", img1_D_RL);

	Mat img1_DL = imread("./input/pair1-D_L.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img1_DR = imread("./input/pair1-D_R.png", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("Disparity LR - GT", img1_DL);
	imshow("Disparity RL - GT", img1_DR);
	waitKey(0);

	return 0;
}