


#ifndef WIN32_WINDOW_HPP
#define WIN32_WINDOW_HPP


#include <windows.h>
#include <dwmapi.h>



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Win32Window {

private :
   HINSTANCE hInstance;
   WNDCLASSEX wcex;
   HWND hwnd;
   HDC hDC;
   HGLRC hGLRC;
   LPCTSTR cname;
   
   BLENDFUNCTION blender;
   COLORREF colkey;
      
   typedef HRESULT (WINAPI *PFNDwmEnableBlurBehindWindow)   (HWND,const DWM_BLURBEHIND *);
   typedef HRESULT (WINAPI *PFNDwmExtendFrameIntoClientArea)(HWND, MARGINS const*);
   typedef HRESULT (WINAPI *PFNDwmGetColorizationColor)        (DWORD*,BOOL*);

   static HMODULE hDWMAPI;
   static PFNDwmEnableBlurBehindWindow    fnDwmEnableBlurBehindWindow;
   static PFNDwmExtendFrameIntoClientArea fnDwmExtendFrameIntoClientArea;
   static PFNDwmGetColorizationColor      fnDwmGetColorizationColor;

   /* register window class */
   LPCTSTR RegisterWinClass();

public :
   Win32Window(HINSTANCE instance);
   ~Win32Window();
   
   void Free();
   bool Create();
   
   MSG Run();
};



#endif // WIN32_WINDOW_HPP
