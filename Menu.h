
static const char* KeyBind[] =
{
	"LMouse",
	"RMouse",
	"Shift",
	"Caps",
	"MButton1",
	"MButton2",
};

DWORD KEY[] = { VK_LBUTTON, VK_RBUTTON, VK_SHIFT, VK_CAPITAL, 0x05, 0x06 };
bool GetAimKey()
{
	return KeyState(KEY[Settings::Aim::Aimkey]);
}

void TipTool(const char* Tip) {
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
	ImGui::PushFont(FontSecondaryC);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 20);
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.60f), Tip);
	ImGui::PopFont();
}

float MenuColor = Settings::MenuColor;
std::set<std::string> displayedNames;
std::vector<std::string> HeroNameVec;
std::vector<std::string> SelectedHero;

float FontValue2 = 0.0f;
float FontValue = 0.0f;
ImVec2 LogoWindowPos{};
ImVec2 LogoWindowSize{};
void Menu() {

	if (Settings::ControlPanel) {
		if (ImGui::Begin(XorCrypt("Control Panel"), nullptr, ImGuiWindowFlags_NoCollapse)) {
			ImDrawList* DrawList = ImGui::GetWindowDrawList();
			ImVec2 WindowPos = ImGui::GetWindowPos();
			ImVec2 WindowSize = ImGui::GetWindowSize();
			ImGui::SetWindowSize({ 285, 300 });

			int lineNumber = 1;
			for (const std::string& playerName : HeroNameVec) {
				if (displayedNames.find(playerName) != displayedNames.end()) {
					continue;
				}

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(-1, -1));
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

				ImGui::TextColored(ImVec4(1.00f, 1.00f, 1.00f, 1.0f), XorCrypt("%d)"), lineNumber);
				ImGui::SameLine();

				ImGui::TextColored(ImVec4(1.00f, 1.00f, 1.00f, 1.0f), XorCrypt("%s  "), playerName.c_str());
				ImGui::SameLine();

				ImGui::SetCursorPosX(WindowSize.x - 95);
				if (ImGui::Button((XorCrypt("Fix BoneID##") + playerName).c_str(), ImVec2(85, 23))) {
					SelectedHero.push_back(playerName);
					displayedNames.insert(playerName);
				}

				ImVec2 buttonMin = ImGui::GetItemRectMin();
				ImVec2 buttonMax = ImGui::GetItemRectMax();
				ImGui::GetWindowDrawList()->AddRect(buttonMin, buttonMax, IM_COL32(255, 255, 255, 255));

				ImGui::PopStyleVar();
				ImGui::Spacing();

				lineNumber++;
			}
			ImGui::End();
		}
	}

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 255.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.0f));
	if (ImGui::Begin(XorCrypt("Main"), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)) {
		ImGui::SetWindowSize({ 385, 545 });
		LogoWindowPos = ImGui::GetWindowPos();
		LogoWindowSize = ImGui::GetWindowSize();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));	

		static Texture image(DirectX9.pDevice);
		image.LoadTextureFromFile(ImagePath1.c_str());
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
		ImGui::Image(image.GetTexture(), ImVec2(385, 100), ImVec2(0, 0), ImVec2(1.0f, 1.0f), ImVec4(1.0, 1.0, 1.0, 1.0));
		

		if (ImGui::Button(XorCrypt("Aimbot"), ImVec2(125, 30))) {
			Settings::Tab = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button(XorCrypt("Visual"), ImVec2(125, 30))) {
			Settings::Tab = 1;
		}
		ImGui::SameLine();
		if (ImGui::Button(XorCrypt("Misc"), ImVec2(125, 30))) {
			Settings::Tab = 2;
		}


		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.95f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.0f));
		ImGui::BeginChild(XorCrypt("Background"), ImVec2(375, 365), true); {

			ImVec2 WindowPos = ImGui::GetWindowPos();
			ImVec2 WindowSize = ImGui::GetWindowSize();
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			static const int numParticles = 64;
			static ImVec2 particlePositions[numParticles];
			static ImVec2 particleDistance;
			static ImVec2 particleVelocities[numParticles];
			static bool initialized = false;
			if (!initialized)
			{
				for (int i = 0; i < numParticles; ++i)
				{
					particlePositions[i] = ImVec2(
						ImGui::GetWindowPos().x + ImGui::GetWindowSize().x * static_cast<float>(rand()) / RAND_MAX,
						ImGui::GetWindowPos().y + ImGui::GetWindowSize().y * static_cast<float>(rand()) / RAND_MAX
					);

					particleVelocities[i] = ImVec2(
						static_cast<float>((rand() % 11) - 5),
						static_cast<float>((rand() % 11) - 5)
					);

				}

				initialized = true;
			}

			ImVec2 cursorPos = ImGui::GetIO().MousePos;
			for (int i = 0; i < numParticles; ++i)
			{
				for (int j = i + 1; j < numParticles; ++j)
				{
					float distance = std::hypotf(particlePositions[j].x - particlePositions[i].x, particlePositions[j].y - particlePositions[i].y);
					float opacity = 0.2f - (distance / 20.0f);

					if (opacity > 0.0f)
					{
						ImU32 lineColor = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, opacity));
						drawList->AddLine(particlePositions[i], particlePositions[j], lineColor);
					}
				}

				float distanceToCursor = std::hypotf(cursorPos.x - particlePositions[i].x, cursorPos.y - particlePositions[i].y);
				float opacityToCursor = 1.0f - (distanceToCursor / 35.0f);
				if (opacityToCursor > 0.0f)
				{
					ImU32 lineColorToCursor = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, opacityToCursor));
					drawList->AddLine(cursorPos, particlePositions[i], lineColorToCursor);
				}
			}

			float deltaTime = ImGui::GetIO().DeltaTime;
			for (int i = 0; i < numParticles; ++i)
			{
				particlePositions[i].x += particleVelocities[i].x * deltaTime;
				particlePositions[i].y += particleVelocities[i].y * deltaTime;

				if (particlePositions[i].x < ImGui::GetWindowPos().x)
					particlePositions[i].x = ImGui::GetWindowPos().x + ImGui::GetWindowSize().x;
				else if (particlePositions[i].x > ImGui::GetWindowPos().x + ImGui::GetWindowSize().x)
					particlePositions[i].x = ImGui::GetWindowPos().x;

				if (particlePositions[i].y < ImGui::GetWindowPos().y)
					particlePositions[i].y = ImGui::GetWindowPos().y + ImGui::GetWindowSize().y;
				else if (particlePositions[i].y > ImGui::GetWindowPos().y + ImGui::GetWindowSize().y)
					particlePositions[i].y = ImGui::GetWindowPos().y;

				ImU32 particleColour = ImGui::ColorConvertFloat4ToU32(Settings::MenuColor);
				drawList->AddCircleFilled(particlePositions[i], 1.5f, particleColour, 1.0f);
			}

			if (Settings::Tab == 0) {
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
				ImGui::Checkbox(XorCrypt("Aimbot"), &Settings::Aim::Aimbot);
				TipTool(XorCrypt("Enables Aimbot / MouseAim."));

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
				ImGui::Checkbox(XorCrypt("TargetLine"), &Settings::Aim::TargetLine);
				TipTool(XorCrypt("Locks a red line on the targeted player."));

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
				ImGui::Checkbox(XorCrypt("Prediction"), &Settings::Aim::Prediction);
				TipTool(XorCrypt("Adds prediction to the aimbot"));

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
				ImGui::SliderFloatMain(XorCrypt("Y-Smooth"), &Settings::Aim::YSmooth, 2.0f, 20.0f, "%.2f", 0, Settings::MenuColor);
				TipTool(XorCrypt("Adjustment X-Smoothness"));

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
				ImGui::SliderFloatMain(XorCrypt("X-Smooth"), &Settings::Aim::XSmooth, 2.0f, 20.0f, "%.2f", 0, Settings::MenuColor);
				TipTool(XorCrypt("Adjustment Y-Smoothness"));

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
				ImGui::SliderFloatMain(XorCrypt("Distance"), &Settings::Aim::AimDistance, 10.0f, 500.0f, "%.2f", 0, Settings::MenuColor);
				TipTool(XorCrypt("Aim distance"));

				if (Settings::Aim::Prediction) {
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
					ImGui::Checkbox(XorCrypt("Prediction.line"), &Settings::Aim::PredictionLine);
					TipTool(XorCrypt("See Prediction velocity with a Line"));

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
					ImGui::Checkbox(XorCrypt("Prediction.circle"), &Settings::Aim::PredictionCircle);
					TipTool(XorCrypt("See Prediction velocity with a Circle"));

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
					ImGui::SliderFloatMain(XorCrypt("ProjectionSpeed"), &Settings::Aim::ProjectileSpeed, 5.0f, 15.0f, "%.2f", 0, Settings::MenuColor);
					TipTool(XorCrypt("Adjust the ProjectionSpeed"));
				}

				if (Settings::Aim::TargetLine) {
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
					ImGui::SliderFloatMain(XorCrypt("TargetLine.Size"), &Settings::Aim::TargetSize, 1.0f, 5.0f, "%.2f", 0, Settings::MenuColor);
					TipTool(XorCrypt("Adjustment size of TargetLine"));
				}
			}
			else if (Settings::Tab == 1) {
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
				ImGui::SliderFloatMain(XorCrypt("RenderDistance"), &Settings::Visuals::RenderDistance, 50.0f, 500.0f, "%.2f", 0, Settings::MenuColor);
				TipTool(XorCrypt("Visual Distance"));

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
				ImGui::Checkbox(XorCrypt("ControlPanel"), &Settings::ControlPanel);
				TipTool(XorCrypt("Displays HeroNames & Bone Options"));

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
				ImGui::Checkbox(XorCrypt("Box"), &Settings::Visuals::Box);
				TipTool(XorCrypt("Displays boxes around enemies"));

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
				ImGui::Checkbox(XorCrypt("SnapLines"), &Settings::Visuals::SnapLines);
				TipTool(XorCrypt("Displays Lines towards enemies"));

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
				ImGui::Checkbox(XorCrypt("Distance"), &Settings::Visuals::Distance);
				TipTool(XorCrypt("Show the players Distance"));

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
				ImGui::Checkbox(XorCrypt("Health"), &Settings::Visuals::Health);
				TipTool(XorCrypt("Show the players Health"));

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
				ImGui::Checkbox(XorCrypt("Names"), &Settings::Visuals::HeroNames);
				TipTool(XorCrypt("Display the Players HeroNames"));

				if (Settings::Visuals::Box) {
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
					ImGui::Checkbox(XorCrypt("BoxFilled"), &Settings::Visuals::BoxFilled);
					TipTool(XorCrypt("Fills the displayed box."));

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
					ImGui::SliderFloatMain(XorCrypt("Box.Width"), &Settings::Visuals::BoxWidth, 2.0f, 10.0f, "%.2f", 0, Settings::MenuColor);
					TipTool(XorCrypt("Width of boxes."));

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
					ImGui::SliderFloatMain(XorCrypt("Box.Thickness"), &Settings::Visuals::BoxThickness, 1.0f, 5.0f, "%.2f", 0, Settings::MenuColor);
					TipTool(XorCrypt("Thickness of boxes."));

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
					ImGui::SliderFloatMain(XorCrypt("Box.Clearness"), &Settings::Visuals::BoxClearness, 0.01f, 1.0f, "%.2f", 0, Settings::MenuColor);
					TipTool(XorCrypt("Clearness of boxes."));
				}
			}
			else if (Settings::Tab == 2) {

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
				ImGui::SliderFloatMain(XorCrypt("ThreadSpeed"), &Settings::Misc::LoopThread, 200.0f, 3500.0f, "%.2f", 0, Settings::MenuColor);
				TipTool(XorCrypt("Thread reading speed"));

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
				ImGui::Checkbox(XorCrypt("CrossHair"), &Settings::Misc::Crosshair);
				TipTool(XorCrypt("Visible Crosshair placed center-screen."));

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
				ImGui::Checkbox(XorCrypt("FovCircle"), &Settings::Misc::FovCircle);
				TipTool(XorCrypt("Aimbot enemies inside the circle"));
				if (Settings::Misc::FovCircle) {
					Settings::Misc::FovCircle = true;
					Settings::Misc::FovSquare = false;
				}

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
				ImGui::Checkbox(XorCrypt("FovSquare"), &Settings::Misc::FovSquare);
				TipTool(XorCrypt("Aimbot enemies inside the Square"));
				if (Settings::Misc::FovSquare) {
					Settings::Misc::FovCircle = false;
					Settings::Misc::FovSquare = true;
				}

				if (Settings::Misc::Crosshair) {
					ImGui::SliderFloatMain(XorCrypt("Crosshair.length"), &Settings::Misc::Length, 1.0f, 10.0f, "%.2f", 0, Settings::MenuColor);
					TipTool(XorCrypt("Length of the Crosshair."));
				}

				if (Settings::Misc::FovCircle || Settings::Misc::FovSquare) {
					ImGui::SliderFloatMain(XorCrypt("FOV.size"), &Settings::Misc::Fovsize, 1.0f, 500.0f, "%.2f", 0, Settings::MenuColor);
					TipTool(XorCrypt("Increase the FOV size."));

					ImGui::SliderFloatMain(XorCrypt("FOV.clear"), &Settings::Misc::Transparent, 1.0f, 250.0f, "%.2f", 0, Settings::MenuColor);
					TipTool(XorCrypt("Clearness of the FOV."));
				}

				if (Settings::Misc::FovCircle) {
					ImGui::SliderFloatMain(XorCrypt("FOV.detail"), &Settings::Misc::FovDetail, 1.0f, 50.0f, "%.2f", 0, Settings::MenuColor);
					TipTool(XorCrypt("Detail of the FOV-Circle."));
				}

				if (Settings::Misc::FovCircle) {
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
					ImGui::Checkbox(XorCrypt("CircleFilled"), &Settings::Misc::FovCircleFull);
					TipTool(XorCrypt("Display a fill FOV circle."));
				}

				if (Settings::Misc::FovSquare) {
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
					ImGui::Checkbox(XorCrypt("SquareFilled"), &Settings::Misc::FovSquareFull);
					TipTool(XorCrypt("Display a fill FOV Square."));
				}

				//LINE
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6);
				ImVec2 pos = ImGui::GetCursorScreenPos();
				float length = ImGui::GetContentRegionAvail().x - 1.5f;
				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				draw_list->AddLine(pos, ImVec2(pos.x + length, pos.y), ImColor(255, 255, 255, 255), 1.0f);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6);

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(3.5, 3.5));
				ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(3.5, 3.5));
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
				ImGui::ColorEdit4(XorCrypt("MenuColor"), (float*)&Settings::MenuColor, ImGuiColorEditFlags_NoInputs);
				ImGui::SameLine();
				ImGui::ColorEdit4(XorCrypt("Boxes"), (float*)&Settings::BoxColor, ImGuiColorEditFlags_NoInputs);
				ImGui::SameLine();
				ImGui::ColorEdit4(XorCrypt("Name"), (float*)&Settings::NameColor, ImGuiColorEditFlags_NoInputs);
				ImGui::SameLine();
				ImGui::ColorEdit4(XorCrypt("Distance"), (float*)&Settings::DistanceColor, ImGuiColorEditFlags_NoInputs);

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8.0f);
				ImGui::ColorEdit4(XorCrypt("SnapLines"), (float*)&Settings::SnapLinesColor, ImGuiColorEditFlags_NoInputs);
				ImGui::SameLine();
				ImGui::ColorEdit4(XorCrypt("Target"), (float*)&Settings::TargetColor, ImGuiColorEditFlags_NoInputs);
				ImGui::PopStyleVar(3);
			}

		}

		ImGui::PopStyleColor(2);
	}

	ImGui::End();
}
