#include "custom_hough.h"

Mat hough_lines_acc(Mat &edgeImage, Mat &originalImage)
{
	// Construct and initialize hough bins
	// Assuming rows and columns divide evenly
	int d_max = sqrt(edgeImage.rows*edgeImage.rows + edgeImage.cols*edgeImage.cols);
	int theta_min = -90;
	int theta_max = 90;
	int d_res = 1;
	int theta_res = 1;

	Mat hough_image(d_max, theta_max - theta_min, CV_8UC1, Scalar(0, 0, 0));

	// Iterate through edge pixels
	vector<Point> edge_locs;
	findNonZero(edgeImage, edge_locs);

	cout << "NUMBER OF EDGE PIXELS FOUND = " << edge_locs.size() << endl;

	for (int i = 0; i < edge_locs.size(); i++)
	{
		Point curr_pt = edge_locs[i];
		int x = curr_pt.x;
		int y = curr_pt.y;
		int curr_d;

		for (int theta = theta_min; theta < theta_max; theta += theta_res)
		{
			curr_d = abs(x*cos(theta*PI / 180.0) + y*sin(theta*PI / 180.0));
			//cout << "CURRENT D=" << curr_d << endl;

			// Find bin and increment it
			int bin_iterator = 0;
			while (bin_iterator < curr_d)
			{
				bin_iterator += d_res;
			}

			if (bin_iterator >= d_res)
				bin_iterator -= d_res;

			//cout << "CORRESPONDING BIN=" << bin_iterator << endl;
			hough_image.at<uchar>(bin_iterator, theta - theta_min) += 1;
		}
	}

	threshold(hough_image, hough_image, 100, 1.0, CV_THRESH_BINARY);
	vector<Point> peak_locs;
	findNonZero(hough_image, peak_locs);

	cout << "NUMBER OF PEAKS FOUND=" << peak_locs.size() << endl;

	// Remove redundant peaks
	int d_merge_bound = 10;
	int theta_merge_bound = 10;
	for (int i = 0; i < peak_locs.size(); i++)
	{
		Point curr_pt = peak_locs[i];
		int curr_theta = peak_locs[i].x + theta_min;
		int curr_d = peak_locs[i].y;
		int curr_index = i;
		int curr_votes = hough_image.at<uchar>(curr_pt);

		//cout << "THETA = " << peak_theta << ", RHO = " << peak_d << endl;

		// Find similar points, compare votes, remove the loser
		for (int j = 0; j < peak_locs.size(); j++)
		{
			Point comparison_pt = peak_locs[j];
			int comparison_theta = peak_locs[j].x + theta_min;
			int comparison_d = peak_locs[j].y;
			int comparison_index = j;
			
			// If similar ...
			if ((i != j) && (abs(curr_theta - comparison_theta) < theta_merge_bound && abs(curr_d - comparison_d) < d_merge_bound))
			{
				// Compare votes
				if (hough_image.at<uchar>(comparison_pt) > curr_votes)
				{
					peak_locs.erase(peak_locs.begin() + curr_index);
					curr_pt = comparison_pt;
					curr_theta = comparison_theta;
					curr_d = comparison_d;
					curr_index = comparison_index;
				}
				else
				{
					peak_locs.erase(peak_locs.begin() + comparison_index);
				}
			}	
		}
	}

	// Remove lines without proximal points that are parallel
	theta_merge_bound = 2;
	d_merge_bound = 50;

	for (int i = 0; i < peak_locs.size(); i++)
	{
		bool counterpart_found = false;
		int curr_theta = peak_locs[i].x + theta_min;
		int curr_d = peak_locs[i].y;

		for (int j = 0; j < peak_locs.size(); j++)
		{
			if (i != j)
			{
				int comp_theta = peak_locs[j].x + theta_min;
				int comp_d = peak_locs[j].y;

				if (abs(comp_theta - curr_theta) < theta_merge_bound && abs(comp_d - curr_d) < d_merge_bound)
				{
					counterpart_found = true;
					break;
				}
			}
		}

		if (!counterpart_found)
		{
			peak_locs.erase(peak_locs.begin() + i);
			i--;
		}
	}
	cout << "REMAINING PEAKS = " << peak_locs.size() << endl;
	for (int i = 0; i < peak_locs.size(); i++)
	{
		Point curr_pt = peak_locs[i];
		int peak_theta = peak_locs[i].x + theta_min;
		int peak_d = peak_locs[i].y;

		int x_a, x_b, y_a, y_b;
		// Draw lines
		if (peak_theta == 0)
		{
			x_a = peak_d;
			x_b = peak_d;

			y_a = 0;
			y_b = edgeImage.rows;
		}
		else if (abs(peak_theta) == 90)
		{
			x_a = 0;
			x_b = edgeImage.cols;

			y_a = peak_d;
			y_b = peak_d + (cos(peak_theta*PI / 180.0) / sin(peak_theta*PI / 180.0));
		}
		else
		{
			x_a = 0;
			y_a = peak_d / sin(peak_theta*PI / 180.0);
			x_b = edgeImage.cols;
			y_b = y_a - x_b / tan(peak_theta*PI / 180.0);
		}
		line(originalImage, Point(x_a, y_a), Point(x_b, y_b), Scalar(0, 255, 0));
	}

	//imshow("Line Image", originalImage);
	//waitKey(0);
	//destroyAllWindows();

	return originalImage;
}

