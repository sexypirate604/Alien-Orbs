#ifndef __P4_STATE_H
#define __P4_STATE_H

#include "common.h"
#include "objects.h"
#include "utils.h"

// Spawn an alien orb at WCS x,y,z.
void SpawnOrb(OrbType type, double x, double y, double z);


void SpawnLaser(double targetx, double targety, double targetz);

void SetSpaceCraftAltitude( void );

void IncreaseHitScore( OrbType orbType );

void DestroyOrb( Orbs::iterator orbIt );

// Updates positions of spacecraft and lasers.
// May randomly spawn an alien orb.
// May remove alien orbs and lasers that are no longer in view (behind camera).
// Checks if alien orb has intersected the spacecraft.
void UpdateGameState( void );

void RemoveOrbsBehindCamera( void );


#endif