//
//  Blob.h
//  Lab02CompVis
//
//  Created by Hung Nguyen on 28/11/2022.
//
#include <format>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>
#include <ctime>

#ifndef Blob_h
#define Blob_h

typedef cv::Mat Matrix;
typedef cv::SimpleBlobDetector::Params BlobParams;
typedef cv::SimpleBlobDetector BlobDet;
typedef std::vector<cv::KeyPoint> KeyPoints;
typedef std::string string;

class BlobDetector
{
public:
    static BlobParams makeParams(
        // Change thresholds
        int minThreshold = 10,
        int maxThreshold = 200,
        // Filter by Area.
        bool filterByArea = true,
        int minArea = 1500,
        // Filter by Circularity
        bool filterByCircularity = true,
        float minCircularity = 0.1,
        // Filter by Convexity
        bool filterByConvexity = true,
        float minConvexity = 0.87,
        // Filter by Inertia
        bool filterByInertia = true,
        float minInertiaRatio = 0.01)
    {
        BlobParams params;

        // Engineer params
        if (maxThreshold < minThreshold)
        {
            std::cout << "[WARNING] MaxThreshold: " << maxThreshold
                      << " is smaller than minThreshold" << minThreshold
                      << ".\n MaxThreshold will be forced-set to minThreshold + 1";
            maxThreshold = minThreshold + 1;
        }

        params.minThreshold = minThreshold;
        params.maxThreshold = maxThreshold;
        params.filterByArea = filterByArea;
        params.minArea = minArea;
        params.filterByCircularity = filterByCircularity;
        params.minCircularity = minCircularity;
        params.filterByConvexity = filterByConvexity;
        params.minConvexity = minConvexity;
        params.filterByInertia = filterByInertia;
        params.minInertiaRatio = minInertiaRatio;

        return params;
    }

    static BlobParams autoMakeParams()
    {
        return makeParams(
            minThreshold,
            maxThreshold,

            (bool)filterByArea,
            minArea,

            (bool)filterByCircularity,
            ((float)minCircularity / 100.0),

            (bool)filterByConvexity,
            ((float)minConvexity / 100.0),

            (bool)filterByInertia,
            ((float)minInertiaRatio / 100.0));
    }

    static KeyPoints blobDetect(Matrix img, BlobParams params)
    {
        std::vector<cv::KeyPoint> keypoints;

        cv::Ptr<BlobDet> detector = BlobDet::create(params);
        detector->detect(img, keypoints);
        return keypoints;
    }

    inline static Matrix scrImg;
    inline static const string SRC_WINDOW = "Original Image";
    inline static const string BLOB_WINDOW = "Corner Image";
    // Static Params
    inline static int minThreshold = 10;
    inline static int maxThreshold = 200;

    inline static int filterByArea = 0;
    inline static int minArea = 1500;

    inline static int filterByCircularity = 0;
    inline static int minCircularity = int(0.0 * 100);

    inline static int filterByConvexity = 0;
    inline static int minConvexity = int(0.00 * 100);

    inline static int filterByInertia = 0;
    inline static int minInertiaRatio = int(0.00 * 100);

    void render(Matrix img)
    {
        scrImg = img;
        cv::namedWindow(BLOB_WINDOW);
        // MIN-MAX THRESHOLD
        cv::createTrackbar("minThreshold",
                           BLOB_WINDOW,
                           &minThreshold,
                           100,
                           &BlobDetector::onChange);
        cv::createTrackbar("maxThreshold",
                           BLOB_WINDOW,
                           &maxThreshold,
                           100,
                           &BlobDetector::onChange);
        // AREA-FILTER
        cv::createTrackbar("bArea",
                           BLOB_WINDOW,
                           &filterByArea,
                           1,
                           &BlobDetector::onChange);

        cv::createTrackbar("minArea",
                           BLOB_WINDOW,
                           &minArea,
                           200,
                           &BlobDetector::onChange);

        // CIRCULARITY-FILTER
        cv::createTrackbar("bCirculrity",
                           BLOB_WINDOW,
                           &filterByCircularity,
                           1,
                           &BlobDetector::onChange);

        cv::createTrackbar("minCircularity",
                           BLOB_WINDOW,
                           &minCircularity,
                           100,
                           &BlobDetector::onChange);

        // CONVEXITY-FILTER
        cv::createTrackbar("bConvexity",
                           BLOB_WINDOW,
                           &filterByConvexity,
                           1,
                           &BlobDetector::onChange);

        cv::createTrackbar("minConvexity",
                           BLOB_WINDOW,
                           &minConvexity,
                           100,
                           &BlobDetector::onChange);

        // INERTIA-FILTER
        cv::createTrackbar("bInertia",
                           BLOB_WINDOW,
                           &filterByInertia,
                           1,
                           &BlobDetector::onChange);

        cv::createTrackbar("minInertiaRatio",
                           BLOB_WINDOW,
                           &minInertiaRatio,
                           100,
                           &BlobDetector::onChange);

        // imshow(SRC_WINDOW, scrImg);
        onChange(0, 0);
        cv::waitKey(0);
    }

    static void onChange(int, void *)
    {
        BlobParams params = autoMakeParams();
        KeyPoints points = blobDetect(scrImg, params);
        renderKeyPoint(scrImg, points);
    }

    static void renderKeyPoint(Matrix img, KeyPoints keypoints)
    {
        Matrix imWithKeyPoints;
        cv::drawKeypoints(
            img,
            keypoints,
            imWithKeyPoints,
            cv::Scalar(0, 0, 255),
            cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

        showImage(imWithKeyPoints, BLOB_WINDOW);
        imWithKeyPoints.release();
    }
};

#endif /* Blob_h */
