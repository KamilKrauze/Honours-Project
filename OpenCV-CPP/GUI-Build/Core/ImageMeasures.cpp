#include "ImageMeasures.h"

#include <iostream>
#include <cassert>
#include <vector>
#include <thread>

static uint8_t maxOfMatrix(const cv::Mat& mat)
{
    uint8_t max = 0;
    for (size_t i = 0; i < mat.rows; i++)
    {
        for (size_t j = 0; j < mat.cols; j++)
        {
            (mat.data[i * (mat.cols + j)] > max) ? (max = mat.data[i * (mat.cols + j)]) : (max = max);
        }
    }

    return max;
}

double ImgMeasure::PSNR(const cv::Mat& org, const cv::Mat& enh)
{
    const std::pair<size_t, size_t> ORG_SIZE = { org.rows, org.cols  };
    const std::pair<size_t, size_t> ENH_SIZE = { enh.rows, enh.cols  };

    assert(ORG_SIZE == ENH_SIZE);

    // (L_1) - The max intensity of the image
    const uint8_t L_1 = maxOfMatrix(enh);

    // Total number of pixels
    const size_t N = ENH_SIZE.first * ENH_SIZE.second;

    double summation_x_y = 0;
#pragma omp parallel for
    for (size_t i = 0; i < ENH_SIZE.first; i++)
    {
        for (size_t j = 0; j < ENH_SIZE.second; j++)
        {
            int pixel = org.data[i * (ORG_SIZE.second + j)] - enh.data[i * (ENH_SIZE.second + j)];
            pixel = std::pow(std::abs(pixel), 2);
            summation_x_y += pixel;
        }
    }

    double fraction = std::pow(L_1, 2) / ( summation_x_y / N);

    return 10 * std::log10(fraction);
}
