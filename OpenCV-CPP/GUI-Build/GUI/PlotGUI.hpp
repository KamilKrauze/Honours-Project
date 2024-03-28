#ifndef PLOT_GUI_HPP
#define PLOT_GUI_HPP

#include <vector>
#include <string_view>


enum class ImgMeasuringAlgorithm
{
	CII = 0,
	PSNR = 1,
	SSIM = 2,
	// Add more here following this convention --> [ NAME = n ]
};

static const std::string img_measure_selection[3] =
{
	"CII",
	"PSNR",
	"SSIM"
};

void showPlotGraphSettings();

void showImageMeasurePlots();

#endif // !PLOT_GUI_HPP