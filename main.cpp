#include "main.h"


void Render() {

	SPOOF_FUNC;
	static bool InsertPressed = false;
	if (KeyState(VK_INSERT) & 0x8000) {
		if (!InsertPressed) {
			Settings::ShowMenu = !Settings::ShowMenu;
			InsertPressed = true;
		}
	}
	else {
		InsertPressed = false;
	}

	if (KeyState(VK_END) & 1) {
		LI_FN(exit)(2);
	}

	Spoof(ImGui_ImplDX9_NewFrame)();
	Spoof(ImGui_ImplWin32_NewFrame)();
	ImGui::NewFrame();
	ImGui::GetIO().MouseDrawCursor = Settings::ShowMenu;

	if (ImGui::Begin(XorCrypt("Logo"), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs)) {
		ImGui::SetWindowPos({ 15, 20 });
		ImGui::SetWindowSize({ 195, 45 });
		ImGui::PushFont(ThickFontC);
		ImGui::GetForegroundDrawList()->AddText(ImVec2(30, 28), ImColor(255, 255, 255, 255), XorCrypt("AIMXRIVALS"));
		ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(20, 1060), ImVec2(300, Settings::Height), ImColor(0, 0, 0, 255));

		float fps = ImGui::GetIO().Framerate;
		char fpsText[32];
		snprintf(fpsText, sizeof(fpsText), XorCrypt("FPS: %.0f"), fps);
		ImGui::GetForegroundDrawList()->AddText(ImVec2(30, 42), ColorMain, fpsText);
		ImGui::PopFont();
		ImGui::End();
	}


	if (Settings::ShowMenu == true) {

		Menu();
		if (Settings::Tab == 2) {
			ScreenTools();
		}
		SetWindowLongW(Overlay.Hwnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
	}
	else {
		ScreenTools();
		SetWindowLongW(Overlay.Hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	}

	Visuals();
	ImGui::EndFrame();
	DirectX9.pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (DirectX9.pDevice->BeginScene() >= 0) {
		ImGui::Render();
		Spoof(ImGui_ImplDX9_RenderDrawData)(ImGui::GetDrawData());
		DirectX9.pDevice->EndScene();
	}

	HRESULT result = DirectX9.pDevice->Present(NULL, NULL, NULL, NULL);
	if (result == D3DERR_DEVICELOST && DirectX9.pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
		Spoof(ImGui_ImplDX9_InvalidateDeviceObjects)();
		DirectX9.pDevice->Reset(&DirectX9.pParameters);
		Spoof(ImGui_ImplDX9_CreateDeviceObjects)();
	}
}

void MainLoop() {
	static RECT OldRect;
	ZeroMemory(&DirectX9.Message, sizeof(MSG));

	ImGuiIO& io = ImGui::GetIO();
	static const ImWchar IconRange[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig FontConfig2;
	ImFontConfig IconConfig;
	ImFontConfig BigFontFT;
	IconConfig.MergeMode = true;
	IconConfig.PixelSnapH = true;
	FontSecondaryB = io.Fonts->AddFontFromMemoryTTF((void*)SecondaryFont, sizeof(SecondaryFont), 20, &FontConfig2);
	FontSecondaryA = io.Fonts->AddFontFromMemoryTTF((void*)SecondaryFont, sizeof(SecondaryFont), 17, &FontConfig2);
	FontSecondaryC = io.Fonts->AddFontFromMemoryTTF((void*)SecondaryFont, sizeof(SecondaryFont), 15.5, &FontConfig2);
	ThickFontA = io.Fonts->AddFontFromMemoryTTF((void*)ThickFont, sizeof(ThickFont), 24, &FontConfig2);
	ThickFontB = io.Fonts->AddFontFromMemoryTTF((void*)ThickFont, sizeof(ThickFont), 18, &FontConfig2);
	ThickFontC = io.Fonts->AddFontFromMemoryTTF((void*)ThickFont, sizeof(ThickFont), 16, &FontConfig2);
	FontMain = io.Fonts->AddFontFromMemoryTTF((void*)MainFont, sizeof(MainFont), 18, &IconConfig, IconRange);
	if (FontMain == NULL || FontSecondaryA == NULL)
	{
		MessageBoxA(0, XorCrypt("Failed to Load Fonts!"), XorCrypt("Error"), MB_ICONERROR);
		LI_FN(Sleep)(3000);
		LI_FN(exit)(2);
	}

	while (DirectX9.Message.message != WM_QUIT) {

		Style();
		ColorMain = Settings::MenuColor;
		ColorMainVec = Settings::MenuColor;

		if (PeekMessage(&DirectX9.Message, Overlay.Hwnd, 0, 0, PM_REMOVE)) {
			Spoof(TranslateMessage)(&DirectX9.Message);
			DispatchMessage(&DirectX9.Message);
		}

		HWND TempHwnd = Spoof(GetWindow)(Process.Hwnd, GW_HWNDPREV);
		Spoof(SetWindowPos)(Overlay.Hwnd, TempHwnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		RECT TempRect;
		POINT TempPoint;
		ZeroMemory(&TempRect, sizeof(RECT));
		ZeroMemory(&TempPoint, sizeof(POINT));

		Spoof(GetClientRect)(Process.Hwnd, &TempRect);
		Spoof(ClientToScreen)(Process.Hwnd, &TempPoint);
		TempRect.left = TempPoint.x;
		TempRect.top = TempPoint.y;
		ImGuiIO& io = ImGui::GetIO();
		if (TempRect.left != OldRect.left || TempRect.right != OldRect.right || TempRect.top != OldRect.top || TempRect.bottom != OldRect.bottom) {
			OldRect = TempRect;
			Settings::Width = TempRect.right;
			Settings::Height = TempRect.bottom;
			DirectX9.pParameters.BackBufferWidth = Settings::Width;
			DirectX9.pParameters.BackBufferHeight = Settings::Height;
			Spoof(SetWindowPos)(Overlay.Hwnd, (HWND)0, TempPoint.x, TempPoint.y, Settings::Width, Settings::Height, SWP_NOREDRAW);
			DirectX9.pDevice->Reset(&DirectX9.pParameters);
		}

		Render();
	}

	Spoof(ImGui_ImplDX9_Shutdown)();
	Spoof(ImGui_ImplWin32_Shutdown)();
	ImGui::DestroyContext();
	if (DirectX9.pDevice != NULL) {
		DirectX9.pDevice->EndScene();
		DirectX9.pDevice->Release();
	}
	if (DirectX9.IDirect3D9 != NULL) {
		DirectX9.IDirect3D9->Release();
	}
	Spoof(DestroyWindow)(Overlay.Hwnd);
	UnregisterClass(Overlay.WindowClass.lpszClassName, Overlay.WindowClass.hInstance);
}


bool ImageBytes() {

	if (!std::filesystem::exists(ResourcePath)) {
		std::filesystem::create_directory(ResourcePath); 
	}

	if (std::filesystem::exists(ResourcePath)) { 
		BytesToMemory(ImagePath1, reinterpret_cast<const char*>(Interptr), sizeof(Interptr));
	}

	return true;
}


int main()
{
	SPOOF_FUNC;
	LI_FN(SetConsoleTitleA)(XorCrypt(""));

	if (!aimxpert::find_driver()) {
		MessageBoxA(0, XorCrypt("Driver isn't loaded?"), XorCrypt("Error"), MB_ICONERROR);
		Sleep(3000);
		exit(2);
	}

	if (!ImageBytes()) {
		MessageBoxA(0, XorCrypt("Image Bytes Failed."), XorCrypt("Error"), MB_ICONERROR);
		Sleep(3000);
		exit(2);
	}

	if (!StoreMouseEvent()) {
		MessageBoxA(0, XorCrypt("MouseEvent Pointer Failed."), XorCrypt("Error"), MB_ICONERROR);
		LI_FN(Sleep)(3000);
		LI_FN(exit)(2);
	}

	if (!StoreAsyncKeyState()) {
		MessageBoxA(0, XorCrypt("GetAsyncKeyState Pointer Failed."), XorCrypt("Error"), MB_ICONERROR);
		LI_FN(Sleep)(3000);
		LI_FN(exit)(2);
	}

	Overlay.Name = RandomString(10).c_str();
	if (Overlay.Name == NULL) {
		MessageBoxA(0, XorCrypt("Failed to create Overlay."), XorCrypt("Error"), MB_ICONERROR);
		LI_FN(Sleep)(3000);
		LI_FN(exit)(2);
	}



	printf(XorCrypt("\n [+] Waiting for Marvel-Win64-Shipping.exe\n"));
	HWND GameTarget = { 0 };
	while (GameTarget == NULL) {
		GameTarget = FindWindowA(NULL, XorCrypt("Marvel Rivals  "));
	}


	if (aimxpert::process_id == NULL) {
		SetupOverlay();
		DirectXInit();
		aimxpert::process_id = aimxpert::find_process((XorCryptW(L"Marvel-Win64-Shipping.exe")));
		//::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	}

	aimxpert::CR3();

	OFF_REGION = aimxpert::find_image();
	if (!OFF_REGION) {
		MessageBoxA(0, XorCrypt("Failed BaseAddress"), XorCrypt("Failed!"), MB_ICONERROR);
		LI_FN(Sleep)(3000);
		LI_FN(exit)(2);
	}

	if (OFF_REGION) {
		_beginthread(ActorThread, 0, nullptr);
		while (TRUE) {
			MainLoop();
		}
	}
}
