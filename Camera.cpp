#include "pch.h"
#include "Camera.h"

//camera for our app simple directX application. While it performs some basic functionality its incomplete. 
//

Camera::Camera()
{
	//initalise values. 
	//Orientation and Position are how we control the camera. 
	m_orientation.x = -90.0f;		//rotation around x - pitch
	m_orientation.y = 0.0f;		//rotation around y - yaw
	m_orientation.z = 0.0f;		//rotation around z - roll	//we tend to not use roll a lot in first person

	m_position.x = 0.0f;		//camera position in space. 
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	//These variables are used for internal calculations and not set.  but we may want to queary what they 
	//externally at points
	m_lookat.x = 0.0f;		//Look target point
	m_lookat.y = 0.0f;
	m_lookat.z = 0.0f;

	m_forward.x = 0.0f;		//forward/look direction
	m_forward.y = 0.0f;
	m_forward.z = 0.0f;

	m_right.x = 0.0f;
	m_right.y = 0.0f;
	m_right.z = 0.0f;
	
	//
	m_movespeed = 0.30;
	m_camRotRate = 3.0;

	//force update with initial values to generate other camera data correctly for first update. 
	Update();
}


Camera::~Camera()
{
}

void Camera::Update()
{
	//rotation in yaw - using the paramateric equation of a circle
	m_forward.x = sin((m_orientation.y)*3.1415f / 180.0f);
	m_forward.z = cos((m_orientation.y)*3.1415f / 180.0f);
	m_forward.Normalize();

	//create right vector from look Direction
	m_forward.Cross(DirectX::SimpleMath::Vector3::UnitY, m_right);

	//update lookat point
	m_lookat = m_position + m_forward;

	//apply camera vectors and create camera matrix
	m_cameraMatrix = (DirectX::SimpleMath::Matrix::CreateLookAt(m_position, m_lookat, DirectX::SimpleMath::Vector3::UnitY));


}

DirectX::SimpleMath::Matrix Camera::getCameraMatrix()
{
	return m_cameraMatrix;
}

void Camera::setPosition(DirectX::SimpleMath::Vector3 newPosition)
{
	m_position = newPosition;
}

DirectX::SimpleMath::Vector3 Camera::getPosition()
{
	return m_position;
}

DirectX::SimpleMath::Vector3 Camera::getForward()
{
	return m_forward;
}

void Camera::setRotation(DirectX::SimpleMath::Vector3 newRotation)
{
	m_orientation = newRotation;
}

DirectX::SimpleMath::Vector3 Camera::getRotation()
{
	return m_orientation;
}

float Camera::getMoveSpeed()
{
	return m_movespeed;
}

float Camera::getRotationSpeed()
{
	return m_camRotRate;
}

void Camera::RenderReflection(float height) {
	DirectX::SimpleMath::Vector3 up, position, lookAt;
	float radians;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 0.1f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	// For planar reflection invert the Y position of the camera.
	position.x = m_position.x;
	position.y = -m_position.y + (height * 0.5f);
	position.z = m_position.z;


	// Calculate the rotation in radians.
	//radians = m_orientation.y * 0.0174532925f;
	radians = m_orientation.y*(2*3.1315926f/360.0);

	// Setup where the camera is looking.
	lookAt.x = sinf(radians) + m_position.x;
	lookAt.y = position.y;
	lookAt.z = cosf(radians) + m_position.z;
	/*lookAt.x = m_lookat.x;
	lookAt.y = m_lookat.y;
	lookAt.z = m_lookat.z;*/
	// Create the view matrix from the three vectors.
	m_reflectionMatrix = (DirectX::SimpleMath::Matrix::CreateLookAt(position, lookAt, up));

	return;
}
DirectX::SimpleMath::Matrix Camera::getReflectionMatrix()
{
	return m_reflectionMatrix;
}