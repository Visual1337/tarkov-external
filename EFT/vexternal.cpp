#define _CRT_SECURE_NO_WARNINGS
#define GLFW_EXPOSE_NATIVE_WIN32
#include "includes.h"
#include "variable.h"
#include "driver.hpp"
#include "vectors.h"
#include "skcrypt.h"
#include "C:\Users\liam\Downloads\Foropia Loader\xorstr.hpp"
#include "imgui/imgui_internal.h"
#include <list>
#include "../../unburned-master/unburned/ImGui/imconfig.h"



#define GOM 0x17FFD28
#define CameraManage 0x179F500
static bool stamina = true;
bool timescale;
bool watermark = true;
static bool nosway = true;
static bool  aimbot = true;
static bool crosshair = true;
static bool daytime = true;
static bool skeleton = true;
static bool ads = true;
float fovchanger = 75.f;
bool fly;
int WallHackKeyStatus;
static int Bones = 1;
static bool RemoveVitalWeaponParts;
static bool box = true;
static bool fovonoff = true;
static bool is_loot_thru_walls = true;

int WallHackKey = 6;
static float espcolor[4] = { 3.10f, 0.09f, 2.12f, 1.00f };
static float boxcolor[4] = { 3.10f, 0.09f, 2.12f, 1.00f };
static float skeletoncolor[4] = { 3.10f, 0.09f, 2.12f, 1.00f };
static float namecolor[4] = { 3.10f, 0.09f, 2.12f, 1.00f };
static float fovcolor[4] = { 3.10f, 0.09f, 2.12f, 1.00f };
static float crosshaircolor[4] = { 3.10f, 0.09f, 2.12f, 1.00f };
static float accentcolor[4] = { 209 / 255.f, 165 / 255.f, 255 / 255.f, 1.f };
static float menucolor[4] = { 0.f, 0.f, 0.f, 1.f };







static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}



static const char* keyNames[] = {
	"",
	"Left Mouse",
	"Right Mouse",
	"Cancel",
	"Middle Mouse",
	"Mouse 5",
	"Mouse 4",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
};


void WallHackChangeKey(void* blank)
{
	WallHackKeyStatus = 1;
	while (true)
	{
		for (int i = 0; i < 0x87; i++)
		{
			if (GetKeyState(i) & 0x8000)
			{
				WallHackKey = i;
				WallHackKeyStatus = 0;
				return;
			}
		}
	}
}
void WallHackKeyButton(int WallKey, void* changekey, int status, ImVec2 a)
{
	const char* preview_value = NULL;
	if (WallHackKey >= 0 && WallHackKey < IM_ARRAYSIZE(keyNames))
		Items_ArrayGetter(keyNames, WallHackKey, &preview_value);

	std::string aimkeys;
	if (preview_value == NULL)
		aimkeys = ("Select A Key");
	else
		aimkeys = preview_value;
	if (status == 1)
	{
		aimkeys = ("Press A key");
	}
	if (ImGui::Button(aimkeys.c_str(), a))
	{
		if (status == 0)
		{

			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
		}
	}
}



struct color
{
	float r, g, b, a;
	bool rainbow = false;
	float rainbow_speed = 1.f;
	color()
	{
		r = 1.f, g = 1.f, b = 1.f, a = 1.f;
		registered_colors.push_back(this);
	}
	color(float _r, float _g, float _b, float _a)
	{
		this->r = _r;
		this->g = _g;
		this->b = _b;
		this->a = _a;
		registered_colors.push_back(this);
	}
	ImVec4 x()
	{
		return ImVec4(r, g, b, a);
	}
	ImU32 c()
	{
		return IM_COL32(r * 255.f, g * 255.f, b * 255.f, a * 255.f);
	}
	ImU32 c(int m)
	{
		return IM_COL32((r * 255.f) - m, (g * 255.f) - m, (b * 255.f) - m, (a * 255.f) - m);
	}
};

mat4x4_t matrix;
std::mutex our_mutex;
__forceinline Vector2 world_to_screen(const Vector3 position)
{
	const auto w = dot({ matrix._14, matrix._24, matrix._34 }, position) + matrix._44;
	if (w < 1.f)
	{
		return Vector2{ 0.f, 0.f };
	}
	else {
		return
			Vector2{
				(static_cast<float>(g_width) * 0.5f) * (1.f + (dot({ matrix._11, matrix._21, matrix._31 }, position) + matrix._41) / w),
				(static_cast<float>(g_height) * 0.5f) * (1.f - (dot({ matrix._12, matrix._22, matrix._32 }, position) + matrix._42) / w)
		};
	}
}

class COverlay
{
public:
	COverlay();
	bool InitWindows(HWND GameHwnd);
	bool CreateDeviceD3D(HWND hWnd);
	void CleanupDeviceD3D();

