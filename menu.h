#pragma once
#include <iostream>
#include "Imgui/imgui.h"
#include "settings.h"
#include "functions.h"
#include <string>

#define MENU_TAB_AIMBOT 1
#define MENU_TAB_VISUAL 2
#define MENU_TAB_RADAR 3
#define MENU_TAB_MISC 4
#define MENU_TAB_SETTINGS 5

#define WEAPON_TAB_DEFAULT 0
#define WEAPON_TAB_RIFLE 1
#define WEAPON_TAB_SHOTGUN 2
#define WEAPON_TAB_SMG 3
#define WEAPON_TAB_PISTOL 4
#define WEAPON_TAB_SNIPER 5

#define MENU_SETTINGS_SETTINGS 1

// faggot useless code
//#define UINCODE_ICON_GUN "\xEE\xA7\x9B"
//#define ICON_FA_GUN "\xef\x90\x8e"
//#define UINCODE_ICON_EYE "\uf06e"
//#define UINCODE_ICON_RADAR "\ue024"
//#define UINCODE_ICON_GEAR "\uf4fe"

namespace menu {
	int tab = MENU_TAB_AIMBOT;
	int weapon_tab = WEAPON_TAB_DEFAULT;
	int settings_page = MENU_SETTINGS_SETTINGS;
}

namespace image {
	IDirect3DTexture9* scene = nullptr;
	bool style = false;
}

bool LoadTextureFromFile(const char* filename, LPDIRECT3DDEVICE9 pDevice, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
	// Load texture from disk
	PDIRECT3DTEXTURE9 texture;

	HRESULT hr = D3DXCreateTextureFromFileA(pDevice, filename, &texture);
	if (hr != S_OK)
		return false;

	// Retrieve description of the texture surface so we can access its size
	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*out_texture = texture;
	*out_width = (int)my_image_desc.Width;
	*out_height = (int)my_image_desc.Height;
	return true;
}

void ApplyColors(ImVec4* colors)
{
	//colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.f);
	//colors[ImGuiCol_CheckMark] = ImVec4(0.3f, 0.0f, 0.3f, 1.0f);
	//colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.25f, 0.25f, 1.f);
	//colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
	//colors[ImGuiCol_FrameBg] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	//colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	//colors[ImGuiCol_FrameBgActive] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

	//colors[ImGuiCol_ChildBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
	//colors[ImGuiCol_PopupBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);

	//colors[ImGuiCol_SliderGrab] = ImVec4(0.5f, 0.0f, 0.5f, 1.0f);
	//colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5f, 0.0f, 0.5f, 1.0f);

	//colors[ImGuiCol_Header] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	//colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	//colors[ImGuiCol_HeaderActive] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

	//colors[ImGuiCol_ScrollbarBg] = ImVec4(0.03f, 0.03f, 0.03f, 1.0f);
	//colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3f, 0.0f, 0.3f, 1.0f);
	//colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.3f, 0.0f, 0.3f, 1.0f);
	//colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.3f, 0.0f, 0.3f, 1.0f);

	//colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	//colors[ImGuiCol_BorderShadow] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);


	colors[ImGuiCol_Text] = ImColor(255, 255, 255);
	colors[ImGuiCol_TextDisabled] = ImColor(255, 255, 255);
	colors[ImGuiCol_WindowBg] = ImColor(31, 31, 31);
	colors[ImGuiCol_ChildBg] = ImColor(31, 31, 31);
	colors[ImGuiCol_PopupBg] = ImVec4(0.93f, 0.93f, 0.93f, 0.98f);
	colors[ImGuiCol_Border] = ImColor(113, 88, 224);
	colors[ImGuiCol_BorderShadow] = ImColor(113, 88, 224);
	colors[ImGuiCol_FrameBg] = ImColor(31, 31, 31);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.94f, 0.94f, 0.94f, 0.55f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.71f, 0.71f, 0.71f, 0.55f);
	colors[ImGuiCol_TitleBg] = ImColor(113, 88, 224);
	colors[ImGuiCol_TitleBgCollapsed] = ImColor(113, 88, 224);
	colors[ImGuiCol_TitleBgActive] = ImColor(113, 88, 224);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.61f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.90f, 0.90f, 0.90f, 0.30f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.92f, 0.92f, 0.78f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImColor(113, 88, 224);
	colors[ImGuiCol_SliderGrab] = ImColor(113, 88, 224);
	colors[ImGuiCol_SliderGrabActive] = ImColor(113, 88, 224);
	colors[ImGuiCol_Button] = ImVec4(0.71f, 0.78f, 0.69f, 0.00f);
	colors[ImGuiCol_ButtonHovered] = ImColor(113, 88, 224);
	colors[ImGuiCol_ButtonActive] = ImColor(113, 88, 224);
	colors[ImGuiCol_Header] = ImVec4(0.71f, 0.71f, 0.71f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.71f, 0.71f, 0.71f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.71f, 0.71f, 0.71f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.45f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
}

