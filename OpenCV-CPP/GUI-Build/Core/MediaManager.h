#ifndef MEDIA_MANAGER_H
#define MEDIA_MANAGER_H

#include <vector>

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
		
	// Retrieves the original media container.
	std::vector<cv::Mat>& original_media() { return m_media_org; }
	
	// Retrieves the enhanced media container.
	std::vector<cv::Mat>& enhanced_media() { return m_media_enh; }

	// Gets the currently attached texture index.
	size_t get_current_index() const { return m_currently_attached; }

	// Retrieves the currently attached texture ID.
	ImTextureID texture();

public:
	// Load single image into buffer
	bool load_image(cv::String filepath, const cv::ImreadModes mode);

	// Load many images into buffer
	bool load_images(StringConstItr start, StringConstItr end);
	
	// Bind texture to memory.
	void bind(const size_t&& index);

	// Unbind texture from memory.
	void unbind();

public:
	void show_next_image();
	void show_previours_image();

public:
	void equalizeHistogram();

private:
	size_t m_currently_attached; // The index of the currently attached image.
	std::vector<cv::Mat> m_media_org; // Media container for the imported.
	std::vector<cv::Mat> m_media_enh; // Media container for the enhanced versions.
	std::vector<ImTextureID> m_textures; // Texture ID's loaded into memory using glBindTexture().
	static MediaManager* s_Instance; // Static reference to self (this).
};


#endif //!IMG_LOADER_H