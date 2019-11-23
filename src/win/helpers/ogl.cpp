



#include "ogl.hpp"


#include <GL/gl.h>
#include <GL/glext.h>

#include <cstdio>




PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate = nullptr;


void Draw(double theta , double width , double height) {
   glEnable(GL_BLEND);
   glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
   glDisable(GL_DEPTH_TEST);

   glBlendFuncSeparate(GL_ONE , GL_ZERO , GL_ONE , GL_ZERO);
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT);
   glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
   for (int i = -30 ; i <= 0 ; ++i) {
      DrawOurTriangle(theta + i , width , height , (i + 30)*(i + 30)/900.0);
   }
   glFlush();
   
}



void DrawOurTriangle(double theta , double width , double height , double alpha) {

   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();
   glTranslated(width/2.0 , height/2.0 , 0.0);
   glScaled(width , height , 1.0);
   glRotated(theta, 0.0, 0.0, 1.0);
   glTranslated(-0.5 , -0.5 , 0.0);

   glBegin(GL_TRIANGLES);

      glColor4d(1.0*alpha , 1.0*alpha , 1.0*alpha , 1.0*alpha);
      glVertex2d(0.5 , 0.5);

      glColor4d(0.0*alpha , 1.0*alpha , 0.0*alpha , 1.0*alpha);
      glVertex2d(1.0 , 0.5);

      glColor4d(1.0*alpha , 0.0*alpha , 0.0*alpha , 0.5*alpha);
      glVertex2d(0.0 , 0.0);

      glColor4d(1.0*alpha , 1.0*alpha , 1.0*alpha , 0.5*alpha);
      glVertex2d(0.5 , 0.5);
      
      glColor4d(0.0*alpha , 0.0*alpha , 1.0*alpha , 1.0*alpha);
      glVertex2d(0.0 , 1.0);
      
      glColor4d(0.0*alpha , 1.0*alpha , 0.0*alpha , 1.0*alpha);
      glVertex2d(1.0 , 0.5);

   glEnd();

   glPopMatrix();
}



