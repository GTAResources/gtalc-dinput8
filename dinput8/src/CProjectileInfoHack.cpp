#include "CProjectileInfoHack.h"
#include "Globals.h"
#include "SilentCall.h"
#include "WeaponTypes.h"
#include "vcversion.h"

using ::VCGlobals::gaProjectileInfo;

// UpdateHack
static unsigned long projectileInfoUpdateProceedJump = vcversion::AdjustOffset(0x005C6ECC);
static unsigned long projectileInfoUpdateEndJump = vcversion::AdjustOffset(0x005C6F45);

bool CProjectileInfoHack::initialise()
{
	// ProjectileInfo null thrower fix
	InjectHook(0x005C6EC6, &CProjectileInfoHack::UpdateHack, PATCH_JUMP);

	return true;
}

bool CProjectileInfoHack::IsProjectileInRange(float x1, float y1, float z1, float x2, float y2, float z2, bool destroy)
{
	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (CObject *object = ms_apProjectile[i]) {
			if (gaProjectileInfo[i].doesProjectileExist &&
				gaProjectileInfo[i].weaponType >= WEAPONTYPE_GRENADE &&
				gaProjectileInfo[i].weaponType <= WEAPONTYPE_ROCKET &&
				object->GetX() >= x1 && object->GetX() <= x2 &&
				object->GetY() >= y1 && object->GetY() <= y2 &&
				object->GetZ() >= z1 && object->GetZ() <= z2) {
				if (destroy) {
					gaProjectileInfo[i].doesProjectileExist = 0;
					auto Destroy = (void(__thiscall *)(CObject *, int))*(unsigned long *)(object->vtbl + 8);
					CWorld::Remove(object);
					Destroy(object, 1);
				}
				return true;
			}
		}
	}
	return false;
}

void __declspec(naked) CProjectileInfoHack::UpdateHack()
{
	__asm
	{
		mov eax, [ebp+4] // get pointer of projectile thrower
		test eax, eax    // compare pointer with 0
		jz end
		mov eax, [ebp+0]
		cmp eax, 0Ch
		jmp projectileInfoUpdateProceedJump
	end:
		jmp projectileInfoUpdateEndJump
	}
}
