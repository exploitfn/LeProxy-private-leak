#pragma once
#include <math.h>
#include <array>
#include "stdafx.h"
#include "fn.h"
#include "driver.h"
#include "defs.h"

struct FQuat
{
	float x;
	float y;
	float z;
	float w;
};

struct FTransform
{
	FQuat rot;
	fn::fvector translation;
	fn::fvector scale;
	D3DMATRIX ToMatrixWithScale()
	{
		D3DMATRIX m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;

		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};

D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}

std::string string_To_UTF8(const std::string& str)
{
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

namespace math
{

	double RadianToDegree(double radian) {
		return radian * (180 / M_PI);
	}

	double DegreeToRadian(double degree) {
		return degree * (M_PI / 180);
	}

	fn::fvector RadianToDegree(fn::fvector radians) {
		fn::fvector degrees;
		degrees.x = radians.x * (180 / M_PI);
		degrees.y = radians.y * (180 / M_PI);
		degrees.z = radians.z * (180 / M_PI);
		return degrees;
	}

	fn::fvector DegreeToRadian(fn::fvector degrees) {
		fn::fvector radians;
		radians.x = degrees.x * (M_PI / 180);
		radians.y = degrees.y * (M_PI / 180);
		radians.z = degrees.z * (M_PI / 180);
		return radians;
	}

	void ClampAngle(fn::fvector& angle) {
		if (angle.x > 89.0f) angle.x = 89.f;
		if (angle.x < -89.0f) angle.x = -89.f;

		if (angle.y > 180.f) angle.y = 180.f;
		if (angle.y < -180.f) angle.y = -180.f;

		angle.z = 0.0f;
	}

	void Normalise(fn::fvector& angle) {
		if (angle.x > 89.0f) angle.x -= 180.0f;
		if (angle.x < -89.0f) angle.x += 180.0f;

		while (angle.y > 180) angle.y -= 360.f;
		while (angle.y < -180) angle.y += 360.f;
	}

	double __fastcall Atan2(double a1, double a2)
	{
		double result; // xmm0_8

		result = 0.0;
		if (a2 != 0.0 || a1 != 0.0)
			return atan2(a1, a2);
		return result;
	}
	double __fastcall FMod(double a1, double a2)
	{
		if (fabs(a2) > 0.00000001)
			return fmod(a1, a2);
		else
			return 0.0;
	}

	double ClampAxis(double Angle)
	{
		// returns Angle in the range (-360,360)
		Angle = FMod(Angle, (double)360.0);

		if (Angle < (double)0.0)
		{
			// shift to [0,360) range
			Angle += (double)360.0;
		}

		return Angle;
	}

	double NormalizeAxis(double Angle)
	{
		// returns Angle in the range [0,360)
		Angle = ClampAxis(Angle);

		if (Angle > (double)180.0)
		{
			// shift to (-180,180]
			Angle -= (double)360.0;
		}

		return Angle;
	}

	fn::fvector CalcAngle(fn::fvector src, fn::fvector dst) {
		fn::fvector OutAngle;
		OutAngle.x = -atan2f(dst.x - src.x, dst.y - src.y) / M_PI * 180 + 180;
		OutAngle.y = asinf((dst.z - src.z) / src.Distance(dst)) * 180 / M_PI;
		OutAngle.z = 0;

		return OutAngle;
	}

	void Angles(const fn::fvector& angles, fn::fvector* forward)
	{
		float	sp, sy, cp, cy;

		sy = sin(DegreeToRadian(angles.y));
		cy = cos(DegreeToRadian(angles.y));

		sp = sin(DegreeToRadian(angles.x));
		cp = cos(DegreeToRadian(angles.x));

		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	fn::fvector NewRotation(fn::fvector& zaz, fn::fvector& daz) {
		fn::fvector dalte = zaz - daz;
		fn::fvector ongle;
		float hpm = sqrtf(dalte.x * dalte.x + dalte.y * dalte.y);
		ongle.y = atan(dalte.y / dalte.x) * 57.295779513082f;
		ongle.x = (atan(dalte.z / hpm) * 57.295779513082f) * -1.f;
		if (dalte.x >= 0.f) ongle.y += 180.f;
		return ongle;
	}

	fn::FRotator Rotator(FQuat* F)
	{
		const double SingularityTest = F->z * F->x - F->w * F->y;
		const double YawY = 2.f * (F->w * F->z + F->x * F->y);
		const double YawX = (1.f - 2.f * ((F->y * F->y) + (F->z * F->z)));

		const double SINGULARITY_THRESHOLD = 0.4999995f;
		const double RAD_TO_DEG = 57.295776;
		double Pitch, Yaw, Roll;

		if (SingularityTest < -SINGULARITY_THRESHOLD)
		{
			Pitch = -90.f;
			Yaw = (Atan2(YawY, YawX) * RAD_TO_DEG);
			Roll = NormalizeAxis(-Yaw - (2.f * Atan2(F->x, F->w) * RAD_TO_DEG));
		}
		else if (SingularityTest > SINGULARITY_THRESHOLD)
		{
			Pitch = 90.f;
			Yaw = (Atan2(YawY, YawX) * RAD_TO_DEG);
			Roll = NormalizeAxis(Yaw - (2.f * Atan2(F->x, F->w) * RAD_TO_DEG));
		}
		else
		{
			Pitch = (asin(2.f * SingularityTest) * RAD_TO_DEG);
			Yaw = (Atan2(YawY, YawX) * RAD_TO_DEG);
			Roll = (Atan2(-2.f * (F->w * F->x + F->y * F->z), (1.f - 2.f * ((F->x * F->x) + (F->y * F->y)))) * RAD_TO_DEG);
		}

		fn::FRotator RotatorFromQuat = fn::FRotator{ Pitch, Yaw, Roll };
		return RotatorFromQuat;
	}

}

DWORD rainbow(float time = 0.40f);
DWORD rainbow(float time) {
	static float x = 0, y = 0;
	static float r = 0, g = 0, b = 0;

	if (y >= 0.0f && y < 255.0f) {
		r = 255.0f;
		g = 0.0f;
		b = x;
	}
	else if (y >= 255.0f && y < 510.0f) {
		r = 255.0f - x;
		g = 0.0f;
		b = 255.0f;
	}
	else if (y >= 510.0f && y < 765.0f) {
		r = 0.0f;
		g = x;
		b = 255.0f;
	}
	else if (y >= 765.0f && y < 1020.0f) {
		r = 0.0f;
		g = 255.0f;
		b = 255.0f - x;
	}
	else if (y >= 1020.0f && y < 1275.0f) {
		r = x;
		g = 255.0f;
		b = 0.0f;
	}
	else if (y >= 1275.0f && y < 1530.0f) {
		r = 255.0f;
		g = 255.0f - x;
		b = 0.0f;
	}

	x += 0.40f; //increase this value to switch colors faster
	if (x >= 255.0f)
		x = 0.0f;

	y += 0.40f; //increase this value to switch colors faster
	if (y > 1530.0f)
		y = 0.0f;

	return ImColor((int)r, (int)g, (int)b, 255);
}

fn::fvector smooth_aim(fn::fvector target, fn::fvector rot, float smooth)
{
	fn::fvector diff = target - rot;
	math::Normalise(diff);
	return rot + diff / smooth;
}

bool IsVec3Valid(fn::fvector vec3)
{
	return (vec3.x == 0 && vec3.y == 0 && vec3.z == 0);
}

bool IsInScreen(fn::fvector pos, int over = 30)
{
	if (pos.x > -over && pos.x < center_x + over && pos.y > -over && pos.y < center_y + over)
	{
		return true;
	}
	else {
		return false;
	}
}

boolean bIsInRectangle(double centerX, double centerY, double radius, double x, double y) {
	return x >= centerX - radius && x <= centerX + radius &&
		y >= centerY - radius && y <= centerY + radius;
}

float powf_(float _X, float _Y) {
	return (_mm_cvtss_f32(_mm_pow_ps(_mm_set_ss(_X), _mm_set_ss(_Y))));
}

double GetDistance(double x1, double y1, double z1, double x2, double y2) {
	return sqrtf(powf((x2 - x1), 2) + powf_((y2 - y1), 2));
}

boolean in_rect(double centerX, double centerY, double radius, double x, double y) {
	return x >= centerX - radius && x <= centerX + radius &&
		y >= centerY - radius && y <= centerY + radius;
}

float RandomFloat(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void draw_rect(int x, int y, int w, int h, ImColor color, int thickness) {
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 3.f, 15, thickness);
}

void draw_rect_filled(int x, int y, int w, int h, ImColor color, int thickness) {
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 3.f, 15);
}

void box_esp(float OffsetW, float OffsetH, int X, int Y, int W, int H, ImU32 Color, int thickness, bool filled, bool outlined) {
	if (filled) {
		ImU32 sdfg = ImColor(0, 0, 0, 100);
		draw_rect_filled(X, Y, W, H, sdfg, 2.0f);
	}

	if (outlined) {
		draw_rect(X, Y, W, H, ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 0.5f);
	}

	draw_rect(X, Y, W, H, Color, 2.0f);
}

void corner_esp(float OffsetW, float OffsetH, int X, int Y, int W, int H, ImU32 Color, int thickness, bool filled, bool outlined) {
	ImDrawList* p = ImGui::GetOverlayDrawList();

	float lineW = (W / OffsetW);
	float lineH = (H / OffsetH);

	if (filled) {
		ImU32 sdfg = ImColor(0, 0, 0, 100);
		draw_rect_filled(X, Y, W, H, sdfg, 0.f);
	}

	if (outlined) {
		p->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 0.5f);
		p->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 0.5f);
		p->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 0.5f);
		p->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 0.5f);
		p->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 0.5f);
		p->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 0.5f);
		p->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 0.5f);
		p->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 0.5f);
	}

	p->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), Color, thickness);
	p->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), Color, thickness);
	p->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), Color, thickness);
	p->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), Color, thickness);
	p->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), Color, thickness);
	p->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), Color, thickness);
	p->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), Color, thickness);
	p->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), Color, thickness);

}

