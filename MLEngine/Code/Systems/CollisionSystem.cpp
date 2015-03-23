#include "CollisionSystem.h"

std::vector<BoundingVolume*> CollisionSystem::dynamics;
std::vector<BoundingVolume*> CollisionSystem::statics;


CollisionSystem::~CollisionSystem()
{

}

void CollisionSystem::Init()
{
}

bool CollisionSystem::Update(float dt)
{
	return true;
}

void CollisionSystem::Destroy()
{
	dynamics.clear();
	statics.clear();
}

void CollisionSystem::AddStaticVolume(BoundingVolume* volume)
{
	NULLPTRCHECK(volume,"Static Volume is a null pointer");
	statics.push_back(volume);
}

void CollisionSystem::AddDynamicVolume(BoundingVolume* volume)
{
	NULLPTRCHECK(volume,"Dynamic Volume is a null pointer");
	dynamics.push_back(volume);
}

void CollisionSystem::CheckCollisions()
{
	if(dynamics.size()>0)
	{
		for(auto volume = dynamics.begin(); volume != dynamics.end(); ++volume)
		{
			(*volume)->SetCollided(false);
		}
		for(auto volume = statics.begin(); volume != statics.end(); ++volume)
		{
			(*volume)->SetCollided(false);
		}

		for(auto volumei = dynamics.begin(); volumei != dynamics.end(); ++volumei)
		{
			for(auto volumej = volumei+1; volumej != dynamics.end(); ++volumej)
			{
				if(CheckVolumes(*volumei,*volumej))
				{
					(*volumei)->SetCollided(true);
					(*volumej)->SetCollided(true);
				}
			}
			for(auto volumej = statics.begin(); volumej != statics.end(); ++volumej)
			{
				if(CheckVolumes(*volumei,*volumej))
				{
					(*volumei)->SetCollided(true);
					(*volumej)->SetCollided(true);
				}
			}
		}
	}
}

bool CollisionSystem::CheckVolumes(BoundingVolume* volumea, BoundingVolume* volumeb)
{
	NULLPTRCHECK(volumea,"volumea passed into CheckVolumes is a null pointer");
	NULLPTRCHECK(volumeb,"volumeb passed into CheckVolumes is a null pointer");

	if(volumea->GetType()==BOUNDING_TYPE_AABBOX && volumeb->GetType()==BOUNDING_TYPE_AABBOX)
	{
		BoundingBox* boxa = static_cast<BoundingBox*>(volumea);
		BoundingBox* boxb = static_cast<BoundingBox*>(volumeb);
		return HasCollided(boxa,boxb);
	}
	else if(volumea->GetType()==BOUNDING_TYPE_OBBOX && volumeb->GetType()==BOUNDING_TYPE_OBBOX)
	{
		BoundingBoxO* boxa = static_cast<BoundingBoxO*>(volumea);
		BoundingBoxO* boxb = static_cast<BoundingBoxO*>(volumeb);
		return HasCollided(boxa,boxb);
	}
	else if(volumea->GetType()==BOUNDING_TYPE_CAPSULE && volumeb->GetType()==BOUNDING_TYPE_OBBOX)
	{
		BoundingCapsule* cap = static_cast<BoundingCapsule*>(volumea);
		BoundingBoxO* box = static_cast<BoundingBoxO*>(volumeb);
		return HasCollided(box,cap);
	}
	else if(volumea->GetType()==BOUNDING_TYPE_OBBOX && volumeb->GetType()==BOUNDING_TYPE_CAPSULE)
	{
		BoundingBoxO* box = static_cast<BoundingBoxO*>(volumea);
		BoundingCapsule* cap = static_cast<BoundingCapsule*>(volumeb);
		return HasCollided(box,cap);
	}
	return false;
}

bool CollisionSystem::HasCollided(BoundingBox* boxa, BoundingBox* boxb)
{
	NULLPTRCHECK(boxa,"boxa passed into HasCollided is a null pointer");
	NULLPTRCHECK(boxb,"boxb passed into HasCollided is a null pointer");

	glm::vec3 amin = boxa->GetLeftBotFore() +  boxa->GetParentTransform()->GetPosition();
	glm::vec3 amax = boxa->GetRightTopRear() + boxa->GetParentTransform()->GetPosition();
	glm::vec3 bmin = boxb->GetLeftBotFore() +  boxb->GetParentTransform()->GetPosition();
	glm::vec3 bmax = boxb->GetRightTopRear() + boxb->GetParentTransform()->GetPosition();
	
	return(amax.x >= bmin.x &&
		amin.x <= bmax.x &&
		amax.y >= bmin.y &&
		amin.y <= bmax.y &&
		amax.z >= bmin.z &&
		amin.z <= bmax.z);
}

