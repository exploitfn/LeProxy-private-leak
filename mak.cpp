#include <dwmapi.h>
#include <vector>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "D3DX/d3dx9math.h"
#include <thread>
#include <Windows.h>
#include <direct.h>
#include <string>
#include <intrin.h>
#include <list>
#include <WinUser.h>
#include <Windows.h>
#include <filesystem>
#include <random>
#include <TlHelp32.h>
#include "auth.hpp"
#include "game.h"
#include "xor.h"
#include "console.h"
#include "stream.h"
#include "obsidium/obsidium64.h"

using namespace std;
using namespace KeyAuth;

std::string name = "backbone"; // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = "TG8Ri7dF6K"; // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = "6b57e34aa341a90fc6a4a146ec59657b78d9e1bea01de6435eab5a244fd659db"; // app secret, the blurred text on licenses tab and other tabs
std::string version = "1.0"; // leave alone unless you've changed version on website
std::string url = "https://keyauth.win/api/1.2/"; // change if you're self-hosting
api KeyAuthApp(name, ownerid, secret, version, url);

DWORD FindProcessID(std::string name) {
	PROCESSENTRY32 ProcessEntry;
	ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &ProcessEntry) == (int)1) {
		while (Process32Next(snapshot, &ProcessEntry) == (int)1) {
			if (std::string(ProcessEntry.szExeFile) == name)
				return ProcessEntry.th32ProcessID;
		}
	}

	CloseHandle(snapshot);
	return NULL;
}

void main()
{
	OBSIDIUM_VM_START;
	OBSIDIUM_ENC_START;
	OBSIDIUM_LIC_START;

	SetConsoleTitleA("Backbone External");
	//system("start https://discord.gg/4RVNKyKTDD");
	//if (KeyAuthApp.checkblack()) abort();

	if (driver->Init(FALSE))
	{
		cout << " [" << hue::yellow << ">" << hue::reset << "] Initializing" << endl;

		KeyAuthApp.init();
		if (!KeyAuthApp.data.success)
		{
			std::cout << "[" << hue::red << ">" << hue::reset << XorStr("] Auth Error: Failed to get session please restart").c_str() << std::endl;
			Sleep(1500);
			exit(0);
		}

		//if (FindAtomA("Shellcode!!??") == 0) {
		//	//ShellExecuteA(NULL, NULL, "taskkill"), "/F /T /IM FortniteClient-Win64-Shipping.exe"), NULL, SW_HIDE);
		//	//ShellExecuteA(NULL, NULL, "taskkill"), "/F /T /IM EpicGamesLauncher.exe"), NULL, SW_HIDE);
		//	auto c1(xParseByteArray(XorStr("48 89 5C 24 08 57 48 83 EC 30 49 8B C0 48 8B FA 48 8B D0 4C 8D 05 CC CC CC CC 48 8B CF E8 CC CC CC CC 8B D8 85 C0 78 09").c_str()));
		//	auto c2(xParseByteArray(XorStr("48 89 5C 24 08 57 48 83 EC 30 49 8B C0 48 8B FA 48 8B D0 4C 8D 05 CC CC CC CC 48 8B CF B8 00 00 00 00 8B D8 85 C0 78 09").c_str()));
		//	xPatchProcess(L"dwm.exe", c1, c2, NULL, 0);
		//	std::cout << " [" << hue::blue << ">" << hue::reset << "] Enabled Streamproof!" << endl;
		//	AddAtomA("Shellcode!!??");
		//	Sleep(2000);
		//}

		//std::cout << endl << XorStr(" [1] Login\n [2] Register\n\n Choose option: ").c_str();

		//int option;
		//std::string username;
		//std::string pass;
		//std::string key;

		//std::cin >> option;
		//cout << endl << endl;

		//switch (option)
		//{
		//case 1:
		//	std::cout << " [" << hue::yellow << "?" << hue::reset << XorStr("] Enter username: ").c_str();
		//	std::cin >> username;
		//	KeyAuthApp.login(username, username);
		//	break;
		//case 2:
		//	std::cout << " [" << hue::yellow << "?" << hue::reset << XorStr("] Enter username: ").c_str() << endl;
		//	std::cin >> username;
		//	std::cout << " [" << hue::yellow << "?" << hue::reset << XorStr("] Enter license: ").c_str() << endl;
		//	std::cin >> key;
		//	KeyAuthApp.regstr(username, username, key);
		//	break;
		//default:
		//	std::cout << std::endl << " [" << hue::red << "!" << hue::reset << XorStr("] Input Error: Invalid choice please restart").c_str();
		//	Sleep(1500);
		//	exit(0);
		//	break;
		//}

		//if (!KeyAuthApp.data.success)
		//{
		//	std::cout << std::endl << " [" << hue::red << "!" << hue::reset << XorStr("] Auth Error: Login failed please restart").c_str() << std::endl;
		//	Sleep(2500);
		//	exit(0);
		//}
		//else
		//{
		//	std::cout << std::endl << " [" << hue::green << "!" << hue::reset << XorStr("] Login was successfull follow the next instructions").c_str() << std::endl;
		//	Sleep(2500);
		//	system("cls");
		//}

		std::cout << std::endl << " [" << hue::red << "!" << hue::reset << XorStr("] load the driver found in the same folder before hooking to fortnite").c_str() << std::endl;
		Sleep(3000);
		std::cout << " [" << hue::green << ">" << hue::reset << XorStr("] Start FortniteClient-Win64-Shipping.exe to hook").c_str() << std::endl;

		while (true) {
			auto FnCheck = FindWindowA("UnrealWindow", "Fortnite  ");
			if (FnCheck)
				break;
		}

		std::cout << " [" << hue::green << ">" << hue::reset << XorStr("] Found fortnite").c_str() << std::endl;
		Sleep(1000);

		fn::pointers::ProcessId = FindProcessID("FortniteClient-Win64-Shipping.exe");
		if (!fn::pointers::ProcessId) {
			std::cout << " [" << hue::red << "!" << hue::reset << XorStr("] Driver Error: Failed to get games PID please restart and remap driver").c_str() << std::endl;
			Sleep(2000);
			return;
		}

		driver->Attach(fn::pointers::ProcessId);
		fn::pointers::base_address = driver->GetModuleBase(L"FortniteClient-Win64-Shipping.exe");
		if (!fn::pointers::base_address) {
			std::cout << " [" << hue::red << "!" << hue::reset << XorStr("] Driver Error: Failed to get games base address please restart and remap driver").c_str() << std::endl;
			Sleep(2000);
			return;
		}

		std::cout << " [" << hue::light_blue << "!" << hue::reset << XorStr("] Hooked to fortnite!").c_str() << std::endl << std::endl;
		Sleep(200);

		std::cout << " [" << hue::purple << ">" << hue::reset << XorStr("] Proccess ID -> ").c_str() << std::hex << fn::pointers::ProcessId << std::endl << " [" << hue::purple << ">" << hue::reset << XorStr("] Base address -> ").c_str() << std::hex << fn::pointers::base_address << std::endl;
		std::cout << " [" << hue::purple << ">" << hue::reset << XorStr("] Backbone External -> FortniteClient-Win64-Shipping 4.20.0-4008490+++Fortnite_Release-3.5 1998-2018 x64").c_str() << std::endl;

		game::hijack_gaynite(); // LeProxy had been here
	}

	OBSIDIUM_VM_END;
	OBSIDIUM_ENC_END;
	OBSIDIUM_LIC_END;

	std::cout << XorStr(" Please restart your device").c_str() << std::endl;
	Sleep(2000);
	exit(-1);
}
