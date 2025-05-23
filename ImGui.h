
struct CurrentProcess {
	HWND Hwnd;
}Process;

struct OverlayWindow {
	WNDCLASSEX WindowClass;
	HWND Hwnd;	LPCSTR Name;
}Overlay;


struct DirectX9Interface {
	IDirect3D9Ex* IDirect3D9 = NULL;
	IDirect3DDevice9Ex* pDevice = NULL;
	D3DPRESENT_PARAMETERS pParameters = { NULL };
	MARGINS Margin = { -1 };
	MSG Message = { NULL };
}DirectX9;

ImFont* FontMain = nullptr;
ImFont* FontSecondaryA = nullptr;
ImFont* FontSecondaryB = nullptr;
ImFont* FontSecondaryC = nullptr;
ImFont* ThickFontA = nullptr;
ImFont* ThickFontB = nullptr;
ImFont* ThickFontC = nullptr;

ImColor ColorMain{};
ImVec4 ColorMainVec{};

void Style() {
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowRounding = 0;
	style->ChildRounding = 0;
	style->FrameRounding = 0;
	style->ChildBorderSize = 1.5f;
	style->FramePadding = ImVec2(16, 5);
	style->FrameBorderSize = 0.0f;
	style->ScrollbarRounding = 0.0f;
	style->ScrollbarSize = 5.0f;
	style->ItemSpacing = ImVec2(4, 4);
	style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style->WindowBorderSize = 2.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.8f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.09f, 0.11f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ColorMainVec;
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.09f, 0.09f, 0.011f, 0.45f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.80f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 1.00f, 1.00f, 0.35f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.35f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.35f);
}


void ScreenTools() {

	float centerX = Settings::Width / 2;
	float centerY = Settings::Height / 2;

	int Convert = (int)Settings::Misc::Transparent;
	if (Settings::Misc::FovCircle) {
		ImGui::GetForegroundDrawList()->AddCircle(ImVec2(centerX, centerY),
			float(Settings::Misc::Fovsize),
			ImColor(255, 255, 255, Convert),
			Settings::Misc::FovDetail);

		if (Settings::Misc::FovCircleFull) {
			ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(centerX, centerY),
				float(Settings::Misc::Fovsize),
				ImColor(0, 0, 0, 65),
				Settings::Misc::FovDetail);
		}
	}

	if (Settings::Misc::FovSquare) {
		ImGui::GetForegroundDrawList()->AddRect(ImVec2(centerX + Settings::Misc::Fovsize, centerY + Settings::Misc::Fovsize),
			ImVec2(centerX - Settings::Misc::Fovsize, centerY - Settings::Misc::Fovsize),
			ImColor(255, 255, 255, Convert),
			0, 2, 1.0);

		if (Settings::Misc::FovSquareFull) {
			ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(centerX + Settings::Misc::Fovsize, centerY + Settings::Misc::Fovsize),
				ImVec2(centerX - Settings::Misc::Fovsize, centerY - Settings::Misc::Fovsize),
				ImColor(0, 0, 0, 65),
				0, 1.0);
		}
	}

	if (Settings::Misc::Crosshair) {
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(centerX - Settings::Misc::Length, centerY),
			ImVec2(centerX + Settings::Misc::Length, centerY),
			ImColor(255, 255, 255, 255), 1.0f);

		ImGui::GetForegroundDrawList()->AddLine(ImVec2(centerX, centerY - Settings::Misc::Length),
			ImVec2(centerX, centerY + Settings::Misc::Length),
			ImColor(255, 255, 255, 255), 1.0f);
	}
}

typedef struct
{
	DWORD R;
	DWORD G;
	DWORD B;
	DWORD A;
}RGBA;

std::string string_To_UTF8(const std::string& str) {
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);
	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
	std::string retStr(pBuf);
	delete[]pwBuf;
	delete[]pBuf;
	pwBuf = NULL;
	pBuf = NULL;
	return retStr;
}

void DrawStrokeText(int x, int y, ImColor color, const char* str) {
	ImFont a;
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y - 1), color, utf_8_2.c_str());
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y - 1.1), color, utf_8_2.c_str());
}

void DrawNewText(int x, int y, RGBA* color, const char* str) {
	ImFont a;
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), utf_8_2.c_str());
}

void DrawRect(int x, int y, int w, int h, ImColor color, int thickness) {
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImColor(color), 0, 0, thickness);
}

void DrawFilledRect(int x, int y, int w, int h, ImColor color) {
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImColor(color), 0, 0);
}

void DrawCircleFilled(int x, int y, int radius, RGBA* color) {
	ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
}

void DrawCircle(int x, int y, int radius, RGBA* color, int segments) {
	ImGui::GetForegroundDrawList()->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), segments);
}

void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, RGBA* color, float thickne) {
	ImGui::GetForegroundDrawList()->AddTriangle(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), thickne);
}

void DrawTriangleFilled(int x1, int y1, int x2, int y2, int x3, int y3, RGBA* color) {
	ImGui::GetForegroundDrawList()->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
}

void DrawLine(int x1, int y1, int x2, int y2, RGBA* color, int thickness) {
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), thickness);
}


