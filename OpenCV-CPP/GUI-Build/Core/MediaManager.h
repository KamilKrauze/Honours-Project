#ifndef MEDIA_MANAGER_H
#define MEDIA_MANAGER_H

#include <vector>
#include <unordered_map>
#include <string_view>

#include <imgui.h>
#include <opencv2/imgcodecs.hpp>

class cv::Mat;

constexpr std::string_view ORIGINAL = { "original" };

class MediaManager
{
public:
	using StringConstItr = std::vector<cv::String>::const_iterator; // Shortened down variation
	using Frame = cv::Mat;
	using Frames = std::vector<Frame>;

public:
	MediaManager();
	~MediaManager();

public:
	// Get media-loader instance
	static MediaManager& Get() { return *s_Instance; }
		
	// Retrieves a frame container associated to the valid key.
	Frames& getMedia(std::string_view key) { return m_media[key]; }

	// Gets the size of the frame container by the valid key.
	size_t getTotal(std::string_view key) const noexcept { return m_media.at(key).size(); }

	// Gets the currently attached frame key.
	std::string_view getCurrentKey() const noexcept { return m_currently_attached.first; }
	
	// Gets the currently attached frame index.
	size_t getCurrentIndex() const noexcept { return m_currently_attached.second; }

	// Get list of keys
	std::vector<std::string_view> getKeys() const noexcept;

	// Retrieves the currently attached texture ID.
	ImTextureID getTextureID() const noexcept;

public:
	// Load single image into buffer
	bool load_image(std::string_view key, cv::String filepath, const cv::ImreadModes mode);

	// Load many images into buffer
	bool load_images(std::string_view key, StringConstItr start, StringConstItr end);
	
	// Bind texture to memory.
	void bind(std::string_view key, const size_t&& index);

	// Unbind texture from memory.
	void unbind();

public:
	void equalizeHistogram();

private:
	std::unordered_map<std::string_view, Frames> m_media;
	std::pair<std::string_view, size_t> m_currently_attached; // The index of the currently attached image.
	std::vector<ImTextureID> m_textures; // Texture ID's loaded into memory using glBindTexture().
	static MediaManager* s_Instance; // Static reference to self (this).
};


#endif //!IMG_LOADER_H