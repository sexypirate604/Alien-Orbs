#include "state.h"

static GLuint lastOrbId = 0;

// x,y,z are the location of the orb in WCS.
void SpawnOrb(OrbType type, double x, double y, double z) {
    // Create id for orb.
    GLuint id = glutGet(GLUT_ELAPSED_TIME);
    Orbs::iterator it;

    if (id == lastOrbId)
        id++;

    lastOrbId = id;
    
    Orb* newOrb = new Orb(type, id);
    double location[3];
    location[X] = x;
    location[Y] = y;
    location[Z] = z;

    newOrb->SetWorldLocation( location );
    alienOrbs[id] = newOrb;
}

void SpawnLaser(double targetx, double targety, double targetz) {
    
    // TODO:
    // Change this starting poing to location at front of ship.
    double x = playerShip->cameraX;
    double y = playerShip->cameraY;
    double z = playerShip->cameraZ;
    x = 0;
    y = 0;
    z = 0;

    //Camera2WorldCoord(x,y,z);

    oneLaser = new Laser(x,y,z, targetx, targety, targetz);
}

void RandomXYCameraLocation(double angle, double &x, double &y, double z) {
    
    int viewWidth, viewHeight;    
    viewWidth = tan(angle) * z *2;
    viewHeight = viewWidth;
    x = static_cast<double>(viewWidth/2 - (rand() % viewWidth));
    //y = static_cast<double>(-1*(viewHeight/8 - (rand() % viewHeight/8)) + 19);
    y = static_cast<double>(viewHeight/2 - (rand() % viewHeight));
}

void RandomSpawnOrb( void ) {    
    double x;
    double y;
    double z = 1998;    // Close to far plane.
    
    RandomXYCameraLocation(10.0, x, y, z);
    Camera2WorldCoord(x, y, z);

    // Randomly choose orb type.
    switch ( rand() % 3 ) {
        case 0: SpawnOrb( OrbType_Large, x, y, z );
            break;
        case 1: SpawnOrb( OrbType_Medium, x, y, z );
            break;
        case 2: SpawnOrb( OrbType_Small, x, y, z );
            break;
        default:
            assert(0);
    }
}

void RemoveOrbsBehindCamera( void )
{
    // Get current camera coordinates in WCS.
    // Iterate all alien orbs,
    //  if alien-orbs z coord > camera z coord remove it.
    float modelview[16];
    glMatrixMode(GL_MODELVIEW);
    glGetFloatv( GL_MODELVIEW_MATRIX, modelview );
    if (!invert_pose( modelview ))
        assert(0);
    float cameraZ = modelview[14];

    Orb* orb;

    Orbs::iterator orbIt;
    for (orbIt = alienOrbs.begin(); orbIt != alienOrbs.end(); orbIt++) {
        orb = orbIt->second;
        if (orb->GetWorldLocation()[2] > cameraZ) {
            alienOrbs.erase(orbIt);
            delete orb;
        }
    }
}

void IncreaseHitScore( OrbType orbType )
{
    switch (orbType)
    {
        case OrbType_Large:
            gameState->playerScore += 200;
            break;
        case OrbType_Medium:
            gameState->playerScore += 300;
            break;
        case OrbType_Small:
            gameState->playerScore += 500;
            break;
        case OrbType_Tiny:
            gameState->playerScore += 1000;
            break;
        default:
            break;
    }

    gameState->orbsHit += 1;
}

void DestroyOrb( Orbs::iterator orbIt )
{
    // Get orbs world coordinates.
    double *pLocation = orbIt->second->GetWorldLocation();
    double x = pLocation[X];
    double y = pLocation[Y];
    double z = pLocation[Z];

    OrbType orbType = orbIt->second->GetOrbType();

    //srand(glutGet(GLUT_ELAPSED_TIME));

    // Spawn smaller orbs.
    switch (orbType)
    {
        case OrbType_Large:
            SpawnOrb(OrbType_Medium, x -40.0, y - 80 + (rand() %80), z );
            SpawnOrb(OrbType_Medium, x +40.0, y - 80 + (rand() %80), z );
            break;
        case OrbType_Medium:
            SpawnOrb(OrbType_Small, x -30.0, y - 60 + (rand() %60), z );
            SpawnOrb(OrbType_Small, x +30.0, y - 60 + (rand() %60), z );
            break;
        case OrbType_Small:
            SpawnOrb(OrbType_Tiny, x -20.0, y - 40 + (rand() %40), z );
            SpawnOrb(OrbType_Tiny, x +20.0, y - 40 + (rand() %40), z );
            break;
        case OrbType_Tiny:
            break;
        default:
            break;
    }

    // Remove orb.
    delete orbIt->second;
    alienOrbs.erase(orbIt);
}

