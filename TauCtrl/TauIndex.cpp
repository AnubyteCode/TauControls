/*
  Name: TauIndex.hpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: TauIndex, simple indexing list
*/

#include "TauIndex.hpp"


struct TauIndex_Data {
    COLORREF color,hicolor;
    
    int top;
    int cur;
    int ndigits;
    int height;
    
    HFONT hFont;
};

LRESULT CALLBACK TauIndex_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

bool TauIndex_already_inited = false;
__export int InitTauIndexControl(HINSTANCE hInst) {
    if (TauIndex_already_inited) return 0;
    TauIndex_already_inited = true;

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

    wcl.lpszClassName = TAUI_CLASS;
    wcl.lpfnWndProc = TauIndex_Proc;
    RegisterClassEx(&wcl);

    return 0;
}

LRESULT CALLBACK TauIndex_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
    TauIndex_Data *data =  (TauIndex_Data*)GetWindowLong(hwnd,GWL_USERDATA);
    
    switch (msg){
        case WM_CREATE:
            data = (TauIndex_Data*) GlobalAlloc(GPTR,sizeof(TauIndex_Data));
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)data);
            // contructor
            data->color = 0x00FFFFFF;
            data->hicolor = 0x00FFFFFF;
            data->height = 18;
            data->hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
            data->ndigits = 3;
        return 0;
        case WM_DESTROY:
            GlobalFree(data);
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)NULL);
        break;
        case WM_PAINT: {
            HDC hDC,hDCx;
            HBITMAP hBitmap,hBitmapo;
            PAINTSTRUCT ps;
            hDCx = BeginPaint( hwnd , &ps );
            hDC = CreateCompatibleDC(hDCx);
            
            RECT r;
            GetClientRect(hwnd,&r);

            hBitmap = CreateCompatibleBitmap(hDCx,r.right,r.bottom);
            hBitmapo = (HBITMAP)SelectObject(hDC,hBitmap);

            int max = r.bottom/data->height+1;
            HBRUSH hbro,hbr = CreateSolidBrush(0);
            hbro = (HBRUSH)SelectObject(hDC,hbr);
            FillRect(hDC,&r,hbr);
            
            SetBkMode(hDC,TRANSPARENT);
            SelectObject(hDC,data->hFont);
            
            for (int i=data->top;i<data->top+max;i++) {
                if (i == data->cur)
                    SetTextColor(hDC,data->hicolor);
                else
                    SetTextColor(hDC,data->color);

                char aaa[256];
                char ms[16];
                sprintf(ms,"%%0%dd",data->ndigits);
                sprintf(aaa,ms,i);
                
                TextOut(hDC,0,(i-data->top)*data->height,aaa,strlen(aaa));
            }

            SelectObject(hDC,hbro);
            DeleteObject(hbr);
            
            BitBlt(hDCx,0,0,r.right,r.bottom,hDC,0,0,SRCCOPY);
            SelectObject(hDC,hBitmapo);
            DeleteObject(hBitmap);
            DeleteDC(hDC);
            
            EndPaint( hwnd, &ps );
        } break;
        
        case TAUI_SETCOLOR:
            data->color = (COLORREF)lParam;
        break;
        case TAUI_SETHICOLOR:
            data->hicolor = (COLORREF)lParam;
        break;
        case TAUI_SETTOP:
            data->top = wParam;
            InvalidateRect(hwnd,NULL,0);
        break;
        case TAUI_SETHILITE:
            data->cur = wParam;
            InvalidateRect(hwnd,NULL,0);
        break;
        case TAUI_SETNDIGITS:
            data->ndigits = (wParam <? 255);
        break;

        case TAUI_SETFONT:
            data->hFont = (HFONT)lParam;
        break;
        case TAUI_SETHEIGHT:
            data->height = wParam;
        break;
  
        default: return DefWindowProc (hwnd, msg, wParam, lParam);
    }
    return 0;
}


