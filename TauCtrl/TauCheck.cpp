/*
  Name: TauCheck.cpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: Graphics check box
*/

#include "TauCheck.hpp"
#define ACC_BUTTON_EFECT_LEN   400 // in ms
#define ACC_BUTTON_EFECT_STEPS 40 // in steps

struct TauCheck_Data {
    COLORREF color,hicolor;
    HFONT hFont;
    int state;
    int over;
    float ef1,ef2,ef3;
    bool in,on;
};

LRESULT CALLBACK TauCheck_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

bool TauCheck_already_inited = false;
__export int InitTauCheckControl(HINSTANCE hInst) {
    if (TauCheck_already_inited) return 0;
    TauCheck_already_inited = true;

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

    wcl.lpszClassName = TAUCHECK_CLASS;
    wcl.lpfnWndProc = TauCheck_Proc;
    RegisterClassEx(&wcl);
    
    return 0;
}

LRESULT CALLBACK TauCheck_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
    TauCheck_Data *data = (TauCheck_Data*)GetWindowLong(hwnd,GWL_USERDATA);
    
    switch (msg){
        case WM_CREATE: {
            data = (TauCheck_Data*) GlobalAlloc(GPTR,sizeof(TauCheck_Data));
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)data);
            // contructor
            RECT r;
            GetClientRect(hwnd,&r);

            data->color = 0x00FFFFFF;
            data->hicolor = 0x00FFFFFF;
            data->hFont = CreateFont(-((130*r.bottom)/100),0,0,0,700,0,0,0,DEFAULT_CHARSET,
                                     OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
                                     DEFAULT_PITCH|FF_DECORATIVE,"Wingdings");
            data->ef1 = data->ef2 = data->ef3 = 0.0;
        } return 0;
        case WM_DESTROY:
            DeleteObject(data->hFont);
            GlobalFree(data);
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)NULL);
        break;

        case WM_CHAR:
            if (wParam==32||wParam==13) {
                InvalidateRect(hwnd,NULL,0);
                PostMessage( GetParent(hwnd), WM_COMMAND, 
                             MAKEWPARAM( GetWindowLong(hwnd,GWL_ID), BN_CLICKED),
                             (LONG)hwnd);
                data->ef3 = 0;
                data->state=!data->state;
                SetTimer(hwnd,3,ACC_BUTTON_EFECT_LEN/ACC_BUTTON_EFECT_STEPS,NULL);
            }
        break;
        case WM_LBUTTONDOWN:
            SetFocus(hwnd);
            InvalidateRect(hwnd,NULL,0);
            PostMessage( GetParent(hwnd), WM_COMMAND, 
                         MAKEWPARAM( GetWindowLong(hwnd,GWL_ID), BN_CLICKED),
                         (LONG)hwnd);

            data->ef3 = 0;
            data->state=!data->state;
            SetTimer(hwnd,3,ACC_BUTTON_EFECT_LEN/ACC_BUTTON_EFECT_STEPS,NULL);
        break;
        case WM_SETFOCUS:
            InvalidateRect(hwnd,NULL,0);
        break;
        case WM_KILLFOCUS:
            InvalidateRect(hwnd,NULL,0);
        break;
        
        case WM_PAINT: {
            bool en = IsWindowEnabled(hwnd);
            
            COLORREF hicolor = data->hicolor;
            COLORREF color = data->color;
            if (!en) {
                hicolor = RGB(GetRValue(hicolor)/2,GetGValue(hicolor)/2,GetBValue(hicolor)/2);
                color = RGB(GetRValue(color)/2,GetGValue(color)/2,GetBValue(color)/2);
            }            
         
            HDC hDCx,hDC;
            HBITMAP hBitmap,hBitmapo;
            PAINTSTRUCT ps;
            RECT r;
            GetClientRect(hwnd,&r);

            hDCx = BeginPaint( hwnd , &ps );
            hDC = CreateCompatibleDC(hDCx);
            hBitmap = CreateCompatibleBitmap(hDCx,r.right,r.bottom);
            hBitmapo = (HBITMAP)SelectObject(hDC,hBitmap);
            
            HPEN hpo,hp = CreatePen(PS_SOLID,0,
                                    (GetFocus()==hwnd)?hicolor:color);
            hpo = (HPEN)SelectObject(hDC,hp);
            HBRUSH hbro,hbr = CreateSolidBrush(0);
            hbro = (HBRUSH)SelectObject(hDC,hbr);
            FillRect(hDC,&r,hbr);
            
            if (data->in || data->ef2) {
                if (data->ef1) {
                    SelectObject(hDC,hpo);
                    DeleteObject(hp);
                    hp = CreatePen(PS_SOLID,0,(GetFocus()==hwnd)?
                                   RGB(GetRValue(hicolor)*data->ef1,
                                       GetGValue(hicolor)*data->ef1,
                                       GetBValue(hicolor)*data->ef1):
                                   RGB(GetRValue(color)*data->ef1,
                                       GetGValue(color)*data->ef1,
                                       GetBValue(color)*data->ef1));
                    hpo = (HPEN)SelectObject(hDC,hp);
                }
                if (data->ef2) {
                    SelectObject(hDC,hpo);
                    DeleteObject(hp);
                    hp = CreatePen(PS_SOLID,0,(GetFocus()==hwnd)?
                                   RGB(GetRValue(hicolor)*(1.0-data->ef2),
                                       GetGValue(hicolor)*(1.0-data->ef2),
                                       GetBValue(hicolor)*(1.0-data->ef2)):
                                   RGB(GetRValue(color)*(1.0-data->ef2),
                                       GetGValue(color)*(1.0-data->ef2),
                                       GetBValue(color)*(1.0-data->ef2)));
                    hpo = (HPEN)SelectObject(hDC,hp);
                }
                
                MoveToEx(hDC,0,0,NULL); LineTo(hDC,r.right/2,0);
                MoveToEx(hDC,0,0,NULL); LineTo(hDC,0,r.bottom/2);
                MoveToEx(hDC,r.right-1,r.bottom-1,NULL); LineTo(hDC,r.right/2,r.bottom-1);
                MoveToEx(hDC,r.right-1,r.bottom-1,NULL); LineTo(hDC,r.right-1,r.bottom/2);
            }
            
            if (data->state || data->ef3) {
                SelectObject(hDC,data->hFont);
                if (data->ef3!=0.0) {
                    if (data->state)
                        SetTextColor(hDC,(GetFocus()==hwnd)?
                                   RGB(GetRValue(hicolor)*(data->ef3),
                                       GetGValue(hicolor)*(data->ef3),
                                       GetBValue(hicolor)*(data->ef3)):
                                   RGB(GetRValue(color)*(data->ef3),
                                       GetGValue(color)*(data->ef3),
                                       GetBValue(color)*(data->ef3)));
                    else
                        SetTextColor(hDC,(GetFocus()==hwnd)?
                                   RGB(GetRValue(hicolor)*(1.0-data->ef3),
                                       GetGValue(hicolor)*(1.0-data->ef3),
                                       GetBValue(hicolor)*(1.0-data->ef3)):
                                   RGB(GetRValue(color)*(1.0-data->ef3),
                                       GetGValue(color)*(1.0-data->ef3),
                                       GetBValue(color)*(1.0-data->ef3)));
                } else
                    SetTextColor(hDC,(GetFocus()==hwnd)?hicolor:color);
                
                SetBkMode(hDC,TRANSPARENT);
                char aaa[2];
                aaa[0] = 0xFC;
                aaa[1] = 0;
                TextOut(hDC,0,-3,aaa,1);
            }
            
            SelectObject(hDC,hpo);
            DeleteObject(hp);
            SelectObject(hDC,hbro);
            DeleteObject(hbr);

            hp = CreatePen(PS_SOLID,0,(GetFocus()==hwnd)?hicolor:color);
            hpo = (HPEN)SelectObject(hDC,hp);
            hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
            hbro = (HBRUSH)SelectObject(hDC,hbr);

            Rectangle(hDC,2,2,r.right-2,r.bottom-2);
            
            SelectObject(hDC,hpo);
            DeleteObject(hp);
            SelectObject(hDC,hbro);
            DeleteObject(hbr);
            
            BitBlt(hDCx,0,0,r.right,r.bottom,hDC,0,0,SRCCOPY);
            
            SelectObject(hDC,hBitmapo);
            DeleteObject(hBitmap);
            DeleteDC(hDC);
            
            EndPaint( hwnd, &ps );
        } break;

        case WM_TIMER:
            switch(wParam) {
                case 1: // mouse over efect
                    data->ef1+=((1000.0/(float)ACC_BUTTON_EFECT_LEN)/(float)ACC_BUTTON_EFECT_STEPS);
                    InvalidateRect(hwnd,NULL,0);
                    if (data->ef1>1.0) { KillTimer(hwnd,1); data->ef1=0; data->on = true; }
                break;
                case 2: // mouse out efect
                    data->ef2+=((1000.0/(float)ACC_BUTTON_EFECT_LEN)/(float)ACC_BUTTON_EFECT_STEPS);
                    InvalidateRect(hwnd,NULL,0);
                    if (data->ef2>1.0) { KillTimer(hwnd,2); data->ef2=0; data->on = false;}
                break;
                case 3: // odskrtnuti
                    data->ef3+=2*((1000.0/(float)ACC_BUTTON_EFECT_LEN)/(float)ACC_BUTTON_EFECT_STEPS);
                    InvalidateRect(hwnd,NULL,0);
                    if (data->ef3>1.0) { KillTimer(hwnd,3); data->ef3=0;}
                break;
            }        
        break;
        case WM_MOUSEMOVE: { 
            POINT pt = { LOWORD(lParam), HIWORD(lParam) };  
            RECT r; GetClientRect(hwnd,&r);
            
            if (PtInRect(&r,pt)) {
                if (!data->in) {
                    SetTimer(hwnd,1,ACC_BUTTON_EFECT_LEN/ACC_BUTTON_EFECT_STEPS,NULL);
                    data->in = true;
                }
                SetCapture(hwnd);
            } else {
                data->in = false;
                data->on = false;
                SetTimer(hwnd,2,ACC_BUTTON_EFECT_LEN/ACC_BUTTON_EFECT_STEPS,NULL);
                ReleaseCapture();
            }        
        } break;
        case WM_CAPTURECHANGED:
            if ((HWND)lParam!=hwnd) {
                data->in = false;
                data->on = false;
                SetTimer(hwnd,2,ACC_BUTTON_EFECT_LEN/ACC_BUTTON_EFECT_STEPS,NULL);
            }
        break;

        case TAUCHECK_SETCOLOR:
            data->color = (COLORREF)lParam;
        break;
        case TAUCHECK_SETHICOLOR:
            data->hicolor = (COLORREF)lParam;
        break;
        case TAUCHECK_SET:
            data->state = !!wParam;
            InvalidateRect(hwnd,NULL,0);
        break;
        case TAUCHECK_GET:
        return data->state;
  
        default: return DefWindowProc (hwnd, msg, wParam, lParam);
    }
    return 0;
}



