#ifndef DATA_PLOTTER_H
#define DATA_PLOTTER_H

#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>

class DataPlotter
{
public:
	// Quick definitions of certain elements in the pair and hashmap data structures.
	using DatasetKey = std::string_view; // The key associated with the dataset stored in MediaManager
	using DataPlotKey = std::string_view; // The key associated to which image measure algorithm
	using PlotDatasets = std::unordered_map<DataPlotKey, std::vector<double>>;

	using DataKeyPair = std::pair<DatasetKey, DataPlotKey>;
	using DataPlot = std::pair<std::vector<double>, std::vector<double>>;

public:
	DataPlotter();
	~DataPlotter();

public:
	static DataPlotter& Get() { return *s_Instance; }

public:
	void addDataPlot(DataKeyPair keyPair, const std::vector<double>&& plotdata);
	std::vector<std::string_view> getDatasetKeys() const noexcept;
	std::vector<std::string_view> getDataPlotKeys(std::string_view key) const noexcept;
	DataPlot getPlotData(std::string_view datasetKey, std::string_view dataPlotKey) noexcept;

private:
	DataKeyPair m_currently_shown;
	std::unordered_map<DatasetKey, PlotDatasets> m_dataset_plot_data;
	
	static DataPlotter* s_Instance; // Static reference to self (this).

};

#endif