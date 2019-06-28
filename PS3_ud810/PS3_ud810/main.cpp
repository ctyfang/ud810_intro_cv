#include "custom_calib.h"

int main()
{
	// Q1 - Direct Linear Calibration with known points (2D and corresponding 3D)
	vector<Vec3d> points_2D = load_2D_points("./input/pts2d-pic_a.txt");
	vector<Vec4d> points_3D = load_3D_points("./input/pts3d.txt");

	// CONSTRUCT "A" MATRIX
	Mat A = build_A_matrix(points_2D, points_3D);
	cout << A << endl;
	// Check that SVD method works
	/*
	Mat SVD_test(3, 4, CV_64F);
	SVD_test.at<double>(0, 0) = 1;
	SVD_test.at<double>(0, 1) = 1;
	SVD_test.at<double>(0, 2) = 0;
	SVD_test.at<double>(0, 3) = 1;

	SVD_test.at<double>(1, 0) = 0;
	SVD_test.at<double>(1, 1) = 0;
	SVD_test.at<double>(1, 2) = 0;
	SVD_test.at<double>(1, 3) = 1;

	SVD_test.at<double>(2, 0) = 1;
	SVD_test.at<double>(2, 1) = 1;
	SVD_test.at<double>(2, 2) = 0;
	SVD_test.at<double>(2, 3) = 0;

	Mat w, u, vt;
	SVD::compute(SVD_test, w, u, vt);
	cout << w << endl;
	*/

	
	// SVD to find M matrix
	Mat w, u, vt;
	SVD::compute(A, w, u, vt);
	//cout << w << endl;

	cout << vt << endl;
	transpose(vt, vt);
	
	//cout << vt(Range(0, vt.rows), Range(vt.cols - 1, vt.cols)) << endl;

	//Mat M = vt(Range(0, vt.rows), Range(vt.cols - 1, vt.cols));
	Mat M = vt(Range(0, vt.rows), Range(vt.cols - 2, vt.cols - 1));
	cout << M << endl;
	M = M.clone();
	M = M.reshape(1, 3);
	cout << M << endl;
	M /= M.at<double>(2, 3);
	cout << M << endl;

	// CHECK ON NORM POINTS
	vector<Vec3d> points_2D_norm = load_2D_points("./input/pts2d-norm-pic_a.txt");
	vector<Vec4d> points_3D_norm = load_3D_points("./input/pts3d-norm.txt");

	Mat single_3D_pt(points_3D_norm[points_3D_norm.size() - 1]);
	cout << single_3D_pt << endl;
	Mat result = M*single_3D_pt;
	cout << result << endl;
	cout << points_2D_norm[points_2D_norm.size() - 1] << endl;
	

	// Least Squares to find M Matrix

	return 0;
}