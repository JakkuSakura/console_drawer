
#include "hook.h"
using namespace std;
HHOOK g_KeybdHook = NULL;
HHOOK g_MouseHook = NULL;
HINSTANCE g_hInstance = NULL;

bool isMouse = true;
bool isKey = true;

bool isDownevent = true;
bool isUPevent = false;
bool isMousemove = false;


int unhook()
{
	UnhookWindowsHookEx ( g_KeybdHook );
	g_KeybdHook = NULL;
	UnhookWindowsHookEx ( g_MouseHook );
	g_MouseHook = NULL;
	return 0;
}

bool setKeyBoardHook ( HOOKPROC LowLevelKeybdProc )
{
	if ( !g_KeybdHook && isKey )
	{
		g_KeybdHook = SetWindowsHookEx ( WH_KEYBOARD_LL, LowLevelKeybdProc, g_hInstance, 0 );
		return g_KeybdHook;
	}
	return false;
}

bool setMouseHook ( HOOKPROC LowLevelMouseProc )
{
	if ( !g_MouseHook && isMouse )
	{
		g_MouseHook = SetWindowsHookEx ( WH_MOUSE_LL, LowLevelMouseProc, g_hInstance, 0 );
		return g_MouseHook;

	}
	return false;
}


int listenEvent()
{
    MSG msg;
	while ( GetMessage ( &msg, NULL, 0, 0 ) )
	{
		TranslateMessage ( &msg );
        DispatchMessage ( &msg );
	}

	unhook();
	return 0;
}

