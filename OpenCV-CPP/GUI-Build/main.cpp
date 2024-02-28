#include <iostream>
#include <string>

#include "Core/Application.h"
#include "Core/MediaManager.h"
#include "Core/CAEHelper.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
    Application app(1200, 800, "Honours Project");

    std::vector<cv::String> list = {
        "../../exports/dicom-data/frame25.png",
        "../../exports/dicom-data/frame01.png"
    };

    MediaManager::Get().load_images(list.begin(), list.end());

    return app.run();
}