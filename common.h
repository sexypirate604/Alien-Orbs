#ifndef __P4_COMMON_H
#define __P4_COMMON_H

#if defined(__APPLE_CC__)
#include<OpenGL/gl.h>
#include<OpenGL/glu.h>
#include<GLUT/glut.h>
#elif defined(WIN32)
#include<windows.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#else
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<stdint.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <iostream>
#include <map>
#include <string>
#include <fstream>

#define null 0
#define PI atan(1)*4

#define ShipSpeed_Normal 1.0
#define ShipSpeed_Turbo 40.0
#define ShipRotate 20.0

using std::cout;
using std::cerr;
using std::endl;
using std::string;

struct GameState {
	bool quit;
	bool reset;
	bool paused;
	bool done;
	int frameCounter;
	int time;
	int timebaseFPS;
	int fps;
	bool leftClick;
	int leftClickX;
	int leftClickY;
	bool spawnLaser;
	int laserX;
	int laserY;
	int laserZ;
	uint playerScore;
	uint orbsHit;
	bool keypressLeft;
	bool keypressRight;
	bool keypressUp;
	bool keypressDown;
};

struct ObjectState {
	int timebase;
	int interval;
};

extern GameState* gameState;

enum CoordType {X=0, Y=1, Z=2};
enum OrbType {OrbType_Large, OrbType_Medium, OrbType_Small, OrbType_Tiny};


#endif