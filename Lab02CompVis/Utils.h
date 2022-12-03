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

Matrix readImage(std::string path)
{
    cv::Mat image = imread(path, cv::IMREAD_COLOR);
    return image;
}

void showImage(cv::Mat image, string win_name)
{
    cv::namedWindow(win_name);
    imshow(win_name, image);
    //    cv::waitKey(0);
    return;
}

char *getCurTime()
{
    auto start = std::chrono::system_clock::now();
    std::time_t start_time = std::chrono::system_clock::to_time_t(start);
    char buffer[32];
    std::strncpy(buffer, std::ctime(&start_time), 26);
    return buffer;
}

std::tuple<Matrix, Matrix> makeOverlapImage(Matrix img)
{
    // This function serve the testing of matching algo
    int nCols = img.cols;
    int nRows = img.rows;
    // Divide the image into 3x3 block, and then return a <(3x2), (3x2)>
    // where the middle cols is the overlapped region
    int nLeftCols = int(nCols / 3 * 2);
    int nRightCols = nCols - nLeftCols;
    Matrix left = img.colRange(0, nLeftCols);
    Matrix _right = img.colRange(nRightCols, nCols - 1);
    cv::Point2f pc(_right.cols / 2., _right.rows / 2.);
    cv::Mat r = cv::getRotationMatrix2D(pc, -45, 1.0);
    cv::Mat right;
    cv::warpAffine(_right, right, r, _right.size());
    _right.release();
    return std::tuple<Matrix, Matrix>{left, right};
}

#endif /* Utils_h */
