#ifndef _DRAWER_H_
#define _DRAWER_H_
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#include <windows.h>

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
#define NORMALIZE(x) ((x)>255?255:(x)<0?0:(x))
#define N_RGB(r,g,b) RGB(NORMALIZE(r),NORMALIZE(g),NORMALIZE(b))
#define RGBA(r,g,b,a) (RGB((r),(g),(b))|(a)<<24)
#define PRGBA(r,g,b,a) (RGBA((r)*(a)/255,(g)*(a)/255,(b)*(a)/255,(a)))
// Converts HSL to RGB
COLORREF HSL ( int hue, float saturation, float luminance )
{
	float h = ( hue % 360 + 360 ) % 360;                // h must be [0, 360]
	float s = saturation / 100.f; // s must be [0, 1]
	float l = luminance / 100.f;    // l must be [0, 1]
	float R, G, B;
	if ( saturation == 0 )
	{
		// achromatic color (gray scale)
		R = G = B = l * 255.0f;
	}
	else
	{
		float q = ( l < 0.5f ) ? ( l * ( 1.0f + s ) ) : ( l + s - ( l * s ) );
		float p = ( 2.0f * l ) - q;
		float Hk = h / 360.0f;
		float T[3];
		T[0] = Hk + 0.3333333f; // Tr   0.3333333f=1.0/3.0
		T[1] = Hk;              // Tb
		T[2] = Hk - 0.3333333f; // Tg
		for ( int i = 0; i < 3; i++ )
		{
			if ( T[i] < 0 ) T[i] += 1.0f;
			if ( T[i] > 1 ) T[i] -= 1.0f;
			if ( ( T[i] * 6 ) < 1 )
			{
				T[i] = p + ( ( q - p ) * 6.0f * T[i] );
			}
			else if ( ( T[i] * 2.0f ) < 1 ) //(1.0/6.0)<=T[i] && T[i]<0.5
			{
				T[i] = q;
			}
			else if ( ( T[i] * 3.0f ) < 2 ) // 0.5<=T[i] && T[i]<(2.0/3.0)
			{
				T[i] = p + ( q - p ) * ( ( 2.0f / 3.0f ) - T[i] ) * 6.0f;
			}
			else T[i] = p;
		}
		R = T[0] * 255.0f;
		G = T[1] * 255.0f;
		B = T[2] * 255.0f;
	}

	return N_RGB ( R, G, B );
}


class Drawer
{
public:
	Drawer ( HWND hwnd )
	{
		window = hwnd;
		window_hdc = GetDC ( hwnd );
		usePen ( RGB ( 255, 255, 255 ) );
		useBrush ( RGB ( 255, 255, 255 ) );
		useBuffer ( 1 );
		SetProcessDPIAware ();
		desktop_hdc = GetDC ( GetDesktopWindow () );
		x_dpi = GetDeviceCaps ( desktop_hdc, LOGPIXELSX );
		y_dpi = GetDeviceCaps ( desktop_hdc, LOGPIXELSY );

	}
	int getXDPI()
	{
		return x_dpi;
	}

	int getYDPI()
	{
		return y_dpi;
	}
	void setCursorVisible ( bool visible )
	{
		HANDLE handle = GetStdHandle ( STD_OUTPUT_HANDLE );
		CONSOLE_CURSOR_INFO CursorInfo;
		GetConsoleCursorInfo ( handle, &CursorInfo ); //获取控制台光标信息
		CursorInfo.bVisible = visible; //隐藏控制台光标
		SetConsoleCursorInfo ( handle, &CursorInfo ); //设置控制台光标状态
	}
	void useBuffer ( int buffernum )
	{


		if ( buffernum == 1 )
		{
			if ( bufNum > 1 )
			{
				DeleteObject ( buffer_hdc );
				DeleteObject ( bmp );
			}
			hdc = window_hdc;
		}
		else if ( buffernum == 2 )
		{
			if ( bufNum == 1 )
			{
				hdc = buffer_hdc = CreateCompatibleDC ( NULL );
				bmp = CreateCompatibleBitmap ( window_hdc,  getWidth(), getHeight() );
				SelectObject ( buffer_hdc, bmp );
			}

		}
		bufNum = buffernum;
	}
	void useBrush ( COLORREF color )
	{
		if ( currentBrush )
			DeleteObject ( currentBrush );
		currentBrush = CreateSolidBrush ( color );
		SelectObject ( hdc, currentBrush );
	}
	void usePen ( COLORREF color )
	{
		if ( currentPen )
			DeleteObject ( currentPen );
		currentPen = CreatePen ( PS_SOLID, 1, color );
		SelectObject ( hdc, currentPen );
	}
	void MoveToEx ( int x, int y )
	{
		using ::MoveToEx;
		MoveToEx ( hdc, x, y, NULL );
	}
	void Rect ( int x, int y, int width, int height )
	{
		Rectangle ( hdc, x, y, x + width, y + height );
	}
	RECT getRect()
	{
		RECT rect;
		GetClientRect ( window, &rect );
		return rect;
	}
	int getWidth()
	{
		RECT rect = getRect();
		return ( rect.right - rect.left ) * getXDPI() / DEFALUT_DPI;
	}
	int getHeight()
	{
		RECT rect = getRect();
		return ( rect.bottom - rect.top ) * getYDPI() / DEFALUT_DPI;
	}
	void Line ( int x1, int y1, int x2, int y2 )
	{
		MoveToEx ( x1, y1 );
		LineTo ( x2, y2 );
	}
	void LineTo ( int x, int y )
	{
		using ::LineTo;
		LineTo ( hdc, x, y );
	}
	void Point ( int x, int y, COLORREF color )
	{
		SetPixel ( hdc, x, y, color );
	}

	void Clear ( COLORREF c = RGB ( 0, 0, 0 ) )
	{
		useBrush ( c );
		usePen ( c );
		Rect ( 0, 0, getWidth(), getHeight() );

	}
	void Flush()
	{
		/* 将buffer_hdc的内容显示到窗口 */
		if ( window_hdc != hdc )
		{
			TransparentBlt ( window_hdc, 0, 0, getWidth(), getHeight(), hdc, 0, 0, getWidth(), getHeight(), RGB ( 0, 0, 0 ) );

		}
	}
	void FlushAndClear()
	{
		Flush();
		Clear();
	}
	POINT MousePosition()
	{
		POINT p;
		GetCursorPos ( &p );
		ScreenToClient ( window, &p );
		return p;
	}

	~Drawer()
	{
		useBuffer ( 1 );
		if ( currentPen )
			DeleteObject ( currentPen );
		if ( currentBrush )
			DeleteObject ( currentBrush );
		ReleaseDC ( window, hdc );
		ReleaseDC ( GetDesktopWindow(), desktop_hdc );
	}
private:
	HDC buffer_hdc = 0;
	HWND window = 0;
	HDC hdc = 0;
	HDC window_hdc = 0;
	HPEN currentPen = 0;
	HBRUSH currentBrush = 0;
	HBITMAP bmp = 0;
	HDC desktop_hdc = 0;
	int x_dpi;
	int y_dpi;
	static const int DEFALUT_DPI = 92;
	int bufNum = 1;
};
#endif // _DRAWER_H_
