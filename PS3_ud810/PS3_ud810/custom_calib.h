#pragma once
#include <fstream>
#include <iostream>
#include <opencv\cv.hpp>

using namespace cv;
using namespace std;

Mat build_A_matrix(vector<Vec3d> points_2D, vector<Vec4d> points_3D)
{
	Mat A(2 * points_2D.size(), 12, CV_64F, Scalar(0, 0, 0));
	for (int pt_index = 0; pt_index < points_3D.size(); pt_index++)
	{
		A.at<double>(2 * pt_index, 0) = points_3D[pt_index][0];
		A.at<double>(2 * pt_index, 1) = points_3D[pt_index][1];
		A.at<double>(2 * pt_index, 2) = points_3D[pt_index][2];
		A.at<double>(2 * pt_index, 3) = points_3D[pt_index][3];

		A.at<double>(2 * pt_index, 8) = -points_2D[pt_index][0] * points_3D[pt_index][0];
		A.at<double>(2 * pt_index, 9) = -points_2D[pt_index][0] * points_3D[pt_index][1];
		A.at<double>(2 * pt_index, 10) = -points_2D[pt_index][0] * points_3D[pt_index][2];
		A.at<double>(2 * pt_index, 11) = -points_2D[pt_index][0];

		A.at<double>(2 * pt_index + 1, 4) = points_3D[pt_index][0];
		A.at<double>(2 * pt_index + 1, 5) = points_3D[pt_index][1];
		A.at<double>(2 * pt_index + 1, 6) = points_3D[pt_index][2];
		A.at<double>(2 * pt_index + 1, 7) = points_3D[pt_index][3];

		A.at<double>(2 * pt_index + 1, 8) = -points_2D[pt_index][1] * points_3D[pt_index][0];
		A.at<double>(2 * pt_index + 1, 9) = -points_2D[pt_index][1] * points_3D[pt_index][1];
		A.at<double>(2 * pt_index + 1, 10) = -points_2D[pt_index][1] * points_3D[pt_index][2];
		A.at<double>(2 * pt_index + 1, 11) = -points_2D[pt_index][1];
	}

	return A;
}

vector<Vec3d> load_2D_points(string filepath)
{
	vector<Vec3d> points_2D;

	ifstream infile_2Dpts;
	infile_2Dpts.open(filepath);

	if (!infile_2Dpts.is_open())
		return points_2D;

	//cout << "Q1 - READING 2D POINTS" << endl;
	Vec3d curr_2D_pt;
	curr_2D_pt[2] = 1;
	string curr_line;
	while (std::getline(infile_2Dpts, curr_line))
	{
		stringstream ss(curr_line);
		ss >> curr_2D_pt[0];
		ss >> curr_2D_pt[1];
		//cout << "X = " << curr_2D_pt[0] << ", Y = " << curr_2D_pt[1] << endl;
		points_2D.push_back(curr_2D_pt);
	}
	infile_2Dpts.close();
	int num_2D_pts = points_2D.size();

	return points_2D;
}

vector<Vec4d> load_3D_points(string filepath)
{
	vector<Vec4d> points_3D;

	//cout << "READING 3D POINTS" << endl;
	ifstream infile_3Dpts;
	infile_3Dpts.open(filepath);

	if (!infile_3Dpts.is_open())
		return points_3D;

	Vec4d curr_3D_pt;
	curr_3D_pt[3] = 1;
	string curr_line;
	while (std::getline(infile_3Dpts, curr_line))
	{
		stringstream ss(curr_line);
		ss >> curr_3D_pt[0];
		ss >> curr_3D_pt[1];
		ss >> curr_3D_pt[2];
		//cout << "X = " << curr_3D_pt[0] << ", Y = " << curr_3D_pt[1] << ", Z = " << curr_3D_pt[2] << endl;
		points_3D.push_back(curr_3D_pt);
	}
	infile_3Dpts.close();
	int num_3D_pts = points_3D.size();

	return points_3D;
}