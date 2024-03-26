#include "GUI/PlotGUI.hpp"

#include <imgui.h>
#include <implot.h>

#include "Core/CAEHelper.h"
#include "Core/MediaManager.h"
#include "Core/DataPlotter.hpp"

#include "ImgCompute/Measure/ImageMeasures.hpp"

#include <iostream>

using Keys = std::vector<std::string_view>;

// REMOVE LATER

static double x_data[60], y_data[60];

static inline void createTempPlot()
{
	for (size_t i = 0; i < 60; i++)
	{
		x_data[i] = i;
		(std::sin(i * 0.25f) >= 0.0f) ? (y_data[i] = std::sin(i * 0.25f)) : (y_data[i] = (1.0f + std::sin(i * 0.25f)));
	}
}

// !REMOVE LATER

static size_t current_item_org = 0;
static size_t current_item_enh = 0;
static size_t current_item_img_measure = 0;

static void CreatePlotMenuButton()
{
	if (ImGui::Button("Create Plot"))
	{
		ImGui::OpenPopup("Select Datasets");
	}

	if (ImGui::BeginPopupModal("Select Datasets", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
	{
		ImGui::SetItemDefaultFocus();

		ImGui::Text("Select the datasets to be computed with which measuring algorithm: ");

		Keys keys = MediaManager::Get().getKeys();

		if (ImGui::BeginCombo("Original Dataset", keys[current_item_org].data(), ImGuiComboFlags_PopupAlignLeft))
		{
			for (size_t i = 0; i < keys.size(); i++)
			{
				const bool isSelected = (current_item_org == i);
				if (ImGui::Selectable(keys[i].data(), &isSelected))
					current_item_org = i;
			}
			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("Enhanced Dataset", keys[current_item_enh].data(), ImGuiComboFlags_HeightLargest))
		{
			for (size_t i = 0; i < keys.size(); i++)
			{
				const bool isSelected = (current_item_enh == i);
				if (ImGui::Selectable(keys[i].data(), &isSelected))
					current_item_enh = i;
			}
			ImGui::EndCombo();
		}

		ImGui::NewLine();

		ImGui::Text("Select the image measure to be computed");
		if (ImGui::BeginCombo("Image Measure", img_measure_selection->c_str(), ImGuiComboFlags_HeightLargest))
		{
			for (size_t i = 0; i < 3; i++)
			{
				const bool isSelected = (current_item_img_measure == i);
				if (ImGui::Selectable(img_measure_selection[i].c_str(), &isSelected))
					current_item_img_measure = i;
			}
			ImGui::EndCombo();
		}

		ImGui::NewLine();

		if (ImGui::Button("Apply"))
		{		
			if (current_item_org == current_item_enh)
				ImGui::CloseCurrentPopup(); goto APPLY_SKIP;

			// Run calculations here!
			

			ImGui::CloseCurrentPopup();
		}

	APPLY_SKIP:
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
			ImGui::CloseCurrentPopup();


		ImGui::EndPopup();
	}
}


void showPlotGraphSettings()
{
	ImGui::Begin("Plot Settings");

	CreatePlotMenuButton();

	ImGui::End();
}

void showImageMeasurePlots(std::string_view plot_name, const std::vector<double>& points)
{
	showPlotGraphSettings();

	std::string name = plot_name.data() + std::string(" time series");
	const size_t size = points.size();

	//auto data = DataPlotter::Get().getPlotData("heq", "CII");

	createTempPlot();

	ImGui::Begin("Contrast Measure Plot");
	if (ImPlot::BeginPlot(name.data(), "Frame No.", plot_name.data(), {-0.1,-1}, ImPlotFlags_Crosshairs | ImPlotFlags_NoLegend))
	{
		ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
		//ImPlot::PlotScatter("Scatter Plot", data.first.data(), points.data(), size);
		ImPlot::PlotScatter("Scatter Plot", x_data, y_data, 25);

		//ImPlot::PlotLine("Line Plot", data.first.data(), points.data(), size);
		ImPlot::PlotLine("Line Plot", x_data, y_data, 25);
		ImPlot::EndPlot();
	}
	ImGui::End();
}