void ThreeD_Box(bool StartIsCenter, float Offset, float Thickness, ImU32 Clr, fn::fvector Start, float Top)
{
	ImDrawList* pDrawList = ImGui::GetOverlayDrawList();

	if (StartIsCenter)
	{
		fn::fvector BottomOneW2S = fn::fvector(Start.x + Offset, Start.y + Offset, Start.z - Offset);

		fn::fvector BottomTwoW2S = fn::fvector(Start.x - Offset, Start.y - Offset, Start.z - Offset);

		fn::fvector BottomThreeW2S = fn::fvector(Start.x + Offset, Start.y - Offset, Start.z - Offset);

		fn::fvector BottomFourW2S = fn::fvector(Start.x - Offset, Start.y + Offset, Start.z - Offset);

		fn::fvector TopOneW2S = fn::fvector(Start.x + Offset, Start.y + Offset, Start.z + Offset);

		fn::fvector TopTwoW2S = fn::fvector(Start.x - Offset, Start.y - Offset, Start.z + Offset);

		fn::fvector TopThreeW2S = fn::fvector(Start.x + Offset, Start.y - Offset, Start.z + Offset);

		fn::fvector TopFourW2S = fn::fvector(Start.x - Offset, Start.y + Offset, Start.z + Offset);

		pDrawList->AddLine(ImVec2(BottomOneW2S.x, BottomOneW2S.y), ImVec2(BottomThreeW2S.x, BottomThreeW2S.y), ImGui::GetColorU32( Clr ), Thickness);
		pDrawList->AddLine(ImVec2(BottomThreeW2S.x, BottomThreeW2S.y), ImVec2(BottomTwoW2S.x, BottomTwoW2S.y), ImGui::GetColorU32( Clr ), Thickness);
		pDrawList->AddLine(ImVec2(BottomTwoW2S.x, BottomTwoW2S.y), ImVec2(BottomFourW2S.x, BottomFourW2S.y), ImGui::GetColorU32( Clr ), Thickness);
		pDrawList->AddLine(ImVec2(BottomFourW2S.x, BottomFourW2S.y), ImVec2(BottomOneW2S.x, BottomOneW2S.y), ImGui::GetColorU32( Clr ), Thickness);

		pDrawList->AddLine(ImVec2(TopOneW2S.x, TopOneW2S.y), ImVec2(TopThreeW2S.x, TopThreeW2S.y), ImGui::GetColorU32( Clr ), Thickness);
		pDrawList->AddLine(ImVec2(TopThreeW2S.x, TopThreeW2S.y), ImVec2(TopTwoW2S.x, TopTwoW2S.y), ImGui::GetColorU32( Clr ), Thickness);
		pDrawList->AddLine(ImVec2(TopTwoW2S.x, TopTwoW2S.y), ImVec2(TopFourW2S.x, TopFourW2S.y), ImGui::GetColorU32( Clr ), Thickness);
		pDrawList->AddLine(ImVec2(TopFourW2S.x, TopFourW2S.y), ImVec2(TopOneW2S.x, TopOneW2S.y), ImGui::GetColorU32( Clr ), Thickness);

		pDrawList->AddLine(ImVec2(BottomOneW2S.x, BottomOneW2S.y), ImVec2(TopOneW2S.x, TopOneW2S.y), ImGui::GetColorU32( Clr ), Thickness);
		pDrawList->AddLine(ImVec2(BottomTwoW2S.x, BottomTwoW2S.y), ImVec2(TopTwoW2S.x, TopTwoW2S.y), ImGui::GetColorU32( Clr ), Thickness);
		pDrawList->AddLine(ImVec2(BottomThreeW2S.x, BottomThreeW2S.y), ImVec2(TopThreeW2S.x, TopThreeW2S.y), ImGui::GetColorU32( Clr ), Thickness);
		pDrawList->AddLine(ImVec2(BottomFourW2S.x, BottomFourW2S.y), ImVec2(TopFourW2S.x, TopFourW2S.y), ImGui::GetColorU32( Clr ), Thickness);
	}
	else
	{
		fn::fvector BottomOneW2S = fn::fvector(Start.x + Offset, Start.y + Offset, Start.z);

		fn::fvector BottomTwoW2S = fn::fvector(Start.x - Offset, Start.y - Offset, Start.z);

		fn::fvector BottomThreeW2S = fn::fvector(Start.x + Offset, Start.y - Offset, Start.z);

		fn::fvector BottomFourW2S = fn::fvector(Start.x - Offset, Start.y + Offset, Start.z);

		fn::fvector TopOneW2S = fn::fvector(Start.x + Offset, Start.y + Offset, Top);

		fn::fvector TopTwoW2S = fn::fvector(Start.x - Offset, Start.y - Offset, Top);

		fn::fvector TopThreeW2S = fn::fvector(Start.x + Offset, Start.y - Offset, Top);

		fn::fvector TopFourW2S = fn::fvector(Start.x - Offset, Start.y + Offset, Top);

		pDrawList->AddLine(ImVec2(BottomOneW2S.x, BottomOneW2S.y), ImVec2(BottomThreeW2S.x, BottomThreeW2S.y), ImGui::GetColorU32( Clr ), Thickness);
		pDrawList->AddLine(ImVec2(BottomThreeW2S.x, BottomThreeW2S.y), ImVec2(BottomTwoW2S.x, BottomTwoW2S.y), ImGui::GetColorU32( Clr ), Thickness);
		pDrawList->AddLine(ImVec2(BottomTwoW2S.x, BottomTwoW2S.y), ImVec2(BottomFourW2S.x, BottomFourW2S.y), ImGui::GetColorU32( Clr ), Thickness);
		pDrawList->AddLine(ImVec2(BottomFourW2S.x, BottomFourW2S.y), ImVec2(BottomOneW2S.x, BottomOneW2S.y), ImGui::GetColorU32( Clr ), Thickness);

		pDrawList->AddLine(ImVec2(TopOneW2S.x, TopOneW2S.y), ImVec2(TopThreeW2S.x, TopThreeW2S.y), ImGui::GetColorU32( Clr ), Thickness);
		pDrawList->AddLine(ImVec2(TopThreeW2S.x, TopThreeW2S.y), ImVec2(TopTwoW2S.x, TopTwoW2S.y), ImGui::GetColorU32( Clr ), Thickness);
		pDrawList->AddLine(ImVec2(TopTwoW2S.x, TopTwoW2S.y), ImVec2(TopFourW2S.x, TopFourW2S.y), ImGui::GetColorU32( Clr ), Thickness);
		pDrawList->AddLine(ImVec2(TopFourW2S.x, TopFourW2S.y), ImVec2(TopOneW2S.x, TopOneW2S.y), ImGui::GetColorU32( Clr ), Thickness);

		pDrawList->AddLine(ImVec2(BottomOneW2S.x, BottomOneW2S.y), ImVec2(TopOneW2S.x, TopOneW2S.y), ImGui::GetColorU32( Clr ), Thickness);
		pDrawList->AddLine(ImVec2(BottomTwoW2S.x, BottomTwoW2S.y), ImVec2(TopTwoW2S.x, TopTwoW2S.y), ImGui::GetColorU32( Clr ), Thickness);
		pDrawList->AddLine(ImVec2(BottomThreeW2S.x, BottomThreeW2S.y), ImVec2(TopThreeW2S.x, TopThreeW2S.y), ImGui::GetColorU32( Clr ), Thickness);
		pDrawList->AddLine(ImVec2(BottomFourW2S.x, BottomFourW2S.y), ImVec2(TopFourW2S.x, TopFourW2S.y), ImGui::GetColorU32( Clr ), Thickness);
	}
}

