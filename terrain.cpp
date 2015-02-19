#include "terrain.h"

void DrawFloor(void) {
  // Draw a floor. Since it is transparent, we need to do it AFTER all of
  // the opaque objects.

  for (int x = -50; x < 50; x++) {
    for (int y = -50; y < 50; y++) {
      glColor4f(1, 0, 0, (x + y) % 2 ? 0.75 : 0.5);
      glNormal3f(0, 1, 0);
      glBegin(GL_POLYGON);
      glVertex3f(x    , 0, y    );
      glVertex3f(x + 1, 0, y    );
      glVertex3f(x + 1, 0, y + 1);
      glVertex3f(x    , 0, y + 1);
      glEnd();
    }
  }
  // visible from initial side angle
  glBegin(GL_POLYGON);
  glVertex3f(-5,    0, 0);
  glVertex3f(-5, -.05, 0);
  glVertex3f( 5, -.05, 0);
  glVertex3f( 5,    0, 0);
  glEnd();
  
  // visible from front angle
  glBegin(GL_POLYGON);
  glVertex3f(0,    0, -5);
  glVertex3f(0, -.05, -5);
  glVertex3f(0, -.05, 5);
  glVertex3f(0,    0, 5);
  glEnd();
}