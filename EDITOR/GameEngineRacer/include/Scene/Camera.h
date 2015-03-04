#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS
#include "glm\glm.hpp"
#include "GameObjects\GameObject.h"
#include "glm\gtx\rotate_vector.hpp"
class Camera
{
private:
	float _pitch;
	float _yaw;

	float _fieldOfView;
	float _nearPlane;
	float _farPlane;
	float _aspectRatio;

	glm::vec3 _position;
	glm::vec3 _target;
	glm::vec3 _right;
	glm::vec3 _up;
	glm::vec3 _forward;
	
	glm::mat4 _view;
	glm::mat4 _projection;

	std::string m_name;

public:
	Camera();
	~Camera();


	const std::string& getName()const{return m_name;};
	void setName(const std::string& name){m_name = name;};


	const glm::vec3& position() const; //position getter method
	void setPosition(const glm::vec3& position); //position setter method

	float fieldOfView() const; //fieldOfView getter method
	void setFieldOfView(float fieldOfView); //fieldOfView setter method

	float aspectRatio() const; //aspectRatio getter method
	void setAspectRatio(float aspectRatio);  //aspectRatio setter method

	float nearPlane() const;  //nearPlane getter method
	float farPlane() const;   //farPlane getter method
	void setNearAndFarPlanes(float nearPlane, float farPlane); //nearPlane and farPLane setter method


	void rotate(const float x, const float y); //Rotate camera
	void pan(const float x, const float y);  //Pan camera
	void zoom(const float z); //Zoom camera

	void lookAt(const glm::vec3& target); //Set up the view matrix
	const glm::vec3& getTarget()const{return _target;};

	void setYaw(float yaw);
	void setPitch(float pitch);
	float getPitch()const{return _pitch;};
	float getYaw()const{return _yaw;};

	void update();  //Update the camera

	void init(); //Reset the camera

	glm::mat4 view(); //Get the View matrix

	glm::mat4 projection(); //Get the Projection matrix
};
#endif
