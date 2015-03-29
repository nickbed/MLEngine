#ifndef CollisionMessage_H
#define CollisionMessage_H
#include "BaseMessage.h"
#include "GLM\vec3.hpp"
#include "..\Components\BoundingVolume.h"

struct CollisionManifold
{
	CollisionManifold();
	CollisionManifold(BoundingVolume* volumea, BoundingVolume* volumeb, float pen, glm::vec3 axis, char axisBox, float sign, bool top);
	bool Collision;
	BoundingVolume* VolumeA;
	BoundingVolume* VolumeB;
	float Penetration;
	glm::vec3 Axis;
	char AxisBox;
	float Sign;
	bool Top;
};

namespace mauvemessage
{
	//Class for sending position data to any listners
	class CollisionMessage : public BaseMessage
	{
	public:
		CollisionMessage(const char* messageType, CollisionManifold content);

		//Conversion straight to int
		operator CollisionManifold();
	private:
		CollisionManifold contents;
	};
}
#endif
