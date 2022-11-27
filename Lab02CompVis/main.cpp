//
//  main.cpp
//  Lab02CompVis
//
//  Created by Hung Nguyen on 27/11/2022.
//

#include <format>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>
#include <ctime>

typedef cv::Mat Matrix;
typedef std::string string;

// Don't use namesapce
//using namespace cv;
//using namespace std:

int count = 0;

Matrix readImage(std::string path){
    cv::Mat image = imread(path, cv::IMREAD_COLOR);
    return image;
}

void showImage(cv::Mat image, string win_name){
    cv::namedWindow(win_name);
    imshow(win_name , image);
    cv::waitKey(0);
    return;
}

char* getCurTime(){
    auto start = std::chrono::system_clock::now();
    std::time_t start_time = std::chrono::system_clock::to_time_t(start);
    char buffer[32];
    std::strncpy(buffer, std::ctime(&start_time), 26);
    return buffer;
}


class CornerHarris{
public:
    inline static const string SRC_WINDOW = "Original Image";
    inline static const string CORNER_WINDOW = "Corner Image";
    inline static int trackBarThresh = 200;
    const int MAX_TRACKBAR_THRESH = 255;
    inline static Matrix srcImg = Matrix();
    
    CornerHarris(){;};
    
    
    static Matrix harrisCorner(cv::Mat image, int blockSize = 2, int apertureSize = 3, double k = 0.04){
        cv::Mat imgGray;
        printf("Render Index (%d) -- Start Harris", count);
        cv::Mat dst = Matrix::zeros(image.size(), CV_32FC1);
        cvtColor(image, imgGray, cv::COLOR_BGR2GRAY);
        cornerHarris(imgGray, dst, blockSize, apertureSize, k);
        printf("Render Index (%d) -- Finish Harris (%s)", count, getCurTime());
        Matrix dstNorm, dstNormScaled;
        
        normalize(dst, dstNorm, 0, 255, cv::NORM_MINMAX, CV_32FC1, Matrix());
        
        convertScaleAbs(dstNorm, dstNormScaled);
        printf("Render Index (%d) -- Finish Norm and Convert Scale (%s)", count, getCurTime());
        
        for( int i = 0; i < dstNorm.rows ; i++ )
        {
            for( int j = 0; j < dstNorm.cols; j++ )
            {
                if((int) dstNorm.at<float>(i,j) > trackBarThresh)
                {
                    cv::circle(dstNormScaled, cv::Point(j,i), 5,  cv::Scalar(0), 2, 8, 0 );
                }
            }
        }
        
        printf("Render Index (%d) -- Finish Keypoint Render (%s)", count, getCurTime());
        dst.release();
        dstNorm.release();
        
        return dstNormScaled;
    }
    
    
    
    void render(Matrix image){
        srcImg = image;
        
        cv::namedWindow(SRC_WINDOW);
        cv::createTrackbar(
                           "Threshold: ",
                           SRC_WINDOW,
                           &trackBarThresh,
                           MAX_TRACKBAR_THRESH,
                           &CornerHarris::renderHarrisCallback
                           );
        
        imshow(SRC_WINDOW, srcImg);
        renderHarrisCallback(0, 0);
        
    }
    
    static void renderHarrisCallback(int, void*){
        std::cout << "Render Count: " << count << std::endl;
        count++;
        Matrix dstNormScaled = harrisCorner(srcImg);
        showImage(dstNormScaled, CORNER_WINDOW);
    }
    
};



int main(){
    std::cout << "Hello world!" << std::endl;
    
    
    // read the input image from file
    cv::Mat image = readImage("/Users/hung.nh/Downloads/ava.jpeg");
    printf("Image dimension %d\n", image.dims);
    printf("Image channels %d\n", image.channels());
    CornerHarris demo = CornerHarris();
    demo.render(image);
    //    harrisCorner(image);
    //    cv::namedWindow(source_window);
    //    cv::createTrackbar(
    //        "Threshold: ",
    //        source_window,
    //        &thresh,
    //        max_thresh, cornerHarris_demo);
    
    //    cornerHarris_demo(0, 0);
    return 0;
}
