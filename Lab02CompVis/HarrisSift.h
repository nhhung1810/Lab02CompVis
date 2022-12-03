//
//  HarrisSift.h
//  Lab02CompVis
//
//  Created by Hung Nguyen on 29/11/2022.
//

#ifndef HarrisSift_h
#define HarrisSift_h

#include <format>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>
#include <ctime>
#include "Utils.h"

typedef cv::Mat Matrix;
typedef std::vector<cv::KeyPoint> KeyPoints;
typedef std::string string;
typedef std::vector<cv::DMatch> Matches;

const int MAX_THRESH = 255;

struct _HarrisMatchResul
{
    KeyPoints kp1;
    KeyPoints kp2;
    Matches m;
};

class HarrisSiftMatcher
{

public:
    // Params
    inline static int HARRIS_KEYPOINT_THRESHOLD = 200;
    inline static int HARRIS_BLOCK_SIZE = 2;
    inline static int HARRIS_APERTURE_SIZE = 3;
    inline static int HARRIS_K = 4;
    inline static int MATCHING_K = 10;

    static KeyPoints _harrisDetect(Matrix image,
                                   int blockSize = 2,
                                   int apertureSize = 3,
                                   double k = 0.04,
                                   int thres = 180)
    {
        int _apertureSize = apertureSize % 2 == 0 ? apertureSize - 1 : apertureSize;
        Matrix imgGray;
        cvtColor(image, imgGray, cv::COLOR_BGR2GRAY);
        cv::Mat dst = Matrix::zeros(image.size(), CV_32FC1);
        cv::cornerHarris(imgGray, dst, blockSize, _apertureSize, k);

        cv::Mat dstNorm;
        cv::normalize(dst, dstNorm, 0, MAX_THRESH, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
        KeyPoints keypoints;

        for (int i = 0; i < dstNorm.rows; i++)
        {
            for (int j = 0; j < dstNorm.cols; j++)
            {
                if (dstNorm.at<float>(i, j) > thres)
                {
                    keypoints.emplace_back(j, i, int(1 / dstNorm.at<float>(i, j) * MAX_THRESH * 5));
                }
            }
        }

        // std::cout << "Num. of keypoints: " << keypoints.size() << std::endl;

        // // Too many point case -> fail
        // auto [x, y] = dst.size();
        // if (float(keypoints.size()) / (x * y) > 0.8)
        // {
        //     keypoints.clear();
        // }
        return keypoints;
    };

    inline static Matrix img1 = Matrix();
    inline static KeyPoints kp1 = KeyPoints();
    inline static Matrix img2 = Matrix();
    inline static KeyPoints kp2 = KeyPoints();

    static void _renderKeyPoint(Matrix img, KeyPoints keypoints, string window_name)
    {
        Matrix imWithKeyPoints;
        cv::drawKeypoints(
            img,
            keypoints,
            imWithKeyPoints,
            cv::Scalar(0, 0, 255),
            cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

        showImage(imWithKeyPoints, window_name);
        imWithKeyPoints.release();
    }

    static void render(Matrix _img1, Matrix _img2)
    {
        img1 = _img1;
        img2 = _img2;
        // auto matchData = detectAndMatch(img1, img2);
        // kp1 = matchData.kp1;
        // kp2 = matchData.kp2;
        // _renderKeyPoint(img1, kp1, "Harris Image 1");
        // _renderKeyPoint(img2, kp2, "Harris Image 2");
        // cv::waitKey(0);
        // matches = matchData.m;

        cv::namedWindow(HARRIS_SIFT_WIN);
        cv::createTrackbar("Threshold",
                           HARRIS_SIFT_WIN,
                           &HARRIS_KEYPOINT_THRESHOLD,
                           200,
                           &HarrisSiftMatcher::show_matches);

        cv::createTrackbar("BlockSize",
                           HARRIS_SIFT_WIN,
                           &HARRIS_BLOCK_SIZE,
                           5,
                           &HarrisSiftMatcher::show_matches);

        cv::createTrackbar("ApertureSize div 2",
                           HARRIS_SIFT_WIN,
                           &HARRIS_APERTURE_SIZE,
                           5,
                           &HarrisSiftMatcher::show_matches);

        cv::createTrackbar("K",
                           HARRIS_SIFT_WIN,
                           &HARRIS_K,
                           100,
                           &HarrisSiftMatcher::show_matches);

        cv::createTrackbar("mK",
                           HARRIS_SIFT_WIN,
                           &MATCHING_K,
                           100,
                           &HarrisSiftMatcher::show_matches);

        show_matches(0, 0);
        cv::waitKey(0);
    }

    static _HarrisMatchResul detectAndMatch(Matrix img1, Matrix img2)
    {
        auto keypoint1 = _harrisDetect(img1,
                                       HARRIS_BLOCK_SIZE,
                                       HARRIS_APERTURE_SIZE,
                                       (float)HARRIS_K / 100.0f,
                                       HARRIS_KEYPOINT_THRESHOLD);
        auto keypoint2 = _harrisDetect(img2,
                                       HARRIS_BLOCK_SIZE,
                                       HARRIS_APERTURE_SIZE,
                                       (float)HARRIS_K / 100.0f,
                                       HARRIS_KEYPOINT_THRESHOLD);
        auto matcher = cv::DescriptorMatcher::create(
            cv::DescriptorMatcher::FLANNBASED);

        std::vector<Matches> knn_matches;
        auto _tmp = std::vector<cv::Mat>(2);
        auto _matches = Matches();

        matcher->knnMatch(
            _tmp[0],
            _tmp[1],
            knn_matches, MATCHING_K);

        std::cout << "~KNN-match" << knn_matches.size() << std::endl;
        for (auto &knn_match : knn_matches)
        {
            std::move(knn_match.begin(),
                      knn_match.end(), std::back_inserter(_matches));
        }
        std::sort(
            _matches.begin(),
            _matches.end(),
            [](auto &a, auto &b)
            { return a.distance < b.distance; });

        return _HarrisMatchResul{
            keypoint1, keypoint2, _matches};
    };

    inline static Matches matches = Matches();
    inline static string HARRIS_SIFT_WIN = "HarrisSift Window";

    static void show_matches(int, void *)
    {
        auto matchData = detectAndMatch(img1, img2);
        kp1 = matchData.kp1;
        kp2 = matchData.kp2;
        matches = matchData.m;

        cv::Mat img_matches;
        std::vector<cv::DMatch> shown;
        auto to_shown = int(matches.size());

        if (!matches.empty())
        {
            std::copy_n(matches.begin(), to_shown, std::back_inserter(shown));
        }
        if (shown.size() > 0)
            std::cout << "~GoodMatch: " << shown.size() << std::endl;

        cv::drawMatches(
            img1,
            kp1,
            img2,
            kp2,
            shown,
            img_matches,
            cv::Scalar(-1), cv::Scalar(-1),
            std::vector<char>(),
            cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

        cv::imshow(HARRIS_SIFT_WIN, img_matches);
    };
};

#endif /* HarrisSift_h */
