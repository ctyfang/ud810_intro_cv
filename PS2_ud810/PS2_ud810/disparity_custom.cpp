#include "disparity_custom.h"

bool compare_map_vals(const pair<int, int>&a, const pair<int, int>&b)
{
	return a.second < b.second;
}

int find_disparity_scanline(map<int, int> inputMap)
{
	return (min_element(inputMap.begin(), inputMap.end(), compare_map_vals))->first;
}

Mat mod_disparity_img(Mat disparityImg, int scaleFactor, int shiftAmount)
{
	return (disparityImg * scaleFactor) + shiftAmount;
}

window_dims calc_window_dims(Size imgDims, int row, int col, int window_radius)
{
	int window_h1 = window_radius, window_h2 = window_radius;

	if (row - window_radius < 0)
		window_h1 = window_radius + (row - window_radius);

	if (row + window_radius >= imgDims.height)
		window_h2 = window_radius - (row + window_radius - (imgDims.height - 1));

	int window_w1 = window_radius, window_w2 = window_radius;

	if (col - window_radius < 0)
		window_w1 = window_radius + (col - window_radius);

	if (col + window_radius >= imgDims.width)
		window_w2 = window_radius - (col + window_radius - (imgDims.width - 1));

	window_dims window = { window_h1, window_h2, window_w1, window_w2 };
	return window;
}

Mat disparity_SSD(Mat img1, Mat img2)
{
	Mat disparity_img(img1.size(), CV_8SC1, Scalar(0,0,0));

	static Ptr<StereoBM> stereoObj = StereoBM::create();
	stereoObj->setMinDisparity(0);
	stereoObj->compute(img1, img2, disparity_img);
	
	/*
	int block_size = 15;
	int block_radius = floor(block_size/2);
	for (int r = 0; r < img1.rows; r++)
	{
		for (int c = 0; c < img1.cols; c++)
		{
			// Create window - extract template from that pixel location
			// Get max dims in four directions
			int c_left = 0, c_right = 0;
			
			window_dims window = calc_window_dims(img1.size(), r, c, block_radius);
			// Get scan range
			c_left = window.w_pos;
			c_right = (img1.cols - 1 - window.w_neg);
			
			Mat curr_template = img1(Range(r - window.h_pos, r + window.h_neg), Range(c - window.w_pos, c + window.w_neg));
			
			map<int, int> ssd_scanline;
			// Scan left
			for (int c_scan = c_left; c_scan <= c_right; c_scan++)
			{
				// Extract template from scan-line location
				Mat curr_source = img2(Range(r - window.h_pos, r + window.h_neg), Range(c_scan - window.w_pos, c_scan + window.w_neg));

				//imshow("Source", curr_source);
				//imshow("Temp", curr_template);
				//waitKey(0);

				// Calculate SSD
				Mat abs_diff(window.h_neg + window.h_pos, window.w_pos + window.w_neg, CV_8SC1);
				absdiff(curr_template, curr_source, abs_diff);

				//cout << "SUM = " << sum(abs_diff)[0] << endl;

				Mat sqr_diff;
				pow(abs_diff, 2.0, sqr_diff);

				// Append SSD to map with disparity
				int disparity = c_scan - c;
				int ssd = sum(sqr_diff)[0];
				ssd_scanline.insert(pair<int, int>(disparity, ssd));
			}
			
			if (ssd_scanline.size() == 0) {
				cout << "Row = " << r << " Col = " << c << endl;
			}

			// Find min disparity and write pixel value to disparity image
			int min_ssd_disparity = find_disparity_scanline(ssd_scanline);
			disparity_img.at<uchar>(r, c) = min_ssd_disparity;
			//cout << "Disparity at r=" << r << ", c=" << c << " is D=" << find_disparity_scanline(ssd_scanline) << endl;
		}
	}
	*/
	return disparity_img;
}
