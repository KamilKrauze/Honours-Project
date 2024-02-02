#include "CAEHelper.h"
#include <iostream>

namespace CAE::Helper
{
    void DrawBackgroundImage(ImTextureID texture, ImVec2 windowSize, ImVec2 imgSize)
    {
        imgSize = { 512,512 };
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 center = ImVec2(windowPos.x + windowSize.x * 0.5f, windowPos.y + windowSize.y * 0.5f);

        // Get the original image size
        float aspectRatio = imgSize.x / imgSize.y;

        // Calculate the size to maintain the aspect ratio
        float targetWidth = windowSize.x;
        float targetHeight = windowSize.x / aspectRatio;

        if (targetHeight > windowSize.y) {
            targetHeight = windowSize.y;
            targetWidth = windowSize.y * aspectRatio;
        }

        ImVec2 imagePos = ImVec2(center.x - targetWidth * 0.5f, center.y - targetHeight * 0.5f);

        ImGui::GetWindowDrawList()->AddImage(texture, imagePos, ImVec2(imagePos.x + targetWidth, imagePos.y + targetHeight));
    }

    GLuint MatToImTextureID(cv::Mat& mat)
    {

        //std::cout << "Empty matrix is: " << mat << std::endl;

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, mat.ptr());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        return textureID;
    }
};
