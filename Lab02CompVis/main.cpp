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
#include "DoGDetector.h"
#include "HarrisSift.h"

typedef cv::Mat Matrix;
typedef std::string string;

static int runCamera();

static void demoCornerHarris();

static void demoDoGDetector();

static void demoBlobDetector();

static void demoOverlapImage(Matrix image);

static void demoHarrisSift(Matrix image);

int main()
{
//    cv::Mat image = readImage("/Users/hung.nh/Downloads/ava.jpeg");
//    demoHarrisSift(image);
    demoCornerHarris();
}

static void demoHarrisSift(Matrix image)
{
    std::tuple<Matrix, Matrix> data = makeOverlapImage(image);
    Matrix left = std::get<0>(data);
    Matrix right = std::get<1>(data);
    showImage(left, "Left");
    showImage(right, "Right");
    cv::waitKey(0);

    HarrisSiftMatcher::render(left, right);
    cv::waitKey(0);
}

static void demoOverlapImage(Matrix image)
{
    std::tuple<Matrix, Matrix> data = makeOverlapImage(image);
    Matrix left = std::get<0>(data);
    Matrix right = std::get<1>(data);
    showImage(left, "Left");
    showImage(right, "Right");
    cv::waitKey(0);
}

static void demoCornerHarris()
{
    std::cout << "Hello world!" << std::endl;
    // read the input image from file
    cv::Mat image = readImage("/Users/hung.nh/Downloads/ava.jpeg");
    printf("Image dimension %d\n", image.dims);
    printf("Image channels %d\n", image.channels());
    CornerHarris harrisDemo = CornerHarris();
    harrisDemo.render(image);
}

static void demoDoGDetector()
{
    cv::Mat image = readImage("/Users/hung.nh/Downloads/ava.jpeg");
    DoGDetector b = DoGDetector();
    b.render(image);
}

static void demoBlobDetector()
{
    cv::Mat image = readImage("/Users/hung.nh/Downloads/ava.jpeg");
    BlobDetector b = BlobDetector();
    b.render(image);
}

static int runCamera()
{
    Matrix myImage;                  // Declaring a matrix to load the frames//
    cv::namedWindow("Video Player"); // Declaring the video to show the video//
    cv::VideoCapture cap(0);
    while (!cap.isOpened())
    {
        int tmp;
        std::cout << "Try again: 1 or 0?" << std::endl;
        std::cin >> tmp;
        if (tmp != 1)
            break;
        cap.open(0);
    }

    while (true)
    { // Taking an everlasting loop to show the video//
        cap >> myImage;
        if (myImage.empty())
        { // Breaking the loop if no video frame is detected//
            break;
        }
        imshow("Video Player", myImage); // Showing the video//
        char c = (char)cv::waitKey(25);  // Allowing 25 milliseconds frame processing time and initiating break condition//
        if (c == 27)
        { // If 'Esc' is entered break the loop//
            break;
        }
    }
    cap.release(); // Releasing the buffer memory//
    return 0;
}
