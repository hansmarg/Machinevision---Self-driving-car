#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

#include "../filter/filters.cpp"

using namespace cv;
using namespace std;


void showim(string windowName, Mat image){
	imshow(windowName, image);
	while(waitKey() != 32) continue;
}


Mat crop_img(Mat frame){
    // MOD THE IMAGE
    cv::Mat image = frame.clone();							// C3
    cv::Mat mask = cv::Mat::zeros(image.rows, image.cols, CV_8U); // all 0

    std::string windowName = "Trapezoid";

    std::vector<cv::Point> points;
    points.push_back(cv::Point(520 , 470));   //point1
    points.push_back(cv::Point(800 , 470));   //point2
    points.push_back(cv::Point(1120, 665));   //point3
    points.push_back(cv::Point(200 , 665));   //point4

    cv::fillConvexPoly(mask,              // Image to be drawn on
                       points,            // C-Style array of points
                       //cv::Scalar(1,1,1), // Color , BGR form
                       1,
                       CV_AA,             // connectedness, 4 or 8
                       0);                // Bits of radius to treat as fraction


    image = image.mul(mask);

    return image;
}

void test_houghLines(cv::Mat img){

	cv::Mat frame = img.clone();
	cv::Mat cframe; // cropped frame
	cv::Mat cdst, src;
	vector<Vec4i> lines;

	//double rho = 10;
	//double theta = 0.5;
	//int threshold = 50; 

	//frame = sobel_func(frame);
	//cv::Scalar min_color = cv::Scalar(0,200,200);
	//cv::Scalar max_color = cv::Scalar(255,255,255);
	//cv::inRange(frame, min_color,  max_color, src);

	//src = frame.clone();
	showim("Frame", frame);
	
	GaussianBlur( frame, src, Size(7,7), 5, 5, BORDER_DEFAULT );

	Canny(src, frame, 80,100); 

	showim("Canny", frame);

	cframe = crop_img(frame);

	HoughLinesP(cframe, lines, 1, CV_PI/180, 25, 30, 250 );
	
	cvtColor(frame, cdst, CV_GRAY2BGR);

	//HoughLines(frame, frame, rho, theta, threshold);


	for( size_t i = 0; i < lines.size(); i++ )
	{
		Vec4i l = lines[i];
		line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
	}


	showim("Detected lines", cdst);

}
int main(int argc, char** argv){

	cv::Mat frame;

	frame = imread( argv[1], IMREAD_COLOR ); // Read image

	test_houghLines(frame); 
	

	//showim("Frame end", frame);
	return 0; 
}


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
