#include "utils.h"

// inversion routine originally from MESA
bool invert_pose( float *m ){
	float inv[16], det;
	int i;

	inv[0] = m[5] * m[10] * m[15] -
	m[5] * m[11] * m[14] -
	m[9] * m[6] * m[15] +
	m[9] * m[7] * m[14] +
	m[13] * m[6] * m[11] -
	m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
	m[4] * m[11] * m[14] +
	m[8] * m[6] * m[15] -
	m[8] * m[7] * m[14] -
	m[12] * m[6] * m[11] +
	m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
	m[4] * m[11] * m[13] -
	m[8] * m[5] * m[15] +
	m[8] * m[7] * m[13] +
	m[12] * m[5] * m[11] -
	m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
	m[4] * m[10] * m[13] +
	m[8] * m[5] * m[14] -
	m[8] * m[6] * m[13] -
	m[12] * m[5] * m[10] +
	m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
	m[1] * m[11] * m[14] +
	m[9] * m[2] * m[15] -
	m[9] * m[3] * m[14] -
	m[13] * m[2] * m[11] +
	m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
	m[0] * m[11] * m[14] -
	m[8] * m[2] * m[15] +
	m[8] * m[3] * m[14] +
	m[12] * m[2] * m[11] -
	m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
	m[0] * m[11] * m[13] +
	m[8] * m[1] * m[15] -
	m[8] * m[3] * m[13] -
	m[12] * m[1] * m[11] +
	m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
	m[0] * m[10] * m[13] -
	m[8] * m[1] * m[14] +
	m[8] * m[2] * m[13] +
	m[12] * m[1] * m[10] -
	m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
	m[1] * m[7] * m[14] -
	m[5] * m[2] * m[15] +
	m[5] * m[3] * m[14] +
	m[13] * m[2] * m[7] -
	m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
	m[0] * m[7] * m[14] +
	m[4] * m[2] * m[15] -
	m[4] * m[3] * m[14] -
	m[12] * m[2] * m[7] +
	m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
	m[0] * m[7] * m[13] -
	m[4] * m[1] * m[15] +
	m[4] * m[3] * m[13] +
	m[12] * m[1] * m[7] -
	m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
	m[0] * m[6] * m[13] +
	m[4] * m[1] * m[14] -
	m[4] * m[2] * m[13] -
	m[12] * m[1] * m[6] +
	m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
	m[1] * m[7] * m[10] +
	m[5] * m[2] * m[11] -
	m[5] * m[3] * m[10] -
	m[9] * m[2] * m[7] +
	m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
	m[0] * m[7] * m[10] -
	m[4] * m[2] * m[11] +
	m[4] * m[3] * m[10] +
	m[8] * m[2] * m[7] -
	m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
	m[0] * m[7] * m[9] +
	m[4] * m[1] * m[11] -
	m[4] * m[3] * m[9] -
	m[8] * m[1] * m[7] +
	m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
	m[0] * m[6] * m[9] -
	m[4] * m[1] * m[10] +
	m[4] * m[2] * m[9] +
	m[8] * m[1] * m[6] -
	m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0)
	return false;

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
	m[i] = inv[i] * det;

	return true;
}

void calcFPS( bool incrementFrames ){ 
    // Check timebase initialized.
    if (-1 == gameState->timebaseFPS)
        gameState->timebaseFPS = glutGet(GLUT_ELAPSED_TIME);

	// Get elapsed time since call to glutMainLoop().
    gameState->time = glutGet(GLUT_ELAPSED_TIME);

    if (incrementFrames)
    	gameState->frameCounter++;
        
    if (gameState->time - gameState->timebaseFPS > 1000) {
        gameState->fps = gameState->frameCounter * 1000.0 /
            (gameState->time - gameState->timebaseFPS);
        gameState->timebaseFPS = gameState->time;
        gameState->frameCounter = 0;
        //cout << "fps " << gameState->fps << endl;
    }
}

void Camera2WorldCoord( double &x, double &y, double &z ) {

    // Calculate world location.
    // 1. grab current model view matrix (VCS).
    // 2. position orb in VCS.
    // 3. grab current matrix and invert for WCS.
    float modelview[16];
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(x,y,z);
    glGetFloatv( GL_MODELVIEW_MATRIX, modelview );
    glPopMatrix();
    if (!invert_pose( modelview ))
        assert(0);

    x = modelview[12];
    y = modelview[13];
    z = modelview[14];
}