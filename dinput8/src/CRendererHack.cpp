#include "CRendererHack.h"
#include "vcversion.h"
#include "Globals.h"
#include "SilentCall.h"
#include <Windows.h>

unsigned long renderEverythingBarRoadsIntermediate = vcversion::AdjustOffset(0x004C9FA8);
unsigned long renderEverythingBarRoadsEndJump = vcversion::AdjustOffset(0x004C9F90);
bool isStateOne = false;
char buildings[8000] = {};

bool CRendererHack::initialise()
{
	if (GetPrivateProfileInt("Misc", "DisableCulling", 1, "./gta-lc.ini")) {
		// disable backface culling from aap's skygfx_vc - temporary
		Patch<unsigned char>(0x004C9E5F, 1);
		Patch<unsigned char>(0x004C9F08, 1);
		//Patch<unsigned char>(0x004C9F5D, 1); // replaced
		Patch<unsigned char>(0x004CA157, 1);
		Patch<unsigned char>(0x004CA199, 1);

		// disable backface culling during cutscene
		Patch<unsigned char>(0x004E0146, 1);
	}

	// control backface culling of non-transparent buildings
	InjectHook(0x004C9F87, &CRendererHack::RenderEverythingBarRoadsHackProxy, PATCH_JUMP);

	// get optional data to draw backfaces
	InjectHook(0x004CA421, &CRendererHack::InitHack, PATCH_JUMP);
	return true;
}

void __declspec(naked) CRendererHack::RenderEverythingBarRoadsHackProxy(void)
{
	__asm
	{
		movsx ebx, word ptr [esi+5Ch] // get model index
		push ebx
		call CRendererHack::RenderEverythingBarRoadsHack
		pop ecx
		mov al, byte ptr [esi+50h]
		and al, 7
		cmp al, 1
		jnz intermediate
		jmp renderEverythingBarRoadsEndJump
	intermediate:
		jmp renderEverythingBarRoadsIntermediate
	}
}

void CRendererHack::RenderEverythingBarRoadsHack(int model)
{
	if (buildings[model]) {
		RwRenderStateSet(0x14, 1);
		isStateOne = true;
		return;
	}
	if (isStateOne) {
		RwRenderStateSet(0x14, 2);
		isStateOne = false;
	}
}

void CRendererHack::InitHack(void)
{
	int filename = CFileMgr::OpenFile("drawBackfaces.dat", "r");
	if (filename) {
		char buffer[200];
		int line;
		while (CFileMgr::ReadLine(filename, buffer, 200)) {
			VCGlobals::sscanf(buffer, "%d", &line);
			buildings[line] = 1;
		}
		CFileMgr::CloseFile(filename);
	}
}
