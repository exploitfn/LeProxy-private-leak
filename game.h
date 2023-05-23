#pragma once
#include <string>
#include <xstring>
#include <d3d9.h>
#include "stdafx.h"
#include "functions.h"
#include "settings.h"
#include "menu.h"
#include "fn.h"
#include "settings.h"
#include "controller.h"
#include "console.h"
#pragma comment (lib, "d3d9.lib")

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace game {
	RECT GameRect = { NULL };
	D3DPRESENT_PARAMETERS d3dpp;
	static HWND Window = NULL;
	IDirect3D9Ex* p_Object = NULL;
	static LPDIRECT3DDEVICE9 D3dDevice = NULL;
	static LPDIRECT3DVERTEXBUFFER9 TriBuf = NULL;
	HWND hwnd = NULL;
	const MARGINS Margin = { -1 };
	float Width, Height = 0.f;
	MSG Message = { NULL };
	std::vector<fn::actor> playerlist;

	D3DXMATRIX Matrix(fn::fvector rot, fn::fvector origin = fn::fvector(0, 0, 0))
	{
		double radPitch = (rot.x * double(M_PI) / 180.f);
		double radYaw = (rot.y * double(M_PI) / 180.f);
		double radRoll = (rot.z * double(M_PI) / 180.f);

		double SP = sinf(radPitch);
		double CP = cosf(radPitch);
		double SY = sinf(radYaw);
		double CY = cosf(radYaw);
		double SR = sinf(radRoll);
		double CR = cosf(radRoll);

		D3DMATRIX matrix;
		matrix.m[0][0] = CP * CY;
		matrix.m[0][1] = CP * SY;
		matrix.m[0][2] = SP;
		matrix.m[0][3] = 0.f;

		matrix.m[1][0] = SR * SP * CY - CR * SY;
		matrix.m[1][1] = SR * SP * SY + CR * CY;
		matrix.m[1][2] = -SR * CP;
		matrix.m[1][3] = 0.f;

		matrix.m[2][0] = -(CR * SP * CY + SR * SY);
		matrix.m[2][1] = CY * SR - CR * SP * SY;
		matrix.m[2][2] = CR * CP;
		matrix.m[2][3] = 0.f;

		matrix.m[3][0] = origin.x;
		matrix.m[3][1] = origin.y;
		matrix.m[3][2] = origin.z;
		matrix.m[3][3] = 1.f;

		return matrix;
	}

	struct Camera
	{
		float FieldOfView;
		fn::fvector Rotation;
		fn::fvector Location;

	}; Camera vCamera;

	Camera update_camera(uintptr_t rootcomp)
	{
		Camera LocalCamera;
		uint64_t ViewMatrix = read<uint64_t>(fn::globals::Localplayer + 0xd0);
		uint64_t ViewMatrixCorrect = read<uint64_t>(ViewMatrix + 0x8);

		//LocalCamera.FieldOfView = 80.f / (read<double>(ViewMatrixCorrect + 0x7F0) / 1.19f); // 0x600
		LocalCamera.FieldOfView = read<float>(fn::globals::PlayerController + 0x38C) * 90.f;
		LocalCamera.Rotation.x = read<double>(ViewMatrixCorrect + 0x9C0);
		LocalCamera.Rotation.y = read<double>(rootcomp + 0x148);
		LocalCamera.Location = read<fn::fvector>(read<uint64_t>(fn::globals::Uworld + 0x110));

		return LocalCamera;
	}

	fn::fvector w2s(fn::fvector WorldLocation)
	{

		vCamera.Rotation.x = (asin(vCamera.Rotation.x)) * (180.0 / M_PI);

		if (vCamera.Rotation.y < 0)
			vCamera.Rotation.y = 360 + vCamera.Rotation.y;

		fn::fvector Screenlocation = fn::fvector(0, 0, 0);
		D3DMATRIX tempMatrix = Matrix(vCamera.Rotation);

		fn::fvector vAxisX, vAxisY, vAxisZ;
		vAxisX = fn::fvector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
		vAxisY = fn::fvector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
		vAxisZ = fn::fvector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

		fn::fvector vDelta = WorldLocation - vCamera.Location;
		fn::fvector vTransformed = fn::fvector(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

		if (vTransformed.z < 1.f)
			vTransformed.z = 1.f;

		float ScreenCenterX = Width / 2.0f;
		float ScreenCenterY = Height / 2.0f;

		return fn::fvector(ScreenCenterX + vTransformed.x * ((ScreenCenterX / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, ScreenCenterY - vTransformed.y * ((ScreenCenterX / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, 0);
	}


	FTransform GetBoneIndex(DWORD_PTR mesh, int index)
	{
		DWORD_PTR bonearray;
		bonearray = read<DWORD_PTR>(mesh + 0x5f0);

		if (bonearray == NULL)
		{
			bonearray = read<DWORD_PTR>(mesh + 0x5f0 + 0x10);
		}
		return  read<FTransform>(bonearray + (index * 0x60));
	}

	fn::fvector BoneMatrix(DWORD_PTR mesh, int id)
	{
		FTransform bone = GetBoneIndex(mesh, id);
		FTransform ComponentToWorld = read<FTransform>(mesh + 0X240);

		D3DMATRIX Matrix;
		Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

		return fn::fvector(Matrix._41, Matrix._42, Matrix._43);
	}

	//fn::fvector BoneMatrix(DWORD_PTR mesh, int index)
	//{
	//	uintptr_t bone_pointer = read<uintptr_t>(mesh + 0x5f0);

	//	if (bone_pointer == NULL)
	//		bone_pointer = read<uintptr_t>(mesh + 0x5f0 + 0x10);

	//	FTransform ComponentToWorld = read<FTransform>(mesh + 0x240);
	//	FTransform bone = read<FTransform>(bone_pointer + (index * 0x60));
	//	D3DMATRIX Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

	//	return fn::fvector(Matrix._41, Matrix._42, Matrix._43);
	//}

	static std::string find_object_class(int key)
	{
		uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
		uint16_t NameOffset = (uint16_t)key;

		uint64_t NamePoolChunk = read<uint64_t>(fn::pointers::base_address + 0xEEFD140 + (8 * ChunkOffset) + 16) + (unsigned int)(4 * NameOffset); //((ChunkOffset + 2) * 8) ERROR_NAME_SIZE_EXCEEDED
		uint16_t nameEntry = read<uint16_t>(NamePoolChunk);

		int nameLength = nameEntry >> 6;
		char buff[1024];
		if ((uint32_t)nameLength)
		{
			for (int x = 0; x < nameLength; ++x)
			{
				buff[x] = read<char>(NamePoolChunk + 4 + x);
			}
			char* v2 = buff; // rbx
			unsigned int v4 = nameLength;
			unsigned int v5; // eax
			int v6; // eax
			int v7; // ecx
			unsigned int v8; // kr00_4
			__int64 v9; // ecx

			v5 = 0;
			v6 = read<unsigned int>(fn::pointers::base_address + 0xEED8124) >> 5;
			if (v4)
			{
				do
				{
					v7 = *v2++;
					v6 += v5 + 96;
					++v5;
					*(v2 - 1) = (16 * v7) | (v6 ^ (v7 >> 4)) & 0xF;
				} while (v5 < v4);
			}
			buff[nameLength] = '\0';
			return std::string(buff);
		}
		else
		{
			return "";
		}
	}

	static std::string find_object_name(int key)
	{
		uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
		uint16_t NameOffset = (uint16_t)key;

		uint64_t NamePoolChunk = read<uint64_t>(fn::pointers::base_address + 0xEEFD140 + (8 * ChunkOffset) + 16) + (unsigned int)(4 * NameOffset); //((ChunkOffset + 2) * 8) ERROR_NAME_SIZE_EXCEEDED
		if (read<uint16_t>(NamePoolChunk) < 64)
		{
			auto a1 = read<DWORD>(NamePoolChunk + 4);
			return find_object_class(a1);
		}
		else
		{
			return find_object_class(key);
		}
	}

	bool WasRecentlyRendered(DWORD_PTR mesh)
	{
		float fLastSubmitTime = read<float>(mesh + 0x338);
		float fLastRenderTimeOnScreen = read<float>(mesh + 0x340);
		const float fVisionTick = 0.06f;
		bool bVisible = fLastRenderTimeOnScreen + fVisionTick >= fLastSubmitTime;
		return bVisible;
	}

	void skeleton_esp(uintptr_t mesh)
	{
		fn::fvector head, neck, chest, leftshoulder, leftelbow, leftwrist, lefthand, rightshoulder, rightelbow, rightwrist, righthand, penis, leftthigh, lefthip, leftcalf, leftfoot, righthip, rightthigh, rightcalf, rightfoot;
		ImU32 color = ImColor(255, 0, 0, 255);

		head = w2s(BoneMatrix(mesh, 68));
		neck = w2s(BoneMatrix(mesh, 67));
		chest = w2s(BoneMatrix(mesh, 7));
		leftshoulder = w2s(BoneMatrix(mesh, 37));
		leftelbow = w2s(BoneMatrix(mesh, 65));
		leftwrist = w2s(BoneMatrix(mesh, 63));
		lefthand = w2s(BoneMatrix(mesh, 58));
		rightshoulder = w2s(BoneMatrix(mesh, 8));
		rightelbow = w2s(BoneMatrix(mesh, 36));
		rightwrist = w2s(BoneMatrix(mesh, 34));
		righthand = w2s(BoneMatrix(mesh, 23));
		penis = w2s(BoneMatrix(mesh, 2));
		lefthip = w2s(BoneMatrix(mesh, 78));
		leftthigh = w2s(BoneMatrix(mesh, 81));
		leftcalf = w2s(BoneMatrix(mesh, 72));
		leftfoot = w2s(BoneMatrix(mesh, 73));
		righthip = w2s(BoneMatrix(mesh, 71));
		rightthigh = w2s(BoneMatrix(mesh, 74));
		rightcalf = w2s(BoneMatrix(mesh, 86));
		rightfoot = w2s(BoneMatrix(mesh, 76));

		ImGui::GetOverlayDrawList()->AddLine(ImVec2(head.x, head.y), ImVec2(head.x, neck.y), color, 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(neck.x, neck.y), ImVec2(chest.x, chest.y), color, 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(chest.x, chest.y), ImVec2(leftshoulder.x, leftshoulder.y), color, 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(leftshoulder.x, leftshoulder.y), ImVec2(leftelbow.x, leftelbow.y), color, 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(leftelbow.x, leftelbow.y), ImVec2(leftwrist.x, leftwrist.y), color, 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(leftwrist.x, leftwrist.y), ImVec2(lefthand.x, lefthand.y), color, 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(chest.x, chest.y), ImVec2(rightshoulder.x, rightshoulder.y), color, 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(rightshoulder.x, rightshoulder.y), ImVec2(rightelbow.x, rightelbow.y), color, 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(rightelbow.x, rightelbow.y), ImVec2(leftwrist.x, leftwrist.y), color, 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(rightwrist.x, rightwrist.y), ImVec2(righthand.x, righthand.y), color, 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(neck.x, neck.y), ImVec2(penis.x, penis.y), color, 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(penis.x, penis.y), ImVec2(leftthigh.x, leftthigh.y), color, 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(righthip.x, righthip.y), ImVec2(lefthip.x, lefthip.y), color, 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(lefthip.x, lefthip.y), ImVec2(leftcalf.x, leftcalf.y), color, 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(leftcalf.x, leftcalf.y), ImVec2(leftfoot.x, leftfoot.y), color, 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(penis.x, penis.y), ImVec2(righthip.x, righthip.y), color, 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(righthip.x, righthip.y), ImVec2(rightthigh.x, rightthigh.y), color, 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(rightthigh.x, rightthigh.y), ImVec2(rightcalf.x, rightcalf.y), color, 2.0f);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(rightfoot.x, rightfoot.y), ImVec2(rightfoot.x, rightfoot.y), color, 2.0f);
	}

	namespace PlayerWeapon
	{

		bool IsHoldingAssaultRifle() {
			if (fn::globals::WeaponName == "Combat Assault Rifle" ||
				fn::globals::WeaponName == "Assault Rifle  " ||
				fn::globals::WeaponName == "Light Machine Gun" ||
				fn::globals::WeaponName == "Scoped Assault Rifle" ||
				fn::globals::WeaponName == "Makeshift Rifle" ||
				fn::globals::WeaponName == "Sideways Rifle" ||
				fn::globals::WeaponName == "Heavy Assault Rifle" ||
				fn::globals::WeaponName == "Infantry Rifle" ||
				fn::globals::WeaponName == "Suppressed Assault Rifle" ||
				fn::globals::WeaponName == "Ranger Assault Rifle" ||
				fn::globals::WeaponName == "Burst Assault Rifle" ||
				fn::globals::WeaponName == "Primal Rifle" ||
				fn::globals::WeaponName == "Striker Burst Rifle" ||
				fn::globals::WeaponName == "Tactical Assault Rifle" ||
				fn::globals::WeaponName == "Sideways Minigun" ||
				fn::globals::WeaponName == "Burst Pulse Rifle" ||
				fn::globals::WeaponName == "Jules' Drum Gun" ||
				fn::globals::WeaponName == "Brutus' Minigun" ||
				fn::globals::WeaponName == "Spire Guardian's Primal Assault Rifle" ||
				fn::globals::WeaponName == "Drum Gun" ||
				fn::globals::WeaponName == "Shadow Midas' Drum Gun" ||
				fn::globals::WeaponName == "MK-Seven Assault Rifle" ||
				fn::globals::WeaponName == "Slone's Burst Assault Rifle" ||
				fn::globals::WeaponName == "Thermal Scoped Assault Rifle" ||
				fn::globals::WeaponName == "Skye's Assault Rifle" ||
				fn::globals::WeaponName == "Minigun" ||
				fn::globals::WeaponName == "Meowscle's Peow Peow Rifle" ||
				fn::globals::WeaponName == "Slone's Pulse Rifle" ||
				fn::globals::WeaponName == "Combat Assault Rifle" ||
				fn::globals::WeaponName == "Ocean's Burst Assault Rifle")
				return true;

			return false;
		}

		bool IsHoldingShotgun() {
			if (fn::globals::WeaponName == "Lever Action Shotgun" ||
				fn::globals::WeaponName == "Combat Shotgun" ||
				fn::globals::WeaponName == "Striker Pump Shotgun" ||
				fn::globals::WeaponName == "Tactical Shotgun" ||
				fn::globals::WeaponName == "Ranger Shotgun" ||
				fn::globals::WeaponName == "Primal Shotgun" ||
				fn::globals::WeaponName == "Pump Shotgun" ||
				fn::globals::WeaponName == "Auto Shotgun" ||
				fn::globals::WeaponName == "Charge Shotgun" ||
				fn::globals::WeaponName == "Drum Shotgun" ||
				fn::globals::WeaponName == "Dragon's Breath Shotgun" ||
				fn::globals::WeaponName == "Spire Assassin's Primal Shotgun" ||
				fn::globals::WeaponName == "Kit's Charge Shotgun")
				return true;

			return false;
		}

		bool IsHoldingSMG() {
			if (fn::globals::WeaponName == "Combat SMG" ||
				fn::globals::WeaponName == "Burst SMG" ||
				fn::globals::WeaponName == "Primal SMG" ||
				fn::globals::WeaponName == "Tactical Submachine Gun" ||
				fn::globals::WeaponName == "Rapid Fire SMG" ||
				fn::globals::WeaponName == "Suppressed Submachine Gun" ||
				fn::globals::WeaponName == "Makeshift Submachine Gun" ||
				fn::globals::WeaponName == "Compact SMG" ||
				fn::globals::WeaponName == "Kymera Ray Gun" ||
				fn::globals::WeaponName == "Zyg and Choppy's Ray Gun")
				return true;

			return false;
		}

		bool IsHoldingPistol() {
			if (fn::globals::WeaponName == "Hand Cannon" ||
				fn::globals::WeaponName == "Sidearm Pistol" ||
				fn::globals::WeaponName == "Revolver" ||
				fn::globals::WeaponName == "Scoped Revolver" ||
				fn::globals::WeaponName == "Dual Pistols" ||
				fn::globals::WeaponName == "Suppressed Pistol" ||
				fn::globals::WeaponName == "Flint-Knock Pistol" ||
				fn::globals::WeaponName == "Tactical Pistol")
				return true;

			return false;
		}

		bool IsHoldingSniper() {
			if (fn::globals::WeaponName == "Storm Scout" ||
				fn::globals::WeaponName == "Dragon's Breath Sniper" ||
				fn::globals::WeaponName == "Boom Sniper Rifle" ||
				fn::globals::WeaponName == "Storm Scout Sniper Rifle" ||
				fn::globals::WeaponName == "Hunter Bolt-Action Sniper" ||
				fn::globals::WeaponName == "Rail Gun" ||
				fn::globals::WeaponName == "Heavy Sniper Rifle" ||
				fn::globals::WeaponName == "Suppressed Sniper Rifle" ||
				fn::globals::WeaponName == "Bolt-Action Sniper Rifle" ||
				fn::globals::WeaponName == "Lever Action Rifle" ||
				fn::globals::WeaponName == "Automatic Sniper Rifle" ||
				fn::globals::WeaponName == "Semi-Auto Sniper Rifle   ")
				return true;

			return false;
		}

		VOID WeaponConfig() {

			while (true)
			{
				if (settings::WeaponConfigs && strstr(fn::globals::FName.c_str(), ("PlayerPawn"))) {

					if (IsHoldingAssaultRifle)
					{
						settings::AimFOV = settings::configs::fov_AR;
						settings::smooth = settings::configs::smoothness_AR;
					}

					else if (IsHoldingShotgun)
					{
						settings::AimFOV = settings::configs::fov_SHOTGUN;
						settings::smooth = settings::configs::smoothness_SHOTGUN;
					}

					else if (IsHoldingSMG)
					{
						settings::AimFOV = settings::configs::fov_SMG;
						settings::smooth = settings::configs::smoothness_SMG;
					}

					else if (IsHoldingPistol)
					{
						settings::AimFOV = settings::configs::fov_PISTOL;
						settings::smooth = settings::configs::smoothness_PISTOL;
					}
					
					else if (IsHoldingSniper)
					{
						settings::AimFOV = settings::configs::fov_SNIPER;
						settings::smooth = settings::configs::smoothness_SNIPER;
					}


				}

			}

		}

	}

	void WorldCache()
	{
		while (true)
		{

			fn::globals::Uworld = read<DWORD_PTR>(fn::pointers::base_address + fn::offsets::Uworld);
			//std::cout << " Uworld -> " << fn::globals::Uworld << std::endl;

			fn::globals::Gameinstance = read<DWORD_PTR>(fn::globals::Uworld + fn::offsets::OwningGameInstance);
			//std::cout << " Gameinstance -> " << fn::globals::Gameinstance << std::endl;

			fn::globals::Localplayer = read<DWORD_PTR>(fn::globals::Gameinstance + 0x38);
			//std::cout << " Localplayer -> " << fn::globals::Localplayer << std::endl;

			fn::globals::PlayerController = read<uint64_t>(fn::globals::Localplayer + 0x30);
			//std::cout << " PlayerController -> " << fn::globals::PlayerController << std::endl;

			fn::globals::LocalPawn = read<uintptr_t>(fn::globals::PlayerController + fn::offsets::AcknowledgedPawn);
			std::cout << " LocalPawn -> " << fn::globals::LocalPawn << std::endl;

			//uintptr_t GameState = read<uintptr_t>(fn::globals::Uworld + 0x158);
			//std::cout << " GameState " << GameState << std::endl;
			//if (!GameState) continue;

			//uintptr_t PlayerArray = read<uintptr_t>(GameState + 0x2a0);
			//std::cout << " PlayerArray " << PlayerArray << std::endl;
			//if (!PlayerArray) { playerlist.clear(); continue; }

			//int Count = read<int>(GameState + (0x2a0 + sizeof(uint64_t)));

			DWORD_PTR persistentlevel = read<DWORD_PTR>(fn::globals::Uworld + 0x30);
			//std::cout << " persistentlevel " << persistentlevel << std::endl;
			DWORD ActorCount = read<DWORD>(persistentlevel + (0x98 + 0x8));
			//std::cout << " ActorCount " << ActorCount << std::endl;
			DWORD_PTR AActors = read<DWORD_PTR>(persistentlevel + 0x98);
			//std::cout << " AActors " << AActors << std::endl;

			for (int i = 0; i < ActorCount; i++)
			{
				/*uint64_t CurrentPlayerState = read<uintptr_t>(PlayerArray + (i * sizeof(uint64_t)));
				std::cout << " PlayerState " << CurrentPlayerState << std::endl;
				if (!CurrentPlayerState) continue;
				fn::globals::PlayerState = CurrentPlayerState;
				*/

				DWORD_PTR CurrentPawn = read<DWORD_PTR>(AActors + i * 0x8);
				std::cout << " CurrentPawn " << CurrentPawn << std::endl;
				if (!CurrentPawn) continue;
				if (CurrentPawn == fn::globals::LocalPawn) continue;

				float player_check = read<float>(CurrentPawn + 0x4480);
				std::cout << " Player Check " << player_check << std::endl;

				if (player_check != 10.f)
				{
					INT32 ObjectID = read<INT32>(CurrentPawn + 0x18);
					std::cout << " ObjectID " << ObjectID << std::endl;
					if (!ObjectID) continue;

					uint64_t mesh = read<uint64_t>(CurrentPawn + 0x310);
					std::cout << " mesh " << mesh << std::endl;
					if (mesh) continue;

					uintptr_t PlayerState = read<DWORD_PTR>(fn::globals::LocalPawn + 0x2a8);
					std::cout << " PlayerState -> " << PlayerState << std::endl;
					if (!PlayerState) continue;

					uintptr_t RootComponent = read<uintptr_t>(fn::globals::LocalPawn + fn::offsets::RootComponent);
					std::cout << " RootComponent " << RootComponent << std::endl;
					if (!RootComponent) continue;

					INT32 team_id = read<INT32>(PlayerState + 0x10B0);
					uintptr_t EnemyState = read<uintptr_t>(CurrentPawn + fn::offsets::PlayerState);
					if (team_id == read<uintptr_t>(EnemyState + 0x10B0)) continue;

					fn::fvector RelativeLocation = read<fn::fvector>(RootComponent + fn::offsets::RelativeLocation);
					std::cout << " relativeLocationX " << RelativeLocation.x << " RelativeLocationY " << RelativeLocation.y << std::endl;
					if (IsVec3Valid(RelativeLocation)) continue;

					uintptr_t camera = read<uint64_t>(fn::globals::PlayerController + 0x340);
					uintptr_t CurrentWeapon = read<uintptr_t>(fn::globals::LocalPawn + fn::offsets::CurrentWeapon);
					vCamera = update_camera(RootComponent);
					std::string fname = find_object_name(ObjectID);

					uintptr_t WeaponData = read<uintptr_t>(CurrentWeapon + 0x410);
					uintptr_t DisplayName = read<uintptr_t>(WeaponData + 0x90);
					uint32_t WeaponLength = read<uint32_t>(DisplayName + 0x38);
					wchar_t* WeaponName = new wchar_t[uintptr_t(WeaponLength) + 1];
					std::wstring wWeaponName(WeaponName);
					std::string sWeaponName(wWeaponName.begin(), wWeaponName.end());
					fn::globals::WeaponName = sWeaponName;

					if (strstr(fname.c_str(), ("PlayerPawn_Athena_C")) || strstr(fname.c_str(), ("PlayerPawn_Athena_Phoebe_C")) || strstr(fname.c_str(), ("BP_MangPlayerPawn")) || strstr(fname.c_str(), ("PlayerPawn")))
						std::cout << " UHFDEUHFUEBFUEEFUEFUEFUFEHUFBEUBFUEBFUBEUEFBEUBFUE" << std::endl;

					fn::actor player_pawns{ };
					player_pawns.pawn = CurrentPawn;
					player_pawns.mesh = mesh;
					player_pawns.rootcomp = RootComponent;
					player_pawns.location = RelativeLocation;
					player_pawns.player_state = PlayerState;
					player_pawns.enemy_state = EnemyState;
					player_pawns.weapon = CurrentWeapon;
					player_pawns.camera = camera;
					player_pawns.team_id = team_id;
					player_pawns.DBNO = player_check;
					player_pawns.lobby = false;

					if (!fn::globals::LocalPawn)
						player_pawns.lobby = true;

					if (!playerlist.empty()) {
						auto found_player = std::find(playerlist.begin(), playerlist.end(), player_pawns);
						if (found_player == playerlist.end())
							playerlist.push_back(player_pawns);
					}
					else
						playerlist.push_back(player_pawns);


				}
			}

			playerlist.clear();
			std::this_thread::sleep_for(std::chrono::microseconds(1750));

		}

	}

	void watermelon()
	{
		float closestDistance = FLT_MAX;
		int closestPawn = 1337;

		ImU32 black = ImColor(0, 0, 0);
		ImU32 white = ImColor(255, 255, 255, 255);
		ImU32 red = ImColor(255, 0, 0, 255);
		ImU32 orange = (255, 165, 0);
		ImU32 lightblue = ImColor::HSV(0.55f, 0.6f, 0.95f);
		ImU32 ESPColor{ };

		ImGui::GetOverlayDrawList()->AddCircle(ImVec2(center_x, center_y), settings::AimFOV, white, 2000, 1.0f); //  * center_x / vCamera.FieldOfView) / 2

		for (int x = 0; x < playerlist.size(); x++)
		{
			fn::actor entity = playerlist.at(x);

			if (fn::globals::LocalPawn == entity.pawn) continue;

			uint64_t mesh = entity.mesh;
			if (!mesh) continue;

			fn::fvector Foot = BoneMatrix(mesh, 0);

			uint64_t PlayerState = entity.player_state;
			if (!PlayerState) continue;

			if ((int)Foot.x != 0 && (int)Foot.z != 0)
			{

				fn::fvector vHeadBone = BoneMatrix(mesh, 68);
				if (IsVec3Valid(vHeadBone)) continue;

				fn::fvector vRootBone = BoneMatrix(mesh, 0);
				if (IsVec3Valid(vRootBone)) continue;

				fn::fvector vHeadBoneOut = w2s(vHeadBone);
				if (IsVec3Valid(vHeadBoneOut)) continue;

				fn::fvector vRootBoneOut = w2s(vRootBone);
				if (IsVec3Valid(vRootBoneOut)) continue;

				fn::fvector vTopBoneOut = w2s(fn::fvector(vHeadBone.x, vHeadBone.y, vHeadBone.z + 22));
				if (IsVec3Valid(vTopBoneOut)) continue;

				fn::fvector vBottomBoneOut = w2s(fn::fvector(vRootBone.x, vRootBone.y, vRootBone.z - 5));
				if (IsVec3Valid(vBottomBoneOut)) continue;

				if (settings::bAimbot) {
					auto dx = vHeadBoneOut.x - center_x;
					auto dy = vHeadBoneOut.y - center_y;
					auto dist = sqrtf(dx * dx + dy * dy);
					if (dist < settings::AimFOV && dist < closestDistance)
					{
						closestDistance = dist;
						closestPawn = x;
					}
				}

				if (!entity.lobby)
				{
					std::cout << " NOOOOOOOOOOOOOOOOOOOOOIEFHIFEHEIHFIEHEFIFHIEHEFHIHS" << std::endl;

					if (entity.team_id == read<char>(entity.enemy_state + 0x28c)) { // Local Team
						ESPColor = black;
					}
					else if (entity.team_id != read<char>(entity.enemy_state + 0x28c)) { // Enemy team
						ESPColor = lightblue;
					}

					//if (WasRecentlyRendered(mesh)) {
					//	ESPColor = settings::color::visbox;
					//}
					//else {

					//}

					double player_distance = entity.location.Distance(vRootBoneOut) / 100;

					if (player_distance <= settings::max_distance)
					{

						if (IsInScreen(vHeadBone))
						{

							if (settings::box_type == SELECT_BOXES_NORMAL)
							{
								float BoxHeight = vBottomBoneOut.y - vTopBoneOut.y;
								float BoxWidth = BoxHeight / 1.9f;
								box_esp(4, 4, vBottomBoneOut.x - (BoxWidth / 2), vTopBoneOut.y,
									BoxWidth, BoxHeight, ESPColor, 1.f,
									settings::fill, settings::outline);
							}
							else if (settings::box_type == SELECT_BOXES_CORNER)
							{
								float BoxHeight = vBottomBoneOut.y - vTopBoneOut.y;
								float BoxWidth = BoxHeight / 1.9f;
								corner_esp(4, 4, vBottomBoneOut.x - (BoxWidth / 2), vTopBoneOut.y,
									BoxWidth, BoxHeight, ESPColor, 1.f,
									settings::fill, settings::outline);
							}
							else if (settings::box_type == SELECT_BOXES_THREED)
							{
								fn::fvector headrender = fn::fvector(vHeadBone.x, vHeadBone.y, vHeadBone.z + 15);
								ThreeD_Box(false, 40, 2.0f, ESPColor, vRootBoneOut, headrender.z);
							}

							if (settings::ViewAngles)
							{
								fn::fvector _Angle = read<fn::fvector>(entity.rootcomp + 0x140);
								fn::fvector test2, headpos;
								headpos = vHeadBone;

								math::Angles(_Angle, &test2);
								test2.x *= 160;
								test2.y *= 160;
								test2.z *= 160;

								fn::fvector end = headpos + test2;
								fn::fvector test1, test3;
								test1 = w2s(headpos);
								test3 = w2s(end);

								ImGui::GetOverlayDrawList()->AddLine(ImVec2(test1.x, test1.y), ImVec2(test3.x, test3.y), red, 2.0f);
								ThreeD_Box(true, 10.f, 2.0f, ImColor(255, 255, 255, 255), end, 0.f);
							}

							if (settings::console)
							{
								DWORD_PTR test_platform = read<DWORD_PTR>(entity.player_state + 0x430);
								wchar_t platform[64];
								driver->ReadProcessMemory(test_platform, platform, sizeof(test_platform));
								std::wstring balls_sus(platform);
								std::string str_platform(balls_sus.begin(), balls_sus.end());
								ImVec2 TextSize = ImGui::CalcTextSize(str_platform.c_str());
								DrawFortniteText(ImGui::GetFont(), ImVec2((vRootBone.x) - (TextSize.x / 2), (vTopBoneOut.y - 20)), white, str_platform.c_str());
							}

							if (settings::distance)
							{
								char text_distance[256];
								float BoxHeight = vBottomBoneOut.y - vTopBoneOut.y;
								float BoxWidth = BoxHeight / 1.9f;
								sprintf_s(text_distance, "(%.fm)", player_distance);
								ImVec2 text_size = ImGui::CalcTextSize(text_distance);
								DrawFortniteText(font::fortnite, ImVec2(vBottomBoneOut.x - (BoxWidth / 2), vTopBoneOut.y - (text_size.x / 2)), white, text_distance); // - (text_size.x / 2)
							}

							if (settings::b3Dhead)
							{
								ThreeD_Box(true, 10.f, 1.f, red, vHeadBoneOut, 0.f);
							}

							if (settings::b2Dhead)
							{
								ImGui::GetOverlayDrawList()->AddCircle(ImVec2(vHeadBoneOut.x, vHeadBoneOut.y), player_distance / 7, red, 2000, 0.5f);
							}

							if (settings::Snaplines)
							{
								ImGui::GetOverlayDrawList()->AddLine(ImVec2(1920 / 2, 1080), ImVec2(vRootBoneOut.x, vRootBoneOut.y), ESPColor, 0.5);
							}

							if (settings::skell)
							{
								skeleton_esp(mesh);
							}

						}

					}

				}
			}
		}

		if (settings::bAimbot && closestPawn != 1337 && fn::globals::LocalPawn != 0)
		{
			fn::actor player = playerlist[closestPawn];

			bool Copy_Aimbot = settings::bAimbot;
			bool Copy_MemoryAim = settings::MemoryAim;
			bool Copy_MemoryAim_2 = settings::MemoryAimV2;
			bool Copy_Prediction = settings::SniperPrediction;
			bool Copy_Triggerbot = settings::autofire;
			bool Copy_PlayerVisble = settings::VisbleOnly;
			bool Copy_Unknocked = settings::UnKnockedOnly;
			bool Copy_lines = settings::TargetLines;
			float Copy_Aim_Fieldofview = settings::AimFOV;
			float Copy_Smoothness_Speed = settings::smooth;
			bool bResetAngles = false;

			std::cout << " Aimbot" << std::endl;

			fn::fvector PlayerBone = BoneMatrix(player.mesh, 68);
			fn::fvector PlayerPosition = w2s(PlayerBone);
			//BYTE bIsDying = read<BYTE>(player.pawn + 0x710);

			double player_distance = player.location.Distance(PlayerPosition);

			if (IsInScreen(PlayerPosition) && WasRecentlyRendered(player.mesh))
			{
				if (player_distance <= settings::AimFOV)
				{

					if (player.team_id != read<char>(player.enemy_state + 0x28c))
					{

						if (PlayerPosition.x != 0 || PlayerPosition.y != 0 || PlayerPosition.z != 0)
						{

							if (GetDistance(PlayerPosition.x, PlayerPosition.y, PlayerPosition.z, center_x, center_y) <= Copy_Aim_Fieldofview)
							{

								if (player.weapon && Copy_lines)
								{
									ImGui::GetOverlayDrawList()->AddLine(ImVec2(center_x, center_y), ImVec2(PlayerPosition.x, PlayerPosition.y), orange, 1.0f);
								}

								if (GetAsyncKeyState(VK_RBUTTON) || Controller::IsPressingRightTrigger && Copy_Prediction)
								{
									bool PawnTargetting = read<bool>(fn::globals::LocalPawn + 0x1b3c);
									if ((PawnTargetting) && (PlayerWeapon::IsHoldingSniper)) {
										auto CurActorRootComp = read<DWORD_PTR>(player.rootcomp + player.pawn);
										PlayerPosition = prediction(PlayerPosition, CurActorRootComp, player_distance);
									}
								}

								if (GetAsyncKeyState(VK_RBUTTON) || Controller::IsPressingRightTrigger && Copy_Aimbot)
								{
									mouse(PlayerPosition.x, PlayerPosition.y, Copy_Smoothness_Speed);
									Controller::SetControllerVibration(65535);
								}

								if (GetAsyncKeyState(VK_RBUTTON) || Controller::IsPressingRightTrigger && Copy_MemoryAim)
								{
									move(player.player_state, PlayerPosition.x, PlayerPosition.y, Copy_Smoothness_Speed);
									Controller::SetControllerVibration(65535);
								}

								if (GetAsyncKeyState(VK_RBUTTON) || Controller::IsPressingRightTrigger && Copy_MemoryAim_2)
								{
									SetAngles(player.camera, vCamera.Location, PlayerPosition);
									bResetAngles = true;
								}

								if (Copy_Triggerbot)
								{
									if (settings::triggerkey) {
										if (GetAsyncKeyState(settings::i_triggerkey)) {
											presskeycode(settings::delay);
											Controller::SetControllerTriggers();
										}
									}
									else {
										presskeycode(settings::delay);
										Controller::SetControllerTriggers();
									}

								}

							}

						}
					}

				}
			}

			//Restart:
			//	{
			//		if (bResetAngles)
			//		{
			//			ResetAngles(player.camera);
			//			bResetAngles = false;
			//		}
			//	}

		}
	}

	void render()
	{
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (GetAsyncKeyState(VK_INSERT))
		{
			settings::DrawMenu = !settings::DrawMenu;
			Sleep(200);
		}

		if (settings::DrawMenu)
		{
			render::menu(D3dDevice);
		}

		watermelon();
		//PlayerWeapon::WeaponConfig();

		ImGui::EndFrame();
		D3dDevice->SetRenderState(D3DRS_ZENABLE, false);
		D3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		D3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		D3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

		if (D3dDevice->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			D3dDevice->EndScene();
		}
		HRESULT result = D3dDevice->Present(NULL, NULL, NULL, NULL);

		if (result == D3DERR_DEVICELOST && D3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			D3dDevice->Reset(&d3dpp);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
	}

	void SetWindowToTarget()
	{
		while (true)
		{
			if (hwnd)
			{
				ZeroMemory(&GameRect, sizeof(GameRect));
				GetWindowRect(hwnd, &GameRect);
				Width = GameRect.right - GameRect.left;
				Height = GameRect.bottom - GameRect.top;
				DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);

				if (dwStyle & WS_BORDER)
				{
					GameRect.top += 32;
					Height -= 39;
				}

				MoveWindow(Window, GameRect.left, GameRect.top, Width, Height, true);
			}
			else
			{
				exit(0);
			}
		}
	}

	void stop_render()
	{
		TriBuf->Release();
		D3dDevice->Release();
		p_Object->Release();

		DestroyWindow(Window);
		UnregisterClass("Overlay", NULL);
	}

	HWND GetGameWindowHandle()
	{
		HWND fortnite = FindWindowA("UnrealWindow", "Fortnite  ");
		if (fortnite) return fortnite;
	}

	bool InitDirectX9()
	{
		if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
			return false;

		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.BackBufferWidth = Width;
		d3dpp.BackBufferHeight = Height;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dpp.MultiSampleQuality = D3DMULTISAMPLE_NONE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.hDeviceWindow = Window;
		d3dpp.Windowed = TRUE;

		p_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &D3dDevice);

		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		icons_config.OversampleH = 3;
		icons_config.OversampleV = 3;

		io.FontDefault = font::default_segu = io.Fonts->AddFontFromMemoryTTF(&seguoe, sizeof seguoe, 22, NULL, io.Fonts->GetGlyphRangesCyrillic());
		font::segu = io.Fonts->AddFontFromMemoryTTF(&seguoe, sizeof seguoe, 40, NULL, io.Fonts->GetGlyphRangesCyrillic());
		font::verdana = io.Fonts->AddFontFromMemoryTTF(&Verdana, sizeof Verdana, 22, NULL, io.Fonts->GetGlyphRangesCyrillic());
		font::Arial = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 20.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
		font::skeet = io.Fonts->AddFontFromMemoryTTF(&Skeet, sizeof Skeet, 22, NULL, io.Fonts->GetGlyphRangesCyrillic());
		font::bold_segu = io.Fonts->AddFontFromMemoryTTF(&bold_segue, sizeof bold_segue, 30, NULL, io.Fonts->GetGlyphRangesCyrillic());
		font::bold_verdana = io.Fonts->AddFontFromMemoryTTF(&bold_verdana, sizeof bold_verdana, 22, NULL, io.Fonts->GetGlyphRangesCyrillic());
		font::whimsical = io.Fonts->AddFontFromMemoryTTF(&SkeetEyes, sizeof SkeetEyes, 22, NULL, io.Fonts->GetGlyphRangesCyrillic());
		font::fortnite = io.Fonts->AddFontFromMemoryTTF(&fortnite, sizeof fortnite, 22, NULL, io.Fonts->GetGlyphRangesCyrillic());
		font::gta = io.Fonts->AddFontFromMemoryTTF(&GTA, sizeof GTA, 40, NULL, io.Fonts->GetGlyphRangesCyrillic());
		font::icon = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 25, NULL, io.Fonts->GetGlyphRangesCyrillic());

		ImGui_ImplWin32_Init(Window);
		ImGui_ImplDX9_Init(D3dDevice);

		ImGui::StyleColorsClassic();

		return true;

	}

	void render_loop()
	{
		ZeroMemory(&Message, sizeof(MSG));

		while (Message.message != WM_QUIT)
		{

			if (PeekMessage(&Message, Window, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}

			HWND hwnd_active = GetForegroundWindow();

			if (hwnd_active == hwnd) {
				HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
				SetWindowPos(Window, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}

			RECT rc;
			POINT xy;

			ZeroMemory(&rc, sizeof(RECT));
			ZeroMemory(&xy, sizeof(POINT));
			GetClientRect(hwnd, &rc);
			ClientToScreen(hwnd, &xy);
			rc.left = xy.x;
			rc.top = xy.y;

			ImGuiIO& io = ImGui::GetIO();
			io.IniFilename = NULL;
			io.ImeWindowHandle = hwnd;
			io.DeltaTime = 1.0f / 60.f;
			center_x = Width / 2;
			center_y = Height / 2;

			POINT p;
			GetCursorPos(&p);
			io.MousePos.x = p.x - xy.x;
			io.MousePos.y = p.y - xy.y;

			if (GetAsyncKeyState(VK_LBUTTON)) {
				io.MouseDown[0] = true;
				io.MouseClicked[0] = true;
				io.MouseClickedPos[0].x = io.MousePos.x;
				io.MouseClickedPos[0].x = io.MousePos.y;
			}
			else
				io.MouseDown[0] = false;

			render();

		}

		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		DestroyWindow(Window);
	}

	LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
			return true;

		switch (Message)
		{
		case WM_DESTROY:
			stop_render();
			PostQuitMessage(0);
			exit(4);
			break;
		case WM_SIZE:
			if (D3dDevice != NULL && wParam != SIZE_MINIMIZED)
			{
				ImGui_ImplDX9_InvalidateDeviceObjects();
				d3dpp.BackBufferWidth = LOWORD(lParam);
				d3dpp.BackBufferHeight = HIWORD(lParam);
				HRESULT hr = D3dDevice->Reset(&d3dpp);
				if (hr == D3DERR_INVALIDCALL)
					IM_ASSERT(0);
				ImGui_ImplDX9_CreateDeviceObjects();
			}
			break;
		default:
			return DefWindowProc(hWnd, Message, wParam, lParam);
			break;
		}
		return 0;
	}

	void setup_window()
	{
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetWindowToTarget, 0, 0, 0);

		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(wc));
		wc.cbSize = sizeof(wc);
		wc.lpszClassName = "Overlay";
		wc.lpfnWndProc = WinProc;
		RegisterClassEx(&wc);

		GetClientRect(hwnd, &GameRect);
		POINT xy;
		ClientToScreen(hwnd, &xy);
		GameRect.left = xy.x;
		GameRect.top = xy.y;

		Width = GameRect.right;
		Height = GameRect.bottom;

		Window = CreateWindowEx(NULL, "Overlay", "Overlay Util", WS_POPUP | WS_VISIBLE, 0, 0, Width, Height, 0, 0, 0, 0);

		DwmExtendFrameIntoClientArea(Window, &Margin);
		SetWindowLong(Window, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
		ShowWindow(Window, SW_SHOW);
		UpdateWindow(Window);
	}

	void hijack_gaynite()
	{
		hwnd = FindWindowA("UnrealWindow", "Fortnite  ");

		if (Controller::ControllerConnection())
			std::cout << " [" << hue::red << ">" << hue::reset << " Found a controller" << std::endl;

		setup_window();
		InitDirectX9();

		std::thread(WorldCache).detach();
		Sleep(200);

		render_loop();
		stop_render();
	}

}