#include "CAEHelper.h"


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
};
