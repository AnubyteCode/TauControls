/*
  Name: MSStatic.cpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: MiltiString Static Control
*/

#include "MSStatic.hpp"

struct MSStatic_Data {
    COLORREF color;
    HWND hStatic;    
    HBRUSH hBr;
    
    int last;
    int tmp;
    char *sz[MSST_LIMIT];
};

LRESULT CALLBACK MSStatic_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

bool MSStatic_already_inited = false;
__export int InitMSStaticControl(HINSTANCE hInst) {
    if (MSStatic_already_inited) return 0;
    MSStatic_already_inited = true;

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

    wcl.lpszClassName = MSST_CLASS;
    wcl.lpfnWndProc = MSStatic_Proc;
    RegisterClassEx(&wcl);

    return 0;
}

LRESULT CALLBACK MSStatic_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
    MSStatic_Data *data =  (MSStatic_Data*)GetWindowLong(hwnd,GWL_USERDATA);
    
    switch (msg){
        case WM_CREATE:
            data = (MSStatic_Data*) GlobalAlloc(GPTR,sizeof(MSStatic_Data));
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)data);
            // contructor
            RECT r;
            GetClientRect(hwnd,&r);

            data->color = 0x00FFFFFF;
            data->hBr = CreateSolidBrush(0);
            data->hStatic = CreateWindow("STATIC","",WS_CHILD|WS_VISIBLE,0,0,r.right,r.bottom,
                                         hwnd,NULL,GetModuleHandle(NULL),NULL);
            data->tmp = 0;
            data->last = -1;
        return 0;
        case WM_DESTROY:
            if (data->tmp) for (int i=0;i<data->tmp;i++) GlobalFree(data->sz[i]);
            DeleteObject(data->hBr);
            GlobalFree(data);
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)NULL);
        break;
        case WM_CTLCOLORSTATIC:
            SetTextColor( (HDC)wParam, data->color );
            SetBkColor( (HDC)wParam, 0 );
        return (LRESULT)data->hBr;
        case WM_SETFOCUS:
            SetFocus( data->hStatic );
        break;
        
        case MSST_SETFONT:
            SendMessage(data->hStatic,WM_SETFONT,(WPARAM)lParam,MAKELPARAM(1,0));
        break;
        case MSST_SETCOLOR: {
            if (data->color != (COLORREF)lParam) {
                data->color = (COLORREF)lParam;
                InvalidateRect(data->hStatic,NULL,1);
            }
        } break;
        case MSST_CLEAR:
            if (data->tmp) for (int i=0;i<data->tmp;i++) GlobalFree(data->sz[i]);
            data->last = -1;
        break;
/*        case WM_SETTEXT:
            SetWindowText( data->hStatic, (char*)lParam );
        return TRUE;        //*/
        case MSST_ADDSTRING:
            if (data->tmp == MSST_LIMIT-1 || lParam==0) break;
            data->sz[ data->tmp ] = (char*)GlobalAlloc(GPTR,1024);
            strcpy(data->sz[ data->tmp ],(char*)lParam);                    
            data->tmp++;
        break;
        case MSST_SET: {
            if (wParam < MSST_LIMIT && wParam >= 0) {
                if (wParam != data->last) {
                    SetWindowText( data->hStatic, data->sz[wParam] );
                    data->last = wParam;
                }
            }
        } break;
  
        default: return DefWindowProc (hwnd, msg, wParam, lParam);
    }
    return 0;
}




