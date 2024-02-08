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
	std::vector<cv::Mat>& original_media() { return m_media_org; }
	std::vector<cv::Mat>& enhanced_media() { return m_media_enh; }

	ImTextureID texture();

public:
	// Load single image into buffer
	bool load_image(cv::String filepath, const cv::ImreadModes mode);

	// Load many images into buffer
	bool load_images(StringConstItr start, StringConstItr end);
	
	// Bind texture to memory.
	void attach(const size_t&& index);

	// Unbind texture from memory.
	void dettach();

public:
	void equalizeHistogram();

private:
	size_t m_selected;
	long int m_currently_attached;

	std::vector<cv::Mat> m_media_org; // Media container for the imported.
	std::vector<cv::Mat> m_media_enh; // Media container for the enhanced versions.
	std::vector<ImTextureID> m_textures; // Texture ID's loaded into memory using glBindTexture().
	static MediaManager* s_Instance; // Static reference to self (this).
};


#endif //!IMG_LOADER_H