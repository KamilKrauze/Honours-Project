#include "Core/MediaManager.h"
#include "Core/CAEHelper.h"

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <glad/glad.h>

/* Helper macros */

// Convert GLuint to ImTextureID
#define GLUINT_TO_IMTEXTUREID(VALUE) ((ImTextureID)(intptr_t)VALUE)

// Reference to instance
MediaManager* MediaManager::s_Instance = nullptr;

// Converts OpenCV Matrix to ImGUI legible format
static GLuint MatToImTextureID(cv::Mat& mat);

MediaManager::MediaManager()
{
	if (!s_Instance)
		this->s_Instance = this;

	selected = 0;
}

MediaManager::~MediaManager()
{
	this->s_Instance = nullptr;
	selected = NULL;
}

ImTextureID MediaManager::texture()
{
	return m_textures[0];
}

bool MediaManager::load_image(cv::String filepath, const cv::ImreadModes mode)
{
	cv::Mat img = cv::imread(filepath, mode);
	if (img.empty())
		return false;

	this->m_media.push_back(img);

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

	return true;
}

void MediaManager::attach(const size_t&& selected)
{
	this->selected = selected;
	ImTextureID texture_id = CAE::Helper::MatToImTextureID(this->m_media[this->selected]);
	this->m_textures.push_back(texture_id);

	return;
}

void MediaManager::dettach()
{
	auto texture_id = (GLuint)(intptr_t)m_textures[selected];
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind

	glDeleteTextures(1, &texture_id); // Delete it from memory.
	this->selected = 0;
	return;
}