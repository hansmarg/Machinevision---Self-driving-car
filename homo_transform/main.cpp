#include <opencv2/opencv.hpp>
#include <iostream>

//using namespace cv;
//using namespace std;

void draw_polygon(cv::Mat img, std::vector<cv::Point> corners){

    int s = corners.size();
    if(s < 2) return;

    cv::line(img, corners[s-1], corners[0], cv::Scalar(0,0,0), 10, 8, 0);

    unsigned int i;
    for(i=1; i<corners.size(); i++){
        cv::line(img, corners[i-1], corners[i], cv::Scalar(0,0,0), 10, 8, 0);
    }
}

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
    int wait_time = flag_autoplay ? 1000/fps : 0;
    if(flag_speedup) wait_time = 6;

    // get height and width
    int frame_h = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    int frame_w = cap.get(cv::CAP_PROP_FRAME_WIDTH);

    // create windows
    std::string video_window_name = "DASH CAM";
    cv::namedWindow(video_window_name/*, cv::WINDOW_NORMAL*/); //create a window

    //std::string trans_window_name = "HOMO TRANSFORM";
    //cv::namedWindow(trans_window_name/*, cv::WINDOW_NORMAL*/); //create a window

    //std::string pixel_window_name = "PIXELATED";
    //cv::namedWindow(pixel_window_name/*, cv::WINDOW_NORMAL*/); //create a window

    // create source points for polygon and transform
    std::vector<cv::Point> pts_src;
    int a = 140 - 20;
    int b = 460 + 50;
    int t_center = cvRound(frame_w/2);
    pts_src.push_back(cv::Point(t_center - a, 470));   //point1
    pts_src.push_back(cv::Point(t_center + a, 470));   //point2
    pts_src.push_back(cv::Point(t_center + b, 665));   //point3
    pts_src.push_back(cv::Point(t_center - b, 665));   //point4

    // destination points for transform
    cv::Size warp_img_size(400, 400);
    std::vector<cv::Point> pts_dst;
    pts_dst.push_back(cv::Point(                   0 ,                    0 )); // point1
    pts_dst.push_back(cv::Point( warp_img_size.width ,                    0 )); // point2
    pts_dst.push_back(cv::Point( warp_img_size.width , warp_img_size.height )); // point3
    pts_dst.push_back(cv::Point(                   0 , warp_img_size.height )); // point4

    // Calculate Homography
    cv::Mat h = findHomography(pts_src, pts_dst);

    // Output image
    cv::Mat im_out;

    // polygon figure
    cv::Mat mask(frame_h, frame_w, CV_8UC3, cv::Scalar(1,1,1));
    draw_polygon(mask, pts_src);


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

        // Warp source image to destination based on homography
        cv::warpPerspective(frame, im_out, h, warp_img_size);

        // add trapazoid outline to frame
        cv::Mat marked_img = frame.mul(mask);

        // create pixelated image
        cv::Mat pixel_img(im_out);
        cv::resize(pixel_img, pixel_img, cv::Size(), 0.25, 0.25, cv::INTER_AREA);
        cv::resize(pixel_img, pixel_img, cv::Size(),    4,    4, cv::INTER_AREA);

        // add pictures on top of big marked_img
        im_out.copyTo(marked_img(cv::Rect(marked_img.cols - im_out.cols - 20, 20 , im_out.cols, im_out.rows)));
        pixel_img.copyTo(marked_img(cv::Rect(marked_img.cols - im_out.cols - pixel_img.cols - 40, 20 , pixel_img.cols, pixel_img.rows)));

        //show the frame in the created window
        cv::imshow(video_window_name, marked_img);
        //cv::imshow(trans_window_name, im_out);
        //cv::imshow(pixel_window_name, pixel_img);

        //wait for for 10 ms until any key is pressed.
        //If the 'Esc' key is pressed, break the while loop.
        //If the any other key is pressed, continue the loop
        //If any key is not pressed withing 10 ms, continue the loop
        if (cv::waitKey(wait_time) == 27)
        {
            std::cout << "Esc key is pressed by user. Stoppig the video" << std::endl;
            break;
        }
    }

    // destroy the created window
    //cv::destroyWindow(video_window_name);
    //cv::destroyWindow(trans_window_name);

    return 0;

}
