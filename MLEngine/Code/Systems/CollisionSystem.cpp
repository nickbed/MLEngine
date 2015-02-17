#include "CollisionSystem.h"

std::vector<BoundingVolume*> CollisionSystem::dynamics;
std::vector<BoundingVolume*> CollisionSystem::statics;

void CollisionSystem::Init()
{
}

bool CollisionSystem::Update(float dt)
{
	return true;
}

void CollisionSystem::Destroy()
{
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
		for(auto volumei = dynamics.begin(); volumei != dynamics.end()-1; ++volumei)
		{
			for(auto volumej = volumei+1; volumej != dynamics.end(); ++volumej)
			{
				if(CheckVolumes(*volumei,*volumej))
				{
					std::cout << "Collision message spam activate!!!";
				}
			}
			for(auto volumej = statics.begin(); volumej != statics.end(); ++volumej)
			{
				if(CheckVolumes(*volumei,*volumej))
				{
					std::cout << "Collision message spam activate!!!";
				}
			}
		}
	}
}

bool CollisionSystem::CheckVolumes(BoundingVolume* volumea, BoundingVolume* volumeb)
{
	NULLPTRCHECK(volumea,"volumea passed into CheckVolumes is a null pointer");
	NULLPTRCHECK(volumeb,"volumeb passed into CheckVolumes is a null pointer");

	if(volumea->getType()==BOUNDING_TYPE_AABBOX && volumeb->getType()==BOUNDING_TYPE_AABBOX)
	{
		BoundingBox* boxa = static_cast<BoundingBox*>(volumea);
		BoundingBox* boxb = static_cast<BoundingBox*>(volumeb);
		return HasCollided(boxa,boxb);
	}
	else if(volumea->getType()==BOUNDING_TYPE_OBBOX && volumeb->getType()==BOUNDING_TYPE_OBBOX)
	{
		BoundingBoxO* boxa = static_cast<BoundingBoxO*>(volumea);
		BoundingBoxO* boxb = static_cast<BoundingBoxO*>(volumeb);
		return HasCollided(boxa,boxb);
	}
	return false;
}

