#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

int main()
{
    cv::Mat img = cv::imread("../../exports/dicom-data/frame25.png", cv::IMREAD_GRAYSCALE);
    std::cout << img.rows << "\n" << img.cols << "\n";

    cv::Mat enh;
    cv::equalizeHist(img, enh);

    cv::imshow("Source: Frame 25", img);
    cv::imshow("Enhanced: Frame 25", enh);
    cv::waitKey(0);

    return 0;
}