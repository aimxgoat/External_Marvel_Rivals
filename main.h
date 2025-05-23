
#include "Include.h"


#undef max
#undef min
//Aimbot made to look more human like (Prevent obvious aimbot)
void aimbot(float x, float y, float distance)
{
	float ScreenCenterX = Settings::Width / 2;
	float ScreenCenterY = Settings::Height / 2;
	float CircleRadius = 3.25f - (distance / 120);

	static bool initialized = false;
	if (!initialized) {
		srand(static_cast<unsigned>(time(nullptr)));
		initialized = true;
	}

	static float targetOffsetX = 0.0f;
	static float targetOffsetY = 0.0f;
	static float randomOffsetX = 0.0f;
	static float randomOffsetY = 0.0f;

	static auto lastRandomUpdate = std::chrono::steady_clock::now();
	static int RandomizeTimer = 5 + (rand() % (20 - 5 + 1));
	if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastRandomUpdate).count() > RandomizeTimer){
		targetOffsetX = (rand() % 201 - 100) / 100.0f * CircleRadius; 
		targetOffsetY = (rand() % 201 - 100) / 100.0f * CircleRadius;

		randomOffsetX += (targetOffsetX - randomOffsetX) * 1.25f;
		randomOffsetY += (targetOffsetY - randomOffsetY) * 1.25f;

		float TargetX = ((x - ScreenCenterX) / Settings::Aim::XSmooth) + randomOffsetX;
		float TargetY = ((y - ScreenCenterY) / Settings::Aim::YSmooth) + randomOffsetY / 1.25;

		mouse_event(TargetX, TargetY);
		lastRandomUpdate = std::chrono::steady_clock::now();
	}
}


boolean Area(double centerX, double centerY, double radius, double x, double y)
{
	return x >= centerX - radius && x <= centerX + radius &&
		y >= centerY - radius && y <= centerY + radius;
}

namespace Offsets {
	const uintptr_t GNAME = 0x0DC80A40;
	const uintptr_t UWorld = 0x0DEE7D60;
	const uintptr_t GameInstance = 0x270;
	const uintptr_t LocalPlayers = 0x40;
	const uintptr_t PlayerController = 0x38;
	const uintptr_t LocalPawn = 0x550;
	const uintptr_t GameState = 0x210;
	const uintptr_t PlayerArray = 0x4a8;
	const uintptr_t PawnPrivate = 0x518;
	const uintptr_t ChildActorComponent = 0x530;
	const uintptr_t ChildActor = 0x478;
	const uintptr_t Mesh = 0x490;
	const uintptr_t RootComponent = 0x2b8;
	const uintptr_t ComponentVelocity = 0x1d0;
	const uintptr_t RelativeLocation = 0x188;
	const uintptr_t PlayerState = 0x4c8;
	const uintptr_t TeamID = 0x5C0;
	const uintptr_t IsAlive = 0x5D0;
	const uintptr_t SelectedHeroID = 0x628;
	const uintptr_t UReactivePropertyComponent = 0x12f8;
	const uintptr_t CachedAttributeSet = 0x1820;
	const uintptr_t Health = 0x40;

	// Array
	const uintptr_t BoneArray = 0x998;
	const uintptr_t ComponentToWorld = 0x2E0;

	// Camera
	const uintptr_t APlayerCameraManager = 0x560;
	const uintptr_t CameraCache = 0x1a50;

	const uintptr_t BoundsScale = 0x59c; // BoundScale // https://dumpspace.spuckwaffel.com/Ga...er=BoundsScale
	const uintptr_t LAST_SUMBIT_TIME = BoundsScale + 0x4; // BoundScale + 0x4
	const uintptr_t LAST_SUMBIT_TIME_ON_SCREEN = BoundsScale + 0x8; // BoundScale + 0x8
}

namespace ActorData {
	bool LockedBone;
	int BoneID;
	float distance{};
	float ClosestDistance;
	uintptr_t ClosestMesh;
	uintptr_t ClosestPawn;
	uintptr_t PlayerState_Target;
	std::string HeroName = "";
	float HeroProjectile = 0.0f;
}
typedef struct Actors {
	uint64_t mesh;
	uint64_t PlayerActor;
	uint64_t PlayerState;
	bool IsPlayerAlive;
}Actors;
std::vector<Actors> ActorList;

