//
//  HarrisCorner.h
//  Lab02CompVis
//
//  Created by Hung Nguyen on 27/11/2022.
//

#ifndef HarrisCorner_h
#define HarrisCorner_h

#include <format>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>
#include <ctime>

#include "Utils.h"

typedef cv::Mat Matrix;
typedef std::string string;

int count = 0;

class CornerHarris
{
public:
    inline static const string SRC_WINDOW = "Original Image";
    inline static const string CORNER_WINDOW = "Corner Image";

    // Harris Keypoint Detection params
    inline static int harrisKeypointThreshold = 200;
    inline static int harrisBlockSize = 2;
    inline static int harrisApertureSize = 3;
    inline static int harrisK = 4; // Value (0, 100), but will be scaled down to (0.0, 1.0)

    // Harris Keypoint Detection Max value
    const int MAX_HARRIS_KEYPOINT_THRESHOLD = 255;
    //  The kernel size must be odd and not larger than 31 in function 'getSobelKernels'
    const int MAX_HARRIS_BLOCK_SIZE = 5;
    const int MAX_HARRIS_APERATURE_SIZE = 5;
    const double MAX_HARRIS_K = 100;

    inline static Matrix srcImg = Matrix();

    CornerHarris() { ; };

    static Matrix harrisCorner(cv::Mat image, int blockSize = 2, int apertureSize = 3, double k = 0.04)
    {
        cv::Mat imgGray;
        cv::Mat dst = Matrix::zeros(image.size(), CV_32FC1);
        cvtColor(image, imgGray, cv::COLOR_BGR2GRAY);
        cornerHarris(imgGray, dst, blockSize, apertureSize, k);

        Matrix dstNorm, dstNormScaled;
        normalize(dst, dstNorm, 0, 255, cv::NORM_MINMAX, CV_32FC1, Matrix());
        convertScaleAbs(dstNorm, dstNormScaled);

        //        printf("Render Index (%d) -- Finish Norm and Convert Scale (%s)\n", count, getCurTime());
        for (int i = 0; i < dstNorm.rows; i++)
        {
            for (int j = 0; j < dstNorm.cols; j++)
            {
                if ((int)dstNorm.at<float>(i, j) > harrisKeypointThreshold)
                {
                    cv::circle(dstNormScaled, cv::Point(j, i), 5, cv::Scalar(0), 2, 8, 0);
                }
            }
        }

        //        printf("Render Index (%d) -- Finish Keypoint Render (%s)\n", count, getCurTime());
        imgGray.release();
        dst.release();
        dstNorm.release();

        return dstNormScaled;
    }

    void render(Matrix image)
    {
        srcImg = image;

        cv::namedWindow(SRC_WINDOW);

        // Make TrackBars
        cv::createTrackbar("Keypoint threshold: ",
                           SRC_WINDOW,
                           &harrisKeypointThreshold,
                           MAX_HARRIS_KEYPOINT_THRESHOLD,
                           &CornerHarris::onChange);

        cv::createTrackbar("Block size: ",
                           SRC_WINDOW,
                           &harrisBlockSize,
                           MAX_HARRIS_BLOCK_SIZE,
                           &CornerHarris::onChange);

        cv::createTrackbar("Aperature size: ",
                           SRC_WINDOW,
                           &harrisApertureSize,
                           MAX_HARRIS_APERATURE_SIZE,
                           &CornerHarris::onChange);

        cv::createTrackbar("K value: ",
                           SRC_WINDOW,
                           &harrisK,
                           MAX_HARRIS_K,
                           &CornerHarris::onChange);

        imshow(SRC_WINDOW, srcImg);
        onChange(0, 0);
        cv::waitKey(0);
    }

    static void onChange(int, void *)
    {
        Matrix dstNormScaled = harrisCorner(srcImg,
                                            harrisBlockSize,
                                            harrisApertureSize,
                                            (double)harrisK / 100.0);
        showImage(dstNormScaled, CORNER_WINDOW);
        dstNormScaled.release();
    }
};

#endif /* HarrisCorner_h */
