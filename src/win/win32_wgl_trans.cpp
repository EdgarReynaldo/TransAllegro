


#include <windows.h>
#include <gl/gl.h>

#include "helpers/globals.hpp"
#include "win32_window.hpp"


//void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
//void DisableOpenGL(HWND, HDC, HGLRC);





int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
   MSG msg;
   memset(&msg , 0 , sizeof(MSG));
   
   
   window = new Win32Window(hInstance);

   if (!window->Create()) {
      goto CLEANUP;
   }
   
   msg = window->Run();

   CLEANUP:
   delete window;
   
   return msg.wParam;
}

