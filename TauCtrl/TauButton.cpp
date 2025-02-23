/*
  Name: TauButton.cpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: aCoolControls library modified for GDI
*/

#include "TauButton.hpp"

#define ACC_BUTTON_EFECT_LEN   600 // in ms
#define ACC_BUTTON_EFECT_STEPS 40 // in steps

// internal structures
struct aCCtrl_Button_Data {
    HDC hDC;
    HFONT hFont;
    COLORREF color;
    COLORREF bkcolor; // always black
    
    float ef1,ef2,ef3;
    bool on,in;
};

// internal control procedures
LRESULT CALLBACK aCCtrl_Button_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

bool TauButton_already_inited = false;
__export int InitTauButtonControl(HINSTANCE hInst) {
    if (TauButton_already_inited) return 0;
    TauButton_already_inited = true;

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

    wcl.lpszClassName = ACCTRL_BUTTON;
    wcl.lpfnWndProc = aCCtrl_Button_Proc;
    RegisterClassEx(&wcl);

    return 0;
}

LRESULT CALLBACK aCCtrl_Button_Proc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam) {
    aCCtrl_Button_Data *data =  (aCCtrl_Button_Data*)GetWindowLong(hwnd,GWL_USERDATA);

//    if ((message == WM_KEYDOWN || message == WM_KEYUP)  && wParam == VK_TAB)
//        PostMessage( GetParent(hwnd), message, wParam, lParam );
    
    switch (message){
        case WM_CREATE:
            data = (aCCtrl_Button_Data*) GlobalAlloc(GPTR,sizeof(aCCtrl_Button_Data));
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)data);

            // contructor
            data->color = 0x00FFFFFF;
            data->hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
            data->ef1 = data->ef2 = data->ef3 = 0.0;
            data->on = false;
            
        return 0;
        case WM_SETFOCUS:
        case WM_KILLFOCUS:
            InvalidateRect(hwnd,NULL,1);
        break;
        case WM_DESTROY:
            GlobalFree(data);
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)NULL);
        break;
        case WM_SIZE:
            InvalidateRect(hwnd,NULL,1);
        break;
        case WM_PAINT: {
            HDC hDC,hDCx;
            PAINTSTRUCT ps;
            hDCx = BeginPaint( hwnd , &ps );
            hDC = CreateCompatibleDC(hDCx);
            
            Sleep(0);

            char aaa[256];
            RECT r;
            SIZE s;
            GetWindowText(hwnd,aaa,256);
//            sprintf(aaa,"%5.2f, %5.2f, %5.2f, %d %d",data->ef1,data->ef2,data->ef3,data->in,data->on);
            
            GetClientRect(hwnd,&r);
            HBITMAP hBitmapo,hBitmap =  CreateCompatibleBitmap(hDCx,r.right,r.bottom);
            hBitmapo = (HBITMAP)SelectObject(hDC,hBitmap);

            GetTextExtentPoint32(hDC,aaa,strlen(aaa),&s);

            SetBkMode(hDC,TRANSPARENT);
            SetTextColor(hDC,data->color);
            SetTextAlign(hDC,TA_CENTER|VTA_CENTER);         

            HPEN hpo,hp = CreatePen(PS_SOLID,0,data->color);
            hpo = (HPEN)SelectObject(hDC,hp);
            HBRUSH hbro,hbr = CreateSolidBrush(data->bkcolor);
            hbro = (HBRUSH)SelectObject(hDC,hbr);
            FillRect(hDC,&r,hbr);
            
            Rectangle(hDC,12,12,r.right-12+1,r.bottom-12+1);
            
            int tretx = r.right/3,trety = r.bottom/3;
            if (data->ef3>0) {
                tretx += (int)(tretx*2.0*sin(PI*data->ef3));
                trety += (int)(trety*2.0*sin(PI*data->ef3));
            }
            
            if (data->ef1>0){
                HPEN hpo,hp = CreatePen(PS_SOLID,0,
                                        RGB((1.0-data->ef1)*GetRValue(data->color),
                                            (1.0-data->ef1)*GetGValue(data->color), 
                                            (1.0-data->ef1)*GetBValue(data->color)));
                hpo = (HPEN)SelectObject(hDC,hp);
                MoveToEx(hDC,10,10,NULL); LineTo(hDC,10+tretx,10);
                MoveToEx(hDC,10,10,NULL); LineTo(hDC,10,10+trety);
                MoveToEx(hDC,r.right-10,r.bottom-10,NULL); LineTo(hDC,r.right-10-tretx,r.bottom-10);
                MoveToEx(hDC,r.right-10,r.bottom-10,NULL); LineTo(hDC,r.right-10,r.bottom-10-trety);
                SelectObject(hDC,hpo);
                DeleteObject(hp);
            } else {
                HPEN hpo,hp;
                if ( data->ef3 > 0.5) 
                    hp = CreatePen(PS_SOLID,0,
                                    RGB(2*(0.5-(1.0-data->ef3))*GetRValue(data->color),
                                        2*(0.5-(1.0-data->ef3))*GetGValue(data->color), 
                                        2*(0.5-(1.0-data->ef3))*GetBValue(data->color)));
                else
                    hp = CreatePen(PS_SOLID,0,
                                    RGB(2*(0.5-data->ef3)*GetRValue(data->color),
                                        2*(0.5-data->ef3)*GetGValue(data->color), 
                                        2*(0.5-data->ef3)*GetBValue(data->color)));
                hpo = (HPEN)SelectObject(hDC,hp);
                MoveToEx(hDC,10,10,NULL); LineTo(hDC,10+tretx,10);
                MoveToEx(hDC,10,10,NULL); LineTo(hDC,10,10+trety);
                MoveToEx(hDC,r.right-10,r.bottom-10,NULL); LineTo(hDC,r.right-10-tretx,r.bottom-10);
                MoveToEx(hDC,r.right-10,r.bottom-10,NULL); LineTo(hDC,r.right-10,r.bottom-10-trety);
                SelectObject(hDC,hpo);
                DeleteObject(hp);
            }

            if (data->on || GetFocus()==hwnd) {
                HPEN hpo,hp = CreatePen(PS_SOLID,0,
                                        RGB(0.7*GetRValue(data->color),
                                            0.7*GetGValue(data->color),
                                            0.7*GetBValue(data->color)));
                hpo = (HPEN)SelectObject(hDC,hp);
                Rectangle(hDC,14,14,r.right-14+1,r.bottom-14+1);
                SelectObject(hDC,hpo);
                DeleteObject(hp);
            }
            
            if (data->ef1>0){
                {
                    HPEN hpo,hp = CreatePen(PS_SOLID,0,
                                            RGB((0.7*data->ef1)*GetRValue(data->color),
                                                (0.7*data->ef1)*GetGValue(data->color),
                                                (0.7*data->ef1)*GetBValue(data->color)));
                    hpo = (HPEN)SelectObject(hDC,hp);
                    Rectangle(hDC,14,14,r.right-14+1,r.bottom-14+1);
                    SelectObject(hDC,hpo);
                    DeleteObject(hp);
                }

                int koef = 10-(int)(10*data->ef1);
                HPEN hpo,hp;
                if (data->ef3) {
                    if ( data->ef3 > 0.5) 
                        hp = CreatePen(PS_SOLID,0,
                                       RGB(2*(0.5-(1.0-data->ef3))*GetRValue(data->color),
                                           2*(0.5-(1.0-data->ef3))*GetGValue(data->color),
                                           2*(0.5-(1.0-data->ef3))*GetBValue(data->color)));
                    else
                        hp = CreatePen(PS_SOLID,0,
                                       RGB(2*(0.5-data->ef3)*GetRValue(data->color),
                                           2*(0.5-data->ef3)*GetGValue(data->color),
                                           2*(0.5-data->ef3)*GetBValue(data->color)));
                } else {
                    hp = CreatePen(PS_SOLID,0,
                                   RGB((data->ef1)*GetRValue(data->color),
                                       (data->ef1)*GetGValue(data->color),
                                       (data->ef1)*GetBValue(data->color)));
                }
                hpo = (HPEN)SelectObject(hDC,hp);
                MoveToEx(hDC,10-koef,10-koef,NULL); LineTo(hDC,10+tretx-koef,10-koef);
                MoveToEx(hDC,10-koef,10-koef,NULL); LineTo(hDC,10-koef,10+trety-koef);
                MoveToEx(hDC,koef+r.right-10,koef+r.bottom-10,NULL);
                    LineTo(hDC,koef+r.right-10-tretx,koef+r.bottom-10);
                MoveToEx(hDC,koef+r.right-10,koef+r.bottom-10,NULL);
                    LineTo(hDC,koef+r.right-10,koef+r.bottom-10-trety);
                SelectObject(hDC,hpo);
                DeleteObject(hp);
            }
            if (data->ef2>0) {
                HPEN hpo,hp = CreatePen(PS_SOLID,0,
                                        RGB((0.7-0.7*data->ef2)*GetRValue(data->color),
                                            (0.7-0.7*data->ef2)*GetGValue(data->color),
                                            (0.7-0.7*data->ef2)*GetBValue(data->color)));
                hpo = (HPEN)SelectObject(hDC,hp);
                Rectangle(hDC,14,14,r.right-14+1,r.bottom-14+1);
                SelectObject(hDC,hpo);
                DeleteObject(hp);
            }
            
            SelectObject(hDC,hpo);
            DeleteObject(hp);
            
            HFONT hfo = (HFONT)SelectObject(hDC,data->hFont);
            TextOut(hDC,r.right/2,r.bottom/2-7,aaa,strlen(aaa));
            SelectObject(hDC,hfo);

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
                case 3: // mouse click efect
                    data->ef3+=((1000.0/(float)ACC_BUTTON_EFECT_LEN)/(float)ACC_BUTTON_EFECT_STEPS);
                    InvalidateRect(hwnd,NULL,0);
                    if (data->ef3>1.0) { 
                        KillTimer(hwnd,3);
                        data->ef3=0;
                        PostMessage( GetParent(hwnd), WM_COMMAND, 
                                     MAKEWPARAM( GetWindowLong(hwnd,GWL_ID), BN_CLICKED),
                                     (LONG)hwnd);
                    }
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
        
        case WM_CHAR:
            if (wParam!=13&&wParam!=32) break;
                    
        case WM_LBUTTONDOWN: {
            SetTimer(hwnd,3,ACC_BUTTON_EFECT_LEN/ACC_BUTTON_EFECT_STEPS,NULL);
        } break;        
        case ACCM_SETCOLOR:
            data->color = (COLORREF)lParam;
        break;
        case WM_SETFONT:
            data->hFont = (HFONT)wParam;
        break;
        case ACCM_SETFONT:
            data->hFont = (HFONT)lParam;
        break;
  
        default: return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}


