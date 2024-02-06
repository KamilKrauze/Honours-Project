#ifndef CAE_HELPER_H
#define CAE_HELPER_H

#include <string_view>

#include <opencv2/core.hpp>
#include <imgui.h>
#include <glad/glad.h>

namespace CAE
{
	namespace Helper
	{		
		// Draw the image centered to the frame.
		void DrawBackgroundImage(ImTextureID texture, ImVec2 windowSize, ImVec2 imgSize);

		const ImTextureID MatToImTextureID(const cv::Mat& mat);

	};

};

#endif