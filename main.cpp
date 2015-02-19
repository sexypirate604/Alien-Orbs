#include "lodepng.h"
#include "common.h"
#include "utils.h"
#include "terrain.h"
#include "objects.h"
#include "input.h"
#include "state.h"


// OpenGL select buffer.
#define BUFSIZE 512
GLuint selectBuffer[BUFSIZE];

// display width and height
int disp_width=1280, disp_height=720;

SpaceCraft *playerShip;
Orbs alienOrbs;

GameState *gameState;

Orb* newOrb;

Laser *oneLaser;

// Texture image details.
unsigned img_width, img_height;

// I did not write this function!
// PNG load library taken from:
// URL: http://lodev.org/lodepng/
// And example file:
// URL: http://lodev.org/lodepng/example_opengl.cpp
void InitTextures( void )
{
    // Load texture.
    // Load file and decode image.
    char filename[64];
    sprintf(filename, "texture.png");
    std::vector<unsigned char> image;
    unsigned error = lodepng::decode(image, img_width, img_height, filename);

    // If there's an error, display it.
    if(error != 0)
    {
        std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
        gameState->quit = true;
    }

    // Texture size must be power of two for the primitive OpenGL version this is written for. Find next power of two.
    size_t u2 = 1; while(u2 < img_width) u2 *= 2;
    size_t v2 = 1; while(v2 < img_height) v2 *= 2;
    // Ratio for power of two version compared to actual version, to render the non power of two image with proper size.
    double u3 = (double)img_width / u2;
    double v3 = (double)img_height / v2;
    std::vector<unsigned char> image2(u2 * v2 * 4);

    // Make power of two version of the image.
    for(size_t y = 0; y < img_height; y++)
    for(size_t x = 0; x < img_width; x++)
    for(size_t c = 0; c < 4; c++)
    {
        image2[4 * u2 * y + 4 * x + c] = image[4 * img_width * y + 4 * x + c];
    }

    // Enable the texture for OpenGL.
    //glEnable(GL_TEXTURE_2D);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST = no smoothing
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, u2, v2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image2[0]);
}

// set up opengl state, allocate objects, etc.
void init(){
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    // Viewing stuff.
    glViewport( 0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT) );
    glDepthFunc(GL_LEQUAL);
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_NORMALIZE );

    InitTextures();
    
    // Lighting.
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { 50.0, 50.0, 1.0, 0.0 };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable( GL_COLOR_MATERIAL );
    glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );

    // Projection matrix.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 70.0f, float(glutGet(GLUT_WINDOW_WIDTH)) / float(glutGet(
        GLUT_WINDOW_HEIGHT)), 0.1f, 2000.0f );

    // Seed random number generator.
    srand( time(NULL) );

    // Init game state.
    gameState = new GameState;
    gameState->quit = false;
    gameState->reset = true;
    gameState->paused = true;
    gameState->done = false;
    gameState->frameCounter = 0;
    gameState->timebaseFPS = -1;
    gameState->fps = 0;
    gameState->leftClick = false;
    gameState->spawnLaser = false;
    gameState->playerScore = 0;
    gameState->orbsHit = 0;
    gameState->keypressLeft = false;
    gameState->keypressRight = false;
    gameState->keypressUp = false;
    gameState->keypressDown = false;

    // Init objects.
    playerShip = new SpaceCraft( "Player", ShipSpeed_Normal );
    oneLaser = null;

    SpawnOrb( OrbType_Medium, -40, 40, -400 );
}

void DeleteAlienOrbs()
{
    Orbs::iterator itOrbs;
    for (itOrbs = alienOrbs.begin(); alienOrbs.end() != itOrbs; ++itOrbs) {
        delete itOrbs->second;
        alienOrbs.erase(itOrbs);
    }
}

// Frees all allocated objects and returns.
void cleanup(){

	delete playerShip;
    delete gameState;
}


// Window resize callback.
void resize_callback( int width, int height ){
	glViewport(0, 0, width, height);	// Streches view across window.
}


// Adapted from:
// URL: http://www.lighthouse3d.com/opengl/picking/index.php3?openglway
// URL: http://content.gpwiki.org/index.php/OpenGL:Tutorials:Picking
void processHits (GLint hits, GLuint buffer[]) {
    unsigned int i, j;
    GLuint *ptr, minZ, orbId;

    if (1 > hits)
        return;

    ptr = (GLuint *) buffer;
    minZ = 0xffffffff;
    for (i = 0; i < hits; i++) { 
        ptr++;  // ptr at min z.
        if (*ptr < minZ) {
            minZ = *ptr;
            orbId = *(ptr+2);
        }      
      ptr += 3;
    }

    Orbs::iterator orbIt;
    orbIt = alienOrbs.find(orbId);
    if (alienOrbs.end() != orbIt) {
        IncreaseHitScore(orbIt->second->GetOrbType());
        DestroyOrb(orbIt);
    }
}

