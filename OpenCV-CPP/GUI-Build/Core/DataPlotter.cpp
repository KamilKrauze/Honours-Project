#include "Core/DataPlotter.hpp"
#include <iostream>

DataPlotter* DataPlotter::s_Instance = nullptr;

DataPlotter::DataPlotter()
{
	if (!s_Instance)
		this->s_Instance = this;
}

DataPlotter::~DataPlotter() = default;

void DataPlotter::addDataPlot(PlotKeyPair keyPair, const std::vector<double>& plotdata, std::string_view _relationTo)
{
	m_dataset_plot_data[keyPair.first][keyPair.second].plot_points = plotdata;
	m_dataset_plot_data[keyPair.first][keyPair.second].relationTo = _relationTo;
	return;
}

std::vector<std::string_view> DataPlotter::getDatasetKeys() const noexcept
{
	std::vector<std::string_view> datasetKeys;
	for (auto it = m_dataset_plot_data.begin(); it != m_dataset_plot_data.end(); it++)
		datasetKeys.push_back((*it).first);

	return datasetKeys;
}

std::vector<std::string_view> DataPlotter::getDataPlotKeys(std::string_view key) const noexcept
{
	const auto& measures = m_dataset_plot_data.find(key.data());
	if (measures == m_dataset_plot_data.cend())
		return std::vector<std::string_view>();

	std::vector<std::string_view> dataPlotKeys;
	for (auto it = measures->second.begin(); it != measures->second.end(); it++)
		dataPlotKeys.push_back((*it).first);

	return dataPlotKeys;
}

DataPlotter::GraphPlot DataPlotter::getPlotData(std::string_view datasetKey, std::string_view dataPlotKey) noexcept
{
	// If the dataset key is not found then return empty vector.
	auto& plot_data = m_dataset_plot_data.find(datasetKey);
	if (plot_data == m_dataset_plot_data.end())
		return { std::vector<double>(), std::vector<double>() };

	// If the dataplot key is not found then return empty vector.
	auto& plotpoints = plot_data->second.find(dataPlotKey);
	if (plotpoints == plot_data->second.end())
		return { std::vector<double>(), std::vector<double>() };


	std::vector<double>& y_data = plotpoints->second.plot_points;

	std::vector<double> frames(y_data.size());
	for (size_t i = 0; i < y_data.size(); i++)
		frames[i] = i;

	return { frames , y_data };
}

std::string_view DataPlotter::getPlotDataRelation(std::string_view datasetKey, std::string_view plotKey) noexcept
{
	return this->m_dataset_plot_data[datasetKey][plotKey].relationTo;
}

void DataPlotter::exportPlotData(PlotKeyPair dataKeyPair, std::string_view file_path) noexcept
{


	return;
}
