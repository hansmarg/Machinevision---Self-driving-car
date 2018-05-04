#include "histogram.hpp"

// calculates intesity histogram
cv::Mat histogram::intensity_histogram(
    cv::Mat image,
    cv::Mat mask
){
    /// Establish the number of bins
    int histSize = 256;

    /// Set the ranges ( for B,G,R )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    // prepare return
    cv::Mat histo;

    /// Compute the histograms:
    calcHist( &image, 1, 0, mask, histo, 1, &histSize, &histRange, true, false );

    return histo;
}

