#pragma once

#include "vcclasses.h"

class CPadHack : public CPad
{
public:
	static bool initialise();

	void AddToCheatStringHack(char);
	static void WeaponCheat();
	static void MoneyCheat();
	static void HealthCheat();
	static void WantedLevelUpCheat();
	static void WantedLevelDownCheat();
	static void TankCheat();
	static void BlowUpCarsCheat();
	static void MayhemCheat();
	static void EverybodyAttacksPlayerCheat();
	static void WeaponsForAllCheat();
	static void FastTimeCheat();
	static void SlowTimeCheat();
	static void ArmourCheat();
	static void SunnyWeatherCheat();
	static void CloudyWeatherCheat();
	static void RainyWeatherCheat();
	static void FoggyWeatherCheat();
	static void FastWeatherCheat();
	static void OnlyRenderWheelsCheat();
	static void ChittyChittyBangBangCheat();
	static void StrongGripCheat();
};