void DrawFortniteText(ImFont* Font, ImVec2 pos, ImU32 color, const char* str)
{
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);
	ImDrawList* Drawlist { };
	Drawlist->AddText(Font, 18.0f, pos, ImGui::GetColorU32(color), utf_8_2.c_str());
}

void presskeycode(int delay)
{
	INPUT Input = { 0 }; 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = VK_LBUTTON;
	SendInput(1, &Input, sizeof(INPUT));
	Sleep(delay);
	ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &Input, sizeof(INPUT));
}

void mouse(double x, double y, float smooth)
{
	float ScreenCenterX = center_x;
	float ScreenCenterY = center_y;
	int AimSpeed = smooth;
	float TargetX = 0;
	float TargetY = 0;

	if (x != 0)
	{
		if (x > ScreenCenterX)
		{
			TargetX = -(ScreenCenterX - x);
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
		}

		if (x < ScreenCenterX)
		{
			TargetX = x - ScreenCenterX;
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX < 0) TargetX = 0;
		}
	}

	if (y != 0)
	{
		if (y > ScreenCenterY)
		{
			TargetY = -(ScreenCenterY - y);
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
		}

		if (y < ScreenCenterY)
		{
			TargetY = y - ScreenCenterY;
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY < 0) TargetY = 0;
		}
	}

	float targetx_min = TargetX - 1;
	float targetx_max = TargetX + 1;

	float targety_min = TargetY - 1;
	float targety_max = TargetY + 1;

	float offset_x = RandomFloat(targetx_min, targetx_max);
	float offset_y = RandomFloat(targety_min, targety_max);

	mouse_event(MOUSEEVENTF_MOVE, static_cast<int>((float)offset_x), static_cast<int>((float)offset_y / 1), NULL, NULL);

	return;
}

