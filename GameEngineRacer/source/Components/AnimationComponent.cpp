#include "Components\AnimationComponent.h"
#include "glm\glm.hpp"


AnimationComponent::AnimationComponent()
{
	Angles[LEFT] = 0.0;
	Angles[RIGHT] = 0.0;

	States[LEFT] = FORWARD_STATE;
	States[RIGHT] = BACKWARD_STATE;

	
}

AnimationComponent::~AnimationComponent()
{
}
void AnimationComponent::init(std::string nState)
{
	if(nState == "LEFT")
	{
		state = FORWARD_STATE;
	}
	else if(nState == "RIGHT")
	{
		state = BACKWARD_STATE;
	}

}
void AnimationComponent::Prepare()
{
	float speed = 33.f;
    float angleLimit = 200.0f;
	for (char side = 0; side < 2; side++)
	{
		if (States[side] == FORWARD_STATE)
			Angles[side] += speed ;
		else
			Angles[side] -= speed ;

		if (Angles[side] >= angleLimit)
		{
			States[side] = BACKWARD_STATE;
		}
		else if (Angles[side] <= -angleLimit)
		{
			States[side] = FORWARD_STATE;
		}
	}
}
float AnimationComponent::getAngle()const
{
	return Angles[state];
}
