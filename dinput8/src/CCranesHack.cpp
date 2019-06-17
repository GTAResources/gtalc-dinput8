#include "CCranesHack.h"
#include <cmath>
#include "vcversion.h"
#include "Globals.h"
#include "SilentCall.h"
#include "ModelIndices.h"
#include "Offset.h"

// FindCarInSectorListHack
static unsigned long findCarProceedJump = vcversion::AdjustOffset(0x0005A81EC);
static unsigned long findCarEndJump = vcversion::AdjustOffset(0x005A82F4);

// crane audio
static void DestroyAudioEntityProxy();
static void DestroyAudioEntity();
static unsigned long updateHackEndJump = vcversion::AdjustOffset(0x005A8859);
static int audioEntities[8];

bool CCranesHack::initialise()
{
	Patch<unsigned int>(0x005A8CBF, MILITARYCRANETOTAL);
	Patch<unsigned char>(0x005A8CCC, MILITARYCRANETOTAL);

	InjectHook(0x005A8CB9, &CCranesHack::RegisterCarForMilitaryCrane);
	InjectHook(0x005A8232, &CCranesHack::DoesMilitaryCraneHaveThisOneAlready);
	InjectHook(0x004A4EC4, &CCranesHack::InitCranes);
	InjectHook(0x004A4993, &CCranesHack::InitCranes);
	auto function = &CCraneHack::DoesCranePickUpThisCarType;
	InjectHook(0x005A821E, (unsigned long &)function);
	InjectHook(0x005A8275, (unsigned long &)function);
	InjectHook(0x005A81DF, &CCraneHack::FindCarInSectorListHack, PATCH_JUMP);

	InjectHook(0x005A8842, &CCraneHack::UpdateHackProxy, PATCH_JUMP);
	InjectHook(0x00627073, &DestroyAudioEntityProxy, PATCH_JUMP);

	return true;
}

void CCranesHack::RegisterCarForMilitaryCrane(unsigned int model)
{
	switch (model) {
	case CAR_FIRETRUK:
		CCranes::CarsCollectedMilitaryCrane |= 1;
		break;
	case CAR_AMBULAN:
		CCranes::CarsCollectedMilitaryCrane |= 2;
		break;
	case CAR_POLICE:
		CCranes::CarsCollectedMilitaryCrane |= 4;
		break;
	case CAR_ENFORCER:
		CCranes::CarsCollectedMilitaryCrane |= 8;
		break;
	case CAR_RHINO:
		CCranes::CarsCollectedMilitaryCrane |= 0x10;
		break;
	case CAR_BARRACKS:
		CCranes::CarsCollectedMilitaryCrane |= 0x20;
		break;
	case CAR_FBICAR:
		CCranes::CarsCollectedMilitaryCrane |= 0x40;
		break;
	case CAR_VICECHEE:
		CCranes::CarsCollectedMilitaryCrane |= 0x80;
		break;
	}
}

bool CCranesHack::DoesMilitaryCraneHaveThisOneAlready(unsigned int model)
{
	switch (model) {
	case CAR_FIRETRUK:
		return !!(CCranes::CarsCollectedMilitaryCrane & 1);
	case CAR_AMBULAN:
		return !!(CCranes::CarsCollectedMilitaryCrane & 2);
	case CAR_POLICE:
		return !!(CCranes::CarsCollectedMilitaryCrane & 4);
	case CAR_ENFORCER:
		return !!(CCranes::CarsCollectedMilitaryCrane & 8);
	case CAR_RHINO:
		return !!(CCranes::CarsCollectedMilitaryCrane & 0x10);
	case CAR_BARRACKS:
		return !!(CCranes::CarsCollectedMilitaryCrane & 0x20);
	case CAR_FBICAR:
		return !!(CCranes::CarsCollectedMilitaryCrane & 0x40);
	case CAR_VICECHEE:
		return !!(CCranes::CarsCollectedMilitaryCrane & 0x80);
	}
	return false;
}

bool CCranesHack::IsThisCarPickedUp(float positionX, float positionY, CVehicle *vehicle)
{
	if (CCranes::NumCranes > 0) {
		for (int i = 0; i < CCranes::NumCranes; i++) {
			if (aCranes[i].object) {
				float distance = sqrt(pow(positionX - aCranes[i].object->GetX(), 2) + pow(positionY - aCranes[i].object->GetY(), 2));
				if (distance < 100.0 && aCranes[i].vehicle == vehicle && (aCranes[i].status == 2 || aCranes[i].status == 4)) {
					return true;
				}
			}
		}
	}
	return false;
}

