/*
  Name: TauTickControl.hpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: Tick-based control
*/

#include "TauTickControl.hpp"
#define ACC_BUTTON_EFECT_LEN   600 // in ms
#define ACC_BUTTON_EFECT_STEPS 40 // in steps

struct TauTick_Data {
    COLORREF color,hicolor;
    int curpos;
    float ef1,ef2;
    bool in;
};

LRESULT CALLBACK TauTick_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

bool TauTick_already_inited = false;
__export int InitTauTickControl(HINSTANCE hInst) {
    if (TauTick_already_inited) return 0;
    TauTick_already_inited = true;

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

    wcl.lpszClassName = TAUT_CLASS;
    wcl.lpfnWndProc = TauTick_Proc;
    RegisterClassEx(&wcl);

    return 0;
}

LRESULT CALLBACK TauTick_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
    TauTick_Data *data =  (TauTick_Data*)GetWindowLong(hwnd,GWL_USERDATA);
    
//    if ((msg == WM_KEYDOWN || msg == WM_KEYUP)  && wParam == VK_TAB)
//        PostMessage( GetParent(hwnd), msg, wParam, lParam );
    
    switch (msg){
        case WM_CREATE: {
            data = (TauTick_Data*) GlobalAlloc(GPTR,sizeof(TauTick_Data));
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)data);
            // contructor
            RECT r;
            GetClientRect(hwnd,&r);

            data->color = 0x00FFFFFF;
            data->hicolor = 0x00FFFFFF;
            data->ef1 = data->ef2 = 0.0;
        } return 0;
        case WM_DESTROY:
            GlobalFree(data);
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)NULL);
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
            
            Sleep(0);

            hBitmap = CreateCompatibleBitmap(hDCx,r.right,r.bottom);
            hBitmapo = (HBITMAP)SelectObject(hDC,hBitmap);
            
            HPEN hpo,hp = CreatePen(PS_SOLID,0,(GetFocus()==hwnd)?hicolor:color);
            hpo = (HPEN)SelectObject(hDC,hp);
            HBRUSH hbro,hbr = CreateSolidBrush(0);
            hbro = (HBRUSH)SelectObject(hDC,hbr);
            FillRect(hDC,&r,hbr);

            int posun = 2*(!!(GetWindowLong(hwnd,GWL_STYLE) & TAUTS_VISUAL));

            Rectangle(hDC,0+posun,0+posun,r.right-posun,r.bottom-posun);
            
            SelectObject(hDC,hbro);
            DeleteObject(hbr);
            hbr = CreateSolidBrush(
                  (GetFocus()==hwnd)?
                      RGB(GetRValue(hicolor)/2,GetGValue(hicolor)/2,GetBValue(hicolor)/2)
                  :
                      RGB(GetRValue(color)/2,GetGValue(color)/2,GetBValue(color)/2)
                  );
            hbro = (HBRUSH)SelectObject(hDC,hbr);

            if (GetWindowLong(hwnd,GWL_STYLE) & TAUTS_VERTICAL) {
                if (GetWindowLong(hwnd,GWL_STYLE) & TAUTS_PAN) {
                    if (GetWindowLong(hwnd,GWL_STYLE) & TAUTS_LARGE) {
                        int w = (r.bottom)/10;
                        int pos = ((255-data->curpos)*(r.bottom-4-w-2*posun))/255;
                        Rectangle(hDC,2+posun,2+pos+posun,r.right-2-posun,2+pos+w+posun);
                    } else { // NORMAL
                        int pos = ((255-data->curpos)*(r.bottom-5-2*posun))/255;
                        MoveToEx(hDC,2+posun,2+pos+posun,NULL);
                        LineTo(hDC,r.right-2-posun,2+pos+posun);
                    }                
                } else { // VOLUME
                    int pos = ((255-data->curpos)*(r.bottom-5-2*posun))/255;
                    for (int i=(r.bottom-5-2*posun);i>pos;i-=2) {
                        MoveToEx(hDC,2+posun,2+i+posun,NULL);
                        LineTo(hDC,r.right-2-posun,2+i+posun);
                    }
                }            
            } else { // HORIZONTAL
                if (GetWindowLong(hwnd,GWL_STYLE) & TAUTS_PAN) {
                    if (GetWindowLong(hwnd,GWL_STYLE) & TAUTS_LARGE) {
                        int w = r.right/10;
                        int pos = (data->curpos*(r.right-4-w-2*posun))/255;
                        Rectangle(hDC,2+pos+posun,2+posun,2+pos+w+posun,r.bottom-2-posun);
                    } else { // NORMAL
                        int pos = (data->curpos*(r.right-5-2*posun))/255;
                        MoveToEx(hDC,2+pos+posun,2+posun,NULL);
                        LineTo(hDC,2+pos+posun,r.bottom-2-posun);
                    }                
                } else { // VOLUME
                    int pos = (data->curpos*(r.right-5-2*posun))/255;
                    for (int i=0;i<pos;i+=2) {
                        MoveToEx(hDC,2+i+posun,2+posun,NULL);
                        LineTo(hDC,2+i+posun,r.bottom-2-posun);
                    }
                }            
            }            
            
            if (posun && (data->in || data->ef2)) {
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
                MoveToEx(hDC,0,0,NULL);
                if (GetWindowLong(hwnd,GWL_STYLE) & TAUTS_VERTICAL)
                    LineTo(hDC,r.right/2,0); else  LineTo(hDC,r.right/3,0);
                MoveToEx(hDC,0,0,NULL);
                if (GetWindowLong(hwnd,GWL_STYLE) & TAUTS_VERTICAL)
                    LineTo(hDC,0,r.bottom/3); else LineTo(hDC,0,r.bottom/2);
                MoveToEx(hDC,r.right-1,r.bottom-1,NULL);
                if (GetWindowLong(hwnd,GWL_STYLE) & TAUTS_VERTICAL)
                    LineTo(hDC,r.right/2,r.bottom-1); else LineTo(hDC,2*r.right/3,r.bottom-1);
                MoveToEx(hDC,r.right-1,r.bottom-1,NULL);
                if (GetWindowLong(hwnd,GWL_STYLE) & TAUTS_VERTICAL)
                    LineTo(hDC,r.right-1,2*r.bottom/3); else LineTo(hDC,r.right-1,r.bottom/2);
            }
                        
            SelectObject(hDC,hpo);
            DeleteObject(hp);
            SelectObject(hDC,hbro);
            DeleteObject(hbr);
            