// Adapted from:
// URL: http://www.lighthouse3d.com/opengl/picking/index.php3?openglway
// URL: http://content.gpwiki.org/index.php/OpenGL:Tutorials:Picking
void renderInSelectionMode() {
    double modelview[16];
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); // Save last VCS.
    glLoadIdentity();

    playerShip->Draw();
    playerShip->MoveForward();  // Apply increment matrix.
    
    glMultMatrixd(modelview);   // Apply current VCS matrx.

    Orbs::iterator it;
    
    it = alienOrbs.begin();
    while (alienOrbs.end() != it) {
        glPushName(it->first);
        it->second->Draw();
        glPopName();
        ++it;
    }
}

// Adapted from:
// URL: http://www.lighthouse3d.com/opengl/picking/index.php3?openglway
// URL: http://content.gpwiki.org/index.php/OpenGL:Tutorials:Picking
void startPicking(int cursorX, int cursorY) {

    GLint viewport[4];
    float ratio = 1.0;

    glSelectBuffer(BUFSIZE,selectBuffer);
    
    glGetIntegerv(GL_VIEWPORT,viewport);

    glRenderMode(GL_SELECT);

    glInitNames();

    glMatrixMode(GL_PROJECTION);    
    glPushMatrix();
    glLoadIdentity();
    
    gluPickMatrix(cursorX,viewport[3]-cursorY,
            1,1,viewport);
    gluPerspective(70.0,ratio,0.1,2000);

    glMatrixMode(GL_MODELVIEW);

    renderInSelectionMode();

    // restoring the original projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    // returning to normal rendering mode
    int hits = glRenderMode(GL_RENDER);
    
    // if there are hits process them
    if (hits != 0)
        processHits(hits,selectBuffer);

    glMatrixMode(GL_MODELVIEW);
}

// Adapted from forum posting:
// http://www.gamedev.net/topic/388298-opengl-hud/
void ViewOrtho( void )
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, disp_width, 0, disp_height, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

// Adapted from forum posting:
// http://www.gamedev.net/topic/388298-opengl-hud/
void ViewPerspective(void)
{
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
}

void DrawAlienOrbs( void )
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    Orbs::iterator it = alienOrbs.begin();
    while (alienOrbs.end() != it) {
        it->second->Draw();
        ++it;
    }

    glDisable(GL_LIGHT0);
    glDisable (GL_LIGHTING);
}

// Adapted from forum posting:
// http://programmingexamples.net/wiki/OpenGL/Text
void DrawHeadsUpDisplay( void )
{
    ViewOrtho();
    glColor3f(0.68,0.94,0.0);   // Lime green.

    // FPS.
    char fps[64];
    sprintf(fps, "%d fps", gameState->fps);
    glPushMatrix();
    glTranslatef( 8, disp_height - 20,0);
    glScalef(0.1,0.1,1);
    for (int i=0; i< ((string)fps).size(); i++)
    {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, fps[i]);
    }
    glPopMatrix();

    // Current score.
    char score[64];
    sprintf(score, "Score: %u", gameState->playerScore);
    glPushMatrix();
    glTranslatef( 8, disp_height - 40,0);
    glScalef(0.1,0.1,1);
    for (int i=0; i< ((string)score).size(); i++)
    {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, score[i]);
    }
    glPopMatrix();

    // Altitude.
    char altitude[64];
    sprintf(altitude, "Alt: %.2f km", playerShip->GetAltitude());
    glPushMatrix();
    glTranslatef( 8, disp_height - 60,0);
    glScalef(0.1,0.1,1);
    for (int i=0; i< ((string)altitude).size(); i++)
    {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, altitude[i]);
    }
    glPopMatrix();

    // Hits.
    char hits[64];
    sprintf(hits, "Hits: %u", gameState->orbsHit);
    glPushMatrix();
    glTranslatef( 8, disp_height - 80,0);
    glScalef(0.1,0.1,1);
    for (int i=0; i< ((string)hits).size(); i++)
    {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, hits[i]);
    }
    glPopMatrix();

    ViewPerspective();
}

void DrawGameOverDisplay( void ) {
    ViewOrtho();
    glColor3f(1, 1, 1);   // Lime green.

    char gameOver[64];
    sprintf(gameOver, "Game Over!");

    glPushMatrix();
    glTranslatef( 40, disp_height/2 + 60, 0 );
    glScalef(0.4,0.4,1);
    for (int i=0; i< ((string)gameOver).size(); i++)
    {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, gameOver[i]);
    }
    glPopMatrix();

    // Display score.
    char score[64];
    sprintf(score, "Score: %d", gameState->playerScore);

    glPushMatrix();
    glTranslatef( 80, disp_height/2, 0 );
    glScalef(0.2,0.2,1);
    for (int i=0; i< ((string)score).size(); i++)
    {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, score[i]);
    }
    glPopMatrix();


    // Display hits.
    char hits[64];
    sprintf(hits, "Hits: %d", gameState->orbsHit);
    glPushMatrix();
    glTranslatef( 80, disp_height/2 -40, 0 );
    glScalef(0.2,0.2,1);
    for (int i=0; i< ((string)hits).size(); i++)
    {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, hits[i]);
    }
    glPopMatrix();

    ViewPerspective();
}

