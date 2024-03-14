#include "GUI/EditorGUI.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <string_view>

#include <imgui.h>
#include <opencv2/highgui.hpp>

#include "Core/CAEHelper.h"
#include "Core/MediaManager.h"

using Keys = std::vector<std::string_view>;

static size_t index = 0;
static size_t currentItem = 0;
static size_t lastItem = 0;

static std::string_view currentKey = "";
static std::string dstKey = "";

// Centering buttons - https://github.com/ocornut/imgui/discussions/3862 - 28/02/2024
inline void AlignForWidth(float width, float alignment = 0.5f)
{
	ImGuiStyle& style = ImGui::GetStyle();
	float avail = ImGui::GetContentRegionAvail().x;
	float off = (avail - width) * alignment;
	if (off > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
}

void showSettingsPanel()
{
	ImGui::Begin("Modifiers");

	if (ImGui::Button("Equalize Histogram"))
	{
		ImGui::OpenPopup("Select Dataset");
	}

	if (ImGui::BeginPopupModal("Select Dataset", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
	{
		ImGui::SetItemDefaultFocus();
		ImGui::Text("Select the data source and name the new dataset (careful you may override something if the same name is given!)");

		static size_t current_item = 0;
		Keys keys = MediaManager::Get().getKeys();
		if (ImGui::BeginCombo("Source", keys[current_item].data(), ImGuiComboFlags_PopupAlignLeft))
		{
			for (size_t i = 0; i < keys.size(); i++)
			{
				const bool isSelected = (current_item == i);
				if (ImGui::Selectable(keys[i].data(), &isSelected))
					current_item = i;
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();
		ImGui::Text(" --> ");
		ImGui::SameLine();
		ImGui::InputText(": Destination", dstKey.data(), 20);

		if (ImGui::Button("Ok"))
		{
			lastItem = currentItem;
			currentItem = current_item;
			currentKey = keys[current_item];

			MediaManager::Get().equalizeHistogram(currentKey.data(), dstKey.c_str());
			MediaManager::Get().bind(dstKey, MediaManager::Get().getCurrentIndex());

			ImGui::CloseCurrentPopup();

			dstKey = "";
		}

		ImGui::EndPopup();

	}

	ImGui::End();
}

void showFrameSelectionPanel()

{
	ImGui::Begin("Frame Selector", nullptr, ImGuiWindowFlags_NoCollapse);

	// Centering buttons - https://github.com/ocornut/imgui/discussions/3862 - 28/02/2024
	ImGuiStyle& style = ImGui::GetStyle();
	float width = 25.0f;
	width += ImGui::CalcTextSize("<--").x;
	width += style.ItemSpacing.x;
	width += style.ItemSpacing.x;
	width += ImGui::CalcTextSize("-->").x;
	AlignForWidth(width);

	ImGui::PushButtonRepeat(true);

	index = MediaManager::Get().getCurrentIndex();
	if (ImGui::ArrowButton("LeftArrow", ImGuiDir_Left))
		MediaManager::Get().bind(currentKey, index - 1);

	ImGui::SameLine();

	ImGui::PushItemWidth(25);
	if (ImGui::InputScalar("frame #", ImGuiDataType_U64, &index, NULL, NULL, NULL, ImGuiInputTextFlags_AlwaysOverwrite) && index < MediaManager::Get().getTotal("src1"))
	{
		MediaManager::Get().bind(currentKey, index + 0);
	}
	ImGui::PopItemWidth();

	ImGui::SameLine();

	if (ImGui::ArrowButton("RightArrow", ImGuiDir_Right))
		MediaManager::Get().bind(currentKey, index + 1);
	ImGui::PushButtonRepeat(false);

	ImGui::End();
}

void showDetailsPanel()
{
	ImGui::Begin("Details");

	std::string text = "Frame Number: " + std::to_string(MediaManager::Get().getCurrentIndex());
	ImGui::Text(text.c_str());
	ImGui::NewLine();

	Keys keys = MediaManager::Get().getKeys();
	if (ImGui::BeginCombo("Frame Set Select", keys[currentItem].data(), ImGuiComboFlags_PopupAlignLeft))
	{
		for (size_t i = 0; i < keys.size(); i++)
		{
			const bool isSelected = (currentItem == i);
			if (ImGui::Selectable(keys[i].data(), &isSelected))
				currentItem = i;
		}
		ImGui::EndCombo();
	}
	
	currentKey = keys[currentItem];
	if (lastItem != currentItem)
	{
		lastItem = currentItem;
		MediaManager::Get().bind(currentKey, index + 0);
	}

	ImGui::End();
}

void EditorGUI::RunEditorGUI()
{
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", nullptr, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{

			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::MenuItem("Padding", NULL, &opt_padding);
				ImGui::Separator();

				if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
				if (ImGui::MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
				if (ImGui::MenuItem("Flag: NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
				ImGui::Separator();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		showSettingsPanel();

		{
			ImGui::Begin("Viewport");

			{
				ImGui::BeginChild("Frame Data");

				std::string text = "Frame Number: " + std::to_string(MediaManager::Get().getCurrentIndex());
				ImGui::Text(text.c_str());

				ImGui::EndChild();
			}

			ImVec2 size = ImGui::GetWindowSize();
			CAE::Helper::DrawBackgroundImage(MediaManager::Get().getTextureID(), size, { 512,512 });
			ImGui::End();

			showFrameSelectionPanel();
			showDetailsPanel();

		}

		ImGui::End();
	}