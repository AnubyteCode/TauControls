/*
  Name: TauProgress.hpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: Progress bar
*/

#include "TauProgress.hpp"
#include "TauTickControl.hpp"

struct TauProgress_Data {
    COLORREF color;
    int curpos;
    HWND h;
};

LRESULT CALLBACK TauProgress_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

bool TauProgress_already_inited = false;
__export int InitTauProgressControl(HINSTANCE hInst) {
    if (TauProgress_already_inited) return 0;
    TauProgress_already_inited = true;

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

    wcl.lpszClassName = TAUP_CLASS;
    wcl.lpfnWndProc = TauProgress_Proc;
    RegisterClassEx(&wcl);
    
    InitTauTickControl(hInst);

    return 0;
}

LRESULT CALLBACK TauProgress_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
    TauProgress_Data *data = (TauProgress_Data*)GetWindowLong(hwnd,GWL_USERDATA);
    
    switch (msg){
        case WM_CREATE: {
            data = (TauProgress_Data*) GlobalAlloc(GPTR,sizeof(TauProgress_Data));
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)data);
            // contructor
            RECT r;
            GetClientRect(hwnd,&r);
            
            data->h = CreateWindow(TAUT_CLASS,"",WS_CHILD|WS_VISIBLE|WS_DISABLED,
                                   5,5,r.right-5,r.bottom-5,hwnd,NULL,GetModuleHandle(NULL),NULL);
            data->color = 0x00FFFFFF;
        } return 0;
        case WM_DESTROY:
            DestroyWindow(data->h);
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
            
            MoveToEx(hDC,0,0,NULL); LineTo(hDC,r.right/3,0);
            MoveToEx(hDC,0,0,NULL); LineTo(hDC,0,r.bottom/3);
            MoveToEx(hDC,2,2,NULL); LineTo(hDC,2*r.right/3,2);
            MoveToEx(hDC,2,2,NULL); LineTo(hDC,2,2*r.bottom/3);
            
            SelectObject(hDC,hpo);
            DeleteObject(hp);
            SelectObject(hDC,hbro);
            DeleteObject(hbr);
            
            EndPaint( hwnd, &ps );
        } break;
        
        case TAUP_SETCOLOR:
            data->color = (COLORREF)lParam;
            SendMessage(data->h,TAUT_SETCOLOR,0,lParam);
        break;
        case TAUP_SETPOS:
            if (wParam>=0&&wParam<=100) {
                data->curpos = wParam;
                PostMessage(data->h,TAUT_SETDATA,255*wParam/100,0);
            }
        break;
        case TAUP_STEP:
            if (data->curpos < 100){
                data->curpos++;
                PostMessage(data->h,TAUT_SETDATA,255*data->curpos/100,0);
            }
        break;
  
        default: return DefWindowProc (hwnd, msg, wParam, lParam);
    }
    return 0;
}