void SetSpaceCraftAltitude( void )
{
    double x,y,z;
    x = 0;
    y = 0;
    z = 0;
    Camera2WorldCoord(x,y,z);
    playerShip->SetAltitude( (float) y );
}

bool IsOrbSpaceCraftIntersect( Orb *orb )
{
    // use positive numbers for distance.
    // z - range of space craft.
    float distFromCamera = -1*playerShip->cameraZ;
    float spaceCraftLength = playerShip->length;
    float maxSpaceCraftZ = distFromCamera + spaceCraftLength;

    double cameraX = 0;
    double cameraY = 0;
    double cameraZ = 0;
    Camera2WorldCoord( cameraX, cameraY, cameraZ );

    // minimum distance of orb to camera is
    // world location (z) - radius (r).
    double *orbLocation = orb->GetWorldLocation();
    float minOrbDistanceZ = (float) (-1*orbLocation[Z]) - orb->Radius();
 
    if ((double) (-1*cameraZ) + maxSpaceCraftZ < minOrbDistanceZ)
        // Orb is farther than camera + ship in z.
        return false;

    // Ranges of spacecraft.
    float leftSC = (float) (cameraX) - playerShip->GetWidth();
    float rightSC = (float) (cameraX) + playerShip->GetWidth();
    float topSC = (float) (cameraY) + playerShip->GetHeight();
    float bottomSC = (float) (cameraY) - playerShip->GetHeight();

    leftSC -= orb->Radius();
    rightSC += orb->Radius();
    topSC += orb->Radius();
    bottomSC -= orb->Radius();

    if (leftSC < orbLocation[X] &&
        rightSC > orbLocation[X] &&
        topSC > orbLocation[Y] &&
        bottomSC < orbLocation[Y])
        return true;

    return false;
}

bool IsSpaceCraftCollision( void )
{
    // Check collision with terrain.
    /*float y = playerShip->GetAltitude();
    if (0 >= y)
        return true;*/

    // Check collision with orbs.
    Orbs::iterator it = alienOrbs.begin();
    while (alienOrbs.end() != it) {
        if (IsOrbSpaceCraftIntersect(it->second))
            return true;
        ++ it;
    }

    return false;
}

bool IsSpaceCraftToHigh( void )
{
    float y = playerShip->GetAltitude();
    if (10000 < y)
        return true;

    return false;
}
void UpdateGameState( void )
{
    if (0 == rand() % 10)
        RandomSpawnOrb();
    
    //RemoveOrbsBehindCamera();

    if (gameState->spawnLaser) {
        gameState->spawnLaser = false;
        SpawnLaser(gameState->laserX, gameState->laserY, gameState->laserZ);
    }

    // Increment player score.
    if (ShipSpeed_Turbo == playerShip->GetSpeed())
        gameState->playerScore += 1;

    if (gameState->keypressLeft)
        playerShip->MoveLeft();
    else if (gameState->keypressRight)
        playerShip->MoveRight();
    else
        playerShip->FlyStraightLeftRight();
    if (gameState->keypressUp)
        playerShip->MoveUp();
    else if (gameState->keypressDown)
        playerShip->MoveDown();
    else
        playerShip->FlyStraightUpDown();

    SetSpaceCraftAltitude();

    if ( IsSpaceCraftCollision() )
        gameState->done = true;

    if ( IsSpaceCraftToHigh() && !gameState->keypressDown )
        playerShip->FlyStraightUpDown();
}