	HWND GetLocalHwnd(void)
	{
		return m_hWndOverlay;
	}
	float m_pWidth;
	float m_pHeight;
private:
	static LRESULT CALLBACK	WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND m_hWndOverlay;
};

namespace Config
{
	inline std::string Username = "Yamatos";
}

using namespace std;
inline void hkp() {//handle_key_press
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
	CloseHandle(GetCurrentThread());
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (GetAsyncKeyState(VK_INSERT)) { // just so we dont call this shit a million times
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			g_width = glfwGetVideoMode(monitor)->width;
			g_height = glfwGetVideoMode(monitor)->height; 
			g_overlay_visible = !g_overlay_visible;
			glfwSetWindowAttrib(g_window, GLFW_MOUSE_PASSTHROUGH, !g_overlay_visible);
			if (g_overlay_visible) {
				HWND overlay_window = glfwGetWin32Window(g_window);
				SetForegroundWindow(overlay_window);
			}
			else { SetForegroundWindow(game_window); }
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

	}
}

struct Color
{
	float r, g, b, a;
	Color()
	{
		r = 1.f, g = 1.f, b = 1.f, a = 1.f;
	}
	Color(float _r, float _g, float _b, float _a)
	{
		this->r = _r;
		this->g = _g;
		this->b = _b;
		this->a = _a;
	}
	ImVec4 x()
	{
		return ImVec4(r, g, b, a);
	}
	ImU32 c()
	{
		return IM_COL32(r * 255.f, g * 255.f, b * 255.f, a * 255.f);
	}
	ImU32 c(int m)
	{
		return IM_COL32((r * 255.f) - m, (g * 255.f) - m, (b * 255.f) - m, (a * 255.f) - m);
	}
};

struct transform_access_read_only_t
{
	uint64_t transform_data{};
};

struct transform_data_t
{
	uint64_t transform_array{};
	uint64_t transform_indices{};
};

struct matrix34_t
{
	vec4_t vec0{};
	vec4_t vec1{};
	vec4_t vec2{};
};

enum bones : int
{
	HumanBase = 0,
	HumanPelvis = 14,
	HumanLThigh1 = 15,
	HumanLThigh2 = 16,
	HumanLCalf = 17,
	HumanLFoot = 18,
	HumanLToe = 19,
	HumanRThigh1 = 20,
	HumanRThigh2 = 21,
	HumanRCalf = 22,
	HumanRFoot = 23,
	HumanRToe = 24,
	HumanSpine1 = 29,
	HumanSpine2 = 36,
	HumanSpine3 = 37,
	HumanLCollarbone = 89,
	HumanLUpperarm = 90,
	HumanLForearm1 = 91,
	HumanLForearm2 = 92,
	HumanLForearm3 = 93,
	HumanLPalm = 94,
	HumanRCollarbone = 110,
	HumanRUpperarm = 111,
	HumanRForearm1 = 112,
	HumanRForearm2 = 113,
	HumanRForearm3 = 114,
	HumanRPalm = 115,
	HumanNeck = 132,
	HumanHead = 133
};


inline Vector3 get_transform_position(uintptr_t pTransform)
{
	__m128 result{};

	const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
	const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
	const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

	transform_access_read_only_t transform_access_read_only = driver::read<transform_access_read_only_t>(pTransform + 0x38);
	unsigned int index = driver::read<unsigned int>(pTransform + 0x40);
	transform_data_t transform_data = driver::read<transform_data_t>(transform_access_read_only.transform_data + 0x18);

	if (transform_data.transform_array && transform_data.transform_indices)
	{
		result = driver::read<__m128>(transform_data.transform_array + 0x30 * index);
		int transform_index = driver::read<int>(transform_data.transform_indices + 0x4 * index);
		int safe = 0;
		while (transform_index >= 0 && safe++ < 200)
		{
			matrix34_t matrix = driver::read<matrix34_t>(transform_data.transform_array + 0x30 * transform_index);

			__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x00));	// xxxx
			__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x55));	// yyyy
			__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x8E));	// zwxy
			__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0xDB));	// wzyw
			__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0xAA));	// zzzz
			__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x71));	// yxwy
			__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix.vec2), result);

			result = _mm_add_ps(_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_sub_ps(
							_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
							_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
						_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
					_mm_mul_ps(
						_mm_sub_ps(
							_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
							_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
						_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_sub_ps(
							_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
							_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
						_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
					tmp7)), *(__m128*)(&matrix.vec0));

			transform_index = driver::read<int>(transform_data.transform_indices + 0x4 * transform_index);
		}
	}

	return Vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
}