void DrawTexturedBackground( void )
{
    glPushMatrix();
    glTranslatef(0,0,-1999);
    
    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glNormal3d(0, 0, 1);
    glTexCoord2f(-2000,-2000);
    glVertex3f(-2000,-2000,0);
    glTexCoord2f(2000,-2000);
    glVertex3f(2000,-2000,0);
    glTexCoord2f(2000,2000);
    glVertex3f(2000,2000,0);
    glTexCoord2f(-2000,2000);
    glVertex3f(-2000,2000,0);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void display_callback( void ){
    
    double modelview[16];

    // clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    if (gameState->leftClick) {
        gameState->leftClick = false;
        startPicking(gameState->leftClickX, gameState->leftClickY);
    }

    // Handle camera position.
    if ( gameState->reset ) {
        glLoadIdentity();
        DrawTexturedBackground();
        playerShip->Draw();
        glTranslatef( 0, -10, -80 );	// Initial position.
        DrawAxis();
        //DrawFloor();

        DrawAlienOrbs();
        
        if ( !gameState->paused ) {
            gameState->reset = false;
            CenterMouse();
        }
    }
    else {
        // In VCS.
        // Get current view matrix, wipe matrix stack,
        // apply increment matrix, and apply view matrix.
        glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); // Save last VCS.
        glLoadIdentity();

        DrawTexturedBackground();
        playerShip->Draw();
        playerShip->MoveForward();  // Apply increment matrix.

        glMultMatrixd(modelview);   // Apply current VCS matrx.

        // In WCS
        DrawAlienOrbs();

        if (null != oneLaser)
            oneLaser->Draw();

        DrawAxis();

        // TODO:
        // Draw procedural terrain here.
        //DrawFloor();

        if (playerShip->MovingLeft())
            glTranslatef(8,0,0);       // Move world to the left.
        else if (playerShip->MovingRight())
            glTranslatef(-8,0,0);      // Move world to the right.

        if (playerShip->MovingUp())
            glTranslatef(0,-8,0);       // Move world to the left.
        else if (playerShip->MovingDown())
            glTranslatef(0,8,0);      // Move world to the right.
    }

    if ( !gameState->done ) {
        DrawHeadsUpDisplay();
    }
    else if ( gameState->done && !gameState->reset ) {
        DrawGameOverDisplay();
        gameState->paused = true;
        DeleteAlienOrbs();
    }

    // W2V coordinate change.   
    glutSwapBuffers();
}

// Not exactly a callback, but sets a timer to call itself
// in an endless loop to update the program.
void idle( int value ) {

    // if the user wants to quit the program, then exit the
    // function without resetting the timer or triggering
    // a display update
    if( gameState->quit )
    {
        cleanup();  // cleanup any allocated memory        
        // perform hard exit of the program, since glutMainLoop()
        // will never return
        exit(0);
    }
    
    if ( gameState->paused ) {
        // Do not render, saves CPU cycles.
        glutTimerFunc( 10, idle, 0 );
        calcFPS( false );	// Update FPS but do not increment frame counter.
        return;
    }

    // Call function to calculate the frames per second.
    calcFPS( true );

	// May update position of spacecraft, lasers, and may spawn an alien orb.
	// TODO:
	// - Calculate collision of alien orb with spacecraft,
	// - Generate terrain in distance and remove terrain from behind camera.
    UpdateGameState();

    glutPostRedisplay();
    glutTimerFunc( 10, idle, 0 );
}


int main( int argc, char **argv ){
    // initialize glut
    glutInit( &argc, argv );
    
    // use double-buffered RGB+Alpha framebuffers with a depth buffer.
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    
    // initialize the window.
    glutInitWindowSize( disp_width, disp_height );
    glutInitWindowPosition( 0, 100 );
    glutCreateWindow( "Alien Orbs" );
    glutKeyboardFunc( KeyDownHandler );
    glutKeyboardUpFunc( KeyUpHandler );
    glutMouseFunc( MouseHandler );
    glutDisplayFunc( display_callback );
    glutReshapeFunc( resize_callback );
    
    init();
    
    // start the idle on a fixed timer callback
    idle( 0 );
    
    // start the blug main loop
    glutMainLoop();
    
    return 0;
}
