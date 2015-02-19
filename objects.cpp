#include "objects.h"

Object::Object( void ) {
	state = null;
}

Object::~Object( void ) {
	if (null != state)
		delete state;
}

string Object::Name(void)
{
	return name;
}

GLfloat Object::Radius(void)
{
	return radius;
}

double* Object::GetWorldLocation( void )
{
	return location;
}

void Object::SetWorldLocation(double *newLocation)
{
	memcpy( location, newLocation, 3*sizeof(double) );
}

ObjectState* Object::GetObjectState( void ){
	return state;
}

void Object::SetObjectState( ObjectState* objectState ){
	if (null == state)
		state = new ObjectState;
	memcpy( state, objectState, sizeof(ObjectState) );
}

// Draw a set of coloured cones representing the current local coord system.
// X -> red, Y -> green, Z -> blue.
void DrawAxis(void) {
  float axisHeight = 0.75; // TODO
  float axisBase = 0.075;
  glColor3f(0, 0, 1);
  glutSolidCone(axisBase, axisHeight, 8, 2);
  glPushMatrix();
  glRotatef(90, 0, 1, 0);
  glColor3f(1, 0, 0);
  glutSolidCone(axisBase, axisHeight, 8, 2);
  glPopMatrix();
  glPushMatrix();
  glRotatef(-90, 1, 0, 0);
  glColor3f(0, 1, 0);
  glutSolidCone(axisBase, axisHeight, 8, 2);
  glPopMatrix();
}