bool CollisionSystem::HasCollided(BoundingBoxO* boxa, BoundingBoxO* boxb)
{
	NULLPTRCHECK(boxa,"boxa passed into HasCollided is a null pointer");
	NULLPTRCHECK(boxb,"boxb passed into HasCollided is a null pointer");

	//Get transform and rotation of box a

	glm::vec3 cena = boxa->GetCenter() + boxa->GetParentTransform()->GetPosition();
	glm::mat4 rota = glm::rotate(glm::mat4(1.0), boxa->GetParentTransform()->GetRotation().x, glm::vec3(1, 0, 0));
	rota *= glm::rotate(boxa->GetParentTransform()->GetRotation().y,glm::vec3(0,1,0));							
	rota *= glm::rotate(boxa->GetParentTransform()->GetRotation().z,glm::vec3(0,0,1));

	//Get transform and rotation of box b

	glm::vec3 cenb = boxb->GetCenter() + boxb->GetParentTransform()->GetPosition();
	glm::mat4 rotb = glm::rotate(glm::mat4(1.0), boxb->GetParentTransform()->GetRotation().x, glm::vec3(1, 0, 0));
	rotb *= glm::rotate(boxb->GetParentTransform()->GetRotation().y,glm::vec3(0,1,0));
	rotb *= glm::rotate(boxb->GetParentTransform()->GetRotation().z,glm::vec3(0,0,1));

	//Get extents of boxes
	glm::vec3 a = boxa->GetExtent();
	glm::vec3 b = boxb->GetExtent();

	//Get the transpose of the combined rotation matrices
	glm::mat4 Rt = glm::transpose(rota*rotb);

	glm::mat4 Rabs;
	bool axesParallel;

	for ( unsigned int i = 0; i < 3; ++i )
	{
		for ( unsigned int j = 0; j < 3; ++j )
		{
			Rabs[i][j] = fabs(Rt[i][j]);
			if ( Rabs[i][j] + 0.01f >= 1.0f )
            {
                // box A and box B have nearly parallel axes
                axesParallel = true;
            }
		}
	}


	glm::vec4 c = glm::vec4(cenb - cena,1.0)*rota;

	float aTest, bTest, cTest;

	 // separating axis A0
    cTest = abs(c.x);
    aTest = a.x;
    bTest = b.x*Rabs[0][0]+b.y*Rabs[0][1]+b.z*Rabs[0][2];
    if ( cTest > aTest + bTest )
        return false;

    // separating axis A1
    cTest = abs(c.y);
    aTest = a.y;
    bTest = b.x*Rabs[1][0]+b.y*Rabs[1][1]+b.z*Rabs[1][2];
    if ( cTest > aTest + bTest )
        return false;

    // separating axis A2
    cTest = abs(c.z);
    aTest = a.z;
    bTest = b.x*Rabs[2][0]+b.y*Rabs[2][1]+b.z*Rabs[2][2];
    if ( cTest > aTest + bTest )
        return false;

    // separating axis B0
    cTest = abs( c.x*Rt[0][0] + c.y*Rt[1][0] + c.z*Rt[2][0] );
    aTest = a.x*Rabs[0][0]+a.y*Rabs[1][0]+a.z*Rabs[2][0];
    bTest = b.x;
    if ( cTest > aTest + bTest )
        return false;

    // separating axis B1
    cTest = abs( c.x*Rt[0][1] + c.y*Rt[1][1] + c.z*Rt[2][1] );
    aTest = a.x*Rabs[0][1]+a.y*Rabs[1][1]+a.z*Rabs[2][1];
    bTest = b.y;
    if ( cTest > aTest + bTest )
        return false;

    // separating axis B2
    cTest = abs( c.x*Rt[0][2] + c.y*Rt[1][2] + c.z*Rt[2][2] );
    aTest = a.x*Rabs[0][2]+a.y*Rabs[1][2]+a.z*Rabs[2][2];
    bTest = b.z;
    if ( cTest > aTest + bTest )
        return false;

	// if the two boxes have parallel axes, then there's an intersection
    if ( axesParallel )
        return true;


    // separating axis A0 x B0
    cTest = abs(c.z*Rt[1][0]-c.y*Rt[2][0]);
    aTest = a.y*Rabs[2][0] + a.z*Rabs[1][0];
    bTest = b.y*Rabs[0][2] + b.z*Rabs[0][1];
    if ( cTest > aTest + bTest )
        return false;

    // separating axis A0 x B1
    cTest = abs(c.z*Rt[1][1]-c.y*Rt[2][1]);
    aTest = a.y*Rabs[2][1] + a.z*Rabs[1][1];
    bTest = b.x*Rabs[0][2] + b.z*Rabs[0][0];
    if ( cTest > aTest + bTest )
        return false;

    // separating axis A0 x B2
    cTest = abs(c.z*Rt[1][2]-c.y*Rt[2][2]);
    aTest = a.y*Rabs[2][2] + a.z*Rabs[1][2];
    bTest = b.x*Rabs[0][1] + b.y*Rabs[0][0];
    if ( cTest > aTest + bTest )
        return false;

    // separating axis A1 x B0
    cTest = abs(c.x*Rt[2][0]-c.z*Rt[0][0]);
    aTest = a.x*Rabs[2][0] + a.z*Rabs[0][0];
    bTest = b.y*Rabs[1][2] + b.z*Rabs[1][1];
    if ( cTest > aTest + bTest )
        return false;

    // separating axis A1 x B1
    cTest = abs(c.x*Rt[2][1]-c.z*Rt[0][1]);
    aTest = a.x*Rabs[2][1] + a.z*Rabs[0][1];
    bTest = b.x*Rabs[1][2] + b.z*Rabs[1][0];
    if ( cTest > aTest + bTest )
        return false;

    // separating axis A1 x B2
    cTest = abs(c.x*Rt[2][2]-c.z*Rt[0][2]);
    aTest = a.x*Rabs[2][2] + a.z*Rabs[0][2];
    bTest = b.x*Rabs[1][1] + b.y*Rabs[1][0];
    if ( cTest > aTest + bTest )
        return false;

    // separating axis A2 x B0
    cTest = abs(c.y*Rt[0][0]-c.x*Rt[1][0]);
    aTest = a.x*Rabs[1][0] + a.y*Rabs[0][0];
    bTest = b.y*Rabs[2][2] + b.z*Rabs[2][1];
    if ( cTest > aTest + bTest )
        return false;

    // separating axis A2 x B1
    cTest = abs(c.y*Rt[0][1]-c.x*Rt[1][1]);
    aTest = a.x*Rabs[1][1] + a.y*Rabs[0][1];
    bTest = b.x*Rabs[2][2] + b.z*Rabs[2][0];
    if ( cTest > aTest + bTest )
        return false;

    // separating axis A2 x B2
    cTest = abs(c.y*Rt[0][2]-c.x*Rt[1][2]);
    aTest = a.x*Rabs[1][2] + a.y*Rabs[0][2];
    bTest = b.x*Rabs[2][1] + b.y*Rabs[2][0];
    if ( cTest > aTest + bTest )
        return false;

    // all tests failed, have intersection
    return true;
} 

