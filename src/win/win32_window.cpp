



#include <windows.h>
#include "GL/gl.h"
#include "win32_window.hpp"
#include "win32_wgl.hpp"
#include "helpers/globals.hpp"
#include "helpers/ogl.hpp"

#include <cstdio>


Win32Window::PFNDwmEnableBlurBehindWindow    Win32Window::fnDwmEnableBlurBehindWindow    = NULL;
Win32Window::PFNDwmExtendFrameIntoClientArea Win32Window::fnDwmExtendFrameIntoClientArea = NULL;
Win32Window::PFNDwmGetColorizationColor      Win32Window::fnDwmGetColorizationColor      = NULL;
HMODULE Win32Window::hDWMAPI = NULL;



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg) {
   case WM_CLOSE:
      PostQuitMessage(0);
      break;

   case WM_DESTROY:
      return 0;

   case WM_KEYDOWN:
      {
      switch (wParam)
      {
      case VK_ESCAPE:
      PostQuitMessage(0);
      break;
      }
      }
      break;

   default:
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
   }

   return 0;
}



LPCTSTR Win32Window::RegisterWinClass() {
   
   LPCTSTR classname = TEXT("TransparentWindow");
   
   memset(&wcex , 0 , sizeof(WNDCLASSEX));
   
   wcex.cbSize = sizeof(WNDCLASSEX);
   wcex.style = 0;//???
   wcex.lpfnWndProc = WindowProc;
   wcex.cbClsExtra = 0;
   wcex.cbWndExtra = 0;
   wcex.hInstance = hInstance;
   wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
   wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
   wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
   wcex.lpszMenuName = NULL;
   wcex.lpszClassName = classname;
   wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;

   if (!RegisterClassEx(&wcex)) {
      return 0;
   }
   return classname;
}



Win32Window::Win32Window(HINSTANCE instance) :
      hInstance(instance),
      wcex(),
      hwnd(0),
      hDC(0),
      hGLRC(0),
      cname(0),
      blender(),
      colkey()
{
   cname = RegisterWinClass();
   blender.BlendOp = AC_SRC_OVER;
   blender.BlendFlags = 0;
   blender.SourceConstantAlpha = 255;
   blender.AlphaFormat = AC_SRC_ALPHA;
   colkey = RGB(0,0,0);
}



Win32Window::~Win32Window() {
   Free();
}



void Win32Window::Free() {
   /* shutdown OpenGL */
   if (hGLRC) {
      DisableOpenGL(hwnd, hDC, hGLRC);
      hDC = 0;
      hGLRC = 0;
   }
   if (hwnd) {
      DestroyWindow(hwnd);
      hwnd = 0;
   }
   
   if (hDWMAPI) {
      FreeLibrary(hDWMAPI);
      fnDwmEnableBlurBehindWindow = 0;
      fnDwmExtendFrameIntoClientArea = 0;
      fnDwmGetColorizationColor = 0;
      hDWMAPI = 0;
   }
}



bool Win32Window::Create() {
   Free();

	OSVERSIONINFO os_vinfo;
	memset(&os_vinfo, 0, sizeof(os_vinfo));
	os_vinfo.dwOSVersionInfoSize = sizeof(os_vinfo);
	GetVersionEx(&os_vinfo);

	if( 6 <= os_vinfo.dwMajorVersion ) {
		hDWMAPI = LoadLibrary("dwmapi.dll");
		if(hDWMAPI) {
			*(PROC*)(&fnDwmEnableBlurBehindWindow) =
				GetProcAddress(hDWMAPI, "DwmEnableBlurBehindWindow");
			
			*(PROC*)(&fnDwmExtendFrameIntoClientArea) =
				GetProcAddress(hDWMAPI, "DwmExtendFrameIntoClientArea");

			*(PROC*)(&fnDwmGetColorizationColor) =
				GetProcAddress(hDWMAPI, "DwmGetColorizationColor");
		}
	}

   hwnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOPMOST,
                        cname,
                        TEXT("OpenGL Transparency Test"),
                        WS_POPUP,
                        sxpos,
                        sypos,
                        swidth,
                        sheight,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);

   if (!hwnd) {
      return false;
   }
   hDC = GetDC(hwnd);

	if(hDWMAPI) {
		if(fnDwmEnableBlurBehindWindow) {
			DWM_BLURBEHIND bb = {0,0,0,0};
			
			bb.dwFlags = DWM_BB_ENABLE;
			bb.fEnable = TRUE;
			bb.hRgnBlur = NULL;
			HRESULT hr = fnDwmEnableBlurBehindWindow(hwnd, &bb);
			if (FAILED(hr)) {
            printf("Could not enable blur behind.");
			}
		}

		if(fnDwmExtendFrameIntoClientArea) {
//			MARGINS margins = {-1};
//			fnDwmExtendFrameIntoClientArea(hwnd, &margins);
		}
	}

   /* enable OpenGL for the window */
   if (!EnableOpenGL(hwnd, &hDC, &hGLRC)) {
      printf("Failed to enable opengl!");
   }
   
   if (!SetLayeredWindowAttributes(hwnd , colkey , 255 , ULW_ALPHA)) {
      printf("Failed to set layered window attributes.\n");
   }
//**
   POINT d;
   d.x = sxpos;
   d.y = sypos;
   SIZE s;
   s.cx = swidth;
   s.cy = sheight;
   POINT src;
   src.x = 0;
   src.y = 0;
   
///   Draw(0.0 , swidth , sheight);
   
   HDC screen = GetDC(NULL);
   
//   if (!UpdateLayeredWindow(hwnd , screen , &d , &s , hDC , &src , colkey , &blender , ULW_ALPHA)) {
//      printf("Failed to update layered window!!! (%lu)" , GetLastError());
//   }

   ReleaseDC(NULL , screen);
//*/   
   ShowWindow(hwnd, SW_SHOW);

   return (bool)hwnd;
}



MSG Win32Window::Run() {
   /* program main loop */
   MSG msg;
   BOOL bQuit = false;
   double theta = 0.0;
   while (!bQuit)
   {
      /* check for messages */
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      /* handle or dispatch messages */
         if (msg.message == WM_QUIT) {
            bQuit = TRUE;
         }
         else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
         }
      }
      else
      {
         /* OpenGL animation code goes here */
         Draw(theta , swidth , sheight);
         SwapBuffers(hDC);
         theta += 0.25;
         Sleep (1);
      }
   }
   return msg;
}
