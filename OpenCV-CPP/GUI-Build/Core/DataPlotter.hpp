#ifndef DATA_PLOTTER_H
#define DATA_PLOTTER_H

#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>

struct ComputeData
{
	std::string_view relationTo = ""; // The name of the dataset the plot_points relate to, i.e. if this is HEQ data then it relates to ORG per se.
	std::vector<double> plot_points;
};

class DataPlotter
{
public:
	// Quick definitions of certain elements in the pair and hashmap data structures.
	using DatasetKey = std::string_view; // The key associated with the dataset stored in MediaManager
	using ComputeKey = std::string_view; // The key associated to which image measuring algorithm
	using PlotDatasets = std::unordered_map<ComputeKey, ComputeData>;

	using PlotKeyPair = std::pair<DatasetKey, ComputeKey>;
	using GraphPlot = std::pair<std::vector<double>, std::vector<double>>;

public:
	DataPlotter();
	~DataPlotter();

public:
	static DataPlotter& Get() { return *s_Instance; }

public:
	void addDataPlot(PlotKeyPair keyPair, const std::vector<double>& plotdata, std::string_view _relationTo);
	std::vector<std::string_view> getDatasetKeys() const noexcept;
	std::vector<std::string_view> getDataPlotKeys(std::string_view key) const noexcept;
	GraphPlot getPlotData(std::string_view datasetKey, std::string_view dataPlotKey) noexcept;
	std::string_view getPlotDataRelation(std::string_view datasetKey, std::string_view dataPlotKey) noexcept;
	void exportPlotData(PlotKeyPair dataKeyPair, std::string_view file_path) noexcept;

private:
	std::unordered_map<DatasetKey, PlotDatasets> m_dataset_plot_data;
	
	static DataPlotter* s_Instance; // Static reference to self (this).

};

#endif