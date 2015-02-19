#ifndef __P4_UTILS_H
#define __P4_UTILS_H

#include "common.h"

bool invert_pose( float *m );
void calcFPS( bool incrementFrames );

void Camera2WorldCoord( double &x, double &y, double &z );


#endif
