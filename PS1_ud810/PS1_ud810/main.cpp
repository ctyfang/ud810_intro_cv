#include "custom_hough.h"

int main()
{
	// Canny Edge Detector Params
	int high_threshold;
	int ratio;
	int low_threshold;
	int kernel_size;

	// Q1
	/*
	Mat bw_squares = imread("./input/ps1-input0.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat rgb_squares = imread("./input/ps1-input0.png", CV_LOAD_IMAGE_COLOR);
	imshow("Q1 Original", bw_squares);
	//waitKey(0);
	//destroyAllWindows();

	Mat q1_edges = bw_squares.clone();
	int high_threshold = 150;
	int ratio = 3;
	int low_threshold = high_threshold / ratio;
	int kernel_size = 5;

	GaussianBlur(bw_squares, q1_edges, Size(15, 15), 2, 2); // blur with MATLAB canny's default parameters
	Canny(q1_edges, q1_edges, low_threshold, high_threshold, kernel_size);
	cout << "EDGE IMAGE GENERATED USING CANNY" << endl;

	imshow("Q1 Edges", q1_edges);
	//waitKey(0);
	destroyAllWindows();
	//imwrite("./output/ps1-1-a-1.png", q1_edges);
	*/
	
	// Q2
	// Implement hough transform for finding lines
	// Coordinate system has origin, such that top-left pixel has coordinate (0,0), y-axis points down, x-axis point right
	// At each point, for incremental theta values, calculate rho = xcostheta + ysintheta

	// Q2 - A - write hough_lines_acc(img, theta-bins optional, rho-bins optional) that computes the hough transform lines
	// and produces an accumulator array
	//hough_lines_acc(q1_edges, rgb_squares);

	/*
	// Q3 - Noisy Input Image
	Mat bw_squares_noisy = imread("./input/ps1-input0-noise.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat rgb_squares_noisy = imread("./input/ps1-input0-noise.png", CV_LOAD_IMAGE_COLOR);
	imshow("Original Noisy", bw_squares_noisy);
	GaussianBlur(bw_squares_noisy, bw_squares_noisy, Size(15, 15), 3.0, 3.0);
	medianBlur(bw_squares_noisy, bw_squares_noisy, 7);
	imshow("Median Blur 1", bw_squares_noisy);

	high_threshold = 80;
	ratio = 4;
	low_threshold = high_threshold / ratio;
	kernel_size = 3;
	Canny(bw_squares_noisy, bw_squares_noisy, low_threshold, high_threshold, kernel_size);
	imshow("Smoothed Edges", bw_squares_noisy);
	waitKey(0);

	hough_lines_acc(bw_squares_noisy, rgb_squares_noisy);
	*/

	// Q4 - Find Pen Lines
	Mat mono_q4 = imread("./input/ps1-input1.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat rgb_q4 = imread("./input/ps1-input1.png", CV_LOAD_IMAGE_COLOR);
	imshow("Q4 Original", mono_q4);
	//waitKey(0);

	GaussianBlur(mono_q4, mono_q4, Size(15, 15), 2.0, 2.0);
	high_threshold = 80;
	ratio = 5;
	low_threshold = high_threshold / ratio;
	kernel_size = 3;
	Canny(mono_q4, mono_q4, low_threshold, high_threshold, kernel_size);

	// NON-Q - Testing on Cube Images from MECH 423
	Mat mono_cube = imread("./input/cube_original.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat rgb_cube = imread("./input/cube_original.png", CV_LOAD_IMAGE_COLOR);

	GaussianBlur(mono_cube, mono_cube, Size(7,7), 2.0, 2.0);
	imshow("Blur Cube", mono_cube);

	high_threshold = 80;
	ratio = 4;
	low_threshold = high_threshold / ratio;
	kernel_size = 3;
	Canny(mono_cube, mono_cube, low_threshold, high_threshold, kernel_size);
	imshow("Edge Cube", mono_cube);

	Mat lines_cube = hough_lines_acc(mono_cube, rgb_cube);
	imshow("Cube Lines", lines_cube);
	waitKey(0);

	Mat mono_cube_r = imread("./input/cube_render.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat rgb_cube_r = imread("./input/cube_render.png", CV_LOAD_IMAGE_COLOR);

	GaussianBlur(mono_cube_r, mono_cube_r, Size(21, 21), 2.0, 2.0);
	imshow("Blur Cube Render", mono_cube_r);

	high_threshold = 80;
	ratio = 3;
	low_threshold = high_threshold / ratio;
	kernel_size = 3;
	Canny(mono_cube_r, mono_cube_r, low_threshold, high_threshold, kernel_size);
	imshow("Edge Cube Render", mono_cube_r);

	Mat lines_cube_2 = hough_lines_acc(mono_cube_r, rgb_cube_r);
	imshow("Cube Lines Render", lines_cube_2);
	waitKey(0);

	//imshow("Q4 Edges", mono_q4);
	//waitKey(0);

	// Q5 - Find Pen Circles
	hough_lines_acc(mono_q4, rgb_q4);
	//Mat circle_img = find_circles(mono_q4, rgb_q4, 15, 50);
	//imshow("Circles Found in Range", circle_img);
	//imwrite("./output/ps1-5-b-1.png", circle_img);
	//waitKey(0);

	// Q5 - Noisy Pen Inputs
	Mat mono_q4_noise = imread("./input/ps1-input2.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat rgb_q4_noise = imread("./input/ps1-input2.png", CV_LOAD_IMAGE_COLOR);
	imshow("Noisy Pen Original", mono_q4_noise);
	waitKey(0);

	Mat smooth_q4_noise = mono_q4_noise.clone();
	GaussianBlur(mono_q4_noise, smooth_q4_noise, Size(15, 15), 2.0, 2.0);
	high_threshold = 120;
	ratio = 3;
	low_threshold = high_threshold / ratio;
	kernel_size = 3;

	Mat edges_q4_noise = smooth_q4_noise.clone();
	Canny(smooth_q4_noise, edges_q4_noise, low_threshold, high_threshold, kernel_size);
	imshow("Noisy Pen Edges", edges_q4_noise);
	waitKey(0);

	Mat lines_q4_noise = hough_lines_acc(edges_q4_noise, rgb_q4_noise);
	//imwrite("./output/ps1-6-c-1.png", lines_q4_noise);
	//imshow("Noisy Pen Lines", lines_q4_noise);
	//waitKey(0);
	//destroyAllWindows();

	//Mat circs_q4_noise = find_circles(edges_q4_noise, rgb_q4_noise, 15, 50);
	//imshow("Noisy Pen Circles", circs_q4_noise);
	//waitKey(0);

	Mat mono_q4_distorted = imread("./input/ps1-input3.png", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("Distorted Pen Image", mono_q4_distorted);
	waitKey(0);
	
	return 0;
}

