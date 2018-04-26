#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/**
 * @function main
 */
int main( int argc, char** argv )
{
    std::string img_path;

    // check for failure
    if(argc < 2){
        img_path = "../crap/test1.jpg";
    }else{
        img_path = argv[1];
    }

    // Read the image file
    cv::Mat image = cv::imread(img_path);

    // check for failure
    if(image.empty()){
        std::cout << "Could not open or find image: " << img_path << std::endl;
        return 1;
    }

    Mat dst;

    /// Separate the image in 3 places ( B, G and R )
    vector<Mat> bgr_planes;
    split( image, bgr_planes );

    /// Establish the number of bins
    int histSize = 256;

    /// Set the ranges ( for B,G,R )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    Mat b_hist, g_hist, r_hist;

    /// Compute the histograms:
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, true, false );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, true, false );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, true, false );

    // Draw the histograms for B, G and R
    int hist_h = 400;
    int hist_w = 512;
    int unit_w = cvRound( (double) hist_w/histSize );

    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

    /// Normalize the result to [ 0, histImage.rows ]
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( unit_w*(i-1), cvRound(b_hist.at<float>(i-1)) ) ,
                         Point( unit_w*(i)  , cvRound(b_hist.at<float>(i))   ) ,
                         Scalar( 255, 0, 0), 1, 8, 0  );
        line( histImage, Point( unit_w*(i-1), cvRound(g_hist.at<float>(i-1)) ) ,
                         Point( unit_w*(i)  , cvRound(g_hist.at<float>(i))   ) ,
                         Scalar( 0, 255, 0), 1, 8, 0  );
        line( histImage, Point( unit_w*(i-1), cvRound(r_hist.at<float>(i-1)) ) ,
                         Point( unit_w*(i)  , cvRound(r_hist.at<float>(i))   ) ,
                         Scalar( 0, 0, 255), 1, 8, 0  );
        //line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
        //        Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
        //        Scalar( 0, 255, 0), 2, 8, 0  );
        //line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
        //        Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
        //        Scalar( 0, 0, 255), 2, 8, 0  );
    }

    /// Display
    namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
    imshow("calcHist Demo", histImage );

    waitKey(0);

    return 0;
}
