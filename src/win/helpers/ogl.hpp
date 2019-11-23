



#ifndef OPENGL_HPP
#define OPENGL_HPP

#define GL_VERSION_2_1

#include "GL/gl.h"
#include "GL/glext.h"



extern PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;

void Draw(double theta , double width , double height);

/// Draws a compass triangle that spins
void DrawOurTriangle(double theta , double width , double height , double alpha);


#endif // OPENGL_HPP
