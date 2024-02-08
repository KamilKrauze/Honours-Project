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

	this->m_currently_attached = -1;
}

MediaManager::~MediaManager()
{
	this->m_media_org.clear();
	this->m_media_enh.clear();
	if (m_currently_attached >= 0)
		unbind();
	this->m_textures.clear();
	this->m_currently_attached = NULL;

	//this->s_Instance = nullptr;
}

ImTextureID MediaManager::texture()
{
	return m_textures[m_currently_attached];
}

bool MediaManager::load_image(cv::String filepath, const cv::ImreadModes mode)
{
	cv::Mat img = cv::imread(filepath, mode);
	if (img.empty())
		return false;

	this->m_media_org.push_back(img);

	// Resize enhanced texture pool
	const size_t enh_texture_pool = m_media_enh.size();
	m_media_enh.resize(enh_texture_pool + 1);

	// Resize texture ID pool
	const size_t texture_id_pool = m_textures.size();
	m_textures.resize(texture_id_pool + 1);

	return true;
}

bool MediaManager::load_images(StringConstItr start, StringConstItr end)
{
	if (start._Ptr == nullptr || end._Ptr == nullptr)
		return false;

	for (auto it = start; it != end; it++)
	{
		cv::Mat img = cv::imread(*it._Ptr, cv::IMREAD_ANYCOLOR);
		if (img.empty())
			return false;

		m_media_org.push_back(img);
	}

	size_t size = end - start;

	// Resize enhanced texture pool
	const size_t enh_texture_pool = m_media_enh.size();
	m_media_enh.resize(size + enh_texture_pool);

	// Resize texture ID pool
	size_t current_size = m_textures.size();
	m_textures.resize(size + current_size);
	return true;
}

void MediaManager::bind(const size_t&& index)
{
	if (index >= m_textures.size())
	{
		assert((index > m_texture.size()) && "Index is too large!");
		return;
	}

	if (m_currently_attached >= 0)
		unbind();


	ImTextureID texture_id = CAE::Helper::MatToImTextureID(this->m_media_org[index]);
	this->m_textures[index] = texture_id;
	this->m_currently_attached = index;

	return;
}

void MediaManager::unbind()
{
	if (m_currently_attached < 0)
		return;

	auto texture_id = (GLuint)(intptr_t)(this->m_textures[m_currently_attached]);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind
	m_textures[m_currently_attached] = NULL;
	m_currently_attached = -1;

	glDeleteTextures(1, &texture_id); // Delete it from memory.
	return;
}

void MediaManager::equalizeHistogram()
{
	if (m_currently_attached >= 0)
	{
		cv::Mat& img = m_media_org[m_currently_attached];
		
		// Convert to grayscale (assuming the image is in color)
		if (img.channels() > 1)
			cv::cvtColor(img, img, cv::COLOR_RGB2GRAY);

		// Equalize histogram
		cv::equalizeHist(img, img);
		cv::cvtColor(img, img, cv::COLOR_GRAY2RGB); // Convert colours
	}

}
