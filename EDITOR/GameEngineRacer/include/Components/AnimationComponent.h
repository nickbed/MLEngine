#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H
#include <string>
// constants for arm and leg movement states
	const char BACKWARD_STATE = 0;
	const char FORWARD_STATE  = 1;

	// index constants for accessing arm and leg array data
	const char LEFT  = 0;
	const char RIGHT = 1;

class AnimationComponent
{
private:
 
	bool state;
	char States[2];	
	float Angles[2];
	
public:
	AnimationComponent();
	~AnimationComponent();
	void init(std::string nState);
	void Prepare();
	float getAngle()const;
	bool getState()const{return state;};
};
#endif

