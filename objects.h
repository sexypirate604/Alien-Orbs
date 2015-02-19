#ifndef __P4_OBJECTS_H
#define __P4_OBJECTS_H

#include "common.h"

class Object;
class Orb;
class SpaceCraft;
class Laser;

typedef std::map<GLuint,Orb*> Orbs;

extern SpaceCraft* playerShip;
extern Orbs alienOrbs;
extern Laser* oneLaser;


void DrawAxis(void);

class Object
{
	public:
		Object( void );
		~Object( void );
		string Name( void );
		GLfloat Radius( void );
		double* GetWorldLocation( void );
		void SetWorldLocation( double *newLocation );
		ObjectState* GetObjectState( void );
		void SetObjectState( ObjectState* objectState );
				
	protected:
		string name;
		GLfloat radius;
		double location[3];
		GLfloat rgb[3];
		ObjectState* state;
};

class Orb : public Object
{
	public:
		Orb(OrbType orbType, int orbId);
		void Draw(void);
		OrbType GetOrbType(void);
		int id;

	private:
		OrbType type;
		//int id;
};

class SpaceCraft : public Object
{
	public:
		SpaceCraft(string name, float initSpeed);
		void Draw(void);
		void MoveForward( void );
		void MoveLeft( void );
		void MoveRight( void );
		void MoveUp( void );
		void MoveDown( void );
		void FlyStraightLeftRight( void );
		void FlyStraightUpDown( void );
		bool MovingLeft( void );
		bool MovingRight( void );
		bool MovingUp( void );
		bool MovingDown( void );
		float GetSpeed(void);
		void SetSpeed(float newSpeed);
		float GetWidth(void);
		float GetHeight(void);
		float GetAltitude( void );
		void SetAltitude( float newAltitude );
		float cameraX;
		float cameraY;
		float cameraZ;
		GLfloat length;
				
	private:
		GLfloat width;
		GLfloat altitude;
		float speed;
		float yaw;
		bool movingLeft;
		bool movingRight;
		bool movingUp;
		bool movingDown;
};

class Laser
{
	public:
		Laser(float startx, float starty, float startz,
			float endx, float endy, float endz);

		void Draw( void );

	private:
		float x0, y0, z0;
		float x1, y1, z1;
};


#endif