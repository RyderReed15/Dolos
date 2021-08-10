
#include "Aimbot.h"
#include <Windows.h>

Aimbot::Aimbot(Settings *settingsPoint) {
	settings = settingsPoint;
	targetNum = -1;
}
Aimbot::~Aimbot() {

}
/*
 Handles compensating for weapon recoil
 @param desired angle for the bullet fire at
 @param if an enemy is being targeted by the aimbot
 @return Angles to aim at for compensation
*/
Vector3 Aimbot::RecoilControl(Vector3 angles, bool aimbot) {
	if (settings->recoilControlFactor == 0) {
		return angles;
	}
	

	if (aimbot) {
		return (angles - (currAimPunch * settings->recoilControlFactor));
	}
	else {
		Vector3 newAngs = FixAngs((angles + oldAimPunch) - (currAimPunch * settings->recoilControlFactor));

		oldAimPunch = (currAimPunch * settings->recoilControlFactor);
		return newAngs;
	}

}
/*
 Calculates the closest target to the player in terms of degree from center of screen
 @param player position
 @param player view angles
 @param enemy position
 @param pointer to the distance to the current closest enemy
 @param pointer to view angles pointing at current closest enemy
 @return If a closer target was found
*/
bool Aimbot::findClosestTarget(Vector3 pos, Vector3 angles, Vector3 enemyPos, float* closestTarget, Vector3* CurrentAngs) {

	float distance = pos.Distance(enemyPos);

	Vector3 angleTo = CalcAngle(pos, enemyPos);

	angleTo = RecoilControl(angleTo, true);

	float rotateDist = angleTo.Lerp(angles, 1).Magnitude();
	if (rotateDist + (settings->fov / (.1 + settings->rangeDecMult * 3500 / distance)) < (*closestTarget)) {

		
		if (rotateDist < (settings->fov / (1 + settings->rangeDecMult * distance / 500))) {
			*closestTarget = rotateDist + (settings->fov / (.1 + settings->rangeDecMult * 3500 / distance));

			*CurrentAngs = angleTo;
			return true;
		}

	}

	return false;
}
/*
 Handles overaim for the aimbot
 @param player view angles
 @param target view angles
 @return angles to view enemy + extra
*/
Vector3 Aimbot::doOveraim(Vector3 viewAngles, Vector3 enemyAngles) {


	float distance = settings->overAimFactor * enemyAngles.Lerp(viewAngles, 1).Magnitude() / 15;
	int angle = rand() % 360;
	Vector3 overaim(sin(angle) * distance, cos(angle) * distance, 0);
	
	return overaim;
}
/*
 Gets angles to move to accounting for lerp and overaim
 @param current player view angles
 @param target view angles
 @param the id of the target
 @returb the desired angle to move towards the target
*/
Vector3 Aimbot::getNewAngles(Vector3 viewAngles, Vector3 enemyAngles, int target) {
	if (waitTime <= 0) {
		viewAngles = FixAngs(viewAngles);


		if (targetNum != target) {
			overaimAng = { 0,0,0 };
			if (settings->overaim) {
				overaimAng = doOveraim(viewAngles, enemyAngles);
				
			}
			if (targetNum != -1) {
				waitTime = settings->antiTransfer;
			}
			
			curTime = 0.f;
		}
		targetNum = target;
		enemyAngles = FixAngs(enemyAngles + overaimAng);

		if (viewAngles.Distance(enemyAngles) < .25f) {
			curTime = settings->aimTime / 4;
			overaimAng = { 0,0,0 };
		}

		curTime += timeInc * (curTime < settings->aimTime);
		//Vector3 dst = Move(ViewAngles, storedAng, storedSpeed);
		Vector3 dst = Move(viewAngles, enemyAngles, (1.f < (curTime / settings->aimTime)) ? 1.f : (curTime / settings->aimTime), true);
		

		//if (curTime / settings->aimTime >= 1) waitTime = timeInc;
		return dst;
		
	}
	return viewAngles;

}
/*
 Updates the target and sets a time to wait if needed
 @param id of the new target
*/
void Aimbot::updateTarget(int targetId) {
	if (targetId != -1) {
		targetNum = targetId;

	}
	else {
		targetNum = -1;
		overaimAng = { 0,0,0 };
		curTime = 0.f;

	}

}
/*
 Updates the aim punch angle
 @param current wepon recoil
*/
void Aimbot::setAimPunch(Vector3 punch) {
	currAimPunch = punch;
}
/*
 Gets the current target id
 @return targetNum
*/
int Aimbot::getTargetNum() {
	return targetNum;
}