void CCranesHack::DeActivateCrane(float positionX, float positionY)
{
	int index = -1;
	float minDistance = 100.0;
	if (CCranes::NumCranes > 0) {
		for (int i = 0; i < CCranes::NumCranes; i++) {
			if (aCranes[i].object) {
				float distance = sqrt(pow(positionX - aCranes[i].object->GetX(), 2) + pow(positionY - aCranes[i].object->GetY(), 2));
				if (distance < minDistance) {
					index = i;
					minDistance = distance;
				}
			}
		}
	}
	if (index == -1) return;
	aCranes[index].activity = 2;
	aCranes[index].status = 0;
}

void CCranesHack::ActivateCrane(float pickupX1, float pickupX2, float pickupY1, float pickupY2, float dropoffX, float dropoffY, float dropoffZ, float dropoffHeading, bool isCrusher, bool isMilitary, float positionX, float positionY)
{
	int index = -1;
	float craneObjectX, craneObjectY, minDistance, pickupCenterX, pickupCenterY;
	minDistance = 100.0;
	if (CCranes::NumCranes > 0) {
		for (int i = 0; i < CCranes::NumCranes; i++) {
			if (aCranes[i].object) {
				float distance = sqrt(pow(positionX - aCranes[i].object->GetX(), 2) + pow(positionY - aCranes[i].object->GetY(), 2));
				if (distance < minDistance) {
					index = i;
					minDistance = distance;
				}
			}
		}
	}
	if (index == -1) return;
	aCranes[index].pickupX1 = pickupX1;
	aCranes[index].pickupX2 = pickupX2;
	aCranes[index].pickupY1 = pickupY1;
	aCranes[index].pickupY2 = pickupY2;
	aCranes[index].dropoffX = dropoffX;
	aCranes[index].dropoffY = dropoffY;
	aCranes[index].dropoffZ = dropoffZ;
	aCranes[index].activity = 1;
	aCranes[index].vehicle = 0;
	aCranes[index].countCollected = 0;
	aCranes[index].dropoffHeading = dropoffHeading;
	aCranes[index].isCrusher = isCrusher;
	aCranes[index].isMilitary = isMilitary;
	aCranes[index].timer = 0;
	aCranes[index].status = 0;
	pickupCenterX = (pickupX1 + pickupX2) / 2;
	pickupCenterY = (pickupY1 + pickupY2) / 2;
	craneObjectX = aCranes[index].object->GetX();
	craneObjectY = aCranes[index].object->GetY();
	if (isCrusher) {
		aCranes[index].armPickupHeight = -0.95099998f + OFFSETZ;
	} else if (isMilitary) {
		aCranes[index].armPickupHeight = 10.7862f + OFFSETZ;
	} else {
		aCranes[index].armPickupHeight = CWorld::FindGroundZForCoord(pickupCenterX, pickupCenterY);
	}
	aCranes[index].armPickupRotation = CGeneral::GetATanOfXY(pickupCenterX - craneObjectX, pickupCenterY - craneObjectY);
	aCranes[index].armPickupDistance = sqrt(pow(pickupCenterX - craneObjectX, 2) + pow(pickupCenterY - craneObjectY, 2));
	aCranes[index].armDropoffRotation = CGeneral::GetATanOfXY(dropoffX - craneObjectX, dropoffY - craneObjectY);
	aCranes[index].armDropoffDistance = sqrt(pow(dropoffX - craneObjectX, 2) + pow(dropoffY - craneObjectY, 2));
	aCranes[index].armDropoffHeight = dropoffZ;
}