inline Vector3 get_bone(uintptr_t player, int bone_index)
{
	uintptr_t bone = driver::read<uintptr_t>(player + 0xA8);
	bone = driver::read<uintptr_t>(bone + 0x28);
	bone = driver::read<uintptr_t>(bone + 0x28);
	bone = driver::read<uintptr_t>(bone + 0x10);
	bone = driver::read<uintptr_t>(bone + (0x20 + (bone_index * 0x8)));
	bone = driver::read<uintptr_t>(bone + 0x10);
	return get_transform_position(bone);
}

struct game_object_manager_t
{
	uint64_t lastTaggedObject; //0x0
	uint64_t taggedObjects; //0x8
	uint64_t lastMainCameraTagged; // 0x10
	uint64_t MainCameraTagged; // 0x18
	uint64_t lastActiveObject; //0x20
	uint64_t activeObjects; // 0x28
};

struct base_object_t
{
	uint64_t previousObjectLink; //0x0000
	uint64_t nextObjectLink; //0x0008
	uint64_t object; //0x0010
};
__forceinline uintptr_t get_gameworld(uintptr_t gom)
{
	char name[256];
	uintptr_t class_name_ptr = 0x00;
	auto camera_objects = driver::read<std::array<uint64_t, 2>>(gom + offsetof(game_object_manager_t, lastActiveObject));

	base_object_t activeObject = driver::read<base_object_t>(camera_objects[1]);
	base_object_t lastObject = driver::read<base_object_t>(camera_objects[0]);

	if (activeObject.object)
	{
		while (activeObject.object != 0 && activeObject.object != lastObject.object)
		{
			class_name_ptr = driver::read<uintptr_t>(activeObject.object + 0x60);

			driver::readsize(class_name_ptr, name, sizeof(name));

			if (strcmp(name, e(("GameWorld"))) == 0)
			{
				//std::cout << "Found Game World" << std::endl;
				auto unk1 = driver::read<uintptr_t>(activeObject.object + 0x30);
				auto unk2 = driver::read <uintptr_t>(unk1 + 0x18);
				return driver::read<uintptr_t>(unk2 + 0x28);
			}
		

			activeObject = driver::read<base_object_t>(activeObject.nextObjectLink);
		}
	}

	if (lastObject.object)
	{
		class_name_ptr = driver::read<uint64_t>(lastObject.object + 0x60);
		driver::readsize(class_name_ptr, name, sizeof(name));

		if (strcmp(name, (e("GameWorld"))) == 0)
		{
			auto unk1 = driver::read<uintptr_t>(lastObject.object + 0x30);
			auto unk2 = driver::read<uintptr_t>(unk1 + 0x18);
			return driver::read<uintptr_t>(unk2 + 0x28);
		}
	}
}

__forceinline uintptr_t get_fps_camera(uintptr_t gom)
{

	auto camera_objects = driver::read<std::array<uint64_t, 2>>(gom + offsetof(game_object_manager_t, taggedObjects));

	char name[256];
	uint64_t class_name_ptr = 0x00;

	base_object_t activeObject = driver::read<base_object_t>(camera_objects[1]);
	base_object_t lastObject = driver::read<base_object_t>(camera_objects[0]);

	if (activeObject.object)
	{
		while (activeObject.object != 0 && activeObject.object != lastObject.object)
		{
			class_name_ptr = driver::read<uint64_t>(activeObject.object + 0x60);
			driver::readsize(class_name_ptr, name, sizeof(name));


			if (strcmp(name, ("FPS Camera")) == 0)
			{
				auto unk1 = driver::read<uintptr_t>(activeObject.object + 0x30);
				return  driver::read<uintptr_t>(unk1 + 0x18);
			}

			activeObject = driver::read<base_object_t>(activeObject.nextObjectLink);
		}
	}

	if (lastObject.object)
	{
		class_name_ptr = driver::read<uint64_t>(lastObject.object + 0x60);
		driver::readsize(class_name_ptr + 0x0, &name, sizeof(name));
		if (strcmp(name, ("FPS Camera")) == 0)
		{
			auto unk1 = driver::read<uintptr_t>(activeObject.object + 0x30);
			return driver::read<uintptr_t>(unk1 + 0x18);
		}
	}

	return NULL;
}
struct list_info
{
	int size;
	uintptr_t base;
};

std::vector<std::string> activeNameList;
std::vector<uintptr_t> activePlayerlist;
__forceinline bool is_scav(uintptr_t player)
{ //new registrationdate offset 04 13 22

	uintptr_t profile = driver::read<uintptr_t>(player + offsets::player::profile); // (type: EFT.Profile)
	if (!profile)
		return false;

	uintptr_t account_id = driver::read<uintptr_t>(profile + offsets::player::action);//AccountId (type: System.String) from EFT.Profile
	if (!account_id)
		return false;

	uintptr_t info = driver::read<uintptr_t>(profile + offsets::profile::info);//AccountId (type: System.String) from EFT.Profile
	if (!info)
		return false;

	INT32 regdate = driver::read<INT32>(info + offsets::profile::_info::RegistrationDate);//AccountId (type: System.String) from EFT.Profile >>info >>registrationdate
	if (regdate == 0)
		return true;
}




