#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
    cv::Mat img = cv::imread("./discord_avatar.png", cv::IMREAD_GRAYSCALE);
    cv::imshow("Discord Avatar", img);
    cv::waitKey(0);

    return 0;
}