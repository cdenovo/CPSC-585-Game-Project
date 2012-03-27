#pragma once
#include <time.h>
#include "HUD.h"

class Ability
{
public:
	Ability(AbilityType _abilityType);
	~Ability(void);
	void startCooldownTimer();
	int getCooldownTime();
	bool onCooldown();
	bool currentlyActive();
	void updateCooldown();
	float getBoostValue();
	void update(int levelOfAbility);
	int getAbilityLevel();
	int getLaserDamage();

private:
	time_t oldTime;
	time_t newTime;
	float boostValue;
	float cooldownTime;
	float lengthOfCooldown;
	float boostDuration;
	AbilityType abilityType;
	int abilityLevel;
	int laserDamage;
};