__forceinline std::string player_name(uintptr_t player)
{ //new registrationdate offset 04 13 22
	uintptr_t profile = driver::read<uintptr_t>(player + offsets::player::profile); 
	if (!profile)
		return "";
	uintptr_t info = driver::read<uintptr_t>(profile + offsets::profile::info);
	if (!info)
		return "";
	uint64_t playerName = driver::read<uint64_t>(info + offsets::profile::_info::Nickname);
	if (playerName)
	{
		try {
			uintptr_t pstring = playerName + 0x14;
			int32_t nameLength = driver::read<int32_t>(playerName + 0x10);
			std::shared_ptr<char[]> buff(new char[nameLength]);
			for (int i = 0; i < nameLength; ++i) {
				buff[i] = driver::read<UCHAR>(pstring + (i * 2));// x2 because its unicode.
			}
			return std::string(&buff[0], &buff[nameLength]);
		}
		catch (const std::exception& exc) {}
	}

}
std::vector<uintptr_t> scavList;
__forceinline std::string get_role(uintptr_t scav)
{
	uintptr_t profile = driver::read<uintptr_t>(scav + offsets::player::profile); // (type: EFT.Profile)
	if (!profile)
		return "";
	const uintptr_t info = driver::read<uintptr_t>(profile + offsets::profile::info);

	if (!info)
		return "";

	const uintptr_t settings = driver::read<uintptr_t>(info + offsets::profile::_info::Settings);

	if (!settings)
		return "";

	const unsigned int role = driver::read<unsigned int>(settings + offsets::profile::_info::_Settings::Role);
	if (crosshair) {
		switch (role)
		{
		case 1:
			return e("Sniper Scav").decrypt();
			break;
		case 2:
			return e("Scav").decrypt();
			break;
		case 8:
			return e("Reshala").decrypt();
			break;
		case 16:
			return e("Follower").decrypt();
			break;
		case 32:
			return e("Reshala Guard").decrypt();
			break;
		case 64:
			return e("Killa").decrypt();
			break;
		case 128:
			return e("Shturman").decrypt();
			break;
		case 256:
			return e("Shturman Guard").decrypt();
			break;
		case 512:
			return e("Raider").decrypt();
			break;
		case 1024:
			return e("Cultist").decrypt();
			break;
		case 2048:
			return e("Gluhar").decrypt();
			break;
		case 4096:
			return e("Gluhar Assault").decrypt();
			break;
		case 8192:
			return e("Gluhar Security").decrypt();
			break;
		case 16384:
			return e("Gluhar Scout").decrypt();
			break;
		case 32768:
			return e("Gluhar Sniper").decrypt();
			break;
		case 65536:
			return e("Sanitar Guard").decrypt();
			break;
		case 131072:
			return e("Sanitar").decrypt();
			break;
		case 524288:
			return e("Raider").decrypt();
			break;
		case 4194304:
			return e("Tagilla").decrypt();
			break;
		case 8388608:
			return e("Follower-Tagilla").decrypt();
			break;
		case 16777216:
			return e("EX-USEC").decrypt();
			break;
		case 33554432:
			return e("Santa").decrypt();
			break;
		case 67108864:
			return e("Knight").decrypt();
			break;
		case 134217728:
			return e("Big Pipe").decrypt();
			break;
		case 268435456:
			return e("Birdeye").decrypt();
			break;
		case 536870912:
			return e("Zryachiy").decrypt();
			break;
		case 1073741824:
			return e("Follower Zryachiy").decrypt();
			break;
		case 2147483648:
			return e("USEC").decrypt();
			break;
		case 4294967296:
			return e("BEAR").decrypt();
			break;
		}
	}
	return e("").decrypt();
}


