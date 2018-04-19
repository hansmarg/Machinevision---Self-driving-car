#include <stdio.h>
#include <iostream>
#include <fstream> 
#include <vector>
#include <opencv2/opencv.hpp>


#include "filters.cpp"
//#include <cv_bridge>

using namespace std;
using namespace cv;

void test_sobel(cv::Mat frame){

	cv::Mat sob_frame;

	sob_frame = sobel_func(frame);

	imshow("Sobel", sob_frame);

	waitKey();

}

void test_gaussian_pyramid(cv::Mat frame){

	std::vector<cv::Mat> gauss_pyr = gaussian_pyramid(frame);


	for (int i = 0; i < gauss_pyr.size(); ++i)
	{
		imshow("Gaussian pyramid", gauss_pyr[i]);
		waitKey();
	}
}

void test_laplacian_pyramid(cv::Mat frame){

	std::vector<cv::Mat> lapl_pyr = laplacian_pyramid(frame);


	for (int i = 0; i < lapl_pyr.size(); ++i)
	{
		imshow("Laplacian pyramid", lapl_pyr[i]);
		waitKey();
	}
}

void test_canny(cv::Mat frame) {

	cv::Mat cannied;
	cv::Mat src;

	src = frame.clone();

	GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

	Canny(src, cannied, 0, 30);

	imshow("Canny", cannied);
	waitKey();
}

int main(int argc, char* argv[]) {

	cv::Mat frame;


	frame = imread( argv[1], IMREAD_COLOR ); // Read image

	printf("[x, y] = [%d, %d] \n", frame.rows, frame.cols);
	imshow("Frame", frame);

	test_canny(frame);

//	test_sobel(frame);

//	test_gaussian_pyramid(frame);

//	test_laplacian_pyramid(frame);

	
	return 0;
}