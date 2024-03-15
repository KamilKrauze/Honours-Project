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

static uint8_t meanOfMatrix(const cv::Mat& mat)
{
    const std::pair<size_t, size_t> SIZE = { mat.rows, mat.cols };
    uint8_t pixel_sum = 0;
    for (size_t i = 0; i < SIZE.first; i++)
    {
        for (size_t j = 0; j < SIZE.second; j++)
        {
            pixel_sum += mat.data[i * (mat.cols + j)];
        }
    }

    return pixel_sum / (mat.rows * mat.cols);
}

static double varianceOfMatrix(const cv::Mat& mat, double mean)
{
    const std::pair<size_t, size_t> SIZE = { mat.rows, mat.cols };

    double summation = 0.0f;
    for (size_t i = 0; i < SIZE.first; i++)
    {
        for (size_t j = 0; j < SIZE.second; j++)
        {
            summation += std::pow(mat.data[i + (SIZE.first * j)], 2);
        }
    }

    return summation / (SIZE.first * SIZE.second);
}

static double covarianceOfMatrix(const cv::Mat& org, const cv::Mat& enh)
{
    const std::pair<size_t, size_t> ORG_SIZE = { org.rows, org.cols };
    const std::pair<size_t, size_t> ENH_SIZE = { enh.rows, enh.cols };

    const uint8_t ORG_MEAN = meanOfMatrix(org);
    const uint8_t ENH_MEAN = meanOfMatrix(enh);

    const auto totalPixels = (ENH_SIZE.first * ENH_SIZE.second) - 1;

    double summation = 0.0f;
    for (size_t i = 0; i < ENH_SIZE.first; i++)
    {
        for (size_t j = 0; j < ENH_SIZE.second; j++)
        {
            summation += (org.data[i + (ORG_SIZE.first * j)] - ORG_MEAN)
                * (enh.data[i + (ENH_SIZE.first * j)] - ENH_MEAN);
        }
    }

    return summation / totalPixels;
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

double ImgMeasure::SSIM(const cv::Mat& org, const cv::Mat& enh)
{
    const uint8_t ORG_MEAN = meanOfMatrix(org);
    const uint8_t ENH_MEAN = meanOfMatrix(enh);

    const double ORG_VARIANCE = varianceOfMatrix(org, ORG_MEAN);
    const double ENH_VARIANCE = varianceOfMatrix(enh, ENH_MEAN);

    const double COVARIANCE = std::sqrt(covarianceOfMatrix(org, enh));

    const uint8_t L = maxOfMatrix(enh);
    const double k1 = 0.01f;
    const double c1 = std::pow(k1 * L, 2);

    const double k2 = 0.03f;
    const double c2 = std::pow(k2 * L, 2);

    const double numerator = (2 * ORG_MEAN * ENH_MEAN + c1) * (2 * COVARIANCE + c2);
    const double denominator = (std::pow(ORG_MEAN, 2) + std::pow(ENH_MEAN, 2) + c1)
        + (ORG_VARIANCE + ENH_VARIANCE + c2);

    return numerator / denominator;
}