__forceinline std::vector<uintptr_t> get_player_list(uintptr_t GameWorld)
{
	std::vector<std::string> t_namelist;
	list_info info{};
	uintptr_t list_ptr = driver::read<uintptr_t>(GameWorld + offsets::game_world::registered_players);
	info.size = driver::read<int>(list_ptr + 0x18);
	info.base = driver::read<uintptr_t>(list_ptr + 0x10);
	std::unique_ptr<uintptr_t> entities_buffer(new uintptr_t[info.size * sizeof(uintptr_t)]);

	driver::readsize(info.base + 0x20, entities_buffer.get(), info.size * sizeof(uintptr_t));
	std::vector<uintptr_t> activeList = { entities_buffer.get(), entities_buffer.get() + (uintptr_t)info.size };
	return activeList;
	
}
std::string entities[100];
bool isScav[100];
int activePlayers = 0;
int side[100];
inline int get_side(uintptr_t player)
{
	return driver::read_chain<int>(player, { offsets::player::profile, offsets::profile::info, offsets::profile::_info::Side });
}
__forceinline void updatePlayers() {

	int t_activePlayers = 0;
	for (uintptr_t& player : activePlayerlist)
	{
		
		t_activePlayers++;
		if (player) {
			isScav[t_activePlayers] = is_scav(player);
			side[t_activePlayers] = get_side(player);
			if (isScav[t_activePlayers]) {
				entities[t_activePlayers] = get_role(player);
			}
			else {
				entities[t_activePlayers] = player_name(player);
			}
			
			
		}
		}
	activePlayers = t_activePlayers;
}
__forceinline uintptr_t get_local_player() {

	for (uintptr_t& player : activePlayerlist)
	{

		if (driver::read<int>(player + 0x18))
		{
				return player;
		}
	}
}


ImU32 g_color_white = ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f, 1.f));
ImU32 g_color_black = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 1.f));
__forceinline void DrawCorneredBox(ImDrawList* vList, float X, float Y, float W, float H, const ImU32& color, float thickness)
{
	
	float lineW = (W * 0.3f);
	float lineH = (H * 0.3f);
	//black outlines
	auto col = ImGui::GetColorU32(color);

	//corners
	vList->AddLine(ImVec2(X, Y - thickness / 2), ImVec2(X, Y + lineH), col, thickness);//top left
	vList->AddLine(ImVec2(X - thickness / 2, Y), ImVec2(X + lineW, Y), col, thickness);

	vList->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W + thickness / 2, Y), col, thickness);//top right horizontal
	vList->AddLine(ImVec2(X + W, Y - thickness / 2), ImVec2(X + W, Y + lineH), col, thickness);

	vList->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H + (thickness / 2)), col, thickness);//bot left
	vList->AddLine(ImVec2(X - thickness / 2, Y + H), ImVec2(X + lineW, Y + H), col, thickness);

	vList->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W + thickness / 2, Y + H), col, thickness);//bot right
	vList->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H + (thickness / 2)), col, thickness);

}

__forceinline void cachee() {
	our_mutex.lock();
	cache::CameraManager = driver::read<uintptr_t>(unity_base + CameraManage);
	cache::Game_World = get_gameworld(cache::GameObjectManager);
	cache::FpsCamera = get_fps_camera(cache::GameObjectManager);
	activePlayerlist = get_player_list(cache::Game_World);
	cache::LocalPlayer = get_local_player();
	updatePlayers();
	cache::weapon = driver::read<uintptr_t>(cache::LocalPlayer + 0x1A0);
	cache::shot_effector = driver::read<uintptr_t>(cache::weapon + 0x48);
	cache::motion_effector = driver::read<uintptr_t>(cache::weapon + 0x30);
	cache::walk_effector = driver::read<uintptr_t>(cache::weapon + 0x38);
	cache::breath_effector = driver::read<uintptr_t>(cache::weapon + 0x28);
	cache::force_reactor = driver::read<uintptr_t>(cache::weapon + 0x40);
	//uintptr_t force_effector = driver::read<uintptr_t>(weapon + 0x40);
	 cache::physical = driver::read<uintptr_t>(cache::LocalPlayer + 0x530);
	if (cache::LocalPlayer) {
		cache::body_stamina = driver::read<uintptr_t>(cache::physical + 0x38);
	}
	our_mutex.unlock();
	//driver::write<float>(force_effector + 0x28, 0.f);
}
__forceinline void cache_players() {//handle_key_press
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
	CloseHandle(GetCurrentThread());
	while (true) {
		if (cache::GameObjectManager) {
			cachee();
		}
		std::this_thread::sleep_for(std::chrono::seconds(10));
		

	}
}
std::vector<std::pair<bones, bones>> bone_points{
	{ HumanLCollarbone, HumanLUpperarm },
	{ HumanLCollarbone, HumanRUpperarm },
	{ HumanLUpperarm, HumanLForearm1 },
	{ HumanRUpperarm, HumanRForearm1 },
	{ HumanLForearm1, HumanLForearm3 },
	{ HumanRForearm1, HumanRForearm3 },
	{ HumanLForearm3, HumanLPalm },
	{ HumanRForearm3, HumanRPalm },
	{ HumanNeck, HumanSpine3 },
	{ HumanSpine3, HumanSpine2 },
	{ HumanSpine2, HumanSpine1 },
	{ HumanSpine1, HumanPelvis },
	{ HumanPelvis, HumanLThigh1 },
	{ HumanPelvis, HumanRThigh1 },
	{ HumanLThigh1, HumanLThigh2 },
	{ HumanRThigh1, HumanRThigh2 },
	{ HumanLThigh2, HumanLCalf },
	{ HumanRThigh2, HumanRCalf },
	{ HumanLCalf, HumanLFoot },
	{ HumanRCalf, HumanRFoot },
	{ HumanLFoot, HumanLToe },
	{ HumanRFoot, HumanRToe }
};