bool CollisionSystem::HasCollided(BoundingBox* boxa, BoundingBox* boxb)
{
	NULLPTRCHECK(boxa,"boxa passed into HasCollided is a null pointer");
	NULLPTRCHECK(boxb,"boxb passed into HasCollided is a null pointer");

	glm::vec3 amin = boxa->GetLeftBotFore() +  boxa->getTransform()->GetPosition();
	glm::vec3 amax = boxa->GetRightTopRear() + boxa->getTransform()->GetPosition();
	glm::vec3 bmin = boxb->GetLeftBotFore() +  boxb->getTransform()->GetPosition();
	glm::vec3 bmax = boxb->GetRightTopRear() + boxb->getTransform()->GetPosition();
	
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

	glm::vec3 cena = boxa->GetCenter()+boxa->getTransform()->GetPosition();
	glm::mat4 rota = glm::rotate(boxa->getTransform()->GetRotation().x,glm::vec3(1,0,0));
	rota = glm::rotate(rota,boxa->getTransform()->GetRotation().y,glm::vec3(0,1,0));
	rota = glm::rotate(rota,boxa->getTransform()->GetRotation().z,glm::vec3(0,0,1));
	glm::vec3 axisax = glm::vec3(glm::vec4(1,0,0,1)*rota); 
	glm::vec3 axisay = glm::vec3(glm::vec4(0,1,0,1)*rota);
	glm::vec3 axisaz = glm::vec3(glm::vec4(0,0,1,1)*rota);

	glm::vec3 cenb = boxb->GetCenter()+boxb->getTransform()->GetPosition();
	glm::mat4 rotb = glm::rotate(boxb->getTransform()->GetRotation().x,glm::vec3(1,0,0));
	glm::rotate(rotb,boxb->getTransform()->GetRotation().y,glm::vec3(0,1,0));
	glm::rotate(rotb,boxb->getTransform()->GetRotation().z,glm::vec3(0,0,1));
	glm::vec3 axisbx = glm::vec3(glm::vec4(1,0,0,1)*rotb); 
	glm::vec3 axisby = glm::vec3(glm::vec4(0,1,0,1)*rotb);
	glm::vec3 axisbz = glm::vec3(glm::vec4(0,0,1,1)*rotb);

	glm::vec3 v = cenb - cena;
	glm::vec3 T(glm::dot(v,axisax),glm::dot(v,axisay),glm::dot(v,axisaz));

	glm::mat3 R;
	float ra, rb, t;

	R[0][0] = glm::dot(axisax,axisbx);
	R[0][1] = glm::dot(axisax,axisby);
	R[0][2]	= glm::dot(axisax,axisbz);
	R[1][0]	= glm::dot(axisay,axisbx);
	R[1][1]	= glm::dot(axisay,axisby);
	R[1][2]	= glm::dot(axisay,axisbz);
	R[2][0]	= glm::dot(axisaz,axisbx);
	R[2][1]	= glm::dot(axisaz,axisby);
	R[2][2]	= glm::dot(axisaz,axisbz);

	glm::vec3 a = boxa->GetExtent();
	glm::vec3 b = boxa->GetExtent();

	for(int i=0 ; i<3 ; i++ )
	{
		ra = a[i];
		rb = b[0]*fabs(R[i][0]) + b[1]*fabs(R[i][1]) + b[2]*fabs(R[i][2]);
		t = fabs( T[i] );
		if( t > ra + rb )
		{
			return false;
		}
	}

	for(int k=0 ; k<3 ; k++ )
	{
		ra = a[0]*fabs(R[0][k]) + a[1]*fabs(R[1][k]) + a[2]*fabs(R[2][k]); 
		rb = b[k];
		t =	fabs( T[0]*R[0][k] + T[1]*R[1][k] + T[2]*R[2][k] );
		if( t > ra + rb )
		{
			return false;
		}
	}

	ra = a[1]*fabs(R[2][0]) + a[2]*fabs(R[1][0]);
	rb = b[1]*fabs(R[0][2]) + b[2]*fabs(R[0][1]);
	t =	fabs(T[2]*R[1][0] - T[1]*R[2][0]);
	if( t > ra + rb )
	{
		return false;
	}

	ra = a[1]*fabs(R[2][1]) + a[2]*fabs(R[1][1]);
	rb = b[0]*fabs(R[0][2]) + b[2]*fabs(R[0][0]);
	t =	fabs(T[2]*R[1][1] - T[1]*R[2][1]);
	if( t > ra + rb )
	{
		return false;
	}

	ra = a[1]*fabs(R[2][2]) + a[2]*fabs(R[1][2]);
	rb = b[0]*fabs(R[0][1]) + b[1]*fabs(R[0][0]);
	t =	fabs(T[2]*R[1][2] - T[1]*R[2][2]);
	if( t > ra + rb )
	{
		return false;
	}

	ra = a[0]*fabs(R[2][0]) + a[2]*fabs(R[0][0]);
	rb = b[1]*fabs(R[1][2]) + b[2]*fabs(R[1][1]);
	t =	fabs(T[0]*R[2][0] - T[2]*R[0][0]);
	if( t > ra + rb )
	{
		return false;
	}

	ra = a[0]*fabs(R[2][1]) + a[2]*fabs(R[0][1]);
	rb = b[0]*fabs(R[1][2]) + b[2]*fabs(R[1][0]);
	t =	fabs(T[0]*R[2][1] - T[2]*R[0][1]);
	if( t > ra + rb )
	{
		return false;
	}

	ra = a[0]*fabs(R[2][2]) + a[2]*fabs(R[0][2]);
	rb = b[0]*fabs(R[1][1]) + b[1]*fabs(R[1][0]);
	t =	fabs(T[0]*R[2][2] - T[2]*R[0][2]);
	if( t > ra + rb )
	{
		return false;
	}

	ra = a[0]*fabs(R[1][0]) + a[1]*fabs(R[0][0]);
	rb = b[1]*fabs(R[2][2]) + b[2]*fabs(R[2][1]);
	t =	fabs(T[1]*R[0][0] - T[0]*R[1][0]);

	if( t > ra + rb )
	{
		return false;
	}

	ra = a[0]*fabs(R[1][1]) + a[1]*fabs(R[0][1]);
	rb = b[0] *fabs(R[2][2]) + b[2]*fabs(R[2][0]);
	t =	fabs(T[1]*R[0][1] - T[0]*R[1][1]);
	if( t > ra + rb )
	{
		return false;
	}
	
	ra = a[0]*fabs(R[1][2]) + a[1]*fabs(R[0][2]);
	rb = b[0]*fabs(R[2][1]) + b[1]*fabs(R[2][0]);
	t =	fabs(T[1]*R[0][2] - T[0]*R[1][2]);
	if( t > ra + rb )
	{
		return false;
	}

	return true; 
}