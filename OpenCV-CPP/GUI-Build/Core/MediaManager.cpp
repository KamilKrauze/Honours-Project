#include "Core/MediaManager.h"
#include "Core/CAEHelper.h"

#include <iostream>
#include <cassert>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <glad/glad.h>

/* Helper macros */

// Convert GLuint to ImTextureID
#define GLUINT_TO_IMTEXTUREID(VALUE) ((ImTextureID)(intptr_t)VALUE)

// Reference to instance
MediaManager* MediaManager::s_Instance = nullptr;

MediaManager::MediaManager()
{
	if (!s_Instance)
		this->s_Instance = this;

	this->m_currently_attached.first = "";
	this->m_currently_attached.second = -1;
}

MediaManager::~MediaManager()
{
	if (m_currently_attached.second >= 0)
		unbind();
	this->m_textures.clear();
	this->m_currently_attached.first = "";
	this->m_currently_attached.second = -1;

	//this->s_Instance = nullptr;
}

std::vector<std::string_view> MediaManager::getKeys() const noexcept
{
	std::vector<std::string_view> keys;
	for (auto it = m_media.begin(); it != m_media.end(); it++)
	{
		keys.push_back((*it).first);
	}

	return keys;
}

ImTextureID MediaManager::getTextureID() const noexcept
{
	return m_textures[m_currently_attached.second];
}

bool MediaManager::load_image(std::string_view key, cv::String filepath, const cv::ImreadModes mode)
{
	cv::Mat img = cv::imread(filepath, mode);
	if (img.empty())
		return false;

	this->m_media[key.data()].push_back(img);

	// Resize texture ID pool
	const size_t texture_id_pool = m_textures.size();
	m_textures.resize(texture_id_pool + 1);

	return true;
}

bool MediaManager::load_images(std::string_view key, StringConstItr start, StringConstItr end)
{
	if (start._Ptr == nullptr || end._Ptr == nullptr)
		return false;

	for (auto it = start; it != end; it++)
	{
		cv::Mat img = cv::imread(*it._Ptr, cv::IMREAD_ANYCOLOR);
		if (img.empty())
			return false;

		m_media[key.data()].push_back(img);
	}

	size_t size = end - start;

	// Resize texture ID pool
	size_t current_size = m_textures.size();
	m_textures.resize(size + current_size);
	return true;
}

void MediaManager::bind(std::string_view key, const size_t&& index)
{
	if (index >= m_textures.size())
	{
		assert((index > m_textures.size()) && "Index is too large!");
		return;
	}

	if (m_currently_attached.second >= 0 || index >= 0)
		unbind();

	ImTextureID texture_id = CAE::Helper::MatToImTextureID(this->m_media[key.data()][index]);
	this->m_textures[index] = texture_id;
	this->m_currently_attached.first = {key};
	this->m_currently_attached.second = index;

	return;
}

void MediaManager::unbind()
{
	if (m_currently_attached.second < 0)
		return;

	auto texture_id = (GLuint)(intptr_t)(this->m_textures[m_currently_attached.second]);
	//glBindTexture(GL_TEXTURE_2D, 0); // Unbind
	//m_textures[m_currently_attached] = NULL;
	//m_currently_attached = -1;

	glDeleteTextures(1, &texture_id); // Delete it from memory.
	return;
}

void MediaManager::equalizeHistogram(std::string_view src, std::string_view dst)
{
	if (m_currently_attached.second >= 0)
	{
		Frames& frames = m_media[src.data()];
		Frames new_frames(frames.size());

		// Convert to grayscale
		for (size_t i=0; i<frames.size(); i++)
		{
			if (frames[i].channels() > 1)
				cv::cvtColor(frames[i], new_frames[i], cv::COLOR_RGB2GRAY);

			// Equalize histogram
			cv::equalizeHist(new_frames[i], new_frames[i]);
			cv::cvtColor(new_frames[i], new_frames[i], cv::COLOR_GRAY2RGB); // Convert colours
		}		

		m_media[dst.data()] = new_frames;
		return;
	}
}