inline Vector2 calculate_angle(Vector3 source, Vector3 destination)
{
	Vector3 difference = source - destination;
	float length = difference.Length();

	Vector2 ret{};

	ret.y = asinf(difference.y / length);
	ret.x = -atan2f(difference.x, -difference.z);

	ret = { ret.x * 57.29578f, ret.y * 57.29578f };

	return ret;
}
struct drawing {
	Vector3 head;
	Vector3 root;
	std::vector<std::pair<Vector3, Vector3>> bones;
	std::string player_name;
	uint32_t side;
}_drawing;
struct scavs {
	Vector3 head;
	Vector3 root;
	std::string scav;
	std::vector<std::pair<Vector3, Vector3>> bones;
	uint32_t side;
}_scavs;
bool no_recoil;
std::vector<drawing> drawings;
std::vector<scavs> s_drawings;

float fov = 100.f;





	__forceinline void aimbot_thread() {
		while (true) {
			Sleep(17);
			if (aimbot) {
				if (GetAsyncKeyState(WallHackKey)) {
					float lowestDistance = fov;
					uintptr_t selectedPlayer = 0x0;
					Vector3 selected_position;
					for (uintptr_t& player : activePlayerlist)
					{

						const Vector3 head_location = get_bone(player, bones::HumanHead);
						const Vector2 world = world_to_screen(head_location);
						if (world.Zero())continue;
						if (Vector2(1920 * 0.5f, 1080 * 0.5f).Distance(world) < lowestDistance) {
							lowestDistance = Vector2(1920 * 0.5f, 1080 * 0.5f).Distance(world);
							selectedPlayer = player;
							selected_position = head_location;
						}

					}
					if (selectedPlayer) {
						Vector3 weapon_barrel = get_transform_position(driver::read_chain<uintptr_t>(cache::weapon, { 0x18, 0x88, 0x10 }));
						uintptr_t Movement = driver::read<uintptr_t>(cache::LocalPlayer + 0x40);
						driver::write<Vector2>(Movement + 0x234, calculate_angle(weapon_barrel, selected_position));
					}
				}
			}
		}
	}
		
	


__forceinline void cache_render_positions() {//handle_key_press
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
	CloseHandle(GetCurrentThread());
	while (true) {
	
		
		std::this_thread::sleep_for(std::chrono::milliseconds(17));
		std::vector<drawing> t_drawings;
		std::vector<scavs> t_s_drawings;

		int selected{};
		for (uintptr_t& player : activePlayerlist)
		{
			selected++;
			
			if (player != cache::LocalPlayer) {
				const Vector3 head_location = get_bone(player, bones::HumanHead);
				const Vector3 root_location = (get_bone(player, bones::HumanLFoot) + get_bone(player, bones::HumanRFoot)) * 0.5f;

				std::vector<std::pair<Vector3, Vector3>> bones2;
			

				for (const auto& point : bone_points) {
					bones2.emplace_back(std::pair<Vector3, Vector3>(get_bone(player, point.first), get_bone(player, point.second)));
				}
						
				std::string scavType = entities[selected];
				if (!(isScav[selected])) {
				
					drawing draw = { head_location, root_location, bones2, scavType, side[selected]};
					t_drawings.emplace_back(draw);
				}
				else {
				
					scavs draw = { head_location, root_location, scavType, bones2, side[selected] };
					t_s_drawings.emplace_back(draw);
				
				}
			}
		}

		
		if (ads)
		{
			driver::read<float>(offsets::player::pwa + 0x1b8);
		}
	

		if (stamina) {
			driver::write<float>(cache::body_stamina + 0x48, 100.f);
		}

		if (no_recoil && cache::shot_effector != 0 && cache::LocalPlayer != 0 && cache::CameraManager && cache::weapon) {

			driver::write<float>(cache::shot_effector + 0x78, 0.f);
		}
		drawings = t_drawings;
		s_drawings = t_s_drawings;
	}
}
ImU32 __fastcall FtIM2(float* color) {
	return ImGui::ColorConvertFloat4ToU32(ImVec4(color[0], color[1], color[2], color[3]));
}




