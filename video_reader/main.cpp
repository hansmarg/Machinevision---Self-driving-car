#include <opencv2/opencv.hpp>
#include <iostream>

//using namespace cv;
//using namespace std;

//int main(int argc, char** argv)
int main(void)
{
    // Read the image file
    cv::Mat image = cv::imread("../crap/test3.jpg");

    // check for failure
    if(image.empty()){
        std::cout << "Could not open or find image" << std::endl;
    }

    // create windowName before window (for learning purposes)
    std::string windowName = "fack this life";

    // create the window
    cv::namedWindow(windowName);

    // show the image inside the window
    cv::imshow(windowName, image);

    // wait for any keystroke
    cv::waitKey(0);

    // destroy the created window
    cv::destroyWindow(windowName);

    return 0;
}
