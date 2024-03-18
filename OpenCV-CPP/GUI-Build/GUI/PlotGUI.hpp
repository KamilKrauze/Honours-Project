#ifndef PLOT_GUI_HPP
#define PLOT_GUI_HPP

#include <vector>
#include <string_view>

static const std::string img_measure_selection[3] =
{
	"Contrast Improvement Index (CII)",
	"Peak Signal to Noise Ratio (PSNR)",
	"Structural Similarity Index Measure (SSIM)"
};

void showPlotGraphSettings();

void showImageMeasurePlots(std::string_view plot_name, const std::vector<double>& points);

#endif // !PLOT_GUI_HPP