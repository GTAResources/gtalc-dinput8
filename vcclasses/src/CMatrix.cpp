#include "vcclasses.h"
#include "vcversion.h"

static unsigned long g_SetRotate = vcversion::AdjustOffset(0x004DF240);
static unsigned long g_SetRotateZ = vcversion::AdjustOffset(0x004DF3B0);
static unsigned long g_SetRotateZOnly = vcversion::AdjustOffset(0x004DF590);
static unsigned long g_SetTranslate = vcversion::AdjustOffset(0x004DF6D0);
static unsigned long g_ResetOrientation = vcversion::AdjustOffset(0x004DF780);
static unsigned long g_UpdateRW = vcversion::AdjustOffset(0x004DF8F0);

__declspec(naked) void CMatrix::SetRotate(float, float, float)
{
	__asm jmp g_SetRotate;
}

__declspec(naked) void CMatrix::SetRotateZ(float)
{
	__asm jmp g_SetRotateZ;
}

__declspec(naked) void CMatrix::SetRotateZOnly(float)
{
	__asm jmp g_SetRotateZOnly;
}

__declspec(naked) void CMatrix::SetTranslate(float, float, float)
{
	__asm jmp g_SetTranslate;
}

__declspec(naked) void CMatrix::ResetOrientation(void)
{
	__asm jmp g_ResetOrientation;
}

__declspec(naked) void CMatrix::UpdateRW(void)
{
	__asm jmp g_UpdateRW;
}
