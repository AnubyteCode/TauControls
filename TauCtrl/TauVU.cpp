/*
  Name: TauVU.cpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: Simple graphics VU meter
*/

#include "TauVU.hpp"

struct TauVU_Data {
    COLORREF color;
    int curpos;
};

LRESULT CALLBACK TauVU_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

bool TauVU_already_inited = false;
__export int InitTauVUControl(HINSTANCE hInst) {
    if (TauVU_already_inited) return 0;
    TauVU_already_inited = true;

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

    wcl.lpszClassName = TAUVU_CLASS;
    wcl.lpfnWndProc = TauVU_Proc;
    RegisterClassEx(&wcl);
    
    return 0;
}

LRESULT CALLBACK TauVU_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
    TauVU_Data *data = (TauVU_Data*)GetWindowLong(hwnd,GWL_USERDATA);
    
    switch (msg){
        case WM_CREATE: {
            data = (TauVU_Data*) GlobalAlloc(GPTR,sizeof(TauVU_Data));
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
            HDC hDCx,hDC;
            PAINTSTRUCT ps;
            RECT r;
            GetClientRect(hwnd,&r);

            hDC = BeginPaint( hwnd , &ps );
            
            HPEN hpo,hp = CreatePen(PS_SOLID,0,data->color);
            hpo = (HPEN)SelectObject(hDC,hp);
            HBRUSH hbro,hbr = CreateSolidBrush(0);
            hbro = (HBRUSH)SelectObject(hDC,hbr);
            FillRect(hDC,&r,hbr);
            
            Rectangle(hDC,0,0,r.right,r.bottom);
            float w = (r.bottom-3)/10.0;
            
            SelectObject(hDC,hpo);
            DeleteObject(hp);
            SelectObject(hDC,hbro);
            DeleteObject(hbr);
            
            for (int i=10-(data->curpos/10);i<10;i++) {
                if (i>3) {
                    hbr = CreateSolidBrush( 
                          RGB(GetRValue(NORMALVU)/2,GetGValue(NORMALVU)/2,GetBValue(NORMALVU)/2) );
                    hp = CreatePen(PS_SOLID,0,NORMALVU);
                }
                if (i==3||i==2) {
                    hbr = CreateSolidBrush( 
                          RGB(GetRValue(HIGHVU)/2,GetGValue(HIGHVU)/2,GetBValue(HIGHVU)/2) );
                    hp = CreatePen(PS_SOLID,0,HIGHVU);
                }
                if (i==1||i==0) {
                    hbr = CreateSolidBrush( 
                          RGB(GetRValue(CRITICALVU)/2,GetGValue(CRITICALVU)/2,GetBValue(CRITICALVU)/2) );
                    hp = CreatePen(PS_SOLID,0,CRITICALVU);
                }
                
                hbro = (HBRUSH)SelectObject(hDC,hbr);
                hpo = (HPEN)SelectObject(hDC,hp);
                
                Rectangle(hDC,2,(int)(2+i*w),r.right-2,(int)(2+i*w+w-1));

                SelectObject(hDC,hbro);
                DeleteObject(hbr);
                SelectObject(hDC,hpo);
                DeleteObject(hp);
            }
            
            
            EndPaint( hwnd, &ps );
        } break;
        
        case TAUVU_SETCOLOR:
            data->color = (COLORREF)lParam;
        break;
        case TAUVU_SET:
            if (wParam>=0&&wParam<=100) {
                data->curpos = wParam;
                InvalidateRect(hwnd,NULL,0);
            }
        break;
  
        default: return DefWindowProc (hwnd, msg, wParam, lParam);
    }
    return 0;
}



