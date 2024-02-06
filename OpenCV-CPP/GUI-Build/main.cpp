#include <iostream>
#include <string>

#include "Core/Application.h"
#include "Core/MediaManager.h"
#include "Core/CAEHelper.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main()
{
    Application app(1200, 800, "Honours Project");

    std::vector<cv::String> list = {
        "../../exports/dicom-data/frame25.png",
        "../../exports/dicom-data/frame01.png"
    };

    MediaManager::Get().load_images(list.begin(), list.end());
    
    //cv::Mat img = MediaManager::Get().media()[0];
    //cv::Mat dest;
    //cv::equalizeHist(img, dest);

    return app.run();
}