void move(uint64_t PlayerState, double x, double y, float smooth)
{
	double screen_x = (ImGui::GetIO().DisplaySize.x / 2);
	double screen_y = (ImGui::GetIO().DisplaySize.y / 2);
	double target_x = 0;
	double target_y = 0;

	if (x != 0.f)
	{
		if (x > screen_x)
		{
			target_x = -(screen_x - x);
			target_x /= smooth;
			if (target_x + screen_x > screen_x * 2.f) target_x = 0.f;
		}

		if (x < screen_x)
		{
			target_x = x - screen_x;
			target_x /= smooth;
			if (target_x + screen_x < 0.f) target_x = 0.f;
		}
	}

	if (y != 0.f)
	{
		if (y > screen_y)
		{
			target_y = -(screen_y - y);
			target_y /= smooth;
			if (target_y + screen_y > screen_y * 2.f) target_y = 0.f;
		}

		if (y < screen_y)
		{
			target_y = y - screen_y;
			target_y /= smooth;
			if (target_y + screen_y < 0.f) target_y = 0.f;
		}
	}

	float theNum = floor(target_x / smooth);
	float result = theNum / 6.666666666666667f;

	float theNum1 = floor(target_y / smooth);
	float resulte = theNum1 / 6.666666666666667f;
	float result1 = -(resulte);

	write<float>(fn::globals::PlayerController + PlayerState + 0x0, result1);
	write<float>(fn::globals::PlayerController + PlayerState + 0x4, result);

	return;
}

