#pragma once

#ifndef UTILS_H
#define UTILS_H
#include <random>
#include "Windows.h"


inline float curTime = 0.f;
inline float waitTime = 0;



const int fps = 60;
const int sleepTime = 1000 / fps;
const float timeInc = 1.f / fps;
inline DWORD ProcessID = 0;
inline DWORD ClientAddress = 0;
inline DWORD EngineAddress = 0;
inline DWORD LocalPlayer;
inline DWORD ClientState;
inline DWORD LocalPlayerID;


struct EntityStruct
{
	BYTE Reserved[offsets::m_bDormant];
	bool dormant;
	BYTE Reserved1[offsets::m_iTeamNum - offsets::m_bDormant - sizeof(dormant)];
	int32_t teamNum;
	BYTE Reserved2[offsets::m_iHealth - offsets::m_iTeamNum - sizeof(teamNum)];
	int32_t health;
	BYTE Reserved3[offsets::m_bSpotted - offsets::m_iHealth - sizeof(health)];
	bool spotted;
	BYTE Reserved4[offsets::m_bSpottedByMask - offsets::m_bSpotted - sizeof(spotted)];
	int32_t spottedMask;
	BYTE Reserved5[offsets::m_dwBoneMatrix - offsets::m_bSpottedByMask - sizeof(spottedMask)];
	DWORD boneMatrix;
};

struct GlowStruct
{

	float	r;
	float	g;
	float	b;
	float	a;
	BYTE	Reserved[16];
	bool	rwo;
	bool	rwuo;
};

struct Settings
{

	bool aimOn;
	float overAimFactor;
	bool overaim;
	float antiTransfer;
	bool silentAim;
	int boneId;//8 = head, 6 = chest
	bool teamESP;
	int enemyESP;//0 = off | 1 = only if visible | 2 = always on
	bool bhopOn;
	float rangeDecMult;
	float fov;
	bool triggerOn;
	float recoilControlFactor;
	bool antiFlash;
	float aimTime;
	bool radar;
	int friendly[3];
	int enemy[3];
	bool running;
};


/**
* Calculates angles between the points src and dst
*
* @param Point one (source)
* @param Point two (destination)
* @return Vector3 of angles to between points
*/
inline Vector3 FixAngs(Vector3 angs) {
	while (abs(angs.y) > 180) {
		angs.y += 360 * (angs.y > 0 ? -1 : 1);
	}

	if (abs(angs.x) > 89) {
		angs.x = 89.f * (angs.x > 0 ? 1 : -1);
	}
	return angs;
}inline Vector3 RadConvert(Vector3 angs) {
	return (angs * (2 * acos(0.0f))) / 180;
}
inline Vector3 DegConvert(Vector3 angs) {
	return (angs * 180) / (2 * acos(0.0f));
}
inline Vector3 CalcAngle(Vector3 src, Vector3 dst)
{
	Vector3 angles;

	Vector3 delta = (src - dst);
	float hyp = (float)sqrtf(delta.x * delta.x + delta.y * delta.y);
	angles.x = (float)(atanf(delta.z / hyp) * 180.f / (2 * acos(0)));
	angles.y = (float)(atanf(delta.y / delta.x) * 180.f / (2 * acos(0)));
	angles.z = 0.0f;

	if (delta.x >= 0.0f)
		angles.y += 180.0f;
	return angles;
}

inline Vector3 Move(Vector3 src, Vector3 dst, float percent, bool curve) {
	Vector3 Delta = src.Lerp(dst, percent);
	Delta.x *= percent;
	Delta.y *= percent;
	Delta.x *= (1 + .15f * curve);
	Delta.x *= 1 + (.01f * ((float(rand()) / float((RAND_MAX))) - .5f));
	Delta.y *= 1 + (.01f * ((float(rand()) / float((RAND_MAX))) - .5f));
	return FixAngs(src - Delta);
}



inline std::string toString(int i) {
	std::string s = "aaaa";
	memcpy(&s, &i, sizeof(i));
	return s;
}

#endif