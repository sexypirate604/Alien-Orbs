#include "input.h"

// Keyboard callback.
void KeyDownHandler( unsigned char key, int x, int y ){
    switch( key ){
        case 27:    // ESC key.
            gameState->quit = true;
            break;
        case ' ':
            gameState->paused = (!gameState->paused);
        	break;
        case 'r':
            gameState->done = true;
            break;
        /*case 'w':
            if (!gameState->paused)
                playerShip->SetSpeed(ShipSpeed_Turbo);
            break;*/
        case 'a':
            if (!gameState->paused)
            {
                gameState->keypressLeft = true;
                gameState->keypressRight = false;
            }
            break;
        case 'd':
            if (!gameState->paused)
            {
                gameState->keypressRight = true;
                gameState->keypressLeft = false;
            }
            break;
        case 'w':
            if (!gameState->paused)
            {
                gameState->keypressUp = true;
                gameState->keypressDown = false;
            }
            break;
        case 's':
            if (!gameState->paused)
            {
                gameState->keypressDown = true;
                gameState->keypressUp = false;
            }
            break;
        default:
            break;
    }
}

// Keyboard callback.
void KeyUpHandler( unsigned char key, int x, int y ){
    switch( key ){
        /*case 'w':
            playerShip->SetSpeed(ShipSpeed_Normal);
            break;*/
        case 'a':
            gameState->keypressLeft = false;
            break;
        case 'd':
            gameState->keypressRight = false;
            break;
        case 'w':
            gameState->keypressUp = false;
            break;
        case 's':
            gameState->keypressDown = false;
            break;
        default:
            break;
    }
}

// Mouse callback.
void MouseHandler( int button, int state, int x, int y ){
    switch( button ){
        case GLUT_LEFT_BUTTON:
            if (!gameState->paused && GLUT_DOWN == state) {
                gameState->leftClick = true;
                gameState->leftClickX = x;
                gameState->leftClickY = y;
            }
            // cout << "GLUT_LEFT_BUTTON (" << x << "," << y << ")\n";
            break;
       case GLUT_RIGHT_BUTTON:
            // cout << "GLUT_RIGHT_BUTTON (" << x << "," << y << ")\n";
            if (GLUT_DOWN == state) {
                playerShip->SetSpeed(ShipSpeed_Turbo);
                //playerShip->GetObjectState()->interval = ShipSpeed_Turbo;
                //cout << "down" << endl;
            }
            else {
                playerShip->SetSpeed(ShipSpeed_Normal);
                //playerShip->GetObjectState()->interval = ShipSpeed_Normal;
                //cout << "up" << endl;
            }
            break;
        default:
            break;
    }
}

// Changes cursor to crosshair and places mouse in center of screen.
void CenterMouse(void){
    int width = glutGet( GLUT_WINDOW_WIDTH );
    int height = glutGet( GLUT_WINDOW_HEIGHT );
    glutSetCursor( GLUT_CURSOR_FULL_CROSSHAIR );
    glutWarpPointer( width/2, height/2 );
}