bool CollisionSystem::HasCollided(BoundingBoxO* box, BoundingCapsule* capsule)
{
	NULLPTRCHECK(box,"box passed into HasCollided is a null pointer");
	NULLPTRCHECK(capsule,"capsule passed into HasCollided is a null pointer");

	//Get transform and rotation of box

	glm::vec3 cena = box->GetCenter() + box->GetParentTransform()->GetPosition();
	glm::mat4 rota = glm::rotate(glm::mat4(1.0), box->GetParentTransform()->GetRotation().x, glm::vec3(1, 0, 0));
	rota *= glm::rotate(box->GetParentTransform()->GetRotation().y,glm::vec3(0,1,0));							
	rota *= glm::rotate(box->GetParentTransform()->GetRotation().z,glm::vec3(0,0,1));

	//Get transform and rotation of capsule

	glm::vec3 cenb = capsule->GetCenter() + capsule->GetParentTransform()->GetPosition();
	glm::mat4 rotb = glm::rotate(glm::mat4(1.0), capsule->GetParentTransform()->GetRotation().x, glm::vec3(1, 0, 0));
	rotb *= glm::rotate(capsule->GetParentTransform()->GetRotation().y,glm::vec3(0,1,0));
	rotb *= glm::rotate(capsule->GetParentTransform()->GetRotation().z,glm::vec3(0,0,1));

	glm::vec3 l = glm::vec3(glm::vec4(0.0,1.0,0.0,1.0) * rotb);
	float radius = capsule->GetRadius();
	glm::vec3 a = box->GetExtent()+glm::vec3(radius,radius,radius);
	float b = capsule->GetExtent();
	
	glm::vec4 c = glm::vec4(cenb - cena,1.0)*rota;

	if( fabs(c.x) > a.x + b*fabs(l.x))
	{
		return false;
	}
	if( fabs(c.y) > a.y + b*fabs(l.y))
	{
		return false;
	}
	if( fabs(c.z) > a.z + b*fabs(l.z))
	{
		return false;
	}
	
	float r = a.y*fabs(l.z) + a.z*fabs(l.y);
	if( fabs(c.y*l.z - c.z*l.y) > r )
	{
		return false;
	}

	r = a.x*fabs(l.z) + a.z*fabs(l.x);
	if( fabs(c.z*l.x - c.x*l.z) > r )
	{
		return false;
	}

	r = a.x*fabs(l.y) + a.y*fabs(l.x);
	if( fabs(c.x*l.y - c.y*l.x) > r )
	{
		return false;
	}

	return true; 
}