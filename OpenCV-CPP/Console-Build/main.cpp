#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>

// All of the code has been adapated from https://docs.opencv.org/4.x/d1/dee/tutorial_introduction_to_pca.html - 07/02/2024

using EigenVectors = std::vector<cv::Point2d>;
using EigenValues = std::vector<double>;
using Points = std::vector<cv::Point>;
using Contours = std::vector<Points>;

// Function declarations
void drawAxis(cv::Mat&, cv::Point, cv::Point, cv::Scalar, const float);
double getOrientation(cv::Mat& img, const Points& pts, EigenVectors& eigen_vecs, EigenValues& eigen_vals);
cv::Mat executePCA(cv::Mat src, cv::Mat img_bw, Contours& contours, EigenVectors& eigen_vecs, EigenValues& eigen_vals);


int main()
{
    using namespace cv;

    EigenVectors eigen_vecs(2);
    EigenValues eigen_vals(2);

    Mat src = imread("../../exports/opencv/adaptive-histogram-eq/cl3_frame25.png", cv::IMREAD_GRAYSCALE);
    imshow("Source: Frame 0", src);
    
    // Binary image;
    Mat bw;
    threshold(src, bw, 70, 255, THRESH_BINARY | THRESH_OTSU);
    imshow("Binary: Frame 0", bw);

    // Find all the contours in the thresholded image
    Contours contours;
    executePCA(src, bw, contours, eigen_vecs, eigen_vals);

    imshow("output", src);

    waitKey(0);

    return 0;
}

void drawAxis(cv::Mat& img, cv::Point p, cv::Point q, cv::Scalar colour, const float scale = 0.2)
{
    using namespace cv;

    double angle = atan2((double)p.y - q.y, (double)p.x - q.x); // angle in radians
    double hypotenuse = sqrt((double)(p.y - q.y) * (p.y - q.y) + (p.x - q.x) * (p.x - q.x));
    // Here we lengthen the arrow by a factor of scale
    q.x = (int)(p.x - scale * hypotenuse * cos(angle));
    q.y = (int)(p.y - scale * hypotenuse * sin(angle));
    line(img, p, q, colour, 1, LINE_AA);
    // create the arrow hooks
    p.x = (int)(q.x + 9 * cos(angle + CV_PI / 4));
    p.y = (int)(q.y + 9 * sin(angle + CV_PI / 4));
    line(img, p, q, colour, 1, LINE_AA);
    p.x = (int)(q.x + 9 * cos(angle - CV_PI / 4));
    p.y = (int)(q.y + 9 * sin(angle - CV_PI / 4));
    line(img, p, q, colour, 1, LINE_AA);
}

double getOrientation(cv::Mat& img, const Points& pts, EigenVectors& eigen_vecs, EigenValues& eigen_vals)
{
    using namespace cv;

    //Construct a buffer used by the pca analysis
    int sz = static_cast<int>(pts.size());
    Mat data_pts = Mat(sz, 2, CV_64F);
    for (int i = 0; i < data_pts.rows; i++)
    {
        data_pts.at<double>(i, 0) = pts[i].x;
        data_pts.at<double>(i, 1) = pts[i].y;
    }
    //Perform PCA analysis
    PCA pca_analysis(data_pts, Mat(), PCA::DATA_AS_ROW);
    //Store the center of the object
    Point cntr = Point(static_cast<int>(pca_analysis.mean.at<double>(0, 0)),
        static_cast<int>(pca_analysis.mean.at<double>(0, 1)));
    //Store the eigenvalues and eigenvectors
    for (int i = 0; i < 2; i++)
    {
        eigen_vecs[i] = Point2d(pca_analysis.eigenvectors.at<double>(i, 0),
            pca_analysis.eigenvectors.at<double>(i, 1));
        eigen_vals[i] = pca_analysis.eigenvalues.at<double>(i);
    }
    // Draw the principal components
    circle(img, cntr, 3, Scalar(255, 0, 255), 2);
    Point p1 = cntr + 0.02 * Point(static_cast<int>(eigen_vecs[0].x * eigen_vals[0]), static_cast<int>(eigen_vecs[0].y * eigen_vals[0]));
    Point p2 = cntr - 0.02 * Point(static_cast<int>(eigen_vecs[1].x * eigen_vals[1]), static_cast<int>(eigen_vecs[1].y * eigen_vals[1]));
    drawAxis(img, cntr, p1, Scalar(0, 255, 0), 1);
    drawAxis(img, cntr, p2, Scalar(255, 255, 0), 5);
    double angle = atan2(eigen_vecs[0].y, eigen_vecs[0].x); // orientation in radians
    return angle;
}

// A wrapper function to streamline PCA-based fusion
cv::Mat executePCA(cv::Mat src, cv::Mat img_bw, Contours& contours, EigenVectors& eigen_vecs, EigenValues& eigen_vals)
{
    using namespace cv;

    findContours(img_bw, contours, RETR_LIST, CHAIN_APPROX_NONE);
    for (size_t i = 0; i < contours.size(); i++)
    {
        // Calculate the area of each contour
        double area = contourArea(contours[i]);
        // Ignore contours that are too small or too large
        if (area < 1e2 || 1e5 < area) continue;
        // Draw each contour only for visualisation purposes
        drawContours(src, contours, static_cast<int>(i), Scalar(0, 0, 255), 2);
        // Find the orientation of each shape
        getOrientation(src, contours[i], eigen_vecs, eigen_vals);
    }

    return src;
}