#ifndef IMG_LOADER_H

#include <vector>
#include <string_view>

#include <imgui.h>
#include <opencv2/imgcodecs.hpp>

class cv::Mat;

class MediaManager
{
public:
	using StringConstItr = std::vector<cv::String>::const_iterator; // Shortened down variation

public:
	MediaManager();
	~MediaManager();

public:
	// Get media-loader instance
	static MediaManager& Get() { return *s_Instance; }
		
	// Retrieves media container
	std::vector<cv::Mat> media() { return m_media; }

public:
	// Load single image into buffer
	bool load_image(cv::String filepath, const cv::ImreadModes mode);

	// Load many images into buffer
	bool load_images(StringConstItr start, StringConstItr end, const cv::ImreadModes mode);

public:
	// Convert OpenCV Matrix type to ImTextureID
	const ImTextureID MatToImTextureID(const cv::Mat& mat);

public:
	size_t selected = 0; // Controls for which image is selected in the container.

private:
	std::vector<cv::Mat> m_media = std::vector<cv::Mat>(0); // Media container
	static MediaManager* s_Instance; // Static reference to self (this)
};


#endif //!IMG_LOADER_H