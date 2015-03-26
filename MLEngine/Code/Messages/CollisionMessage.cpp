#include "CollisionMessage.h"

CollisionManifold::CollisionManifold()
{
	Collision = false;
}

CollisionManifold::CollisionManifold(BoundingVolume* volumea, BoundingVolume* volumeb, float pen, glm::vec3 axis, char axisBox, float sign, bool top)
{
	Collision = true;
	VolumeA = volumea;
	VolumeB = volumeb;
	Penetration = pen;
	Axis = axis;
	AxisBox = axisBox;
	Sign = sign;
	Top = top;
}

namespace mauvemessage
{
	CollisionMessage::CollisionMessage(const char* messageType, CollisionManifold content) : contents(content), mauvemessage::BaseMessage(messageType){}

	CollisionMessage::operator CollisionManifold()
	{
		return contents;
	}
}