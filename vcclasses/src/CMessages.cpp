#include "vcclasses.h"
#include "vcversion.h"

BriefMessage *CMessages::BriefMessages = reinterpret_cast<BriefMessage *>(vcversion::AdjustOffset(0x009458D0));

static unsigned long g_AddMessageJumpQWithNumber = vcversion::AdjustOffset(0x00583440);
static unsigned long g_AddMessageWithNumber = vcversion::AdjustOffset(0x00583560);
static unsigned long g_InsertPlayerControlKeysInString = vcversion::AdjustOffset(0x005836B0);
static unsigned long g_InsertStringInString = vcversion::AdjustOffset(0x00583AF0);
static unsigned long g_InsertNumberInString = vcversion::AdjustOffset(0x00583C80);
static unsigned long g_AddToPreviousBriefArray = vcversion::AdjustOffset(0x00583DE0);
static unsigned long g_AddBigMessageQ = vcversion::AdjustOffset(0x00583F40);
static unsigned long g_AddBigMessage = vcversion::AdjustOffset(0x00584050);
static unsigned long g_AddMessageJumpQ = vcversion::AdjustOffset(0x00584300);
static unsigned long g_AddMessage = vcversion::AdjustOffset(0x00584410);
static unsigned long g_Display = vcversion::AdjustOffset(0x00584550);

__declspec(naked) void CMessages::AddMessageJumpQWithNumber(wchar_t *, unsigned int, unsigned short, int, int, int, int, int, int)
{
	__asm jmp g_AddMessageJumpQWithNumber;
}

__declspec(naked) void CMessages::AddMessageWithNumber(wchar_t *, unsigned int, unsigned short, int, int, int, int, int, int)
{
	__asm jmp g_AddMessageWithNumber;
}

__declspec(naked) void CMessages::InsertPlayerControlKeysInString(wchar_t *)
{
	__asm jmp g_InsertPlayerControlKeysInString;
}

__declspec(naked) void CMessages::InsertStringInString(wchar_t *, wchar_t *)
{
	__asm jmp g_InsertStringInString;
}

__declspec(naked) void CMessages::InsertNumberInString(wchar_t *, int, int, int, int, int, int, wchar_t *)
{
	__asm jmp g_InsertNumberInString;
}

__declspec(naked) void CMessages::AddToPreviousBriefArray(wchar_t *, int, int, int, int, int, int, wchar_t *)
{
	__asm jmp g_AddToPreviousBriefArray;
}

__declspec(naked) void CMessages::AddBigMessageQ(wchar_t *, unsigned int, unsigned short)
{
	__asm jmp g_AddBigMessageQ;
}

__declspec(naked) void CMessages::AddBigMessage(wchar_t *, unsigned int, unsigned short)
{
	__asm jmp g_AddBigMessage;
}

__declspec(naked) void CMessages::AddMessageJumpQ(wchar_t *, unsigned int, unsigned short)
{
	__asm jmp g_AddMessageJumpQ;
}

__declspec(naked) void CMessages::AddMessage(wchar_t *, unsigned int, unsigned short)
{
	__asm jmp g_AddMessage;
}

__declspec(naked) void CMessages::Display(void)
{
	__asm jmp g_Display;
}
