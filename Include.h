//Basic includes
#include <iostream>
#include <Windows.h>
#include <thread>
#include <string>
#include <tlhelp32.h>
#include <fstream>
#include <filesystem>
#include <vector>
#include <math.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <tchar.h>
#include <winioctl.h>
#include <Uxtheme.h>
#include <dwmapi.h>
#include <WinInet.h>
#include <Mmsystem.h>
#include <mciapi.h>
#include <shobjidl_core.h>
#include <direct.h>
#include <urlmon.h>
#include <random>
#include <lmcons.h>
#include <d3d9.h>
#include <xstring>
#include <xmmintrin.h>
#include <immintrin.h>
#include <future>
#include <bcrypt.h>
#include <string>
#include <WinInet.h>
#include <vector>
#include <array>
#include <corecrt_math.h>
#include <TlHelp32.h>
#include <wtypes.h>
#include <conio.h>
#include <shlobj.h>
#include <shlwapi.h> 
#include <shlobj.h>
#include <set>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "urlmon.lib")


//Encryption
#include "Encryption/XorCrypt.h"
#include "Encryption/XorStr.h"
#include "Encryption/SpoofCall.h"
#include "Encryption/Li.h"

//Render
#include "ImGui\Snow.hpp"
#include "ImGui\imgui_impl_dx9.h"
#include "ImGui\imgui_impl_win32.h"
#include "ImGui\imgui_internal.h"
#include "ImGui\Fonts.h"

//Images
#include "ImGui\ImageRender.h"
#include "ImGui\ImageBytes.h"

#include "Driver.h"

bool BytesToMemory(const std::string& desired_file_path, const char* address, size_t size)
{
	std::ofstream file_ofstream(desired_file_path.c_str(), std::ios_base::out | std::ios_base::binary);
	if (!file_ofstream.write(address, size))
	{
		file_ofstream.close();
		return false;
	}
	file_ofstream.close();
	return true;
}

//Generate Randomized strings
std::string RandomString(int length) {
	const std::string characters = XorCrypt("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	std::string randomString;
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	for (int i = 0; i < length; ++i) {
		int randomIndex = std::rand() % characters.length();
		randomString += characters[randomIndex];
	}
	return randomString;
}

//Store the GetAsyncKeyState Pointer so we dont recall everytime
typedef int (WINAPI* DKEY_DDW)(DWORD);
static DKEY_DDW KeyState;
bool StoreAsyncKeyState()
{
	HMODULE API = GetModuleHandle(XorCryptW(L"win32u.dll"));
	if (API != NULL)
	{
		KeyState = (DKEY_DDW)GetProcAddress(API, XorCrypt("NtUserGetAsyncKeyState"));
		if (KeyState != NULL)
			return true;
		else
			RtlZeroMemory(&KeyState, sizeof(KeyState));
		return false;
	}
}

typedef void (WINAPI* STORE_EVENT)(DWORD, DWORD, DWORD, DWORD, ULONG_PTR);
static STORE_EVENT MouseEvent;
bool StoreMouseEvent()
{
	HMODULE API = GetModuleHandle(XorCryptW(L"user32.dll"));
	if (API != NULL)
	{
		MouseEvent = (STORE_EVENT)GetProcAddress(API, XorCrypt("mouse_event"));
		if (MouseEvent != NULL)
			return true;
		else
			RtlZeroMemory(&MouseEvent, sizeof(MouseEvent));
		return false;
	}

	return false;
}


//Include Settings
namespace Settings {
	int Tab = 0;
	float Width = GetSystemMetrics(SM_CXSCREEN);
	float Height = GetSystemMetrics(SM_CYSCREEN);
	float ScreenCenterX = 0;
	float ScreenCenterY = 0;
	inline bool ShowMenu = 1;
	inline bool ControlPanel = false;

	inline int MenuStyle = 0;
	namespace Aim {
		//Aimbot
		inline bool Aimbot = true;
		//inline bool VisibleCheck = true;
		inline float XSmooth = 4.5f;
		inline float YSmooth = 4.5f;
		inline float AimDistance = 250.0f;

		//KeyBind
		int Aimkey = 0;

		//TargetLine
		inline bool TargetLine = true;
		inline float TargetSize = 1.0f;

		//Prediction
		inline float ProjectileSpeed = 12.0f;
		inline bool Prediction = false;
		inline bool PredictionLine = false;
		inline bool PredictionCircle = false;
	}

	namespace Visuals {
		inline float RenderDistance = 120;

		//Box
		inline bool Box = true;
		inline bool BoxFilled = false;
		inline float BoxWidth = 3.5f;
		inline float BoxThickness = 1.0f;
		inline float BoxClearness = 1.0f;

		//SnapLines
		inline bool SnapLines = false;

		//HeroNames
		inline bool HeroNames = true;

		//Distance
		inline bool Distance = true;

		//Health
		inline bool Health = true;
	}

	namespace Misc {
		//Crosshair
		inline bool Crosshair = true;
		inline float Length = 6.69f;

		//Fov
		inline bool FovCircle = true;
		inline bool FovCircleFull = false;
		inline bool FovSquare = false;
		inline bool FovSquareFull = false;

		inline float Fovsize = 145;
		inline float FovDetail = 50;
		inline float Transparent = 250;

		//Performance
		inline float LoopThread = 500.0f;
	}


	//Stored Colors 
	ImColor MenuColor = ImColor(0.806f, 0.00f, 0.717f, 1.0f);
	ImColor TargetColor = ImColor(0.806f, 0.00f, 0.717f, 1.0f);
	ImColor SnapLinesColor = ImColor(130.0f, 0.f, 173.0f, 255.0f);
	ImColor BoxColor = ImColor(255.0f, 255.0f, 255.0f, 255.0f);
	ImColor NameColor = ImColor(255.0f, 255.0f, 255.0f, 255.0f);
	ImColor DistanceColor = ImColor(255.0f, 255.0f, 255.0f, 255.0f);
}

//Interface
#include "ImGui.h"

//Menu
#include "Menu.h"

//Struct
#include "Struct.h"


