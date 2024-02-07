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

	this->m_selected = 0;
	this->m_currently_attached = -1;
}

MediaManager::~MediaManager()
{
	this->s_Instance = nullptr;
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

	this->m_media.push_back(img);

	// Resize texture pool
	size_t current_size = m_textures.size();
	m_textures.resize(current_size + 1);

	return true;
}

bool MediaManager::load_images(StringConstItr start, StringConstItr end)
{
	if (start._Ptr == nullptr || end._Ptr == nullptr)
		return false;

	for (auto it = start; it != end; it++)
	{
		cv::Mat img = cv::imread(*it._Ptr, cv::IMREAD_ANYCOLOR);
		m_media.push_back(img);
	}

	// Resize texture pool
	size_t size = end - start;
	size_t current_size = m_textures.size();
	m_textures.resize(size + current_size);
	return true;
}

void MediaManager::attach(const size_t&& index)
{
	if (index > m_textures.size())
	{
		assert((index > m_texture.size()) && "Index is too large!");
		return;
	}

	if (m_currently_attached >= 0)
		dettach();


	ImTextureID texture_id = CAE::Helper::MatToImTextureID(this->m_media[index]);
	this->m_textures[index] = texture_id;
	this->m_selected = index;
	this->m_currently_attached = index;


	return;
}

void MediaManager::dettach()
{
	if (m_currently_attached < 0)
		return;

	auto texture_id = (GLuint)(intptr_t)(this->m_textures[m_currently_attached]);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind
	m_textures[m_currently_attached] = NULL;
	m_currently_attached = -1;

	glDeleteTextures(1, &texture_id); // Delete it from memory.
	//m_textures[m_currently_attached] = NULL;
	return;
}

void MediaManager::equalizeHistogram()
{
	if (m_currently_attached >= 0)
	{
		cv::Mat& img = m_media[m_currently_attached];
		
		// Convert to grayscale (assuming the image is in color)
		if (img.channels() > 1)
			cv::cvtColor(img, img, cv::COLOR_RGB2GRAY);

		// Equalize histogram
		cv::equalizeHist(img, img);
		cv::cvtColor(img, img, cv::COLOR_GRAY2RGB); // Convert colours
	}

}
