#include "objects.h"

Camera::Camera(void)
{
	location[X] = 0;
	location[Y] = 0;
	location[Z] = 0;
}

void Camera::GetWorldLocation(GLfloat *curLocation)
{
	curLocation[X] = location[X];
	curLocation[Y] = location[Y];
	curLocation[Z] = location[Z];
}

void Camera::SetWorldLocation(GLfloat *newLocation)
{
	location[X] = newLocation[X];
	location[Y] = newLocation[Y];
	location[Z] = newLocation[Z];
}

void Camera::SetAltitude(GLfloat newAltitude)
{
	altitude = newAltitude;
	location[Y] = altitude;
}

void Camera::ResetPosition(void)
{
	location[X] = 0;
	location[Y] = altitude;
	location[Z] = 0;
}