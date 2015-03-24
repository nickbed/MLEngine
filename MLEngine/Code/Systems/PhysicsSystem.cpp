#include "PhysicsSystem.h"

void PhysicsSystem::Init()
{
	mauvemessage::RecieverInfo rec;
	rec.listenobjectptr = this;
	using namespace std::placeholders;
	rec.listnerFunction = &PhysicsSystem::msg_HandleCollision;
	rec.typeToListen = MSG_COLLISION;
	mauvemessage::MessageManager::AddMessageListner(MSG_COLLISION, rec);
}

bool PhysicsSystem::Update(float dt)
{
	std::vector<BoundingVolume*> dynamics = CollisionSystem::dynamics;
	for(auto volume = dynamics.begin(); volume != dynamics.end(); ++volume)
	{
		(*volume)->GetParentTransform()->SetRotation((*volume)->GetParentTransform()->GetRotation()+(*volume)->Rigid_ang*dt);
		(*volume)->GetParentTransform()->SetPosition((*volume)->GetParentTransform()->GetPosition()+(*volume)->Rigid_vel*dt);
	}
	IEntity::Update(dt);
	return true;
}

void PhysicsSystem::msg_HandleCollision(mauvemessage::BaseMessage* msg)
{
	mauvemessage::CollisionMessage* colMsg = static_cast<mauvemessage::CollisionMessage*>(msg);
	CollisionManifold collision = *colMsg;
	
	if(collision.VolumeA->GetType()==BOUNDING_TYPE_OBBOX && collision.VolumeB->GetType()==BOUNDING_TYPE_OBBOX)
	{
		if(collision.VolumeB->IsStatic())
		{
			BoundingVolume* boxa = collision.VolumeA;
			glm::mat4 rota = glm::rotate(glm::mat4(1.0), boxa->GetParentTransform()->GetRotation().x, glm::vec3(1,0,0));
			rota *= glm::rotate(boxa->GetParentTransform()->GetRotation().y,glm::vec3(0,1,0));						
			rota *= glm::rotate(boxa->GetParentTransform()->GetRotation().z,glm::vec3(0,0,1));
			glm::vec4 translation(1.0);
			if(collision.AxisBox=='a')
			{
				translation = glm::vec4(collision.Axis,1.0)*-collision.Sign*collision.Penetration;
			}
			else if(collision.AxisBox=='b')
			{
				translation = (glm::vec4(collision.Axis,1.0))*collision.Sign*collision.Penetration;
			}
			//std::cout << collision.AxisBox << collision.Penetration << " " << collision.Sign;					// box axis belongs to and penetration depth
			//std::cout << " " << translation.x << " " << translation.y << " " << translation.z << std::endl;	// translation
			//mauvemessage::PositionMessage posMsg("robotMovement",glm::vec3(translation));
			//mauvemessage::MessageManager::SendListnerMessage(&posMsg,"robotMovement");
			collision.VolumeA->GetParentTransform()->SetPosition(collision.VolumeA->GetParentTransform()->GetPosition()+glm::vec3(translation));
		}
		else if(!collision.VolumeA->IsStatic() && !collision.VolumeB->IsStatic())
		{
			//push box
		}
	}
	if(collision.VolumeA->GetType()==BOUNDING_TYPE_OBBOX && collision.VolumeB->GetType()==BOUNDING_TYPE_CAPSULE)
	{
		//if(collision.VolumeA->IsStatic() && !collision.VolumeB->IsStatic())
		//{
		//	BoundingVolume* capsule = collision.VolumeB;
		//	glm::vec3 captranslate = collision.Axis*collision.Sign*collision.Penetration;
		//	//std::cout << collision.AxisBox << collision.Penetration << " " << collision.Sign;						// box axis belongs to and penetration depth
		//	//std::cout << " " << translation.x << " " << translation.y << " " << translation.z << std::endl;		// translation
		//	collision.VolumeB->GetParentTransform()->SetPosition(collision.VolumeB->GetParentTransform()->GetPosition()+captranslate);
		//}
		/*else if(!collision.VolumeA->IsStatic() && !collision.VolumeB->IsStatic())
		{*/
		BoundingCapsule* capsule = static_cast<BoundingCapsule*>(collision.VolumeB);
		BoundingBoxO* box = static_cast<BoundingBoxO*>(collision.VolumeA);
		glm::vec3 point = capsule->GetParentTransform()->GetPosition() + (collision.Axis * capsule->GetExtent()-collision.Penetration);
		box->Rigid_vel = box->Rigid_inverse * (box->GetParentTransform()->GetPosition()-point) * glm::vec3(1,0,1);
		box->Rigid_ang = box->Rigid_inverse * glm::cross(point,box->GetParentTransform()->GetPosition()) * -collision.Sign * glm::vec3(0,1,0);
		glm::vec3 captranslate = collision.Axis*collision.Sign*collision.Penetration;
		collision.VolumeB->GetParentTransform()->SetPosition(collision.VolumeB->GetParentTransform()->GetPosition()+captranslate);
		/*}*/
	}
}

void PhysicsSystem::Destroy()
{
}