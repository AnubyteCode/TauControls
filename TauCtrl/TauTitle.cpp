/*
  Name: TauTitle.cpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: Graphics title static control
*/

#include "TauTitle.hpp"
#include <stdlib.h>
#include <stdio.h>

#define RBSWAP(c) (RGB(GetBValue(c),GetGValue(c),GetRValue(c)))

struct TauTitle_Data {
    COLORREF color;
    HFONT hFont; 
};

LRESULT CALLBACK TauTitle_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

bool TauTitle_already_inited = false;
__export int InitTauTitleControl(HINSTANCE hInst) {
    if (TauTitle_already_inited) return 0;
    TauTitle_already_inited = true;

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

    wcl.lpszClassName = TAUTITLE_CLASS;
    wcl.lpfnWndProc = TauTitle_Proc;
    RegisterClassEx(&wcl);
    
    return 0;
}

inline int SetAlphaPixel(COLORREF*,COLORREF*,int x,int y,COLORREF c,int a,int w);

LRESULT CALLBACK TauTitle_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
    TauTitle_Data *data = (TauTitle_Data*)GetWindowLong(hwnd,GWL_USERDATA);
    
    switch (msg){
        case WM_CREATE: {
            data = (TauTitle_Data*) GlobalAlloc(GPTR,sizeof(TauTitle_Data));
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)data);
            // contructor
            RECT r;
            GetClientRect(hwnd,&r);

            data->color = 0x00FFFFFF;
        } return 0;
        case WM_DESTROY:
            GlobalFree(data);
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)NULL);
        break;

        case WM_PAINT: {
            HDC hDCx,hDC,hDCt;
            HBITMAP hBitmap,hBitmapo;
            PAINTSTRUCT ps;
            RECT r;
            GetClientRect(hwnd,&r);

            hDCx = BeginPaint( hwnd , &ps );
            hDC = CreateCompatibleDC(hDCx);
            hBitmap = CreateCompatibleBitmap(hDCx,r.right,r.bottom);
            hBitmapo = (HBITMAP)SelectObject(hDC,hBitmap);
            
            HBRUSH hbro,hbr = CreateSolidBrush(0);
            hbro = (HBRUSH)SelectObject(hDC,hbr);
            FillRect(hDC,&r,hbr);
            SelectObject(hDC,hbro);
            DeleteObject(hbr);
            
            SelectObject(hDC,data->hFont);
            
            char aaa[256];
            GetWindowText(hwnd,aaa,256);
            SetTextColor(hDC,data->color);
            SetBkMode(hDC,TRANSPARENT);
            DrawText(hDC,aaa,strlen(aaa),&r,DT_CENTER);
//            SetPixel(hDC,r.right/2,r.bottom/2+1,data->color);
            SelectObject(hDC,hBitmapo);

            BITMAPINFO bi = {
                { sizeof(BITMAPINFOHEADER),
                  r.right,
                  r.bottom,
                  1, 32,
                  BI_RGB
                }
            };
            
            COLORREF *bitmap = (COLORREF*)GlobalAlloc(GPTR,sizeof(COLORREF)*r.right*r.bottom);
            COLORREF *outmap = (COLORREF*)GlobalAlloc(GPTR,sizeof(COLORREF)*r.right*r.bottom);
            
            GetDIBits(hDC,hBitmap,0,r.bottom,bitmap,&bi,DIB_RGB_COLORS);

            // rozmaznout
            
            COLORREF tmp;
            for (int n=0;n<r.bottom;n++) {
                if (!(n%2)) {
                    for (int x=0;x<r.right;x++) {
                        tmp = bitmap[n*r.right+x];
                        tmp = RBSWAP(tmp);
                        
                        if (tmp == data->color) {
                            for (int i=0;i<10;i++)
                                for (int j=0;j<2;j++) { 
                                    int xp = 0;
                                    if (j) xp = x-i+10; else xp = x+i-10;
                                    if (xp<0||xp>r.right) continue;
                                    
                                    int al = 10*(i+1);
                                    
                                    COLORREF e,g;
                                    g = outmap[n*r.right+xp];
                                    g = RBSWAP(g);
                                    
                                    register unsigned char _r, _g, _b;
                                    
                                    _r = ((al*GetRValue(data->color))>>8) + (((255-al)*GetRValue(g))>>8);
                                    _g = ((al*GetGValue(data->color))>>8) + (((255-al)*GetGValue(g))>>8);
                                    _b = ((al*GetBValue(data->color))>>8) + (((255-al)*GetBValue(g))>>8);
                                    
                                    e = RGB(_b,_g,_r);
                                    outmap[n*r.right+xp] = e;
                            }
                            tmp = RBSWAP(tmp);
                            outmap[n*r.right+x] = tmp;
                        }
                    }            
                } else {
                    for (int x=0;x<r.right;x++) {
                        tmp = bitmap[n*r.right+x];
                        if (tmp) outmap[n*r.right+x] = tmp;
                    }
                }
            }
//            CopyMemory(outmap,bitmap,sizeof(COLORREF)*r.right*r.bottom);
            
            // lets update
            SetDIBitsToDevice(hDCx,0,0,r.right,r.bottom,0,0,0,r.bottom,outmap,&bi,DIB_RGB_COLORS);
            
            GlobalFree(bitmap);
            GlobalFree(outmap);
            
            SelectObject(hDC,hBitmapo);
            DeleteObject(hBitmap);
            DeleteDC(hDC);

            EndPaint( hwnd, &ps );
        } break;
        
        case TAUTITLE_SETCOLOR:
            data->color = (COLORREF)lParam;
        break;
        case TAUTITLE_SETFONT:
            data->hFont = (HFONT)lParam;
        break;
        case WM_SETFONT:
            data->hFont = (HFONT)wParam;
        break;
  
        default: return DefWindowProc (hwnd, msg, wParam, lParam);
    }
    return 0;
}


