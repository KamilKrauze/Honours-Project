#include "GUI/PlotGUI.hpp"

#include <vector>

#include <imgui.h>
#include <implot.h>

#include "Core/CAEHelper.h"
#include "Core/MediaManager.h"

static std::vector<double> x_data(51), y_data(51);

void showImageMeasurePlots(std::string_view plot_name)
{
	for (size_t i = 0; i < 51; i++)
	{
		x_data[i] = i;
	}

	for (int i = 0; i < 51; ++i) {
		float result = 0.9 * sin(25 * x_data[i]) * cos(2 * x_data[i]);
		y_data[i] = std::clamp(result, 0.0f, result);
	}

	ImGui::Begin("Contrast Measure Plot");
	if (ImPlot::BeginPlot(plot_name.data(), { -0.1,-1 }, ImPlotFlags_Crosshairs | ImPlotFlags_NoLegend))
	{
		ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
		ImPlot::PlotScatter("Scatter Plot", &*x_data.data(), &*y_data.data(), x_data.size());

		ImPlot::PlotLine("Line Plot", &*x_data.data(), &*y_data.data(), x_data.size());
		ImPlot::EndPlot();
	}
	ImGui::End();
}