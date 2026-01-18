#pragma once

// windows / stl includes.
#include <Windows.h>
#include <cstdint>
#include <intrin.h>
#include <xmmintrin.h>
#include <array>
#include <vector>
#include <algorithm>
#include <cctype>
#include <string>
#include <chrono>
#include <thread>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <deque>
#include <functional>
#include <map>
#include <shlobj.h>
#include <filesystem>
#include <streambuf>
#include <dwmapi.h>
#include <any>
#include <TlHelp32.h>
#include <comip.h>
#pragma comment(lib, "comsuppw.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

// third-party
#define IMGUI_ENABLE_VIEWPORTS
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../third-party/imgui/imgui.h"
#include "../third-party/imgui/imgui_internal.h"
#include "../third-party/imgui/imgui_impl_win32.h"
#include "../third-party/imgui/imgui_impl_dx11.h"

static ID3D11Device*			g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;

#include "../src/utils/utils.h"

#include "../src/menu/menu.h"
