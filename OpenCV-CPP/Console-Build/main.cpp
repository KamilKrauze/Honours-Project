#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

int main()
{
    cv::Mat img = cv::imread("../../exports/dicom-data/frame00.png", cv::IMREAD_GRAYSCALE);

    std::cout << img.rows << "\n" << img.cols << "\n";
    cv::imshow("Source: Frame 0", img);
    cv::waitKey(0);

    return 0;
}