#include <opencv\cv.hpp>

using namespace cv;

int main()
{
	// Load and show originals
	Mat img1 = imread("ps0-1-a-1.png", CV_LOAD_IMAGE_COLOR);
	Mat img2 = imread("ps0-1-a-2.png", CV_LOAD_IMAGE_COLOR);

	namedWindow("img1");
	namedWindow("img2");
	imshow("img1", img1);
	imshow("img2", img2);

	waitKey(0);
	//destroyAllWindows();

	// 2 - Swaps, Monochrome
	Mat img1_bgr[3];
	split(img1, img1_bgr);

	Mat img1_swapped = img1.clone();
	for (int i = 0; i < img1.rows; i++)
	{
		for (int j = 0; j < img1.cols; j++)
		{
			img1_swapped.at<Vec3b>(Point(j, i))[0] = (img1_bgr[1]).at<uchar>(i, j);
			img1_swapped.at<Vec3b>(Point(j, i))[1] = (img1_bgr[0]).at<uchar>(i, j);
		}
	}

	namedWindow("2A");
	imshow("2A", img1_swapped);
	waitKey(0);

	imshow("2A", img1_bgr[1]);
	waitKey(0);

	imshow("2A", img1_bgr[2]);
	waitKey(0);

	imshow("2A", img1_bgr[0]);
	waitKey(0);
	destroyAllWindows();

	/*
	// 3 - Replace face of the girl
	Rect2d face_roi_1 = selectROI(img1, true);
	int face_square_dim = 100;
	face_roi_1.width = face_square_dim;
	face_roi_1.height = face_square_dim;
	Mat face_img = img1(face_roi_1);

	Rect2d face_roi_2 = selectROI(img2, true);
	Mat face_swap = img2.clone();
	for (int i = 0; i < face_square_dim; i++)
	{
		for (int j = 0; j < face_square_dim; j++)
		{
			face_swap.at<Vec3b>(Point(face_roi_2.x + j, face_roi_2.y + i)) = face_img.at<Vec3b>(Point(j, i));
		}
	}
	imshow("2A", face_swap);
	waitKey(0);
	destroyAllWindows();
	*/

	// 4 - Arithmetic and Geometric Operations
	Mat img1_gray = imread("ps0-1-a-1.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img2_gray = imread("ps0-1-a-2.png", CV_LOAD_IMAGE_GRAYSCALE);

	double min, max;
	Point minLoc, maxLoc;
	minMaxLoc(img1_bgr[1], &min, &max, &minLoc, &maxLoc);

	Scalar mean, std;
	meanStdDev(img1_bgr[1], mean, std);

	// 5 - Noise
	//
	Mat img1_noise_1, img1_noise_2;

	Mat img1_green_blur;
	GaussianBlur(img1_bgr[1], img1_green_blur, Size(3, 3), 30, 30);

	Mat img1_blue_blur;
	GaussianBlur(img1_bgr[0], img1_blue_blur, Size(3, 3), 30, 30);

	std::vector<Mat> channels;
	channels.push_back(img1_bgr[0]);
	channels.push_back(img1_green_blur);
	channels.push_back(img1_bgr[2]);
	merge(channels, img1_noise_1);

	std::vector<Mat> channels_2;
	channels_2.push_back(img1_blue_blur);
	channels_2.push_back(img1_bgr[1]);
	channels_2.push_back(img1_bgr[2]);
	merge(channels_2, img1_noise_2);

	imshow("Normal", img1);
	imshow("Blurred Green Channel", img1_noise_1);
	imshow("Blurred Blue Channel", img1_noise_2);
	waitKey(0);

	return 0;
}