#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#include <dwmapi.h>
#pragma comment (lib, "dwmapi.lib")

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

#include "D3DX/d3dx9.h"
#pragma comment (lib, "d3d9.lib")

#define PI 3.14159265358979323846f
#define M_PI 3.14159265358979323846264338327950288419716939937510

int center_x;
int center_y;