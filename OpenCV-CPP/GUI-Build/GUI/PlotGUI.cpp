#include "GUI/PlotGUI.hpp"

#include <imgui.h>
#include <implot.h>

#include "Core/CAEHelper.h"
#include "Core/MediaManager.h"
#include "Core/DataPlotter.hpp"

#include "ImgCompute/Measure/ImageMeasures.hpp"

#include <iostream>
#include <unordered_map>
#include <functional>

using Keys = std::vector<std::string_view>;

/* Boiler plate lambda macro
 -	NAME					=>	String name of the operation.
 -	LAMBDA_OPERATION		=>	The function the lambda uses. Has to take to 2 cv::Mat params and return a double value.
*/
#define IMG_MEASURE_LAMBDA(NAME, LAMBDA_OPERATION) { NAME, [](cv::Mat& MAT1, cv::Mat& MAT2){ return LAMBDA_OPERATION(MAT1, MAT2); } }

static std::unordered_map<ImgMeasuringAlgorithm, std::pair<std::string_view, std::function<double(cv::Mat&, cv::Mat&)>>> measuringAlgos = {
	{ImgMeasuringAlgorithm::CII, IMG_MEASURE_LAMBDA("CII", ImgMeasure::CII)},
	{ImgMeasuringAlgorithm::PSNR, IMG_MEASURE_LAMBDA("PSNR", ImgMeasure::PSNR)},
	{ImgMeasuringAlgorithm::SSIM, IMG_MEASURE_LAMBDA("SSIM", ImgMeasure::SSIM)},
};

static std::vector<double> x_data, y_data;
static bool use_tempdata = true;

static size_t current_item_org = 0;
static size_t current_item_enh = 0;
static size_t current_item_img_measure = 0;

static bool show_error_text = false;
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
		if (ImGui::BeginCombo("Image Measure", img_measure_selection[current_item_img_measure].c_str(), ImGuiComboFlags_HeightLargest))
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

		// Upon pressing 'Apply', an error text message will appear in the dialogue window only if the condition will be satisfied. 
		if ( ImGui::Button("Apply") && !(show_error_text = (current_item_org == current_item_enh)) )
		{		
			use_tempdata = false;
			show_error_text = false;

			//std::cout << "Org-> " << keys[current_item_org] << "\nEnh-> " << keys[current_item_enh] << "\nMeasure-> " << img_measure_selection[current_item_img_measure];
				
			// Get the amount of frame to be computed
			std::vector<cv::Mat>& org_frames = MediaManager::Get().getMedia(keys[current_item_org].data());
			std::vector<cv::Mat>& enh_frames = MediaManager::Get().getMedia(keys[current_item_enh].data());

			// Adjust plot size if need be!
			x_data.resize(org_frames.size());
			y_data.resize(org_frames.size());

			auto algorithm = measuringAlgos[static_cast<ImgMeasuringAlgorithm>(current_item_img_measure)];

			// Generate plot points
#pragma omp parallel for
			for (size_t i = 0; i < org_frames.size(); i++)
			{
				x_data[i] = i;
				y_data[i] = algorithm.second(org_frames[i], enh_frames[i]);
			}

			// Once computed add to the DataPlotter hashmap
			DataPlotter::Get().addDataPlot({ keys[current_item_enh], algorithm.first }, y_data, keys[current_item_org].data());

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			show_error_text = false;
			ImGui::CloseCurrentPopup();
		}

		if (show_error_text)
		{
			ImGui::TextColored({1,0,0,1}, "Selected datasets cannot be the same!");
		}

		ImGui::EndPopup();
	}
}
static size_t current_dataset = 0;
static size_t current_plot = 0;
static void SelectPlotToView()
{
	auto& keys = DataPlotter::Get().getDatasetKeys();

	// If there are no keys, then disable dropdown
	ImGui::BeginDisabled(keys.empty());
	{
		if (ImGui::BeginCombo("Dataset", (keys.empty()) ? ("None") : (keys[current_dataset].data()), ImGuiComboFlags_HeightLargest))
		{
			for (size_t i = 0; i < keys.size(); i++)
			{
				const bool isSelected = (current_dataset == i);
				if (ImGui::Selectable(keys[i].data(), &isSelected))
					current_dataset = i;
			}
			ImGui::EndCombo();
		}

		Keys& plot_keys = std::vector<std::string_view>();
		if (!keys.empty())
			plot_keys = DataPlotter::Get().getDataPlotKeys(keys[current_dataset]);

		if (ImGui::BeginCombo("Dataplot", (plot_keys.empty()) ? ("None") : (plot_keys[current_plot].data()), ImGuiComboFlags_HeightLargest))
		{
			for (size_t i = 0; i < plot_keys.size(); i++)
			{
				const bool isSelected = (current_plot == i);
				if (ImGui::Selectable(plot_keys[i].data(), &isSelected))
					current_plot = i;
			}

			ImGui::EndCombo();
		}
	}
	ImGui::EndDisabled();
}

void showPlotGraphSettings()
{
	ImGui::Begin("Plot Settings");

	CreatePlotMenuButton();
	ImGui::NewLine();
	SelectPlotToView();

	ImGui::End();
}

void showImageMeasurePlots()
{
	showPlotGraphSettings();

	Keys keys = DataPlotter::Get().getDatasetKeys();
	const char* measure_name = measuringAlgos[static_cast<ImgMeasuringAlgorithm>(current_plot)].first.data();

	std::string name = measure_name + std::string(" time series");

	DataPlotter::GraphPlot plot;

	if (!use_tempdata || !DataPlotter::Get().getDatasetKeys().empty())
	{
		plot = DataPlotter::Get().getPlotData(keys[current_dataset], measure_name);
		x_data = plot.first;
		y_data = plot.second;

		const auto plots = DataPlotter::Get().getDataPlotKeys(keys[current_dataset]);
		const std::string relationName = DataPlotter::Get().getPlotDataRelation(keys[current_dataset], plots[current_plot]).data();
		name += std::string(" of '") + keys[current_dataset].data() + "' in relation to '" + relationName + "'";
	}

	ImGui::Begin("Image Measure Plot");
	if (ImPlot::BeginPlot(name.data(), "Frame No.", measure_name, {-0.1,-1}, ImPlotFlags_Crosshairs | ImPlotFlags_NoLegend, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit))
	{
		ImPlot::SetNextMarkerStyle(ImPlotMarker_Diamond);
		//ImPlot::PlotScatter("Scatter Plot", data.first.data(), points.data(), size);
		ImPlot::PlotScatter("Scatter Plot", x_data.data(), y_data.data(), y_data.size());

		//ImPlot::PlotLine("Line Plot", data.first.data(), points.data(), size);
		ImPlot::PlotLine("Line Plot", x_data.data(), y_data.data(), y_data.size());
		ImPlot::EndPlot();
	}
	ImGui::End();
}