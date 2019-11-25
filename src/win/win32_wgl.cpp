


#include "win32_wgl.hpp"
#include "helpers/globals.hpp"
#include "GL/wglext.h"
#include "helpers/wglarb.h"
#include <cstdio>
#include "helpers/ogl.hpp"


bool EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    *hDC = GetDC(hwnd);

	int attribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, TRUE,
		WGL_DOUBLE_BUFFER_ARB, TRUE,
		WGL_SUPPORT_OPENGL_ARB, TRUE, 
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_TRANSPARENT_ARB, TRUE,
		WGL_COLOR_BITS_ARB, 32,
		WGL_RED_BITS_ARB, 8,
		WGL_GREEN_BITS_ARB, 8,
		WGL_BLUE_BITS_ARB, 8,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0, 0
	};

	INT iPF;
	UINT num_formats_choosen;
	if( !wglarb_ChoosePixelFormatARB(
			*hDC, 
			attribs, 
			NULL,
			1,
			&iPF,
			&num_formats_choosen) ) {
		printf("Error choosing proper pixel format\n");
		return NULL;
	}
	if( !num_formats_choosen ) {
		return NULL;
	}

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(pfd));
	/* now this is a kludge; we need to pass something in the PIXELFORMATDESCRIPTOR 
	 * to SetPixelFormat; it will be ignored, mostly. OTOH we want to send something
	 * sane, we're nice people after all - it doesn't hurt if this fails. */
    DescribePixelFormat(*hDC, iPF, sizeof(pfd), &pfd);

    if( !SetPixelFormat(*hDC, iPF, &pfd) ) {
        printf("Error setting proper pixel format\n");
        ReleaseDC(hwnd, *hDC);
        *hDC = 0;
        DestroyWindow(hwnd);
        hwnd = 0;
        return NULL;
    }

    // create and enable the render context (RC)
    int context_attribs[] = {
       WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
       WGL_CONTEXT_MINOR_VERSION_ARB, 0,
       0, 0
    };
    *hRC = wglarb_CreateContextAttribsARB(*hDC, NULL, context_attribs);

    wglMakeCurrent(*hDC, *hRC);
    glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)(PROC)wglGetProcAddress("glBlendFuncSeparate");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0 , swidth , sheight , 0 , -1 , 1);
    glMatrixMode(GL_MODELVIEW);
    return true;
}



void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}



