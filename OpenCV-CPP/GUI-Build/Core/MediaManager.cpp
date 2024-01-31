#include "Core/MediaManager.h"

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <glad/glad.h>

MediaManager* MediaManager::s_Instance = nullptr;


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

bool MediaManager::load_image(cv::String filepath, const cv::ImreadModes mode)
{
	cv::Mat img = cv::imread(filepath, mode);
	if (img.empty())
		return false;

	this->m_media.push_back(img);

	return true;
}


bool MediaManager::load_images(StringConstItr start, StringConstItr end, const cv::ImreadModes mode)
{
	if (start._Ptr == nullptr || end._Ptr == nullptr)
		return false;

	for (auto it = start; it != end; it++)
	{
		cv::Mat img = cv::imread(*it._Ptr, mode);
		m_media.push_back(img);
	}

	return true;
}

const ImTextureID MatToImTextureID(const cv::Mat& mat) {
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, mat.ptr());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	return (ImTextureID)(intptr_t)textureID;
}
