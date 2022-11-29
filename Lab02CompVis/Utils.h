//
//  Utils.h
//  Lab02CompVis
//
//  Created by Hung Nguyen on 27/11/2022.
//

#ifndef Utils_h
#define Utils_h

#include <format>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>
#include <ctime>

typedef cv::Mat Matrix;
typedef std::string string;


Matrix readImage(std::string path){
    cv::Mat image = imread(path, cv::IMREAD_COLOR);
    return image;
}

void showImage(cv::Mat image, string win_name){
    cv::namedWindow(win_name);
    imshow(win_name , image);
//    cv::waitKey(0);
    return;
}

char* getCurTime(){
    auto start = std::chrono::system_clock::now();
    std::time_t start_time = std::chrono::system_clock::to_time_t(start);
    char buffer[32];
    std::strncpy(buffer, std::ctime(&start_time), 26);
    return buffer;
}


#endif /* Utils_h */
