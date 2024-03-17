#include <iostream>
#include <string>
#include <sstream>

#include "Core/Application.h"
#include "Core/MediaManager.h"
#include "Core/CAEHelper.h"
#include "Core/ImageMeasures.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
    cv::Mat img = cv::imread("../../exports/dicom-data/frame00.png");
    cv::Mat enh = cv::imread("../../exports/opencv/adaptive-histogram-eq/cl2_frame00.png");

    cv::imshow("src", img);
    cv::imshow("enh", enh);

    double min = 0, max = 0;
    cv::minMaxLoc(img, &min, &max);
    std::cout << min << "\n" << max << "\n";

    std::cout << ImgMeasure::PSNR(img, enh) << "\n";
    std::cout << ImgMeasure::SSIM(img, enh) << "\n";
    std::cout << ImgMeasure::CII(img, enh) << "\n";

    cv::waitKey(0);
    return 0;

    //Application app(1200, 800, "Honours Project");

    //std::vector<cv::String> list;

    //cv::String directory = "../../exports/dicom-data/";
    //cv::String file_format = ".png";

    //for (size_t i = 0; i <= 60; i++)
    //{
    //    cv::String filename = "frame";
    //    if (i < 10)
    //    {
    //        filename = filename + "0" + std::to_string(i);
    //    }
    //    else {
    //        filename = filename + std::to_string(i);
    //    }
    //    list.push_back(directory + filename +file_format);
    //}

    //MediaManager::Get().load_images("src1", list.begin(), list.end());

    //return app.run();
}