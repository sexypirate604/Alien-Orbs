#include "objects.h"

Orb::Orb(OrbType orbType, int orbId) : Object()
{
	switch (orbType)
	{
		case OrbType_Large:
			name = "large-alien-orb";
			radius = 24.0;
			rgb[0] = 255;
			rgb[1] = 0;
			rgb[2] = 0;
			break;
		case OrbType_Medium:
			name = "medium-alien-orb";
			radius = 16.0;
			rgb[0] = 255;
			rgb[1] = 0;
			rgb[2] = 255;
			break;
		case OrbType_Small:
			name = "small-alien-orb";
			radius = 10.0;
			rgb[0] = 0;
			rgb[1] = 255;
			rgb[2] = 0;
			break;
		case OrbType_Tiny:
			name = "tiny-alien-orb";
			radius = 4.0;
			rgb[0] = 0;
			rgb[1] = 0;
			rgb[2] = 255;
			break;
		default:
			assert(0);
			break;
	}

	id = orbId;
	type = orbType;
}

static void DrawSphere(float radius)
{
	glutSolidSphere(radius,20,20);
}

void Orb::Draw( void )
{
	double x,y,z;
	x = location[X];
	y = location[Y];
	z = location[Z];

	glPushMatrix();
	glTranslated( x, y, z );
	glColor3f( rgb[0], rgb[1], rgb[2] );
	DrawSphere(radius);
	glPopMatrix();
}

OrbType Orb::GetOrbType(void)
{
	return type;
}
