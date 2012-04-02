#pragma once
#include "HUD.h"
#include "DynamicObjManager.h"

class Ability
{
public:
	Ability(AbilityType _abilityType);
	~Ability(void);
	void startCooldownTimer();
	int getCooldownTime();
	bool onCooldown();
	bool currentlyActive();
	void updateCooldown(float seconds);
	float getBoostValue();
	void update(int levelOfAbility);
	int getAbilityLevel();

private:
	float boostValue;
	float cooldownTime;
	float lengthOfCooldown;
	float boostDuration;
	AbilityType abilityType;
	int abilityLevel;
};

