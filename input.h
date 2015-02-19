#ifndef __P4_INPUT_H
#define __P4_INPUT_H

#include "common.h"
#include "objects.h"

// Capture keyboard callback function.
void KeyUpHandler( unsigned char key, int x, int y );
void KeyDownHandler( unsigned char key, int x, int y );

// Capture mouse input.
void MouseHandler( int button, int state, int x, int y );

// Place mouse at center of screen.
void CenterMouse(void);


#endif