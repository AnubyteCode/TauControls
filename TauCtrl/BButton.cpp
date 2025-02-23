/*
  Name: BButton.hpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: Bitmap Button for simple graphics interface
  
  Handles: 3 bitmaps altered in hover and click events
           On hover posts WM_COMMAND with BN_HILITE
           On leave posts WM_COMMAND with BN_UNHILITE
           On click posts WM_COMMAND with BN_CLICKED
*/

#include "BButton.hpp"

struct BButton_Data {
    COLORREF color; // text color
    HBITMAP normal,hover,click;    
    
    int state;
};

LRESULT CALLBACK BButton_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

bool BButton_already_inited = false;
__export int InitBButtonControl(HINSTANCE hInst) {
    if (BButton_already_inited) return 0;
    BButton_already_inited = true;

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

    wcl.lpszClassName = BBTN_CLASS;
    wcl.lpfnWndProc = BButton_Proc;
    RegisterClassEx(&wcl);

    return 0;
}

LRESULT CALLBACK BButton_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
    BButton_Data *data =  (BButton_Data*)GetWindowLong(hwnd,GWL_USERDATA);
    
    if ((msg == WM_KEYDOWN || msg == WM_KEYUP)  && wParam == VK_TAB)
        PostMessage( GetParent(hwnd), msg, wParam, lParam );
    
    switch (msg){
        case WM_CREATE:
            data = (BButton_Data*) GlobalAlloc(GPTR,sizeof(BButton_Data));
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)data);
            // contructor
            data->color = 0x00FFFFFF;
            data->state = 0;
        return 0;
        case WM_DESTROY:
            GlobalFree(data);
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)NULL);
        break;
        case WM_SIZE:
            InvalidateRect(hwnd,NULL,1);
        break;
        case WM_SETFOCUS:
        case WM_KILLFOCUS:
            InvalidateRect(hwnd,NULL,1);
        break;
        case WM_PAINT: {
            HDC hDC;
            PAINTSTRUCT ps;
            hDC = BeginPaint( hwnd , &ps );
            RECT r; GetClientRect(hwnd,&r);
            
            HDC hMemory = CreateCompatibleDC(hDC);
            if (data->state==0) SelectObject(hMemory,data->normal);
            if (data->state==2) SelectObject(hMemory,data->click);
            if (data->state==1 || GetFocus()==hwnd) SelectObject(hMemory,data->hover);
            
            BitBlt(hDC,0,0,r.right,r.bottom,hMemory,0,0,SRCCOPY);
            
            DeleteDC(hMemory);
            
            EndPaint( hwnd, &ps );
        } break;
        case BBTN_SETCOLOR:
            data->color = (COLORREF)lParam;
        break;
        case BBTN_SETBITMAP:
            if (data->normal != (HBITMAP)lParam) {
                data->normal = (HBITMAP)lParam;
                InvalidateRect(hwnd,NULL,0);
            }
        break;
        case BBTN_SETHOVERBITMAP:
            if (data->hover != (HBITMAP)lParam) {
                data->hover = (HBITMAP)lParam;
                InvalidateRect(hwnd,NULL,0);
            }
        break;
        case BBTN_SETDOWNBITMAP:
            if (data->click != (HBITMAP)lParam) {
                data->click = (HBITMAP)lParam;
                InvalidateRect(hwnd,NULL,0);
            }
        break;

        case WM_MOUSEMOVE: { 
            POINT pt = { LOWORD(lParam), HIWORD(lParam) };  
            RECT r; GetClientRect(hwnd,&r);
            
            if (PtInRect(&r,pt)) {
                if (data->state == 0) {
                    data->state = 1;
                    InvalidateRect(hwnd,NULL,1);
                    PostMessage( GetParent(hwnd), WM_COMMAND, 
                                 MAKEWPARAM( GetWindowLong(hwnd,GWL_ID), BN_HILITE),
                                 (LONG)hwnd);
                }
                SetCapture(hwnd);
            } else {
                data->state = 0;
                InvalidateRect(hwnd,NULL,1);
                ReleaseCapture();
                PostMessage( GetParent(hwnd), WM_COMMAND, 
                             MAKEWPARAM( GetWindowLong(hwnd,GWL_ID), BN_UNHILITE),
                             (LONG)hwnd);
            }        
        } break;
        case WM_CAPTURECHANGED:
            if ((HWND)lParam!=hwnd) {
                data->state = 0;
                InvalidateRect(hwnd,NULL,1);
            }
        break;

        case WM_CHAR:
            if (wParam!=13&&wParam!=32) break;
            PostMessage( GetParent(hwnd), WM_COMMAND, 
                         MAKEWPARAM( GetWindowLong(hwnd,GWL_ID), BN_CLICKED),
                         (LONG)hwnd);
        break;
        
        case WM_LBUTTONDOWN:
            data->state = 2;
            InvalidateRect(hwnd,NULL,1);
        break;
        case WM_LBUTTONUP:
            if (data->state == 2)
            PostMessage( GetParent(hwnd), WM_COMMAND, 
                         MAKEWPARAM( GetWindowLong(hwnd,GWL_ID), BN_CLICKED),
                         (LONG)hwnd);

            data->state = 1;
            InvalidateRect(hwnd,NULL,1);
        break;

  
        default: return DefWindowProc (hwnd, msg, wParam, lParam);
    }
    return 0;
}