Mat find_circles(Mat &inputImg, Mat &originalImage, int radiusMin, int radiusMax)
{
	for (int radius = radiusMin; radius <= radiusMax; radius++)
	{
		originalImage = hough_circles_acc(inputImg, originalImage, radius);
	}

	return originalImage;
}

Mat hough_circles_acc(Mat &edgeImage, Mat &originalImage, int radiusPx)
{
	cout << "FINDING CIRCLES OF RADIUS=" << radiusPx << endl;

	// Construct and initialize hough bins
	// Assuming rows and columns divide evenly
	int r_min = 0;
	int r_max = edgeImage.rows;
	int r_res = 1;

	int c_min = 0;
	int c_max = edgeImage.cols;
	int c_res = 1;

	// hough_image is scaled relative to edgeImage to increase resolution 
	Mat hough_image((r_max - r_min) / r_res, (c_max - c_min) / c_res, CV_8UC1, Scalar(0, 0, 0));

	// Iterate through edge pixels
	vector<Point> edge_locs;
	findNonZero(edgeImage, edge_locs);

	//cout << "NUMBER OF EDGE PIXELS FOUND = " << edge_locs.size() << endl;

	double theta_res = 1.0;
	for (int i = 0; i < edge_locs.size(); i++)
	{
		Point curr_pt = edge_locs[i];
		int x = curr_pt.x;
		int y = curr_pt.y;

		int last_x = 0, last_y = 0;
		for (int theta = 0; theta < 360; theta += theta_res)
		{
			int x_bin = round(x + radiusPx*cos(theta*PI / 180.0));
			int y_bin = round(y - radiusPx*sin(theta*PI / 180.0));
			//cout << "CURRENT D=" << curr_d << endl;

			if (x_bin >= edgeImage.cols || y_bin >= edgeImage.rows || x_bin < 0 || y_bin < 0)
				continue;

			//cout << "CORRESPONDING BIN=" << bin_iterator << endl;
			if (theta == 0)
			{
				last_x = x_bin;
				last_y = y_bin;

				hough_image.at<uchar>(y_bin, x_bin) += 1;
			}
			else
			{
				if ((x_bin != last_x) || (y_bin != last_y))
					hough_image.at<uchar>(y_bin, x_bin) += 1;

				last_x = x_bin;
				last_y = y_bin;
			}

		}
	}

	//imshow("Hough Circle Image", hough_image);
	//waitKey(0);

	threshold(hough_image, hough_image, 75, 1.0, CV_THRESH_BINARY);
	vector<Point> peak_locs;
	findNonZero(hough_image, peak_locs);

	cout << "NUMBER OF PEAKS FOUND=" << peak_locs.size() << endl;

	int merge_distance = 15;
	for (int i = 0; i < peak_locs.size(); i++)
	{
		Point curr_pt = peak_locs[i];

		// Check redundancy of this center
		for (int j = 0; j < peak_locs.size(); j++)
		{
			if (i != j)
			{
				Point distance_vec = curr_pt - peak_locs[j];
				int distance = sqrt(distance_vec.x*distance_vec.x + distance_vec.y*distance_vec.y);

				if (distance < merge_distance)
					peak_locs.erase(peak_locs.begin() + j);
			}
		}
	}

	for (int i = 0; i < peak_locs.size(); i++)
	{
		circle(originalImage, peak_locs[i], radiusPx, Scalar(0, 255, 0));
	}

	return originalImage;
}