struct PointerData {
	uintptr_t Uworld,
		Gameinstance,
		LocalPawn,
		LocalPlayers,
		PlayerController,
		PlayerState,
		APlayerCamera,
		GameState,
		PlayerCount,
		PlayerArray,
		RootComponent,
		LocalTeamID;


	FVector RelativeLocation;
} Data;

FVector GetBone(DWORD_PTR mesh, int id)
{
	DWORD_PTR array = aimxpert::read<uintptr_t>(mesh + Offsets::BoneArray); //MeshDeformerInstanceSettings + 0x30 BONEARRAY
	if (!array)
		array = aimxpert::read<uintptr_t>(mesh + Offsets::BoneArray + 0x10);

	FTransform bone = aimxpert::read<FTransform>(array + (id * 0x60));
	FTransform ComponentToWorld = aimxpert::read<FTransform>(mesh + Offsets::ComponentToWorld);
	D3DMATRIX Matrix;
	Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
	return FVector(Matrix._41, Matrix._42, Matrix._43);
}

FVector WorldToScreen(FVector WorldLocation)
{
	FVector Screenlocation = FVector(0, 0, 0);
	auto CameraInfo = aimxpert::read<FMinimalViewInfo>(Data.APlayerCamera + Offsets::CameraCache + 0x10);

	FVector CameraLocation = CameraInfo.Location;
	FVector CameraRotation = CameraInfo.Rotation;

	D3DMATRIX tempMatrix = Matrix(CameraRotation, FVector(0, 0, 0));
	FVector vAxisX = FVector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]),
		vAxisY = FVector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]),
		vAxisZ = FVector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	FVector vDelta = WorldLocation - CameraLocation;
	FVector vTransformed = FVector(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	float FovAngle = CameraInfo.FOV;

	float ScreenCenterX = Settings::Width / 2.0f;
	float ScreenCenterY = Settings::Height / 2.0f;
	Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

	return Screenlocation;
}

uintptr_t LockedActor = NULL;