void ResetAngles(uintptr_t PlayerCamera)
{
	float ViewPitchMin = -89.9999f;
	float ViewPitchMax = 89.9999f;
	float ViewYawMin = 0.0000f;
	float ViewYawMax = 359.9999f;

	write<float>(PlayerCamera + fn::offsets::ViewPitchMin, ViewPitchMin);
	write<float>(PlayerCamera + fn::offsets::ViewPitchMax, ViewPitchMax);
	write<float>(PlayerCamera + fn::offsets::ViewYawMin, ViewYawMin);
	write<float>(PlayerCamera + fn::offsets::ViewYawMax, ViewYawMax);
}

void SetAngles(uintptr_t PlayerCamera, fn::fvector CameraLocation, fn::fvector TargetLocation)
{
	fn::fvector VectorPos = TargetLocation - CameraLocation;

	float distance = (double)(sqrtf(VectorPos.x * VectorPos.x + VectorPos.y * VectorPos.y + VectorPos.z * VectorPos.z));

	float x, y, z;
	x = -((acosf(VectorPos.z / distance) * (float)(180.0f / 3.14159265358979323846264338327950288419716939937510)) - 90.f);
	y = atan2f(VectorPos.y, VectorPos.x) * (float)(180.0f / 3.14159265358979323846264338327950288419716939937510);
	z = 0;

	write<float>(PlayerCamera + fn::offsets::ViewPitchMin, x);
	write<float>(PlayerCamera + fn::offsets::ViewPitchMax, x);
	write<float>(PlayerCamera + fn::offsets::ViewYawMin, y);
	write<float>(PlayerCamera + fn::offsets::ViewYawMax, y);
}

// LeProxy has been here
fn::fvector prediction(fn::fvector player, uint64_t ActorRootComp, float distance)
{
	fn::fvector recalculated = player;
	fn::fvector velocity = read<fn::fvector>(ActorRootComp + 0x170);
	static float ProjectileGravity = 35;
	static float ProjectileSpeed = -504;
	float gravity = fabs(ProjectileGravity);
	float time = distance / fabs(ProjectileSpeed);
	float bulletDrop = (gravity / 250) * time * time;
	recalculated.z += bulletDrop * 120;
	recalculated = fn::fvector(recalculated.x += time * (velocity.x), recalculated.y += time * (velocity.y), recalculated.z += time * (velocity.z));
	return recalculated;
}