__forceinline void runRenderTick() {

	glfwPollEvents();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	
	matrix = driver::read<mat4x4_t>(cache::FpsCamera + 0xDC);
	auto draw_list = ImGui::GetBackgroundDrawList();
	if (fovonoff) {
		ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(1920 * 0.5f, 1080 * 0.5f), fov, FtIM2(fovcolor), 100.f, 1.f);
	}


	for (drawing& draw : drawings) {
		const Vector3 head_location = draw.head;
		const Vector2 screen_location = world_to_screen(head_location);
		if (screen_location.Zero())continue;
		const Vector3 root_location = draw.root;
		const Vector2 r_screen_location = world_to_screen(root_location);
		if (r_screen_location.Zero())continue;

		const float box_height = abs(screen_location.y - r_screen_location.y);
		const float box_width = box_height * 0.75f;
		
		
			DrawCorneredBox(draw_list, screen_location.x - (box_width * 0.5f), screen_location.y, box_width, box_height, FtIM2(boxcolor), 1.f);
			const ImVec2 calc = ImGui::CalcTextSize(draw.player_name.c_str());
			draw_list->AddText(ImVec2(r_screen_location.x - (calc.x * 0.25f) - (box_width * 0.25f), r_screen_location.y + calc.y), FtIM2(namecolor), draw.player_name.c_str());
		
		for (std::pair<Vector3, Vector3>& boneza : draw.bones )
		{
			const	Vector2 screen = 	world_to_screen(boneza.first);
			if (screen.Zero())continue;
			const Vector2 screen_2 = world_to_screen(boneza.second);
			if (screen_2.Zero())continue;
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screen.x, screen.y), ImVec2(screen_2.x, screen_2.y), FtIM2(skeletoncolor), 1.0f);
		}
		
	


		if (draw.side == 0x1) {
			draw_list->AddText(ImVec2(r_screen_location.x - (calc.x * 0.25f) - (box_width * 0.25f), r_screen_location.y + (calc.y * 2.f)), FtIM2(namecolor), "USEC");
		}
		else {
			draw_list->AddText(ImVec2(r_screen_location.x - (calc.x * 0.25f) - (box_width * 0.25f), r_screen_location.y + (calc.y * 2.f) ), FtIM2(namecolor), "BEAR");

		}
		ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(screen_location.x, screen_location.y), 5.f, FtIM2(fovcolor), 100, 1.f);

	}
	
	for (scavs& draw_pls : s_drawings) {
		const Vector3 head_location = draw_pls.head;
		const Vector2 screen_location = world_to_screen(head_location);
		if (screen_location.Zero())continue;
		const Vector3 root_location = draw_pls.root;
		const Vector2 r_screen_location = world_to_screen(root_location);
		if (r_screen_location.Zero())continue;
		const float box_height = abs(screen_location.y - r_screen_location.y);

		const float box_width = box_height * 0.75f;

		if (box) {
			DrawCorneredBox(draw_list, screen_location.x - (box_width * 0.5f), screen_location.y, box_width, box_height, FtIM2(boxcolor), 1.f);
		}

		const ImVec2 calc = ImGui::CalcTextSize(draw_pls.scav.c_str());
		if (crosshair) {
			draw_list->AddText(ImVec2(r_screen_location.x - (calc.x * 0.25f) - (box_width * 0.25f), r_screen_location.y + calc.y), FtIM2(namecolor), draw_pls.scav.c_str());

		}

			for (std::pair<Vector3, Vector3>& boneza : draw_pls.bones)
			{
				const	Vector2 screen = world_to_screen(boneza.first);
				if (screen.Zero())continue;
				const Vector2 screen_2 = world_to_screen(boneza.second);
				if (screen_2.Zero())continue;
				if (skeleton) {
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screen.x, screen.y), ImVec2(screen_2.x, screen_2.y), FtIM2(skeletoncolor), 1.0f);
				}
			}
		}
	    



	if (watermark)
	{

		std::stringstream ss;

		ss << Config::Username << " ~ ";
		ss << "fps / " << static_cast<int>(ImGui::GetIO().Framerate);

		ImGui::SetNextWindowPos(ImVec2(5, 5));
		ImGui::SetNextWindowSize(ImVec2(ImGui::CalcTextSize(ss.str().c_str()).x + 15, 10));
		ImGui::Begin("##watermark", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);

		ImGui::Text(ss.str().c_str());

		ImGui::End();

	}


	if (watermark)
	{
		//ImGui::GetBackgroundDrawList()->AddText(ImVec2(10, 10), ImGui::GetColorU32({ 0, 255, 255, 255 }), "https://yamatos.pub/");
	}


	ImVec4 textcolor = ImLerp(ImVec4(201 / 255.f, 204 / 255.f, 210 / 255.f, 1.f), ImVec4(1.0f, 1.0f, 1.0f, 1.f), 1.f);
		
	DWORD window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = ImGui::GetStyle().Colors;

	colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImColor(48, 48, 48); // this is it
	colors[ImGuiCol_FrameBgActive] = ImColor(48, 48, 48); // this is it
	colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
	colors[ImGuiCol_TitleBgActive] = ImColor(145, 0, 145);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(145, 0, 145);
	colors[ImGuiCol_ScrollbarGrabActive] = ImColor(145, 0, 145);
	colors[ImGuiCol_CheckMark] = ImColor(145, 0, 145);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	colors[ImGuiCol_SliderGrabActive] = ImColor(145, 0, 145);
	colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	colors[ImGuiCol_ButtonHovered] = ImColor(145, 0, 145);
	colors[ImGuiCol_ButtonActive] = ImColor(145, 0, 145);
	colors[ImGuiCol_Header] = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
	colors[ImGuiCol_HeaderHovered] = ImColor(145, 0, 145);
	colors[ImGuiCol_HeaderActive] = ImColor(145, 0, 145);
	colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImColor(145, 0, 145);
	colors[ImGuiCol_SeparatorActive] = ImColor(145, 0, 145);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
	colors[ImGuiCol_ResizeGripHovered] = ImColor(145, 0, 145);
	colors[ImGuiCol_ResizeGripActive] = ImColor(145, 0, 145);
	colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	colors[ImGuiCol_PlotLinesHovered] = ImColor(145, 0, 145);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	colors[ImGuiCol_PlotHistogramHovered] = ImColor(145, 0, 145);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);
	ImGui::PushFontShadow(IM_COL32(0, 0, 0, 255));
	style.FrameRounding = 6;
	style.PopupRounding = 8.f;
	if (g_overlay_visible) {
		{

			ImGui::Begin("Yamatos-EFT", &p_open, ImGuiWindowFlags_NoResize);
			if (p_open) {				
			}
				
				
				
				ImGui::Text("// AIM");
				ImGui::Checkbox("Aimbot", &aimbot);
				ImGui::Text("Aim Keybind"); ImGui::SameLine(); WallHackKeyButton(WallHackKey, WallHackChangeKey, WallHackKeyStatus, ImVec2(125, 22));
				ImGui::Checkbox("Fov Circle", &fovonoff); ImGui::SameLine(); ImGui::ColorEdit4(("FOV Color"), fovcolor);
				ImGui::SliderFloat("FOV", &fov, 1.f, 1000.f);


				ImGui::Text("// ESP");
				ImGui::Checkbox("Box", &box); ImGui::SameLine(); ImGui::ColorEdit4(("Box Color"), boxcolor);
				ImGui::Checkbox("Name", &crosshair); ImGui::SameLine(); ImGui::ColorEdit4(("Name Color"), namecolor);
				ImGui::Checkbox("Skeleton", &skeleton);ImGui::SameLine(); ImGui::ColorEdit4(("Skeleton Color"), skeletoncolor);

				ImGui::Text("// MISC");		
				ImGui::Checkbox("Unlimted Stamina", &stamina);
				ImGui::Checkbox("No Recoil", &no_recoil);
				ImGui::SetWindowSize("Yamatos-EFT", ImVec2(450, 400));
				}
			ImGui::End();
		}
	

	ImGui::Render();
	glViewport(0, 0, g_width, g_height);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(g_window);
	std::this_thread::sleep_for(std::chrono::milliseconds(1));

}


