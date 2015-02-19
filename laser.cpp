#include "objects.h"

Laser::Laser(float startx, float starty, float startz,
	float endx, float endy, float endz) {

	x0 = startx;
	y0 = starty;
	z0 = startz;
	x1 = endx;
	y1 = endy;
	z1 = endz;
}

void Laser::Draw( void ) {
	// Look at target.
	// Translate along z-axis towards target.
	//glPushMatrix();
	//gluLookAt(x0,y0,z0,x1,y1,z1,0,1,0);
	glPushMatrix();
	glBegin(GL_LINES);
	glVertex3f(x1, y1, z1);
	glEnd();
	glPopMatrix();
}