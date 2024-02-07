#include <iostream>
#include <string>

#include "Core/Application.h"
#include "Core/MediaManager.h"
#include "Core/CAEHelper.h"

int main()
{
    Application app(1200, 800, "Honours Project");

    std::vector<cv::String> list = {
        "../../exports/dicom-data/frame00.png",
        "../../exports/dicom-data/frame01.png"
    };
    MediaManager::Get().load_images(list.begin(), list.end());

    return app.run();
}