void DrawBox(int x, int y, int w, int h, const ImColor color, int thickness, float Corners)
{
	float lineW = (w / Corners);
	float lineH = (h / Corners);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x, y + lineH), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x + lineW, y), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w - lineW, y), ImVec2(x + w, y), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + lineH), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y + h - lineH), ImVec2(x, y + h), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y + h), ImVec2(x + lineW, y + h), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w - lineW, y + h), ImVec2(x + w, y + h), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w, y + h - lineH), ImVec2(x + w, y + h), color, thickness);
}

void DrawRect(int x, int y, int w, int h, const ImColor color)
{
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0);
}

void DrawLine(int x1, int y1, int x2, int y2, ImColor Color, int thickness)
{
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), Color, thickness);
}

void FilledRect(int x, int y, int w, int h, ImColor color)
{
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), 0, 0);
}


void DrawFPS() {
	//FPS
	ImColor bgColorText = ImColor(255, 255, 255, 15);
	ImColor bgColorOutline = ImColor(0, 0, 0);

	char FpsInfo[64];
	sprintf(FpsInfo, XorCrypt(" %0.f "), ImGui::GetIO().Framerate);
	ImVec2 textSizeFPS = ImGui::CalcTextSize(FpsInfo);
	ImVec2 rectMin = ImVec2(30, 60);
	ImVec2 rectMax = ImVec2(rectMin.x + textSizeFPS.x, rectMin.y + textSizeFPS.y);
	ImGui::GetForegroundDrawList()->AddRectFilled(rectMin, rectMax, bgColorText);
	ImGui::GetForegroundDrawList()->AddRect(rectMin, rectMax, bgColorOutline);
	ImGui::GetForegroundDrawList()->AddText(ImVec2(30, 60), ImColor(255, 255, 255, 255), FpsInfo);
}



bool DirectXInit() {
	SPOOF_FUNC;

	if (FAILED(Spoof(Direct3DCreate9Ex)(D3D_SDK_VERSION, &DirectX9.IDirect3D9))) {
		return false;
	}

	D3DPRESENT_PARAMETERS Params = { 0 };
	Params.Windowed = TRUE;
	Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Params.hDeviceWindow = Overlay.Hwnd;
	Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	Params.BackBufferWidth = Settings::Width;
	Params.BackBufferHeight = Settings::Height;
	Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	Params.EnableAutoDepthStencil = TRUE;
	Params.AutoDepthStencilFormat = D3DFMT_D16;
	Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	Params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	if (FAILED(DirectX9.IDirect3D9->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Overlay.Hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &Params, 0, &DirectX9.pDevice))) {
		DirectX9.IDirect3D9->Release();
		return false;
	}

	ImGui::CreateContext();
	Spoof(ImGui_ImplWin32_Init)(Overlay.Hwnd);
	Spoof(ImGui_ImplDX9_Init)(DirectX9.pDevice);
	DirectX9.IDirect3D9->Release();
	return true;
}



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	if (Spoof(ImGui_ImplWin32_WndProcHandler)(hWnd, Message, wParam, lParam))
		return true;

	switch (Message) {
	case WM_DESTROY:
		if (DirectX9.pDevice != NULL) {
			DirectX9.pDevice->EndScene();
			DirectX9.pDevice->Release();
		}
		if (DirectX9.IDirect3D9 != NULL) {
			DirectX9.IDirect3D9->Release();
		}
		Spoof(PostQuitMessage)(0);
		exit(4);
		break;
	case WM_SIZE:
		if (DirectX9.pDevice != NULL && wParam != SIZE_MINIMIZED) {
			Spoof(ImGui_ImplDX9_InvalidateDeviceObjects)();
			DirectX9.pParameters.BackBufferWidth = LOWORD(lParam);
			DirectX9.pParameters.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = DirectX9.pDevice->Reset(&DirectX9.pParameters);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			Spoof(ImGui_ImplDX9_CreateDeviceObjects)();
		}
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

void SetupOverlay() {
	SPOOF_FUNC;

	Overlay.WindowClass = {
		sizeof(WNDCLASSEX), 0, WinProc, 0, 0, nullptr, LoadIcon(nullptr, IDI_APPLICATION), LoadCursor(nullptr, IDC_ARROW), nullptr, nullptr, (LPCWSTR)Overlay.Name, LoadIcon(nullptr, IDI_APPLICATION)
	};

	Spoof(RegisterClassEx)(&Overlay.WindowClass);
	if (Process.Hwnd) {
		static RECT TempRect = { NULL };
		static POINT TempPoint;
		Spoof(GetClientRect)(Process.Hwnd, &TempRect);
		Spoof(ClientToScreen)(Process.Hwnd, &TempPoint);
		TempRect.left = TempPoint.x;
		TempRect.top = TempPoint.y;
		Settings::Width = TempRect.right;
		Settings::Height = TempRect.bottom;
	}

	Overlay.Hwnd = CreateWindowEx(NULL, (LPCWSTR)Overlay.Name, (LPCWSTR)Overlay.Name, WS_POPUP | WS_VISIBLE, 0, 0, Settings::Width, Settings::Height, NULL, NULL, 0, NULL);
	Spoof(DwmExtendFrameIntoClientArea)(Overlay.Hwnd, &DirectX9.Margin);
	Spoof(SetWindowLongW)(Overlay.Hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_TOOLWINDOW);
	Spoof(SetWindowPos)(Overlay.Hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	Spoof(UpdateWindow)(Overlay.Hwnd);
}