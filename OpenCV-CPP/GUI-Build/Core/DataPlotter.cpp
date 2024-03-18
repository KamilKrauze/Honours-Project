#include "Core/DataPlotter.hpp"

DataPlotter* DataPlotter::s_Instance = nullptr;

DataPlotter::DataPlotter()
{
	if (!s_Instance)
		this->s_Instance = this;

	this->m_currently_shown = { "", "" };
}

DataPlotter::~DataPlotter()
{
	this->m_currently_shown = { "", "" };
}

void DataPlotter::addDataPlot(DataKeyPair keyPair, const std::vector<double>&& plotdata)
{
	m_dataset_plot_data[keyPair.first][keyPair.second] = plotdata;
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
	const PlotDatasets& measures = m_dataset_plot_data.at(key.data());
	std::vector<std::string_view> dataPlotKeys;
	for (auto it = measures.begin(); it != measures.end(); it++)
		dataPlotKeys.push_back((*it).first);


	return std::vector<std::string_view>();
}

DataPlotter::DataPlot DataPlotter::getPlotData(std::string_view datasetKey, std::string_view dataPlotKey) noexcept
{
	auto& y_data = m_dataset_plot_data.at(datasetKey).at(dataPlotKey);
	this->m_currently_shown = { datasetKey, dataPlotKey };

	std::vector<double> frames(y_data.size());
	for (size_t i = 0; i < y_data.size(); i++)
		frames[i] = i;

	return { frames , y_data };
}