int main()
{
	SetConsoleTitle("Yamtos EFT");


	 







START:
	driver::setup();
	SetConsoleTitle("Yamtos EFT");
	std::cout << " PRESS ENTER: "<< std::endl;
	CloseHandle(GetCurrentThread());
	set_timer_resolution();//windows trick (timer resolution) (make cheat feel better overall)
	SetProcessPriorityBoost(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
	

	getchar();

	int process_id = driver::get_process_id(e("EscapeFromTarkov.exe"));
	game_window = ::FindWindow(e("UnityWndClass"), nullptr);
	std::cout << "eft :  " << process_id << std::endl;
	driver::process_id = process_id;
	base_address = driver::get_game_base();
	mono_base = driver::get_mono_module();
	unity_base = driver::get_unity_player();

	 
	std::cout << " game_base: "  << std::hex  << base_address << std::endl;
	std::cout << " mono_base: " << std::hex << mono_base << std::endl;
	std::cout << " unity_base: " << std::hex << unity_base  << std::endl;
	std::cout << " ------------ " << std::endl;
	cache::GameObjectManager = driver::read<uintptr_t>(unity_base + GOM);

	

	
	setupWindow();
	if (!g_window) {
		std::cout << "Could not setup window.\n";
	
		return 1;
	}
	std::thread(cache_players).detach();
	std::thread(cache_render_positions).detach();
	std::thread(aimbot_thread).detach();
	
	std::thread(hkp).detach();
	
	glfwSetWindowAttrib(g_window, GLFW_MOUSE_PASSTHROUGH, !g_overlay_visible);

	while (!glfwWindowShouldClose(g_window))
	{
	//	handleKeyPresses();
		runRenderTick();
	}


	cleanupWindow();
	
}
