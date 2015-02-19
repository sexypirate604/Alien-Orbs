#include "objects.h"

SpaceCraft::SpaceCraft(string player, float initSpeed)
{
	name = player;

	length = 2.0;
	width = 1.0;
	radius = (length > width)? length : width;

	// In camera coords.
	location[X] = 0;
	location[Y] = 0;
	location[Z] = -10;

	rgb[0] = 255;
	rgb[1] = 0;
	rgb[2] = 0;

	speed = initSpeed;
	movingLeft = false;
	movingRight = false;

	cameraX = 0;
	cameraY = -2;
	cameraZ = -8;
}

void DrawTetrahedron( GLfloat width, GLfloat height )
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(0.5);
	
	// Draw base.
	glBegin(GL_POLYGON);
	glVertex3f( width/2, width/2, 0.0 );
	glVertex3f( width/2, -1*width/2, 0.0) ;
	glVertex3f( -1*width/2, -1*width/2, 0.0 );
	glVertex3f( -1*width/2, width/2, 0.0 );
	glEnd();
	
	// Draw right side.
	glBegin(GL_POLYGON);	
	glVertex3f( width/2, width/2, 0.0 );
	glVertex3f( width/2, -1*width/2, 0.0 );
	glVertex3f( 0.0, 0.0, -1*height );
	glEnd();

	// Draw left side.
	glBegin(GL_POLYGON);
	glVertex3f( -1*width/2, -1*width/2, 0.0 );
	glVertex3f( -1*width/2, width/2, 0.0 );
	glVertex3f( 0.0, 0.0, -1*height );
	glEnd();
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void SpaceCraft::Draw(void)
{
	glPushMatrix();
	// Move distance spacecraft from camera.
	glTranslatef( cameraX, cameraY, cameraZ );
	if (movingLeft) {
		glRotatef(ShipRotate, 0, 0, 1);
	}
	else if (movingRight) {
		glRotatef(-1*ShipRotate, 0, 0, 1);	
	}
	if (movingUp) {
		glRotatef(ShipRotate, 1, 0, 0);
	}
	else if (movingDown) {
		glRotatef(-1*ShipRotate, 1, 0, 0);	
	}

	glColor3f(rgb[0], rgb[1], rgb[2]);

	DrawTetrahedron( width, length );
	glPopMatrix();
}

float SpaceCraft::GetSpeed(void)
{
	return speed;
}

void SpaceCraft::SetSpeed(float newSpeed)
{
	speed = newSpeed;
}

float SpaceCraft::GetWidth(void)
{
	return width;
}

float SpaceCraft::GetHeight(void)
{
	return width;
}

void SpaceCraft::MoveForward( void )
{
	glTranslatef(0, 0, speed);
}

void SpaceCraft::FlyStraightLeftRight( void ) {
	movingLeft = false;
	movingRight = false;
}

void SpaceCraft::FlyStraightUpDown( void ) {
	movingUp = false;
	movingDown = false;
}

void SpaceCraft::MoveLeft( void ) {
	movingRight = false;
	movingLeft = true;
}

void SpaceCraft::MoveRight( void ) {
	movingLeft = false;
	movingRight = true;
}

void SpaceCraft::MoveUp( void ) {
	movingDown = false;
	movingUp = true;
}

void SpaceCraft::MoveDown( void ) {
	movingUp = false;
	movingDown = true;
}

bool SpaceCraft::MovingLeft( void ) {
	return movingLeft;
}

bool SpaceCraft::MovingRight( void ) {
	return movingRight;
}

bool SpaceCraft::MovingUp( void ) {
	return movingUp;
}

bool SpaceCraft::MovingDown( void ) {
	return movingDown;
}

float SpaceCraft::GetAltitude( void ) {
	return altitude;
}

void SpaceCraft::SetAltitude( float newAltitude ) {
	altitude = newAltitude;
}