#include <opencv2/opencv.hpp>
#include <iostream>

#include "../filter/filters.cpp"

using namespace cv;
using namespace std;

/*
void cv::HoughLines 	( 	InputArray  	image,
		OutputArray  	lines,
		double  	rho,
		double  	theta,
		int  	threshold,
		double  	srn = 0,
		double  	stn = 0,
		double  	min_theta = 0,
		double  	max_theta = CV_PI 
	) 		
*/

void test_houghLines(cv::Mat img){

	cv::Mat frame = img.clone();
	cv::Mat cdst, src;
	vector<Vec4i> lines;

	double rho = 10;
	double theta = 0.5;
	int threshold = 50; 

	//frame = sobel_func(frame);
	//cv::Scalar min_color = cv::Scalar(0,200,200);
	//cv::Scalar max_color = cv::Scalar(255,255,255);
	//cv::inRange(frame, min_color,  max_color, frame);

	src = frame.clone();

	GaussianBlur( src, src, Size(7,7), 2, 2, BORDER_DEFAULT );
	GaussianBlur( src, src, Size(7,7), 2, 2, BORDER_DEFAULT );
	GaussianBlur( src, src, Size(7,7), 2, 2, BORDER_DEFAULT );
	GaussianBlur( src, src, Size(7,7), 2, 2, BORDER_DEFAULT );
	GaussianBlur( src, src, Size(7,7), 2, 2, BORDER_DEFAULT );
	GaussianBlur( src, src, Size(7,7), 2, 2, BORDER_DEFAULT );
	GaussianBlur( src, src, Size(7,7), 2, 2, BORDER_DEFAULT );
	GaussianBlur( src, src, Size(7,7), 2, 2, BORDER_DEFAULT );
	GaussianBlur( src, src, Size(7,7), 2, 2, BORDER_DEFAULT );
	GaussianBlur( src, src, Size(7,7), 2, 2, BORDER_DEFAULT );

	//Canny(src, frame, 80,100); 

	


	//HoughLinesP(frame, lines, 1, CV_PI/180, 10, 50, 50 );
	//HoughLines(frame, frame, rho, theta, threshold);
	//cvtColor(frame, cdst, CV_GRAY2BGR);


	for( size_t i = 0; i < lines.size(); i++ )
	{
		Vec4i l = lines[i];
		line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
	}

	imshow("HoughLines", src);
	while(waitKey() != 32 )
		{continue;}
	imshow("detected lines", cdst);


	while(waitKey() != 32 )
		{continue;}

}

int main(int argc, char** argv){

	cv::Mat frame;

	frame = imread( argv[1], IMREAD_COLOR ); // Read image

	test_houghLines(frame); 

}