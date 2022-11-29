//
//  DoGDetector.h
//  Lab02CompVis
//
//  Created by Hung Nguyen on 29/11/2022.
//

#ifndef DoGDetector_h
#define DoGDetector_h

#include <format>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>
#include <ctime>
#include "Utils.h"

typedef cv::Mat Matrix;
typedef cv::SimpleBlobDetector::Params BlobParams;
typedef cv::SimpleBlobDetector BlobDet;
typedef std::vector<cv::KeyPoint> KeyPoints;
typedef std::string string;

struct DoGParams
{
    int nFeatures = 0;
    int nOctaveLayers = 3;
    double contrastThreshold = 0.04;
    double edgeThreshold = 10;
    double sigma = 1.6;
};

class DoGDetector
{
public:
    // Trackbar params
    inline static int nFeatures = 0;
    inline static int nOctaveLayers = 3;
    // inline static int contrastThreshold = int(100 * 0.0);
    // inline static int edgeThreshold = int(100 * 0.0);
    // inline static int sigma = int(100 * 0.0);
    inline static string DOG_WINDOW = "DoG Window";

    static DoGParams autoMakeParams()
    {
        // All input data for double will range from (0 - 100)
        // Int data will be handle by hand
        // sigma = (sigma > 0) ? sigma : 1;
        DoGParams data{
            nFeatures,
            nOctaveLayers};
        return data;
    };

    static KeyPoints
    detect(cv::InputArray image, DoGParams params)
    {
        KeyPoints keypoints;
        auto detector = cv::SIFT::create(params.nFeatures,
                                         params.nOctaveLayers,
                                         params.contrastThreshold,
                                         params.edgeThreshold,
                                         params.sigma);
        detector->detect(image, keypoints);
        return keypoints;
    }

    inline static Matrix srcImg;
    static void onChange(int, void *)
    {
        DoGParams params = autoMakeParams();
        KeyPoints points = detect(srcImg, params);
        renderKeyPoint(srcImg, points);
    };

    static void renderKeyPoint(Matrix img, KeyPoints keypoints)
    {
        Matrix imWithKeyPoints;
        cv::drawKeypoints(
            img,
            keypoints,
            imWithKeyPoints,
            cv::Scalar(0, 0, 255),
            cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

        showImage(imWithKeyPoints, DOG_WINDOW);
        imWithKeyPoints.release();
    }

    void render(Matrix img)
    {
        srcImg = img;
        cv::namedWindow(DOG_WINDOW);
        // TRACK-BAR SESSION
        cv::createTrackbar("nFeatures", DOG_WINDOW,
                           &nFeatures, 10, DoGDetector::onChange);
        cv::createTrackbar("nOctaveLayers", DOG_WINDOW,
                           &nOctaveLayers, 10, DoGDetector::onChange);
        cv::waitKey(0);
    }
};

#endif /* DoGDetector_h */