/*            char aaa[256];
            sprintf(aaa,"%d",data->curpos);
            TextOut(hDC,0,0,aaa,strlen(aaa));*/
            
            BitBlt(hDCx,0,0,r.right,r.bottom,hDC,0,0,SRCCOPY);

            SelectObject(hDC,hBitmapo);
            DeleteObject(hBitmap);
            DeleteDC(hDC);
            
            EndPaint( hwnd, &ps );
        } break;
        
        case TAUT_SETCOLOR:
            data->color = (COLORREF)lParam;
        break;
        case TAUT_SETFCOLOR:
            data->hicolor = (COLORREF)lParam;
        break;
        
        case TAUT_GETDATA:
        return data->curpos;
        case TAUT_SETDATA:
            if (wParam>=0&&wParam<256) {
                data->curpos = wParam;
                InvalidateRect(hwnd,NULL,1);
            }
        break;
        
        case WM_KEYDOWN: {
            switch(wParam) {
                case VK_UP:
                    if (GetWindowLong(hwnd,GWL_STYLE) & TAUTS_VERTICAL) {
                        if (data->curpos<255) {
                            data->curpos++;
                            InvalidateRect(hwnd,NULL,0);
                            PostMessage( GetParent(hwnd), WM_COMMAND, 
                                         MAKEWPARAM( GetWindowLong(hwnd,GWL_ID),EN_UPDATE),
                                         (LONG)hwnd);
                        }
                    }
                break;
                case VK_RIGHT:
                    if (!(GetWindowLong(hwnd,GWL_STYLE) & TAUTS_VERTICAL)) {
                        if (data->curpos<255) {
                            data->curpos++;
                            InvalidateRect(hwnd,NULL,0);
                            PostMessage( GetParent(hwnd), WM_COMMAND, 
                                         MAKEWPARAM( GetWindowLong(hwnd,GWL_ID),EN_UPDATE),
                                         (LONG)hwnd);
                        }
                    }
                break;
                case VK_DOWN:
                    if (GetWindowLong(hwnd,GWL_STYLE) & TAUTS_VERTICAL) {
                        if (data->curpos>0) {
                            data->curpos--;
                            InvalidateRect(hwnd,NULL,0);
                            PostMessage( GetParent(hwnd), WM_COMMAND, 
                                         MAKEWPARAM( GetWindowLong(hwnd,GWL_ID),EN_UPDATE),
                                         (LONG)hwnd);
                        }
                    }
                break;
                case VK_LEFT:
                    if (!(GetWindowLong(hwnd,GWL_STYLE) & TAUTS_VERTICAL)) {
                        if (data->curpos>0) {
                            data->curpos--;
                            InvalidateRect(hwnd,NULL,0);
                            PostMessage( GetParent(hwnd), WM_COMMAND, 
                                         MAKEWPARAM( GetWindowLong(hwnd,GWL_ID),EN_UPDATE),
                                         (LONG)hwnd);
                        }
                    }
                break;
                case VK_HOME:
                    if (data->curpos>0) {
                        data->curpos=0;
                        InvalidateRect(hwnd,NULL,0);
                        PostMessage( GetParent(hwnd), WM_COMMAND, 
                                     MAKEWPARAM( GetWindowLong(hwnd,GWL_ID),EN_UPDATE),
                                     (LONG)hwnd);
                    }
                break;
                case VK_END:
                    if (data->curpos<255) {
                        data->curpos=255;
                        InvalidateRect(hwnd,NULL,0);
                        PostMessage( GetParent(hwnd), WM_COMMAND, 
                                     MAKEWPARAM( GetWindowLong(hwnd,GWL_ID),EN_UPDATE),
                                     (LONG)hwnd);
                    }
                break;
            
            }        
        } break;

        case WM_LBUTTONUP: {
            RECT r;
            GetClientRect(hwnd,&r);
            POINT pt = { LOWORD(lParam), HIWORD(lParam) };
            ReleaseCapture();
            if (data->in && !PtInRect(&r,pt)) {
                data->in = false;
                SetTimer(hwnd,2,ACC_BUTTON_EFECT_LEN/ACC_BUTTON_EFECT_STEPS,NULL);
            }
        } break;
        case WM_LBUTTONDOWN:
            SetFocus(hwnd);
            SetCapture(hwnd);
        case WM_MOUSEMOVE: {
            if (wParam&MK_LBUTTON) { // tracking
                RECT r;
                GetClientRect(hwnd,&r);
                POINT pt = { LOWORD(lParam), HIWORD(lParam) };
                
                int max = r.right-4;
                int cur = (signed short)(LOWORD(lParam))-2;
                int proc = (255*cur)/(max);

                if (GetWindowLong(hwnd,GWL_STYLE) & TAUTS_VERTICAL){
                    max = r.bottom-4;
                    cur = (signed short)(HIWORD(lParam))-2;
                    proc = 255-(255*cur)/(max);
                }

                if (proc<0) proc = 0;
                if (proc>255) proc = 255;
                
                data->curpos = proc;
                
                InvalidateRect(hwnd,NULL,0);
                PostMessage( GetParent(hwnd), WM_COMMAND, 
                             MAKEWPARAM( GetWindowLong(hwnd,GWL_ID),EN_UPDATE),
                             (LONG)hwnd);
                
            } else {
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
                    SetTimer(hwnd,2,ACC_BUTTON_EFECT_LEN/ACC_BUTTON_EFECT_STEPS,NULL);
                    ReleaseCapture();
                }        
            }
        } break;

        case WM_TIMER:
            switch(wParam) {
                case 1: // mouse over efect
                    data->ef1+=((1000.0/(float)ACC_BUTTON_EFECT_LEN)/(float)ACC_BUTTON_EFECT_STEPS);
                    InvalidateRect(hwnd,NULL,0);
                    if (data->ef1>1.0) { KillTimer(hwnd,1); data->ef1=0; data->in = true; }
                break;
                case 2: // mouse out efect
                    data->ef2+=((1000.0/(float)ACC_BUTTON_EFECT_LEN)/(float)ACC_BUTTON_EFECT_STEPS);
                    InvalidateRect(hwnd,NULL,0);
                    if (data->ef2>1.0) { KillTimer(hwnd,2); data->ef2=0; data->in = false;}
                break;
            }
        break;
  
        default: return DefWindowProc (hwnd, msg, wParam, lParam);
    }
    return 0;
}



