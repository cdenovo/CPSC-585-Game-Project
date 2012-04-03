#include "Ability.h"


Ability::Ability(AbilityType _abilityType)
{
	abilityType = _abilityType;
	cooldownTime = 0;

	abilityLevel = 1;

	switch(abilityType){
	case SPEED:
		lengthOfCooldown = 15; // Time until boost can be used again
		boostValue = 0.5f; // Additional speed that the boost provides
		boostDuration = 3; // Time in seconds that the boost lasts
		break;
	case LASER:
		lengthOfCooldown = 2; // Time until laser can be used again
		break;
	case ROCKET:
		lengthOfCooldown = 1.0f; // Time until rocket can be used again
		break;
	case LANDMINE:
		lengthOfCooldown = 0.5f; // Time until landmine can be used again
		break;
	}
	
}


Ability::~Ability(void)
{
}

int Ability::getCooldownTime()
{
	return (int)cooldownTime;
}

void Ability::startCooldownTimer()
{
	cooldownTime = lengthOfCooldown;
}

bool Ability::onCooldown()
{
	if(cooldownTime > 0){
		return true;
	}
	else{
		return false;
	}
}

bool Ability::currentlyActive()
{
	if(lengthOfCooldown - cooldownTime < boostDuration){
		return true;
	}
	else{
		return false;
	}
}

void Ability::updateCooldown(float seconds)
{		
	cooldownTime -= seconds;
}

float Ability::getBoostValue()
{
	if(currentlyActive()){
		return boostValue;
	}
	else{
		return 0.0f;
	}
}

// When an ability changes in level, its paramaters can change.
// For example, a weapons damage at level 1 may be 34, but can
// upgrade to 67 damage at level 2 and 100 damage at level 3.
void Ability::update(int levelOfAbility)
{
	switch(abilityType){
	case SPEED:
		if(levelOfAbility == 1){
			lengthOfCooldown = 15; // Time until boost can be used again
			boostValue = 0.5f; // Additional speed that the boost provides
		}
		else if(levelOfAbility == 2){
			lengthOfCooldown = 12; // Time until boost can be used again
			boostValue = 0.75f; // Additional speed that the boost provides
		}
		else if(levelOfAbility == 3){
			lengthOfCooldown = 9; // Time until boost can be used again
			boostValue = 1.0f; // Additional speed that the boost provides
		}
		break;
	case LASER:
		break;
	case ROCKET:
		break;
	case LANDMINE:
		break;
	}

	abilityLevel = levelOfAbility;
}

int Ability::getAbilityLevel()
{
	return abilityLevel;
}
