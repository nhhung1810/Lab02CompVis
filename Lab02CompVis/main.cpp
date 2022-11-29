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

// Internal Library
#include "HarrisCorner.h"
#include "Utils.h"
#include "Blob.h"

typedef cv::Mat Matrix;
typedef std::string string;

static int runCamera();

static void demoCornerHarris() {
    std::cout << "Hello world!" << std::endl;
    // read the input image from file
    cv::Mat image = readImage("/Users/hung.nh/Downloads/ava.jpeg");
    printf("Image dimension %d\n", image.dims);
    printf("Image channels %d\n", image.channels());
    CornerHarris harrisDemo = CornerHarris();
    harrisDemo.render(image);
}

// Don't use namesapce
//using namespace cv;
//using namespace std:


int main(){
    cv::Mat image = readImage("/Users/hung.nh/Downloads/ava.jpeg");
    BlobDetector b = BlobDetector();
    b.render(image);
    //    demoCornerHarris();
}


static int runCamera() {
    Matrix myImage;//Declaring a matrix to load the frames//
    cv::namedWindow("Video Player");//Declaring the video to show the video//
    cv::VideoCapture cap(0);
    while(!cap.isOpened() ){
        int tmp;
        std::cout << "Try again: 1 or 0?" << std::endl;
        std::cin >> tmp;
        if (tmp != 1) break;
        cap.open(0);
        
    }
    
    while (true){ //Taking an everlasting loop to show the video//
        cap >> myImage;
        if (myImage.empty()){ //Breaking the loop if no video frame is detected//
            break;
        }
        imshow("Video Player", myImage); //Showing the video//
        char c = (char)cv::waitKey(25);  //Allowing 25 milliseconds frame processing time and initiating break condition//
        if (c == 27){ //If 'Esc' is entered break the loop//
            break;
        }
    }
    cap.release();//Releasing the buffer memory//
    return 0;
}
