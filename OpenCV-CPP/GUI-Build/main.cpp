#include <iostream>
#include <string>
#include <sstream>

#include "Core/Application.h"
#include "Core/MediaManager.h"

#include <opencv2/core.hpp>

int main()
{
    Application app(1200, 800, "Honours Project");

    std::vector<cv::String> list;

    cv::String directory = "../../exports/dicom-data/";
    cv::String file_format = ".png";

    for (size_t i = 0; i <= 60; i++)
    {
        cv::String filename = "frame";
        if (i < 10)
        {
            filename = filename + "0" + std::to_string(i);
        }
        else {
            filename = filename + std::to_string(i);
        }
        list.push_back(directory + filename +file_format);
    }

    MediaManager::Get().load_images("src1", list.begin(), list.end());

    return app.run();
}