void __cdecl ActorThread(void*)
{
	while (true)
	{
		ActorList.clear();
		std::vector<Actors> ActorDataList;
		Data.Uworld = aimxpert::read<uintptr_t>(OFF_REGION + Offsets::UWorld);
		if (!Data.Uworld) {
			displayedNames.clear();
			HeroNameVec.clear();
			SelectedHero.clear();
		}

		Data.Gameinstance = aimxpert::read<uintptr_t>(Data.Uworld + Offsets::GameInstance);
		Data.LocalPlayers = aimxpert::read<uintptr_t>(aimxpert::read<uintptr_t>(Data.Gameinstance + Offsets::LocalPlayers));
		Data.PlayerController = aimxpert::read<uintptr_t>(Data.LocalPlayers + Offsets::PlayerController);
		Data.APlayerCamera = aimxpert::read<uintptr_t>(Data.PlayerController + Offsets::APlayerCameraManager);
		Data.LocalPawn = aimxpert::read<uintptr_t>(Data.PlayerController + Offsets::LocalPawn);
		if (Data.LocalPawn)
		{
			Data.RootComponent = aimxpert::read<uintptr_t>(Data.LocalPawn + Offsets::RootComponent);
			Data.RelativeLocation = aimxpert::read<FVector>(Data.RootComponent + Offsets::RelativeLocation);
			Data.PlayerState = aimxpert::read<uintptr_t>(Data.LocalPawn + Offsets::PlayerState);
		}

		Data.GameState = aimxpert::read<uintptr_t>(Data.Uworld + Offsets::GameState);
		Data.PlayerArray = aimxpert::read<uintptr_t>(Data.GameState + Offsets::PlayerArray);
		Data.PlayerCount = aimxpert::read<int>(Data.GameState + (Offsets::PlayerArray + sizeof(uintptr_t)));
		for (int i = 0; i < Data.PlayerCount; i++)
		{
			uintptr_t OtherPlayerState = aimxpert::read<uintptr_t>(Data.PlayerArray + (i * sizeof(uintptr_t)));
			if (!OtherPlayerState) continue;

			uintptr_t PrivatePawn = aimxpert::read<uintptr_t>(OtherPlayerState + Offsets::PawnPrivate);
			if (!PrivatePawn) continue;

			auto ChildActorComponent = aimxpert::read<uintptr_t>(PrivatePawn + Offsets::ChildActorComponent); //APawn->ACharacter
			if (!ChildActorComponent) continue;

			auto ChildActor = aimxpert::read<uintptr_t>(ChildActorComponent + Offsets::ChildActor); //AActor->ChildActor
			if (!ChildActor) continue;

			auto Mesh = aimxpert::read<uintptr_t>(ChildActor + Offsets::Mesh); //ACharacterChildActorBase->Mesh 
			if (!Mesh) continue;

			auto UReactivePropertyComponent = aimxpert::read<uintptr_t>(PrivatePawn + Offsets::UReactivePropertyComponent);
			auto CachedAttributeSet = aimxpert::read<uintptr_t>(UReactivePropertyComponent + Offsets::CachedAttributeSet);
			float PlayerHealth = aimxpert::read<float>(CachedAttributeSet + Offsets::Health + 0xc);
			if (PlayerHealth < 0.1) {
				continue;
			}

	        auto TeamID = aimxpert::read<int>(Data.PlayerState + Offsets::TeamID);
			auto PawnID = aimxpert::read<int>(OtherPlayerState + Offsets::TeamID);
			auto HeroID = aimxpert::read<int>(OtherPlayerState + Offsets::SelectedHeroID);
			if (HeroID == 4016 || HeroID == 4017 || HeroID == 4018) { //Continue the Practice AI Bots
				continue;
			}

			bool Isalive = aimxpert::read<bool>(OtherPlayerState + Offsets::IsAlive); //Check is Actor is Alive
			if (!Isalive) {
				continue;
			}

			if (TeamID == PawnID) continue; //Continue The TeamID
			if (PrivatePawn == Data.LocalPawn) continue;

			Actors ActorStruct{};
			if (Mesh > 0xFFFF) {
				ActorStruct.mesh = Mesh;
				ActorStruct.PlayerActor = PrivatePawn;
				ActorStruct.PlayerState = OtherPlayerState;
				ActorStruct.IsPlayerAlive = Isalive;
				ActorDataList.push_back(ActorStruct);
			}
		}

		ActorList = std::move(ActorDataList);
		std::this_thread::sleep_for(std::chrono::milliseconds((int)Settings::Misc::LoopThread));
	}
}

bool IsVisible(uintptr_t mesh)
{
	float last_sumbit_time = aimxpert::read<float>(mesh + Offsets::LAST_SUMBIT_TIME);
	float last_render_time_on_screen = aimxpert::read<float>(mesh + Offsets::LAST_SUMBIT_TIME_ON_SCREEN);
	return last_render_time_on_screen + 0.15f >= last_sumbit_time;
}

#include <cstdlib>
#include <ctime> 
#include <xutility>

