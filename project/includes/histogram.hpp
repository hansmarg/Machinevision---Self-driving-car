#ifndef _HISTOGRAM_H_
#define _HISTOGRAM_H_

#include <opencv2/opencv.hpp>
#include <iostream>

namespace histogram{
    cv::Mat intensity_histogram(
           cv::Mat image,
           cv::Mat mask );
}

#endif // _HISTOGRAM_H_
