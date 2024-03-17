#include "ImageUtils/Measure/ImageMeasures.hpp"
#include "ImageUtils/Math/ImageMath.hpp"

#include <iostream>
#include <cassert>
#include <vector>
#include <thread>

double ImgMeasure::PSNR(const cv::Mat& org, const cv::Mat& enh)
{
    const std::pair<size_t, size_t> ORG_SIZE = { org.rows, org.cols  };
    const std::pair<size_t, size_t> ENH_SIZE = { enh.rows, enh.cols  };

    assert(ORG_SIZE == ENH_SIZE);

    // (L_1) - The max intensity of the image
    const uint8_t L_1 = ImgMath::maxOfMatrix(enh);

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
    const uint8_t ORG_MEAN = ImgMath::meanOfMatrix(org);
    const uint8_t ENH_MEAN = ImgMath::meanOfMatrix(enh);

    const double ORG_VARIANCE = ImgMath::varianceOfMatrix(org, ORG_MEAN);
    const double ENH_VARIANCE = ImgMath::varianceOfMatrix(enh, ENH_MEAN);

    const double COVARIANCE = std::sqrt(ImgMath::covarianceOfMatrix(org, enh));

    const uint8_t L = ImgMath::maxOfMatrix(enh);
    const double k1 = 0.01f;
    const double c1 = std::pow(k1 * L, 2);

    const double k2 = 0.03f;
    const double c2 = std::pow(k2 * L, 2);

    const double numerator = (2 * ORG_MEAN * ENH_MEAN + c1) * (2 * COVARIANCE + c2);
    const double denominator = (std::pow(ORG_MEAN, 2) + std::pow(ENH_MEAN, 2) + c1)
        + (ORG_VARIANCE + ENH_VARIANCE + c2);

    return numerator / denominator;
}

double ImgMeasure::CII(const cv::Mat& org, const cv::Mat& enh)
{

    std::vector<double> Cloc_org(0), Cloc_enh(0);
    double entropy_org = 0.0f, entropy_enh = 0.0f;

    std::thread t1([&org, &Cloc_org, &entropy_org] {
        Cloc_org = ImgMath::coefficientsOfLocation(org); // Local contrast of original image
        entropy_org = ImgMath::entropy(Cloc_org);
        });

    std::thread t2([&enh, &Cloc_enh, &entropy_enh] {
        Cloc_enh = ImgMath::coefficientsOfLocation(enh); // Local contrast of 'enhanced' image
        entropy_enh = ImgMath::entropy(Cloc_enh);
        });

    t1.join();
    t2.join();

    return entropy_enh / entropy_org;
}
