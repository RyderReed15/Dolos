#pragma once

#include "Vector3.h"
#include "Offsets.h"
#include "Utils.h"

class Aimbot {
public:
	Aimbot(Settings* settings);
	~Aimbot();
	bool findClosestTarget(Vector3 pos, Vector3 angles, Vector3 enemyPos, float* closestTarget, Vector3* CurrentAngs);
	Vector3 getNewAngles(Vector3 viewAngles, Vector3 enemyAngles, int target);
	Vector3 doOveraim(Vector3 viewAngles, Vector3 enemyAngles);
	Vector3 RecoilControl(Vector3, bool);
	void setAimPunch(Vector3 punch);
	int getTargetNum();
	void updateTarget(int targetId);
private:
	int targetNum;
	Vector3 overaimAng;
	Vector3 oldAimPunch;
	Vector3 currAimPunch;
	Settings* settings;



};