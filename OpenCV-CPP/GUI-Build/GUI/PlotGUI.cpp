#include "GUI/PlotGUI.hpp"

#include <imgui.h>
#include <implot.h>

#include "Core/CAEHelper.h"
#include "Core/MediaManager.h"
#include "Core/DataPlotter.hpp"


using Keys = std::vector<std::string_view>;

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

		static size_t current_item_org = 0;
		static size_t current_item_enh = 0;
		static size_t current_item_img_measure = 0;

		Keys keys = MediaManager::Get().getKeys();

		if (ImGui::BeginCombo("Original Dataset", "---", ImGuiComboFlags_PopupAlignLeft))
		{
			for (size_t i = 0; i < keys.size(); i++)
			{
				const bool isSelected = (current_item_org == i);
				if (ImGui::Selectable(keys[i].data(), &isSelected))
					current_item_org = i;
			}
			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("Enhanced Dataset", "---", ImGuiComboFlags_PopupAlignLeft))
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
		if (ImGui::BeginCombo("Image Measure", "---", ImGuiComboFlags_PopupAlignLeft))
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
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}


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

	auto data = DataPlotter::Get().getPlotData("heq", "CII");

	ImGui::Begin("Contrast Measure Plot");
	if (ImPlot::BeginPlot(name.data(), "Frame No.", plot_name.data(), {-0.1,-1}, ImPlotFlags_Crosshairs | ImPlotFlags_NoLegend))
	{
		ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
		ImPlot::PlotScatter("Scatter Plot", data.first.data(), points.data(), size);

		ImPlot::PlotLine("Line Plot", data.first.data(), points.data(), size);
		ImPlot::EndPlot();
	}
	ImGui::End();
}