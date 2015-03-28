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
	//std::vector<BoundingVolume*> dynamics = CollisionSystem::dynamics;
	for(auto volume = CollisionSystem::dynamics.begin(); volume != CollisionSystem::dynamics.end(); ++volume)
	{
		float dragLinear = (*volume)->Rigid_vel.length();
		glm::vec3 linearForce = (*volume)->Rigid_vel/dragLinear;
		dragLinear = (*volume)->Rigid_friction * 100.f * dragLinear;
		linearForce*= -dragLinear;
		float dragAng = (*volume)->Rigid_ang.length();
		glm::vec3 angularForce = (*volume)->Rigid_ang/dragAng;
		dragAng = 2.f* dragAng;
		angularForce*= -dragAng;
		(*volume)->Rigid_vel += linearForce * (dt/2);
		(*volume)->Rigid_ang += angularForce * (dt/2);
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
		BoundingBoxO* boxa = static_cast<BoundingBoxO*>(collision.VolumeA);
		BoundingBoxO* boxb = static_cast<BoundingBoxO*>(collision.VolumeB);
		glm::mat4 rota = glm::rotate(glm::mat4(1.0), boxa->GetParentTransform()->GetRotation().x, glm::vec3(1,0,0));
		rota *= glm::rotate(boxa->GetParentTransform()->GetRotation().y,glm::vec3(0,1,0));						
		rota *= glm::rotate(boxa->GetParentTransform()->GetRotation().z,glm::vec3(0,0,1));
		float totalMass = boxa->Rigid_mass+boxb->Rigid_mass;
		glm::vec4 translation(1.0);
		glm::vec3 point = boxa->GetParentTransform()->GetPosition() + (collision.Axis * boxa->GetExtent());
		boxa->Rigid_vel = (boxa->Rigid_mass/totalMass) * boxa->Rigid_inverse * (boxa->GetParentTransform()->GetPosition()-point) * glm::vec3(1,0,1);
		boxb->Rigid_vel = (boxb->Rigid_mass/totalMass) * boxb->Rigid_inverse * (boxb->GetParentTransform()->GetPosition()-point) * glm::vec3(1,0,1);
		float inertiaa = (1.f/12.f*boxa->Rigid_mass)*(pow((boxa->GetExtent().y*2),2)+pow((boxa->GetExtent().z*2),2));
		float inertiab = (1.f/12.f*boxb->Rigid_mass)*(pow((boxb->GetExtent().y*2),2)+pow((boxb->GetExtent().z*2),2));
		boxa->Rigid_ang = 1.f/inertiaa * glm::cross(point,boxa->GetParentTransform()->GetPosition()) * collision.Sign * glm::vec3(0,1,0);
		boxb->Rigid_ang = 1.f/inertiab * glm::cross(point,boxb->GetParentTransform()->GetPosition()) * collision.Sign * glm::vec3(0,1,0);
		//std::cout << collision.AxisBox << collision.Penetration << " " << collision.Sign;					// box axis belongs to and penetration depth
	}
	else if(collision.VolumeA->GetType()==BOUNDING_TYPE_OBBOX && collision.VolumeB->GetType()==BOUNDING_TYPE_CAPSULE)
	{
		//if(collision.VolumeA->IsStatic() && !collision.VolumeB->IsStatic())
		//{
		//	BoundingVolume* capsule = collision.VolumeB;
		//	glm::vec3 captranslate = collision.Axis*collision.Sign*collision.Penetration;
		//	collision.VolumeB->GetParentTransform()->SetPosition(collision.VolumeB->GetParentTransform()->GetPosition()+captranslate);
		//}
		/*else if(!collision.VolumeA->IsStatic() && !collision.VolumeB->IsStatic())
		{*/
		BoundingCapsule* capsule = static_cast<BoundingCapsule*>(collision.VolumeB);
		BoundingBoxO* box = static_cast<BoundingBoxO*>(collision.VolumeA);
		glm::mat4 rot = glm::rotate(glm::mat4(1.0), box->GetParentTransform()->GetRotation().x, glm::vec3(1,0,0));
		rot *= glm::rotate(box->GetParentTransform()->GetRotation().y,glm::vec3(0,1,0));						
		rot *= glm::rotate(box->GetParentTransform()->GetRotation().z,glm::vec3(0,0,1));
		glm::vec3 point = capsule->GetParentTransform()->GetPosition() + (collision.Axis * capsule->GetExtent()-collision.Penetration);
		if(collision.VolumeA->Rigid_push>0.f)
		{
			capsule->Rigid_vel = 1.f/capsule->Rigid_mass  * (capsule->GetParentTransform()->GetPosition()-point) *  glm::vec3(1,0,1) * box->Rigid_push * -collision.Sign; //knockback
		}
		if(!collision.Top)
		{
			box->Rigid_vel = box->Rigid_inverse * (box->GetParentTransform()->GetPosition()-point) * glm::vec3(1,0,1);
			float inertia = (1.f/12.f*box->Rigid_inverse)*(pow((box->GetExtent().y*2),2)+pow((box->GetExtent().z*2),2));
			box->Rigid_ang = 1.f/inertia * glm::cross(point,box->GetParentTransform()->GetPosition()) * -collision.Sign * glm::vec3(0,1,0);
		}
		glm::vec3 captranslate = collision.Axis*collision.Sign*collision.Penetration;
		collision.VolumeB->GetParentTransform()->SetPosition(collision.VolumeB->GetParentTransform()->GetPosition()+captranslate);
		//std::cout << collision.AxisBox << collision.Penetration << " " << collision.Sign;						// box axis belongs to and penetration depth
		//std::cout << " Translation:" << captranslate.x << " " << captranslate.y << " " << captranslate.z << std::endl;		// translation
		/*}*/
	}
}

void PhysicsSystem::Destroy()
{
}