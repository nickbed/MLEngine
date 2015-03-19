#include "CollisionMessage.h"

namespace mauvemessage
{
	CollisionMessage::CollisionMessage(const char* messageType, CollisionManifold content) : contents(content), mauvemessage::BaseMessage(messageType){}

	CollisionMessage::operator CollisionManifold()
	{
		return contents;
	}
}