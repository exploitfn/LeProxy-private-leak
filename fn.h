#pragma once
#include "driver.h"

namespace fn {

	class fvector {
	public:
		fvector() : x(0.f), y(0.f), z(0.f)
		{

		}

		fvector(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
		{

		}
		~fvector()
		{

		}

		double x;
		double y;
		double z;

		double Dot(const fvector& vOther) const {
			const fvector& a = *this;
			return (a.x * vOther.x + a.y * vOther.y + a.z * vOther.z);
		}

		double Distance(fvector v) {
			return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
		}

		fvector operator-(fvector V) {
			return fvector(x - V.x, y - V.y, z - V.z);
		}

		fvector operator+(fvector V) {
			return fvector(x + V.x, y + V.y, z + V.z);
		}

		fvector operator*(double Scale) {
			return fvector(x * Scale, y * Scale, z * Scale);
		}

		fvector operator/(double Scale) {
			double RScale = 1.f / Scale;
			return fvector(x * RScale, y * RScale, z * RScale);
		}

		fvector operator+(double A) {
			return fvector(x + A, y + A, z + A);
		}

		fvector operator-(double A) {
			return fvector(x - A, y - A, z - A);
		}

		fvector operator*(fvector V) {
			return fvector(x * V.x, y * V.y, z * V.z);
		}

		fvector operator/(fvector V) {
			return fvector(x / V.x, y / V.y, z / V.z);
		}

		double operator|(fvector V) {
			return x * V.x + y * V.y + z * V.z;
		}

		double operator^(fvector V) {
			return x * V.y - y * V.x - z * V.z;
		}

		fvector operator+=(fvector v) {
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		fvector operator-=(fvector v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		fvector operator*=(fvector v) {
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}

		fvector operator/=(fvector v) {
			x /= v.x;
			y /= v.y;
			z /= v.z;
			return *this;
		}

		bool operator==(fvector src) {
			return (src.x == x) && (src.y == y) && (src.z == z);
		}

		bool operator!=(fvector src) {
			return (src.x != x) || (src.y != y) || (src.z != z);
		}

		double Size() {
			return sqrt(x * x + y * y + z * z);
		}

		double Size2D() const {
			return sqrt(x * x + y * y);
		}

		double SizeSquared() {
			return x * x + y * y + z * z;
		}

		double SizeSquared2D() {
			return x * x + y * y;
		}
	};

	class vector2D {
	public:
		vector2D() : x(0.f), y(0.f)
		{

		}

		vector2D(double _x, double _y) : x(_x), y(_y)
		{

		}
		~vector2D()
		{

		}

		double x;
		double y;
	};

	class FRotator
	{
	public:
		FRotator() : Pitch(0.f), Yaw(0.f), Roll(0.f)
		{

		}

		FRotator(double _Pitch, double _Yaw, double _Roll) : Pitch(_Pitch), Yaw(_Yaw), Roll(_Roll)
		{

		}
		~FRotator()
		{

		}

		double Pitch;
		double Yaw;
		double Roll;

		inline FRotator get() {
			return FRotator(Pitch, Yaw, Roll);
		}

		inline void set(double _Pitch, double _Yaw, double _Roll) {
			Pitch = _Pitch;
			Yaw = _Yaw;
			Roll = _Roll;
		}

		inline FRotator Clamp() {
			FRotator result = get();
			if (result.Pitch > 180)
				result.Pitch -= 360;
			else if (result.Pitch < -180)
				result.Pitch += 360;
			if (result.Yaw > 180)
				result.Yaw -= 360;
			else if (result.Yaw < -180)
				result.Yaw += 360;
			if (result.Pitch < -89)
				result.Pitch = -89;
			if (result.Pitch > 89)
				result.Pitch = 89;
			while (result.Yaw < -180.0f)
				result.Yaw += 360.0f;
			while (result.Yaw > 180.0f)
				result.Yaw -= 360.0f;

			result.Roll = 0;
			return result;

		}

		double Length() {
			return sqrt(Pitch * Pitch + Yaw * Yaw + Roll * Roll);
		}

	};

	namespace pointers {
		uint64_t base_address = NULL;
		DWORD_PTR ProcessId = NULL;
	}

	namespace globals
	{
		DWORD_PTR Uworld;
		DWORD_PTR Gameinstance;
		DWORD_PTR Localplayer;
		DWORD_PTR PlayerController;
		DWORD_PTR LocalPawn;
		std::string FName;
		std::string WeaponName;
	};

	enum offsets : uintptr_t
	{
		Uworld = 0x10567408,
		OwningGameInstance = 0x1B8,
		PlayerCameraManager = 0x340,
		AcknowledgedPawn = 0x330,
		PlayerState = 0x2a8,
		RootComponent = 0x190,
		RelativeLocation = 0x128,
		CurrentWeapon = 0x8F8,
		RelativeScale3D = 0x158,
		mesh = 0x310,
		levels = 0x170,
		Tier = 0x73,
		ViewYawMin = 0x317c,
		ViewYawMax = 0x3180,
		ViewPitchMin = 0x3174,
		ViewPitchMax = 0x3178,
	};

	//enum EFortWeaponTriggerType : int
	//{
	//	Aim = VK_RBUTTON,
	//	Shoot = VK_LBUTTON,
	//};

	//std::vector<std::pair<int, int>> SkeletonPairs = {
	//{ 66, 64 },     // Head to Neck
	//{ 64, 5 },      // Neck to Center Hip
	//{ 5, 2 },       // Center Hip to Hip
	//{ 2, 74 },      // Hip to Right Hip
	//{ 2, 67 },      // Hip to Left Hip
	//{ 74, 75 },     // Right Hip to Right Knee
	//{ 67, 68 },     // Left Hip to Left Knee
	//{ 75, 83 },     // Right Knee to Right Foot
	//{ 68, 82 },     // Left Knee to Left Foot
	//{ 64, 9 },      // Neck to Left Shoulder
	//{ 64, 37 },     // Neck to Right Shoulder
	//{ 37, 38 },     // Right Shoulder to Right Elbow
	//{ 9, 10 },      // Left Shoulder to Left Elbow
	//{ 10, 32 },     // Left Elbow to Left Hand
	//{ 38, 60 },     // Right Elbow to Right Hand
	//};

	struct actor
	{
		uint64_t pawn;
		uintptr_t mesh;
		uintptr_t rootcomp;
		uintptr_t player_state;
		uintptr_t enemy_state;
		uintptr_t weapon;
		uintptr_t camera;
		INT32 team_id;
		float DBNO;
		bool lobby;
		fn::fvector location;
	};

	inline bool operator==(const actor& a, const actor& b) {
		if (a.pawn == b.pawn)
			return true;
		return false;
	}
}