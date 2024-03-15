#ifndef IMAGE_MEASURES_H
#define IMAGE_MEASURES_H

#include <opencv2/core.hpp>

namespace ImgMeasure
{
	// Computes the peak signal to noise ratio between the original frame and the 'enhanced' frame.
	double PSNR(const cv::Mat& org, const cv::Mat& enh);

	// Computes the structural similarity index measure.
	double SSIM(const cv::Mat& org, const cv::Mat& enh);

	// Computes the constrast improvement index - Higher the better
	double CII(const cv::Mat& org, const cv::Mat& enh);
};

#endif //! IMAGE_MEASURES_H