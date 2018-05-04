#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>

#include "includes/shapes.hpp"
#include "includes/histogram.hpp"

static int diffclock_ms(clock_t clock1, clock_t clock2){
    double diffticks=clock1-clock2;
    double diffms=(diffticks)/(CLOCKS_PER_SEC/1000);
    return (int)diffms;
}

//using namespace cv;
//using namespace std;

int main(int argc, char* argv[])
{
    std::string video_path = "../../dataset/project_video.mp4";

    // flag default values
    bool flag_autoplay = false;
    bool flag_speedup = false;

    // parse arguments
    for(int i=1; i<argc; i++){
        if(strcmp(argv[i], "--autoplay") == 0){
            flag_autoplay = true;
        }
        else if(strcmp(argv[i], "--speedup") == 0){
            flag_speedup = true;
        }
        else{
            video_path = argv[i];
        }
    }

    // open the video file for reading
    cv::VideoCapture cap(video_path);

    // if not success, exit program
    if (cap.isOpened() == false)
    {
        std::cout << "Cannot open the video file: " << video_path << std::endl;
        std::cin.get(); //wait for any key press
        return -1;
    }

    // Uncomment the following line if you want to start the video in the middle
    //cap.set(CAP_PROP_POS_MSEC, 300);

    // get the frames rate of the video
    double fps = cap.get(cv::CAP_PROP_FPS);
    std::cout << "Frames per seconds : " << fps << std::endl;

    // set the wait time between each frame
    int wait_time = 1000/fps;
    if(flag_speedup) wait_time = 6;

    // get height and width
    int frame_h = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    int frame_w = cap.get(cv::CAP_PROP_FRAME_WIDTH);

    // create windows
    std::string video_window_name = "DASH CAM";
    cv::namedWindow(video_window_name/*, cv::WINDOW_NORMAL*/); //create a window
    //cv::namedWindow("green", cv::WINDOW_NORMAL); //create a window
    //cv::namedWindow("blue", cv::WINDOW_NORMAL); //create a window

    // crate mask
    cv::Mat mask = cv::Mat::zeros(frame_h, frame_w, CV_8U); // all 0
    cv::Mat frame_mask = cv::Mat::zeros(frame_h, frame_w, CV_8UC3); // all 0

    std::vector<cv::Point> mask_points = shapes::trapazoid( 140, 800, cvRound(frame_w/2), 205, frame_h - 265);

    // craete the mask
    cv::fillConvexPoly(mask,              // Image to be drawn on
                       mask_points,            // C-Style array of points
                       1, // Color , BGR form
                       CV_AA,             // connectedness, 4 or 8
                       0);                // Bits of radius to treat as fraction

    // craete the mask
    cv::fillConvexPoly(frame_mask,              // Image to be drawn on
                       mask_points,            // C-Style array of points
                       cv::Scalar(1,1,1), // Color , BGR form
                       CV_AA,             // connectedness, 4 or 8
                       0);                // Bits of radius to treat as fraction


    int max_histo = cv::countNonZero(mask);
    printf("max_histo: %d\n", max_histo);


    // destination points for transform
    cv::Size plot_size(400, 400);
    cv::Mat b_histo, g_histo;
    cv::Mat histo_plot;
    cv::Mat histo_plot2;

    // time calculation
    clock_t time_s;
    time_s = clock();

    while (true)
    {
        cv::Mat frame;
        bool bSuccess = cap.read(frame); // read a new frame from video

        //Breaking the while loop at the end of the video
        if (bSuccess == false)
        {
            std::cout << "Found the end of the video" << std::endl;
            break;
        }

        frame /= 16;
        frame *= 16;

        // split image into planes (bgr)
        std::vector<cv::Mat> bgr_planes;
        cv::split( frame, bgr_planes );

        cv::Mat blue_frame  = bgr_planes[0];
        cv::Mat green_frame = bgr_planes[1];

        // calculate histogram
        b_histo = histogram::intensity_histogram(blue_frame , mask);
        g_histo = histogram::intensity_histogram(green_frame, mask);

        // create plot canvases
        histo_plot  = cv::Mat::zeros(plot_size.height/2, plot_size.width, CV_8UC3);
        histo_plot2 = cv::Mat::zeros(plot_size.height/2, plot_size.width, CV_8UC3);

        // kernel for smoothing
        int kernel_size = 5;
        cv::Mat kernel = cv::Mat::ones(kernel_size, 1, CV_8U); // all 0

        // smooth blue
        cv::Mat b_smooth;
        filter2D(b_histo, b_smooth, -1, kernel);
        b_smooth /= kernel_size;

        // smooth green
        cv::Mat g_smooth;
        filter2D(g_histo, g_smooth, -1, kernel);
        g_smooth /= kernel_size;

        // plot original histos
        shapes::plot(histo_plot , b_histo, cv::Scalar(255,0,0), 1, max_histo);
        shapes::plot(histo_plot2, g_histo, cv::Scalar(0,255,0), 1, max_histo);

        // plot smoothed histos
        //shapes::plot(histo_plot , b_smooth, cv::Scalar(0,0,255), 1, max_histo);
        //shapes::plot(histo_plot2, g_smooth, cv::Scalar(0,0,255), 1, max_histo);

        // add pictures on top of big frame
        frame = frame.mul(frame_mask);
        histo_plot.copyTo(frame(cv::Rect(frame.cols - histo_plot.cols - 20, 20 , histo_plot.cols, histo_plot.rows)));
        histo_plot2.copyTo(frame(cv::Rect(frame.cols - histo_plot2.cols - 20, 40 + histo_plot.rows , histo_plot2.cols, histo_plot2.rows)));

        //show the frame in the created window
        cv::imshow(video_window_name, frame);
        //cv::imshow("blue", blue_frame);
        //cv::imshow("green", green_frame);

        //wait for for 10 ms until any key is pressed.
        //If the 'Esc' key is pressed, break the while loop.
        //If the any other key is pressed, continue the loop
        //If any key is not pressed withing 10 ms, continue the loop

        // time end
        int ms = diffclock_ms(clock(), time_s);
        int wait = wait_time - ms;
        printf("calculation time: %dms | wait: %dms\n", ms, wait);

        wait = (wait > 0) ? wait : 1 ;

        if (cv::waitKey(flag_autoplay ? wait : 0 ) == 27)
        {
            std::cout << "Esc key is pressed by user. Stoppig the video" << std::endl;
            break;
        }
        time_s = clock();
    }

    // destroy the created window
    //cv::destroyWindow(video_window_name);
    //cv::destroyWindow(trans_window_name);

    return 0;

}
