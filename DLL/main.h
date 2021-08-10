#pragma once
#ifndef MAIN_H
#define MAIN_H

#include "bsp_parser.hpp"
#include "Vector3.h"
#include "Offsets.h"
#include "Utils.h"
#include "DriverInterface.h"
#include "Aimbot.h"

#include <bitset>
#include <ctime>
#include <thread>
#include <stdlib.h>
#include <sstream> 
#include <psapi.h>



DriverInterface DRIVER;

int teamId;


rn::bsp_parser bsp;
//Aim

Settings currentSettings{
	true,
	0,
	true,
	0,
	true,
	0,
	true,
	0,
	true,
	0,
	0,
	true,
	0,
	true,
	0,
	true,
	{0,0,0},
	{0,0,0},
	true
};

Aimbot aimbot(&currentSettings);

//Glow Structs

GlowStruct EnemyGlow
{
	1.0f,
	.33f,
	.33f,
	1.0f,
	{},
	true,
	false
};
GlowStruct FriendlyGlow
{
	.25f,
	.6f,
	1.0f,
	1.0f,
	{},
	true,
	false
};


void doGlow(bool isFriendly, int index, int health);
void triggerBot(int enemyId, int crosshairId);

extern "C"
{
	__declspec(dllexport) void __stdcall updateSettings(const Settings& newSettings);
	__declspec(dllexport) void __stdcall updateAntiFlash();
	__declspec(dllexport) void __stdcall doBhop();
	__declspec(dllexport) int __stdcall init();
	__declspec(dllexport) void __stdcall mainLoop();

}
#endif