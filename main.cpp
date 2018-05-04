#include "Drawer.h"
#include <bits/stdc++.h>
#include <ctime>
#include <conio.h>
//
//#include "hook.h"
//FILE *logfile = stdout;
//
//
//    LowLevelKeyboardProc callback function
//    https://msdn.microsoft.com/en-us/library/windows/desktop/ms644985(v=vs.85).aspx
//    low-level keyboard input event structure
//    https://msdn.microsoft.com/en-us/library/windows/desktop/ms644967(v=vs.85).aspx
//*/
//LRESULT CALLBACK LowLevelKeybdProc ( int aCode, WPARAM wParam, LPARAM lParam )
//{
//      cout<<"keyboard event "<<aCode<<endl;
//      KBDLLHOOKSTRUCT kbdStruct;
//
//      if ( aCode >= 0 )
//      {
//              kbdStruct = * ( ( KBDLLHOOKSTRUCT* ) lParam );
//              if ( wParam == WM_KEYDOWN )
//              {
//                      fprintf ( logfile, "key down event %s, %s. \n", \
//                                hhb_vkey_list[kbdStruct.vkCode].name, \
//                                hhb_vkey_list[kbdStruct.vkCode].description );
//
//              }
//              else if ( wParam == WM_SYSKEYDOWN )
//              {
//                      fprintf ( logfile, "syskey down event %s, %s. \n", \
//                                hhb_vkey_list[kbdStruct.vkCode].name, \
//                                hhb_vkey_list[kbdStruct.vkCode].description );
//
//              }
//              else if ( wParam == WM_KEYUP )
//              {
//                      kbdStruct = * ( ( KBDLLHOOKSTRUCT* ) lParam );
//                      printf ( "keyboard event %x ", kbdStruct.vkCode );
//                      WORD key;
//                      ToAscii(kbdStruct.vkCode, kbdStruct.scanCode, &key, );
//                      printf ( "%c\n", key );
//              }
//              else if ( wParam == WM_SYSKEYUP )
//              {
//                      kbdStruct = * ( ( KBDLLHOOKSTRUCT* ) lParam );
//                      printf ( "keyboard event %x ", kbdStruct.vkCode );
//                      unsigned short key;
//                      scan2ascii ( kbdStruct.scanCode, &key );
//                      printf ( "%c\n", key );
//              }
//      }
//
//      return CallNextHookEx ( NULL, aCode, wParam, lParam );;
//}
//
//
//
//    LowLevelMouseProc callback function
//    https://msdn.microsoft.com/en-us/library/windows/desktop/ms644986(v=vs.85).aspx
//    MOUSEHOOKSTRUCT structure
//    https://msdn.microsoft.com/en-us/library/windows/desktop/ms644970(v=vs.85).aspx
//*/
//LRESULT CALLBACK LowLevelMouseProc ( int aCode, WPARAM wParam, LPARAM lParam )
//{
//      cout<<"mouse event "<<aCode<<endl;
//      MSLLHOOKSTRUCT mouseStruct;
//      if ( aCode >= 0 )
//      {
//              mouseStruct = * ( ( MSLLHOOKSTRUCT* ) lParam );
//              if ( wParam == WM_LBUTTONDOWN )
//              {
//                      fprintf ( logfile, "mouse left down event.\n" );
//              }
//              else if ( wParam == WM_RBUTTONDOWN )
//              {
//                      fprintf ( logfile, "mouse left down event.\n" );
//              }
//              else if ( wParam == WM_MOUSEWHEEL )
//              {
//                      if ( HIWORD ( mouseStruct.mouseData ) > 0 )
//                              fprintf ( logfile, "mouse scroll forward event.\n" );
//                      else
//                              fprintf ( logfile, "mouse scroll backward event.\n" );
//              }
//              else if ( isMousemove && wParam == WM_MOUSEMOVE )
//              {
//                      POINT pt = reinterpret_cast<MSLLHOOKSTRUCT*> ( lParam )->pt;
//                      fprintf ( logfile, "mouse move event location (%d, %d).\n", pt.x, pt.y );
//              }
//      }
//      return CallNextHookEx ( NULL, aCode, wParam, lParam );;
//}

int main ( int argc, char **argv )
{
	Drawer drawer ( GetDesktopWindow() );
	drawer.setCursorVisible ( false );
	drawer.useBuffer ( 2 );
	drawer.usePen ( RGB ( 255, 255, 255 ) );
	int x = 0;
	int y = 0;
	char buf[300];


	int last = 0;
	POINT bp;
	while ( 1 )
	{
		if ( kbhit() )
		{
			int ch = getch();
			switch ( ch )
			{
			case 'c':
				drawer.Clear();
				break;
			case 'q':
				exit ( 0 );
			}
		}

		POINT p = drawer.MousePosition();
		if ( KEY_DOWN ( MOUSE_MOVED ) )
		{
			if ( !last )
			{

				bp = p;
				drawer.useBrush ( RGB ( 0, 0, 0 ) );
			}
			x = p.x;
			y = p.y;
			last = 1;
			drawer.Rect ( bp.x, bp.y, p.x - bp.x, p.y - bp.y );
		}
		else
		{
			if ( last )
			{
				drawer.useBrush ( ( ( rand() << 16 ) + rand() ) & ( ( 1 << 24 ) - 1 ) );
				drawer.Rect ( bp.x, bp.y, p.x - bp.x, p.y - bp.y );
			}

			last = 0;
		}

		sprintf ( buf, "x: %d, y: %d  w: %dpx h: %dpx mx: %ld my: %ld dpi: %d", x, y, drawer.getWidth(), drawer.getHeight(), p.x, p.y, drawer.getXDPI() );
		SetConsoleTitle ( buf );

		drawer.Flush();
		Sleep ( 1000 / 120 );
	}
	return 0;
}
