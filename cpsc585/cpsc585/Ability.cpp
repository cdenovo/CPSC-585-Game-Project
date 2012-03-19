#include "Ability.h"


Ability::Ability(AbilityType _abilityType)
{
	abilityType = _abilityType;
	cooldownTime = 0;

	switch(abilityType){
	case SPEED:
		lengthOfCooldown = 15; // Time until boost can be used again
		boostValue = 0.5f; // Additional speed that the boost provides
		boostDuration = 3; // Time in seconds that the boost last
		break;
	case LASER:
		break;
	case LEFT:
		break;
	case RIGHT:
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
	oldTime = time(NULL);
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

void Ability::updateCooldown()
{
	newTime = time(NULL);
		
	cooldownTime = (float)(cooldownTime - difftime(newTime, oldTime));

	oldTime = newTime;
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