void CCranesHack::AddThisOneCrane(CEntity *entity)
{
	entity->GetMatrix().ResetOrientation();
	// add crane to array
	if (CCranes::NumCranes < 8) {
		int index = CCranes::NumCranes;
		aCranes[index].object = entity;
		aCranes[index].activity = 0;
		// initialise rotation
		aCranes[index].armCurrentRotation = static_cast<float>(CCranes::NumCranes);
		while (aCranes[index].armCurrentRotation > 6.283) {
			aCranes[index].armCurrentRotation -= 6.283;
		}
		aCranes[index].armCurrentDistance = 20.0f;
		aCranes[index].armCurrentHeight = 20.0f + OFFSETZ;
		aCranes[index].timer = 0;
		aCranes[index].status = 0;
		aCranes[index].unk3 = 0;
		aCranes[index].isNotCab = entity->modelIndex != ModelIndices::MODELID_CRANE_1;
		aCranes[index].hook = 0;
		// new! magnet for cranes!
		if (aCranes[index].isNotCab || entity->GetY() > 0.0) {
			if (ModelIndices::MI_MAGNET != 0xFFFF) {
				CObject *object = new CObject(ModelIndices::MI_MAGNET, false);
				if (object) {
					object->field_16C = 2;
					object->field_051 &= 0xFE;
					object->field_052 |= 2;
					object->field_11A &= 0xFD;
					aCranes[index].hook = object;
				}
			}
		}
		audioEntities[index] = VCGlobals::DMAudio.CreateEntity(12, &CCranes::aCranes[index]);
		if (audioEntities[index]) {
			VCGlobals::DMAudio.SetEntityStatus(audioEntities[index], 1);
		}
		CCranes::NumCranes++;
	}
}

void CCranesHack::InitCranes()
{
	CCranes::CarsCollectedMilitaryCrane = 0;
	CCranes::NumCranes = 0;
	for (int i = 0; i < CPools::ms_pBuildingPool->totalCount; i++) {
		if ((CPools::ms_pBuildingPool->flags[i] & 0x80) != 0x80) {
			short model = CPools::ms_pBuildingPool->entities[i].modelIndex;
			if (model == ModelIndices::MODELID_CRANE_1 ||
				model == ModelIndices::MODELID_CRANE_2 ||
				model == ModelIndices::MODELID_CRANE_3) {
				CCranesHack::AddThisOneCrane(&CPools::ms_pBuildingPool->entities[i]);
			}
		}
	}
}

void __declspec(naked) DestroyAudioEntityProxy()
{
	__asm
	{
		call DestroyAudioEntity
		retn
	}
}

void DestroyAudioEntity()
{
	for (int i = 0; i < CCranes::NumCranes; i++) {
		VCGlobals::DMAudio.DestroyEntity(audioEntities[i]);
	}
}

bool CCraneHack::DoesCranePickUpThisCarType(unsigned int model)
{
	if (isCrusher) {
		if (model == CAR_FIRETRUK ||
			model == CAR_TRASH ||
			model == CAR_HUNTER ||
			model == CAR_SECURICA ||
			model == CAR_BUS ||
			model == CAR_RHINO ||
			model == CAR_COACH ||
			model == CAR_SEASPAR ||
			model == BOAT_SKIMMER ||
			model == CAR_DODO ||
			model == CAR_MAVERICK ||
			model == CAR_VCNMAV ||
			model == CAR_POLMAV) {
			return false;
		}
		return true;
	} else if (isMilitary) {
		if (model == CAR_FIRETRUK ||
			model == CAR_AMBULAN ||
			model == CAR_POLICE ||
			model == CAR_ENFORCER ||
			model == CAR_RHINO ||
			model == CAR_BARRACKS ||
			model == CAR_FBICAR ||
			model == CAR_VICECHEE) {
			return true;
		}
		return false;
	}
	return true;
}

void __declspec(naked) CCraneHack::FindCarInSectorListHack()
{
	__asm
	{
		cmp dword ptr [ebp+29Ch], 0 // is vehicle type car
		jz proceed
		cmp dword ptr [ebp+29Ch], 5 // is vehicle type bike
		jz proceed
		jmp findCarEndJump
	proceed:
		jmp findCarProceedJump
	}
}

void __declspec(naked) CCraneHack::UpdateHackProxy()
{
	__asm
	{
		mov ecx, ebx
		call CCraneHack::UpdateHack
		jmp updateHackEndJump
	}
}

void CCraneHack::UpdateHack()
{
	if (this->isCrusher) {
		CVehicle *vehicle = this->vehicle;
		vehicle->field_053 = (vehicle->field_053 & 0xF7) | 8;
	}
	int index = ((unsigned long)this - (unsigned long)&CCranes::aCranes[0]) / sizeof(CCrane);
	VCGlobals::DMAudio.PlayOneShot(audioEntities[index], 0, 0.0);
}