void ApplyStyle(ImGuiStyle& style)
{
	//style.WindowRounding = 6.5f;
	//style.ChildRounding = 14;
	//style.ChildBorderSize = 1;
}

namespace Preview
{
	float head_bone_x = 0;
	float head_bone_y = 130;
	float root_y = 324;
}

namespace render
{

	void menu(LPDIRECT3DDEVICE9 D3dDevice)
	{
		static bool i_open = true;
		static int m_size_x = 670;
		static int m_size_y = 500;
		static int m_pos_x;
		static int m_pos_y;
		ImVec4* m_colors = ImGui::GetStyle().Colors;
		auto& m_style = ImGui::GetStyle();
		ImGuiIO& m_io = ImGui::GetIO(); (void)m_io;
		auto menu_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
		auto m_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration;
		auto tab_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
		auto child_flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
		auto Header_Flags = ImGuiTreeNodeFlags_NoTreePushOnOpen;
		auto color_flags = ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs;
		int MenuTab;

		ApplyColors(m_colors);
		ApplyStyle(m_style);

		ImGui::SetNextWindowSize(ImVec2(700, 520));

		//if (ImGui::Begin(TEXT("buttons main"), &i_open, m_flags)) {
		//	int SizeOfThisX = 100; // 63
		//	int c_size_y = m_size_y;
		//	int offset_y = 140;
		//	int test = 23;

		//	ImGui::SetWindowPos(ImVec2(m_pos_x - (SizeOfThisX + 25), m_pos_y + offset_y + (test / 2)));
		//	ImGui::SetWindowSize(ImVec2(SizeOfThisX, c_size_y - ((offset_y * 2) + test)));
		//	ImGui::BeginChild(TEXT("Child Buttons"), ImVec2(100, (c_size_y - ((offset_y * 2))) - 33.5f), false, m_flags);
		//	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.03f, 0.03f, 0.03f, 1.0f));
		//	ImGui::PushFont(font::icon);

		//	bool active = false;

		//	if (ImGui::TabButton("N", "Aimbot", false, ImVec2(60, 40))) {
		//		menu::tab = MENU_TAB_AIMBOT;
		//		active = true;
		//	}
		//	if (ImGui::IsItemHovered()) {
		//		ImGui::PushFont(font::default_segu); {
		//			ImGui::BeginTooltip();
		//			ImGui::Text(TEXT("Aimbot configurations"));
		//			ImGui::EndTooltip();
		//			ImGui::PopFont();
		//		}
		//	}

		//	if (ImGui::TabButton("I", "Visuals", false, ImVec2(190, 40))) {
		//		menu::tab = MENU_TAB_VISUAL;
		//		active = true;
		//	}
		//	if (ImGui::IsItemHovered()) {
		//		ImGui::PushFont(font::default_segu); {
		//			ImGui::BeginTooltip();
		//			ImGui::Text(TEXT("Visual configurations"));
		//			ImGui::EndTooltip();
		//			ImGui::PopFont();
		//		}
		//	}

		//	if (ImGui::TabButton("Q", "Radar", false, ImVec2(190, 40))) {
		//		menu::tab = MENU_TAB_RADAR;
		//		active = true;
		//	}
		//	if (ImGui::IsItemHovered()) {
		//		ImGui::PushFont(font::default_segu); {
		//			ImGui::BeginTooltip();
		//			ImGui::Text(TEXT("Radar configurations"));
		//			ImGui::EndTooltip();
		//			ImGui::PopFont();
		//		}
		//	}

		//	if (ImGui::TabButton("O", "Settings", false, ImVec2(190, 40))) {
		//		menu::tab = MENU_TAB_SETTING;
		//		active = true;
		//	}
		//	if (ImGui::IsItemHovered()) {
		//		ImGui::PushFont(font::default_segu); {
		//			ImGui::BeginTooltip();
		//			ImGui::Text(TEXT("General configurations"));
		//			ImGui::EndTooltip();
		//			ImGui::PopFont();
		//		}
		//	}

		//	ImGui::PopStyleColor();
		//	ImGui::PopFont();
		//	ImGui::EndChild();
		//	ImGui::End();
		//}

		//if (ImGui::Begin("preview", &i_open, menu_flags))
		//{
		//	int SizeOfThisX = 63;
		//	int offset_y = 75;
		//	int test = 19; // height

		//	int bottom = 10;

		//	ImGui::SetWindowPos(ImVec2(m_pos_x - (SizeOfThisX + -1200), m_pos_y + offset_y + (test / 2)));
		//	//ImGui::SetWindowSize(ImVec2(SizeOfThisX, c_size_y - ((offset_y * 2) + test)));
		//	//ImGui::SetNextWindowSize(ImVec2(700, 200), ImGuiCond_Always);

		//	if (!image::scene) {
		//		remove(TEXT("C:\\heafshderahz.jpg"));
		//		int my_image_width, my_image_height = 0;
		//		system(("curl --silent https://cdn.discordapp.com/attachments/1062803570510004325/1100609685607948338/29b1b414c2c4f704a19d2f009b9b74f8.png --output C:\\heafshderahz.jpg >nul 2>&1"));
		//		LoadTextureFromFile(TEXT("C:\\heafshderahz.jpg"), D3dDevice, &image::scene, &my_image_width, &my_image_height);
		//		remove(TEXT("C:\\heafshderahz.jpg"));
		//	}

		//	ImGui::BeginChild("ESP Preview", ImVec2(180, (m_size_y - ((offset_y + bottom))) - 100), false, child_flags);
		//	{

		//		//ImGui::SetCursorPos(ImVec2(500, 200));
		//		//ImGui::Text("ESP Preview");

		//		ImGui::SetCursorPos(ImVec2(-100, -25)); // (m_size_y - ((offset_y * 3)))
		//		ImGui::Image(image::scene, ImVec2(380, 360), ImVec2(0, 0), ImVec2(1, 1));

		//		//ImGui::SetCursorPos(ImVec2(70, 200));
		//		//ImGui::Text("0");

		//		//int lenght = 158;
		//		//int x1 = m_pos_x + m_size_x + 27; // left side
		//		//int y1 = m_pos_y;
		//		//int x2 = m_pos_x + m_size_x + 25 + lenght; // right side
		//		//int y2 = m_pos_y + m_size_y;
		//		//int x3 = (x1 + lenght) - (lenght / 2);
		//		//auto Color_Vec4 = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		//		//auto Color_GCU32 = ImGui::GetColorU32({ 1.0f, 0.0f, 0.0f, 1.0f });
		//		//auto Color_U32 = IM_COL32(255, 0, 0, 255);

		//		//ImGui::GetOverlayDrawList()->AddText(ImVec2(1394, 446), ImColor(255, 255, 255, 255), "0");
		//		//ImGui::GetOverlayDrawList()->AddImage(image::scene, ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));
		//		//ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::GetColorU32({ 0.1f, 0.1f, 0.1f, 1.f }), 6.5f, 0);

		//		//y1 =- 20;
		//		//auto __top_x = x3 + Preview::head_bone_x;
		//		//auto __top_y = (y1 + Preview::head_bone_y + 40); // width
		//		//auto __bottom_x = x3 + Preview::head_bone_x - 10;
		//		//auto __bottom_y = (y1 + Preview::right_footfinger_bone_y) - 20; // height

		//		if (menu::tab == MENU_TAB_VISUAL)
		//		{
		//			//float BoxHeight = __bottom_y - __top_y;
		//			//float BoxWidth = BoxHeight / 1.6f;
		//			//box_esp(4, 4, __bottom_x - (BoxWidth / 2), __top_y,
		//			//	BoxWidth, BoxHeight, ImColor(255, 255, 255, 255), 2.0f, settings::fill, settings::outline);
		//		}

		//		ImGui::EndChild();
		//	}

		//	ImGui::End();
//
		if (ImGui::Begin("Mak0001 Fortnite by LeProxy", &i_open, menu_flags))
		{
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Text("      ");
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::PushFont(font::verdana);

			if (ImGui::Button("  Aimbot", ImVec2(160, 50))) {
				menu::tab = MENU_TAB_AIMBOT;
			}
			ImGui::Spacing();
			if (ImGui::Button("  Player", ImVec2(160, 50))) {
				menu::tab = MENU_TAB_VISUAL;
			}
			ImGui::Spacing();
			if (ImGui::Button("  Radar", ImVec2(160, 50))) {
				menu::tab = MENU_TAB_RADAR;
			}
			ImGui::Spacing();
			if (ImGui::Button("  Misc", ImVec2(160, 50))) {
				menu::tab = MENU_TAB_MISC;
			}
			ImGui::Spacing();
			if (ImGui::Button("  Settings", ImVec2(160, 50))) {
				menu::tab = MENU_TAB_SETTINGS;
			}

			ImGui::PopFont();


			ImGui::PushFont(font::default_segu);
			ImGui::SetCursorPos(ImVec2(27, 480));
			ImGui::Text("Backbone external");
			ImGui::PopFont();

			ImGui::PushFont(font::Arial);

			if (menu::tab == MENU_TAB_AIMBOT) {
				ImGui::SetCursorPos(ImVec2(180, 30));
				ImGui::BeginChild("Aimbot", ImVec2(500, 470));

				ImGui::Checkbox("Aimbot", &settings::bAimbot);
				ImGui::Spacing();
				ImGui::Checkbox("Triggerbot", &settings::autofire);
				ImGui::Spacing();
				ImGui::Checkbox("MemoryAim", &settings::MemoryAim);
				ImGui::Spacing();
				ImGui::Checkbox("MemoryAim V2", &settings::MemoryAimV2);
				ImGui::Spacing();
				ImGui::Checkbox("Prediction", &settings::SniperPrediction);
				ImGui::Spacing();
				ImGui::Checkbox("lines", &settings::TargetLines);
				ImGui::Spacing();
				ImGui::Checkbox("Player Alive", &settings::UnKnockedOnly);
				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					ImGui::TextColored(m_colors[ImGuiCol_BorderShadow], "enemy player needs to be alive");
					ImGui::EndTooltip();
				}
				ImGui::Spacing();
				ImGui::Checkbox("Player Visible", &settings::VisbleOnly);
				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					ImGui::TextColored(m_colors[ImGuiCol_BorderShadow], "enemy player needs to be visble");
					ImGui::EndTooltip();
				}
				ImGui::Spacing();
				ImGui::SliderInt("Mouse Speed", &settings::smooth, 1, 20);
				ImGui::Spacing();
				ImGui::SliderInt("Trigger Delay (MS)", &settings::delay, 1, 10);
				ImGui::Spacing();
				ImGui::SliderFloat("FieldOfView", &settings::AimFOV, 0.f, 500.f, "%.2f");

				ImGui::EndChild();
			}
			if (menu::tab == MENU_TAB_VISUAL) {

				ImGui::SetCursorPos(ImVec2(180, 30));
				ImGui::BeginChild("Visuals", ImVec2(500, 470));

				ImGui::Combo(" ", &settings::box_type, settings::box, sizeof(settings::box) / sizeof(*settings::box));

				ImGui::Checkbox("fill", &settings::fill);
				ImGui::Checkbox("outline", &settings::outline);
				ImGui::Checkbox("distance", &settings::distance);
				ImGui::Checkbox("player lines", &settings::Snaplines);
				ImGui::Checkbox("skeleton", &settings::skell);
				ImGui::Checkbox("2D head", &settings::b2Dhead);
				ImGui::Checkbox("3D head", &settings::b3Dhead);
				ImGui::Checkbox("debug", &settings::debug);
				ImGui::SliderFloat("Distance", &settings::max_distance, 10.f, 200.f);

				ImGui::EndChild();

			}
			if (menu::tab == MENU_TAB_RADAR) {
				ImGui::SetCursorPos(ImVec2(180, 30));
				ImGui::BeginChild("Radar", ImVec2(500, 470));

				ImGui::EndChild();
			}
			if (menu::tab == MENU_TAB_MISC) {
				//ImGui::Particles(ImGui::GetOverlayDrawList(), ImVec2(m_io.DisplaySize.x, m_io.DisplaySize.y), rainbow());
				ImGui::Spacing();
				ImGui::Spacing();

				std::string FPS = std::to_string(m_io.Framerate);
				std::string time = std::to_string(m_io.DeltaTime);

				ImGui::SetCursorPos(ImVec2(180, 30));
				if (ImGui::BeginChild("Misc", ImVec2(480, 420)))
				{
					ImGui::Spacing();

					ImGui::Text("[Cheat information]");

					ImGui::SameLine();

					ImGui::Text("           [Detection]");

					ImGui::TextColored(ImVec4(0.5, 1.0, 0.5, 1.0), "Cheat version"); ImGui::SameLine(); ImGui::Text("->"); ImGui::SameLine();  ImGui::TextColored(ImVec4(0.8f, 0.6f, 0.9f, 1.0f), "1.3");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(1.0, 0.5, 0.0, 1.0), "        EasyAntiCheat"); ImGui::SameLine(); ImGui::Text("->"); ImGui::SameLine();  ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), "Detected");
					ImGui::TextColored(ImVec4(0.5, 1.0, 0.5, 1.0), "Game version"); ImGui::SameLine(); ImGui::Text("->"); ImGui::SameLine();  ImGui::TextColored(ImVec4(0.8f, 0.6f, 0.9f, 1.0f), "24.20");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(1.0, 0.5, 0.0, 1.0), "   BattlEye"); ImGui::SameLine(); ImGui::Text("->"); ImGui::SameLine();  ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Undetected");

					ImGui::Text("				");

					ImGui::Text("[FPS information]");

					ImGui::TextColored(ImVec4(0.678, 0.847, 0.902, 1), "Frames PER second"); ImGui::SameLine(); ImGui::Text("->"); ImGui::SameLine();  ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), FPS.c_str());
					ImGui::TextColored(ImVec4(0.678, 0.847, 0.902, 1), "Time since last frame"); ImGui::SameLine(); ImGui::Text("->"); ImGui::SameLine();  ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), time.c_str());

					ImGui::Text("				");

					ImGui::Text("[Change logs]");

					ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1.0f), "[1.3] New menu");
					ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1.0f), "[1.2] Radar (w/ team, loot, TP)");
					ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1.0f), "[1.2] Debug information");
					ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1.0f), "[1.1] TriggerBot");
					ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1.0f), "[1.1] Aim Prediction");
					ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1.0f), "[1.1] Searchables and Pickup ESP");
					ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1.0f), "[1.0] Weapon config");

					ImGui::Text("				");

					ImGui::EndChild();
				}
			}
			if (menu::tab == MENU_TAB_SETTINGS) {
				ImGui::SetCursorPos(ImVec2(180, 30));
				ImGui::BeginChild("Settings", ImVec2(500, 470));

				auto flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoTooltip |
					ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoOptions;

				ImGui::Text("Box Visible"); ImGui::SameLine(); ImGui::ColorEdit4("##esp1", (float*)&settings::color::visbox, flags);
				ImGui::Text("Box Invisible"); ImGui::SameLine(); ImGui::ColorEdit4("##esp2", (float*)&settings::color::unvisbox, flags);

				ImGui::Text("			");

				bool StreamProof = false;
				ImGui::Checkbox("StreamProof", &StreamProof);

				ImGui::EndChild();
			}

			ImGui::PopFont();
			ImGui::End();
		}
	}
}