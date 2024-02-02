#ifndef MEDIA_MANAGER_H
#define MEDIA_MANAGER_H


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

	ImTextureID texture();

public:
	// Load single image into buffer
	bool load_image(cv::String filepath, const cv::ImreadModes mode);

	// Load many images into buffer
	bool load_images(StringConstItr start, StringConstItr end, const cv::ImreadModes mode);
	
	// Bind texture to memory.
	void attach(const size_t&& selected);

	// Unbind texture from memory.
	void dettach();

private:
	size_t selected = 0; // Controls for which image to load from the container.
	std::vector<cv::Mat> m_media; // Media container.
	std::vector<unsigned int> m_textures; // Texture ID's loaded into memory using glBindTexture().
	static MediaManager* s_Instance; // Static reference to self (this).
};


#endif //!IMG_LOADER_H