//(ISNT PERFECT THE BONES STILL SOMETIMES GLITCH)
int HeroID(uintptr_t PlayerState_Target) {
	srand(static_cast<unsigned int>(time(0)));

	auto randomize = [](int baseValue) {
		if (std::find(HeroNameVec.begin(), HeroNameVec.end(), ActorData::HeroName) == HeroNameVec.end()) {
			HeroNameVec.push_back(ActorData::HeroName);
		}
		return baseValue + (rand() % 2);
	};

	auto randomize2 = [](int baseValue) {

		return 171 + (rand() % 16);
	};

	auto HeroID = aimxpert::read<int>(PlayerState_Target + Offsets::SelectedHeroID);
	if (HeroID == 1011) { // HulkBanner
		ActorData::HeroName = std::string(XorCrypt("Bruce Banner"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(110);
	}
	else if (HeroID == 1014) { // Punisher
		ActorData::HeroName = std::string(XorCrypt("The Punisher"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(171);
	}
	else if (HeroID == 1015) { // Storm
		ActorData::HeroName = std::string(XorCrypt("Storm"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(151);
	}
	else if (HeroID == 1016) { // Loki
		ActorData::HeroName = std::string(XorCrypt("Loki"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(124);
	}
	else if (HeroID == 1018) { // DoctorStrange
		ActorData::HeroName = std::string(XorCrypt("Doctor Strange"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(106);
	}
	else if (HeroID == 1020) { // Mantis
		ActorData::HeroName = std::string(XorCrypt("Mantis"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(114);
	}
	else if (HeroID == 1021) { // Hawkeye
		ActorData::HeroName = std::string(XorCrypt("Hawkeye"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(172);
	}
	else if (HeroID == 1022) { // CaptainAmerica
		ActorData::HeroName = std::string(XorCrypt("CaptainAmerica"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(146);
	}
	else if (HeroID == 1023) { // RocketRaccoon
		ActorData::HeroName = std::string(XorCrypt("RocketRaccoon"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(245);
	}
	else if (HeroID == 1024) { // Hela
		ActorData::HeroName = std::string(XorCrypt("Hela"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(124);
	}
	else if (HeroID == 1025) { // Dagger
		ActorData::HeroName = std::string(XorCrypt("Dagger"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(109);
	}
	else if (HeroID == 1026) { // BlackPanther
		ActorData::HeroName = std::string(XorCrypt("BlackPanther"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(111);
	}
	else if (HeroID == 1027) { // Groot
		ActorData::HeroName = std::string(XorCrypt("Groot"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(170);
	}
	else if (HeroID == 1029) { // Magik
		ActorData::HeroName = std::string(XorCrypt("Magik"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(128);
	}
	else if (HeroID == 1030) { // MoonKnight
		ActorData::HeroName = std::string(XorCrypt("MoonKnight"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(122);
	}
	else if (HeroID == 1031) { // LunaSnow
		ActorData::HeroName = std::string(XorCrypt("LunaSnow"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(171);
	}
	else if (HeroID == 1032) { // SquirrelGirl
		ActorData::HeroName = std::string(XorCrypt("SquirrelGirl"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(171);
	}
	else if (HeroID == 1033) { // BlackWidow
		ActorData::HeroName = std::string(XorCrypt("BlackWidow"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(171);
	}
	else if (HeroID == 1034) { // IronMan
		ActorData::HeroName = std::string(XorCrypt("IronMan"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(185);
	}
	else if (HeroID == 1035) { // Venom
		ActorData::HeroName = std::string(XorCrypt("Venom"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(109);
	}
	else if (HeroID == 1036) { // SpiderMan
		ActorData::HeroName = std::string(XorCrypt("SpiderMan"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(113);
	}
	else if (HeroID == 1037) { // Magneto
		ActorData::HeroName = std::string(XorCrypt("Magneto"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(131);
	}
	else if (HeroID == 1038) { // ScarletWitch
		ActorData::HeroName = std::string(XorCrypt("ScarletWitch"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(124);
	}
	else if (HeroID == 1039) { // Thor
		ActorData::HeroName = std::string(XorCrypt("Thor"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(211);
	}
	else if (HeroID == 1041) { // WinterSoldier
		ActorData::HeroName = std::string(XorCrypt("WinterSoldier"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(133);
	}
	else if (HeroID == 1042) { // PeniParker
		ActorData::HeroName = std::string(XorCrypt("PeniParker"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(159);
	}
	else if (HeroID == 1043) { // Starlord
		ActorData::HeroName = std::string(XorCrypt("Starlord"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(115);
	}
	else if (HeroID == 1045) { // Namor
		ActorData::HeroName = std::string(XorCrypt("Namor"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(130);
	}
	else if (HeroID == 1046) { // AdamWarlock
		ActorData::HeroName = std::string(XorCrypt("AdamWarlock"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(145);
	}
	else if (HeroID == 1047) { // Jeff
		ActorData::HeroName = std::string(XorCrypt("Jeff"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(136);
	}
	else if (HeroID == 1048) { // Psylocke
		ActorData::HeroName = std::string(XorCrypt("Psylocke"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(140);
	}
	else if (HeroID == 1049) { // Wolverine
		ActorData::HeroName = std::string(XorCrypt("Wolverine"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(206);
	}
	else if (HeroID == 1040) { // Mister Fantastic
		ActorData::HeroName = std::string(XorCrypt("MisterFantastic"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(110);
	}
	else if (HeroID == 1050) { // Invisible Woman
		ActorData::HeroName = std::string(XorCrypt("InvisibleWoman"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(108);
	}
	else if (HeroID == 1052) { // IronFist
		ActorData::HeroName = std::string(XorCrypt("IronFist"));
		if (std::find(SelectedHero.begin(), SelectedHero.end(), ActorData::HeroName) != SelectedHero.end()) {
			return randomize2(171);
		}

		return randomize(117);
	}

	return randomize(100);
}

FVector Prediction(FVector TargetPosition, FVector ComponentVelocity, float player_distance, float Projectile)
{
	float Sensitivity_Manager = Projectile + (player_distance / 180);
	FVector PredictedPosition = TargetPosition;
	PredictedPosition.x += ComponentVelocity.x / Sensitivity_Manager;
	PredictedPosition.y += ComponentVelocity.y / Sensitivity_Manager * 0.25;
	PredictedPosition.z += ComponentVelocity.z / Sensitivity_Manager;
	return PredictedPosition;
}

void Visuals()
{
	ActorData::ClosestDistance = FLT_MAX;
	ActorData::ClosestMesh = NULL;

	for (const Actors& Actor : ActorList)
	{
		uintptr_t Mesh = Actor.mesh;
		if (!Mesh) continue;

		uintptr_t PlayerState = Actor.PlayerState;
		if (!PlayerState) continue;
			
		FVector BottomBone = GetBone(Mesh, 0);
		FVector HeadBone = GetBone(Mesh, HeroID(PlayerState));

		FVector Bottom = WorldToScreen(BottomBone);
		FVector Head = WorldToScreen(HeadBone);

		float BoxHeight = abs(Head.y - Bottom.y);
		float BoxWidth = BoxHeight * 0.65f;

		ActorData::distance = Data.RelativeLocation.Distance(BottomBone);
		if (ActorData::distance < Settings::Visuals::RenderDistance)
		{

			if (Settings::Visuals::Box) {
				float r = Settings::BoxColor.Value.x;
				float g = Settings::BoxColor.Value.y;
				float b = Settings::BoxColor.Value.z;

				DrawBox(Head.x - (BoxWidth / 2), Head.y, BoxWidth, BoxHeight, ImColor(r, g, b, Settings::Visuals::BoxClearness), Settings::Visuals::BoxThickness, Settings::Visuals::BoxWidth);
				if (Settings::Visuals::BoxFilled) {
					float r = Settings::BoxColor.Value.x;
					float g = Settings::BoxColor.Value.y;
					float b = Settings::BoxColor.Value.z;
					FilledRect(Head.x - (BoxWidth / 2), Head.y, BoxWidth, BoxHeight, ImColor(r, g, b, 0.35f));
				}
			}

			if (Settings::Visuals::SnapLines) {
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(Settings::Width / 2, Settings::Height / 2), ImVec2(Bottom.x, Bottom.y), Settings::SnapLinesColor, 1.0f);
			}

			float YFloat = 0.0f;
			float VerticalSpacing = -2.5f;
			if (Settings::Visuals::HeroNames) {
				char Txtbuffer[64];
				sprintf_s(Txtbuffer, XorCrypt("%s"), ActorData::HeroName.c_str());
				ImVec2 textSize = ImGui::CalcTextSize(Txtbuffer);
				ImVec2 textPosition(Bottom.x - textSize.x * 0.5f, Bottom.y + YFloat);
				ImGui::GetForegroundDrawList()->AddText(textPosition, Settings::NameColor, Txtbuffer);
				YFloat += textSize.y + VerticalSpacing;
			}

			if (Settings::Visuals::Distance) {
				char dist[64];
				sprintf_s(dist, XorCrypt("%.fm"), ActorData::distance);
				ImVec2 textSize = ImGui::CalcTextSize(dist);
				ImVec2 textPosition(Bottom.x - textSize.x * 0.5f, Bottom.y + YFloat);
				ImGui::GetForegroundDrawList()->AddText(textPosition, Settings::DistanceColor, dist);
				YFloat += textSize.y + VerticalSpacing;
			}

			if (Settings::Visuals::Health) {
				static std::unordered_map<uintptr_t, float> PlayerMAXHealth;

				auto UReactivePropertyComponent = aimxpert::read<uintptr_t>(Actor.PlayerActor + Offsets::UReactivePropertyComponent);
				auto CachedAttributeSet = aimxpert::read<uintptr_t>(UReactivePropertyComponent + Offsets::CachedAttributeSet);
				float Health = aimxpert::read<float>(CachedAttributeSet + Offsets::Health + 0xc);

				uintptr_t playerID = Actor.PlayerActor;
				if (PlayerMAXHealth.find(playerID) == PlayerMAXHealth.end()) {
					PlayerMAXHealth[playerID] = Health;
				}

				float maxHealth = PlayerMAXHealth[playerID];
				Health = std::clamp(Health, 0.0f, maxHealth);      

				int MaxHPBarHeight = BoxHeight - 2;
				float HealthRatio = Health / maxHealth;
				int HPBoxHeight = static_cast<int>(MaxHPBarHeight * HealthRatio);
				HPBoxHeight = std::max(HPBoxHeight, 2);

				int HPBoxX = Head.x - (BoxWidth / 2) - 10;
				int HPBoxY = Head.y + (BoxHeight - HPBoxHeight);

				if (Health > 0.1) {
					DrawFilledRect(HPBoxX, HPBoxY, 5, HPBoxHeight, ImColor(0, 255, 0));
					DrawRect(HPBoxX, HPBoxY, 5, HPBoxHeight, ImColor(0, 0, 0), 2);
				}
			}
		}

		if (IsVisible(Mesh)) {
			double XDirection = Head.x - Settings::Width / 2;
			double YDirection = Head.y - Settings::Height / 2;
			float dist = sqrtf(XDirection * XDirection + YDirection * YDirection);
			if (!LockedActor && dist <= Settings::Misc::Fovsize && dist < ActorData::ClosestDistance)
			{
				if (Actor.IsPlayerAlive) {
					ActorData::ClosestDistance = dist;
					ActorData::ClosestMesh = Mesh;
					ActorData::ClosestPawn = Actor.PlayerActor;
					ActorData::PlayerState_Target = Actor.PlayerState;
				}

				if (Settings::Aim::TargetLine) {
					if (ActorData::ClosestDistance)
					{
						FVector HeadBone = GetBone(ActorData::ClosestMesh, ActorData::BoneID);
						FVector Target = WorldToScreen(HeadBone);
						ImGui::GetForegroundDrawList()->AddLine(ImVec2(Settings::Width / 2, Settings::Height / 2), ImVec2(Target.x, Target.y), Settings::TargetColor, Settings::Aim::TargetSize);
					}
				}
			}
		}
	}

	if (Settings::Aim::Aimbot) {
		if (GetAimKey())
		{
			if (!LockedActor && ActorData::ClosestMesh)
			{
				LockedActor = ActorData::ClosestMesh;
			}

			FVector Target{};
			FVector TargetBone{};

			if (LockedActor)
			{
				if (!ActorData::LockedBone) {
					ActorData::BoneID = HeroID(ActorData::PlayerState_Target);
					ActorData::LockedBone = true;
				}

				TargetBone = GetBone(LockedActor, ActorData::BoneID);
				float Distance = Data.RelativeLocation.Distance(TargetBone);
				if (Distance < Settings::Aim::AimDistance) {

					if (Settings::Aim::Prediction) {
						uint64_t CurrentActorRootComponent = aimxpert::read<uint64_t>(ActorData::ClosestPawn + Offsets::RootComponent);
						FVector vellocity = aimxpert::read<FVector>(CurrentActorRootComponent + Offsets::ComponentVelocity);
						FVector Predicted = Prediction(TargetBone, vellocity, Distance, Settings::Aim::ProjectileSpeed);
						Target = WorldToScreen(Predicted);

						if (Settings::Aim::PredictionLine) {
							ImGui::GetForegroundDrawList()->AddLine(ImVec2(Settings::Width / 2, Settings::Height / 2), ImVec2(Target.x, Target.y), Settings::TargetColor, 1.0f);
						}
						if (Settings::Aim::PredictionCircle) {
							ImGui::GetForegroundDrawList()->AddCircle(ImVec2(Target.x, Target.y), 2.0f, ImColor(255, 2, 2, 255), 25, 2.5f);
						}
					}
					else {				
						Target = WorldToScreen(TargetBone);
					}

					if (!Settings::ShowMenu) {
						if (Target.x > 0.0f && Target.y > 0.0f) {
							aimbot(Target.x, Target.y, Distance);
						}
					}
				}
			}
		}
		else
		{
			ActorData::LockedBone = false;
			ActorData::PlayerState_Target = NULL;
			LockedActor = NULL;
		}
	}
}

