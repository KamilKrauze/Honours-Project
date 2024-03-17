#ifndef IMAGE_COMPUTE_H
#define IMAGE_COMPUTE_H

namespace ImgMath {

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

    static uint8_t minOfMatrix(const cv::Mat& mat)
    {
        uint8_t min = 0;
        for (size_t i = 0; i < mat.rows; i++)
        {
            for (size_t j = 0; j < mat.cols; j++)
            {
                (mat.data[i * (mat.cols + j)] < min) ? (min = mat.data[i * (mat.cols + j)]) : (min = min);
            }
        }

        return min;
    }



    std::vector<double> coefficientsOfLocation(const cv::Mat& mat)
    {
        std::vector<uint8_t> min_vals(0), max_vals(0);

#pragma omp parallel for
        for (size_t i = 0; i < 510; i++)
        {
            for (size_t j = 0; j < 510; j++)
            {
                cv::Mat sub_region = mat(cv::Rect(j, i, 3, 3));

                double min = 0, max = 0;
                cv::minMaxLoc(sub_region, &min, &max);

                min_vals.push_back(static_cast<uint8_t>(min));
                max_vals.push_back(static_cast<uint8_t>(max));
            }
        }

        std::vector<double> coefficients(max_vals.size());
#pragma omp parallel for
        for (size_t i = 0; i < max_vals.size(); i++)
        {
            if ((max_vals[i] + min_vals[i]) == 0)
            {
                i += 2;
            }
            else
            {
                double a = static_cast<double>(max_vals[i] - min_vals[i]) / static_cast<double>(max_vals[i] + min_vals[i]);
                coefficients[i] = (a);
            }
        }

        return coefficients;
    }

    static double entropy(const std::vector<double>& probabilities)
    {
        double entropy = 0.0f;
        for (auto& p : probabilities)
        {
            if (p > 0.0f)
                entropy -= (p * std::log2(p));
        }
        return entropy;
    }

};

#endif // ! IMAGE_COMPUTE_H
