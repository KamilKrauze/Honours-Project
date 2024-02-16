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

struct ImgData
{
    EigenVectors eigen_vectors;
    EigenValues eigen_values;
    Contours contours;

    ImgData() = default;
    ~ImgData() = default;
};

// Function declarations
inline void drawAxis(cv::Mat, cv::Point, cv::Point, cv::Scalar, const float);
inline double getOrientation(cv::Mat img, const Points& pts, EigenVectors& vecs, EigenValues& vals);

// A wrapper function to streamline PCA-based fusion
inline cv::Mat executePCA(cv::Mat src, cv::Mat img_bw, Contours& contours, EigenVectors& vecs, EigenValues& vals);
cv::Mat fusePCAs(const cv::Mat& src1, const cv::Mat& src2, ImgData& img_data1, ImgData& img_data2);

int main()
{
    using namespace cv;

    ImgData img_data1, img_data2;
    img_data1.eigen_values.resize(2);
    img_data1.eigen_vectors.resize(2);
    img_data2.eigen_values.resize(2);
    img_data2.eigen_vectors.resize(2);

    Mat src1, src2, bw1, bw2;

    src1 = imread("../../exports/opencv/adaptive-histogram-eq/cl2_frame25.png", cv::IMREAD_GRAYSCALE);
    imshow("Source 1", src1);

    src2 = imread("../../exports/opencv/adaptive-histogram-eq/cl5_frame25.png", cv::IMREAD_GRAYSCALE);
    imshow("Source 2", src2);
    
    // Binary image
    threshold(src1, bw1, 95, 255, THRESH_BINARY);
    imshow("Binary 1", bw1);

    threshold(src2, bw2, 110, 255, THRESH_BINARY);
    imshow("Binary 2", bw2);

    // Find all the contours in the thresholded image
    auto pca1 = executePCA(src1, bw1, img_data1.contours, img_data1.eigen_vectors, img_data1.eigen_values);
    auto pca2 = executePCA(src2, bw2, img_data2.contours, img_data2.eigen_vectors, img_data2.eigen_values);

    //imshow("Img 1 PCA", pca1);
    //imshow("Img 2 PCA", pca2);

    Mat fused = fusePCAs(src1, src2, img_data1, img_data2);

    imshow("Fused Image", fused);

    waitKey(0);

    return 0;
}

inline void drawAxis(cv::Mat img, cv::Point p, cv::Point q, cv::Scalar colour, const float scale = 0.2)
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

inline double getOrientation(cv::Mat img, const Points& pts, EigenVectors& vecs, EigenValues& vals)
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
        vecs[i] = Point2d(pca_analysis.eigenvectors.at<double>(i, 0),
            pca_analysis.eigenvectors.at<double>(i, 1));
        vals[i] = pca_analysis.eigenvalues.at<double>(i);
    }
    // Draw the principal components
    //circle(img, cntr, 3, Scalar(255, 0, 255), 2);
    Point p1 = cntr + 0.02 * Point(static_cast<int>(vecs[0].x * vals[0]), static_cast<int>(vecs[0].y * vals[0]));
    Point p2 = cntr - 0.02 * Point(static_cast<int>(vecs[1].x * vals[1]), static_cast<int>(vecs[1].y * vals[1]));
    //drawAxis(img, cntr, p1, Scalar(0, 255, 0), 1);
    //drawAxis(img, cntr, p2, Scalar(255, 255, 0), 5);
    double angle = atan2(vecs[0].y, vecs[0].x); // orientation in radians
    return angle;
}

inline cv::Mat executePCA(cv::Mat src, cv::Mat img_bw, Contours& contours, EigenVectors& vecs, EigenValues& vals)
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
        //drawContours(src, contours, static_cast<int>(i), Scalar(0, 0, 255), 2);
        // Find the orientation of each shape
        getOrientation(src, contours[i], vecs, vals);
    }

    return src;
}

cv::Mat fusePCAs(const cv::Mat& src1, const cv::Mat& src2, ImgData& img_data1, ImgData& img_data2)
{
    using namespace cv;

    Mat fused = src1.clone(), fused_low = src1.clone(), fused_high = src1.clone();

    double weight1 = 0.0f, weight2 = 0.0f;

    // Equation 4.1 - https://tigerprints.clemson.edu/cgi/viewcontent.cgi?article=1615&context=all_dissertations - 16/02/2024s
    for (uint8_t i = 0; i < 2; i++)
    {
        // Get weights
        weight1 += img_data1.eigen_vectors[i].x / img_data1.eigen_vectors[i].x + img_data1.eigen_vectors[i].y;
        weight2 += img_data2.eigen_vectors[i].x / img_data2.eigen_vectors[i].x + img_data2.eigen_vectors[i].y;
    }

    weight1 /= 2;
    weight2 /= 2;

    // Low pass coefficients
    // Equation 4.2 - https://tigerprints.clemson.edu/cgi/viewcontent.cgi?article=1615&context=all_dissertations - 16/02/2024
    for (size_t i = 0; i < fused_low.rows; i++)
    {
        for (size_t j = 0; j < fused_low.cols; j++)
        {
            uint8_t pixel1 = src1.at<uint8_t>(i, j);
            uint8_t pixel2 = src2.at<uint8_t>(i, j);

            uint8_t weighted_pixel = saturate_cast<uint8_t>((weight1 * pixel1) + (weight2 * pixel2));

            fused_low.at<uint8_t>(i, j) = weighted_pixel;

        }
    }

    // High pass coefficients
    // Equation 4.3 - https://tigerprints.clemson.edu/cgi/viewcontent.cgi?article=1615&context=all_dissertations - 16/02/2024
    for (size_t i = 0; i < fused_low.rows; i++)
    {
        for (size_t j = 0; j < fused_low.cols; j++)
        {
            fused_high.at<uint8_t>(i,j) = max(src1.at<uint8_t>(i,j) * weight1, src2.at<uint8_t>(i, j) * weight2);
        }
    }


    // Combine low pass and high pass
    for (size_t i = 0; i < fused_low.rows; i++)
    {
        for (size_t j = 0; j < fused_low.cols; j++)
        {
            fused.at<uint8_t>(i, j) = fused_low.at<uint8_t>(i, j) ;
        }
    }

    return fused;
}