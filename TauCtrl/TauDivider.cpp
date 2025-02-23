/*
  Name: TauDivider.cpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: Great divider efect window
*/

#include "TauDivider.hpp"

struct TauDivider_Data {
    COLORREF color;
};

LRESULT CALLBACK TauDivider_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

bool TauDivider_already_inited = false;
__export int InitTauDividerControl(HINSTANCE hInst) {
    if (TauDivider_already_inited) return 0;
    TauDivider_already_inited = true;

    WNDCLASSEX wcl;
    wcl.cbSize = sizeof(WNDCLASSEX);
    wcl.style = CS_HREDRAW|CS_VREDRAW;
    wcl.cbClsExtra = 0;
    wcl.cbWndExtra = 0;
    wcl.hInstance = hInst;
    wcl.hIcon = NULL;
    wcl.hIconSm = NULL;
    wcl.hCursor = LoadCursor(NULL,IDC_ARROW);
    wcl.hbrBackground = CreateSolidBrush( 0 );
    wcl.lpszMenuName = NULL;

    wcl.lpszClassName = TAUD_CLASS;
    wcl.lpfnWndProc = TauDivider_Proc;
    RegisterClassEx(&wcl);

    return 0;
}

LRESULT CALLBACK TauDivider_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
    TauDivider_Data *data =  (TauDivider_Data*)GetWindowLong(hwnd,GWL_USERDATA);
    
    switch (msg){
        case WM_CREATE:
            data = (TauDivider_Data*) GlobalAlloc(GPTR,sizeof(TauDivider_Data));
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)data);
            // contructor
            data->color = 0x00FFFFFF;
        return 0;
        case WM_DESTROY:
            GlobalFree(data);
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)NULL);
        break;
        case WM_SIZE:
            InvalidateRect(hwnd,NULL,1);
        break;
        case WM_PAINT: {
            HDC hDC;
            PAINTSTRUCT ps;
            hDC = BeginPaint( hwnd , &ps );

            RECT r;
            GetClientRect(hwnd,&r);

            BITMAPINFO bi = {
                { sizeof(BITMAPINFOHEADER),
                  r.right,
                  r.bottom,
                  1, 32,
                  BI_RGB
                }
            };
            COLORREF *bitmap = (COLORREF*)GlobalAlloc(GPTR,sizeof(COLORREF)*r.right*r.bottom);

//            COLORREF c;
            BYTE _r,_g,_b;
            float ipi = PI/(float)r.right;
            float npi = PI/(float)r.bottom;
            float x,y,xs,ys;
            
//            MoveToEx(hDC,0,0,NULL);
            for (int n=0;n<r.bottom;n+=2) {
                for (int i=0;i<r.right-1;i+=1) {
                    x = i*ipi;
                    y = n*npi;
                    
                    xs = sin(x);
                    ys = sin(y);
                    
                    _r = (int) ((float)GetRValue( data->color ) * xs * ys);
                    _g = (int) ((float)GetGValue( data->color ) * xs * ys);
                    _b = (int) ((float)GetBValue( data->color ) * xs * ys);
                    
//                    c = RGB( _r,_g,_b );
//                    SetPixel(hDC,i,n,c);                
                    bitmap[ n*r.right+i ] = RGB( _b,_g,_r );
                }            
            }

            SetDIBitsToDevice(hDC,0,0,r.right,r.bottom,0,0,0,r.bottom,
                              bitmap,&bi,DIB_RGB_COLORS);
            
            GlobalFree(bitmap);

            EndPaint( hwnd, &ps );
        } break;
        case TAUD_SETCOLOR:
            data->color = (COLORREF)lParam;
        break;
  
        default: return DefWindowProc (hwnd, msg, wParam, lParam);
    }
    return 0;
}


