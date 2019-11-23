



#ifndef WIN32_WGL_HPP
#define WIN32_WGL_HPP

#include <windows.h>
#include <GL/gl.h>

bool EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC);
void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC);

#endif // WIN32_WGL_HPP
