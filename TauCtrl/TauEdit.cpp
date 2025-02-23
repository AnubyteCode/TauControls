/*
  Name: TauEdit.cpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@mx-3.cz
  Author: Jan Ringos, http://Tringi.Mx-3.cz
  Description: TauEdit Specialized Control
*/

#include "TauEdit.hpp"

#define ACC_BUTTON_EFECT_LEN   400 // in ms
#define ACC_BUTTON_EFECT_STEPS 40 // in steps

struct TauEdit_Data {
    COLORREF color, hicolor;
    HWND hStatic;    
    HBRUSH hBr;
    WNDPROC wpEdit;
    int over;
    float ef1,ef2,ef3;
};

LRESULT CALLBACK TauEdit_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK TauEdit_SubClassProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

bool TauEdit_already_inited = false;
__export int InitTauEditControl(HINSTANCE hInst) {
    if (TauEdit_already_inited) return 0;
    TauEdit_already_inited = true;

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

    wcl.lpszClassName = TAUE_CLASS;
    wcl.lpfnWndProc = TauEdit_Proc;
    RegisterClassEx(&wcl);

    return 0;
}

LRESULT CALLBACK TauEdit_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
    TauEdit_Data *data =  (TauEdit_Data*)GetWindowLong(hwnd,GWL_USERDATA);
    
//    if ((msg == WM_KEYDOWN || msg == WM_KEYUP)  && wParam == VK_TAB)
//        PostMessage( GetParent(hwnd), msg, wParam, lParam );
    
    switch (msg){
        case WM_CREATE: {
            data = (TauEdit_Data*) GlobalAlloc(GPTR,sizeof(TauEdit_Data));
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)data);
            // contructor
            RECT r;
            GetClientRect(hwnd,&r);

            data->color = 0x00FFFFFF;
            data->hicolor = 0x00FFFFFF;
            data->hBr = CreateSolidBrush(0);
            DWORD st = 0;
            bool border = GetWindowLong(hwnd,GWL_STYLE) & TAUES_BORDER;
            
            if (GetWindowLong(hwnd,GWL_STYLE) & TAUES_NUMBER) st = ES_NUMBER|ES_CENTER;
            
            data->hStatic = CreateWindow("EDIT","",WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL|WS_CLIPSIBLINGS|st,
                                         border?3:0,border?4:0,r.right-(border?6:0),r.bottom-(border?7:0),
                                         hwnd,NULL,GetModuleHandle(NULL),NULL);
            data->wpEdit = (WNDPROC) SetWindowLong(data->hStatic, 
                                       GWL_WNDPROC, (LONG) TauEdit_SubClassProc); 
                                       
            data->ef1 = data->ef2 = data->ef3 = 0.0;
            data->over = 0;
        } return 0;
        case WM_DESTROY:
            SetWindowLong(data->hStatic, GWL_WNDPROC, (LONG) data->wpEdit); 
            DeleteObject(data->hBr);
            GlobalFree(data);
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)NULL);
        break;
        case WM_CTLCOLOREDIT:
            SetTextColor( (HDC)wParam, (GetFocus()==data->hStatic)?data->hicolor:data->color );
            SetBkColor( (HDC)wParam, 0 );
        return (LRESULT)data->hBr;
        case WM_SETFOCUS:
            SetFocus( data->hStatic );
            InvalidateRect(hwnd,NULL,1);
        break;
        
        case WM_COMMAND:
            if (HIWORD(wParam) == EN_UPDATE)
                PostMessage( GetParent(hwnd), WM_COMMAND, 
                             MAKEWPARAM( GetWindowLong(hwnd,GWL_ID),EN_UPDATE),
                             (LONG)hwnd);
            if (HIWORD(wParam) == EN_SETFOCUS || HIWORD(wParam) == EN_KILLFOCUS) {
                InvalidateRect(hwnd,NULL,1);
            }
            if (HIWORD(wParam) == EN_ENTER) {
                MessageBox(hwnd,"ENTER","TauTracker",0);
            }
        break;
        
        case WM_PAINT: {
            HDC hDC;
            PAINTSTRUCT ps;
            hDC = BeginPaint( hwnd , &ps );
            
            
            if (GetWindowLong(hwnd,GWL_STYLE) & TAUES_BORDER) { // TODO: Animate by ef1 and ef2
                RECT r;
                GetClientRect(hwnd,&r);

                HPEN hpo,hp = CreatePen(PS_SOLID,0,(GetFocus()==data->hStatic)?data->hicolor:data->color);
                hpo = (HPEN)SelectObject(hDC,hp);
                HBRUSH hbro,hbr = CreateSolidBrush(0);
                hbro = (HBRUSH)SelectObject(hDC,hbr);

                Rectangle(hDC,2,2,r.right-2,r.bottom-2);

                if (data->over || data->ef2) {
                    if (data->ef1) {
                        SelectObject(hDC,hpo);
                        DeleteObject(hp);
                        hp = CreatePen(PS_SOLID,0,(GetFocus()==data->hStatic)?
                                       RGB(GetRValue(data->hicolor)*data->ef1,
                                           GetGValue(data->hicolor)*data->ef1,
                                           GetBValue(data->hicolor)*data->ef1):
                                       RGB(GetRValue(data->color)*data->ef1,
                                           GetGValue(data->color)*data->ef1,
                                           GetBValue(data->color)*data->ef1));
                        hpo = (HPEN)SelectObject(hDC,hp);
                    }
                    if (data->ef2) {
                        SelectObject(hDC,hpo);
                        DeleteObject(hp);
                        hp = CreatePen(PS_SOLID,0,(GetFocus()==data->hStatic)?
                                       RGB(GetRValue(data->hicolor)*(1.0-data->ef2),
                                           GetGValue(data->hicolor)*(1.0-data->ef2),
                                           GetBValue(data->hicolor)*(1.0-data->ef2)):
                                       RGB(GetRValue(data->color)*(1.0-data->ef2),
                                           GetGValue(data->color)*(1.0-data->ef2),
                                           GetBValue(data->color)*(1.0-data->ef2)));
                        hpo = (HPEN)SelectObject(hDC,hp);
                    }
                    MoveToEx(hDC,0,0,NULL); LineTo(hDC,r.right/3,0);
                    MoveToEx(hDC,0,0,NULL); LineTo(hDC,0,r.bottom/2);
                    MoveToEx(hDC,r.right-1,r.bottom-1,NULL); LineTo(hDC,2*r.right/3,r.bottom-1);
                    MoveToEx(hDC,r.right-1,r.bottom-1,NULL); LineTo(hDC,r.right-1,r.bottom/2);
                }
                
                SelectObject(hDC,hpo);
                DeleteObject(hp);
                SelectObject(hDC,hbro);
                DeleteObject(hbr);
            }
            
            EndPaint( hwnd, &ps );
        } break;
        
        case TAUE_SETFONT:
            SendMessage(data->hStatic,WM_SETFONT,(WPARAM)lParam,MAKELPARAM(1,0));
        break;
        case TAUE_SETCOLOR:
            data->color = (COLORREF)lParam;
        break;
        case TAUE_SETFCOLOR:
            data->hicolor = (COLORREF)lParam;
        break;
        
        case WM_SETTEXT:
        case WM_GETTEXT:
            SendMessage(data->hStatic,msg,wParam,lParam);
        break;

        case WM_TIMER:
            switch(wParam) {
                case 1: // mouse over efect
                    data->ef1+=((1000.0/(float)ACC_BUTTON_EFECT_LEN)/
                                        (float)ACC_BUTTON_EFECT_STEPS);
                    InvalidateBorder(hwnd);
                    if (data->ef1>1.0) { KillTimer(hwnd,1); data->ef1=0; data->over = true; }
                break;
                case 2: // mouse out efect
                    data->ef2+=((1000.0/(float)ACC_BUTTON_EFECT_LEN)/
                                        (float)ACC_BUTTON_EFECT_STEPS);
                    InvalidateBorder(hwnd);
                    if (data->ef2>1.0) { KillTimer(hwnd,2); data->ef2=0; data->over = false;}
                break;
            }
        break;
        
        default: return DefWindowProc (hwnd, msg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK TauEdit_SubClassProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
    TauEdit_Data *data =  (TauEdit_Data*)GetWindowLong(GetParent(hwnd),GWL_USERDATA);

    switch (msg){
        case WM_MOUSEMOVE: {
            POINT pt = { LOWORD(lParam), HIWORD(lParam) };  
            RECT r; GetClientRect(hwnd,&r);
            
            if (PtInRect(&r,pt)) {
                if (!data->over) {
                    data->over = true;
                    SetTimer(GetParent(hwnd),1,ACC_BUTTON_EFECT_LEN/ACC_BUTTON_EFECT_STEPS,NULL);
                }
                if (GetCapture()!=hwnd) SetCapture(hwnd);
            } else {
                data->over = false;
                SetTimer(GetParent(hwnd),2,ACC_BUTTON_EFECT_LEN/ACC_BUTTON_EFECT_STEPS,NULL);
                if (GetCapture()==hwnd) ReleaseCapture();
            }
        } break;
/*        case WM_CAPTURECHANGED: {
            if ((HWND)lParam!=hwnd) {
                data->over = false;
                SetTimer(GetParent(hwnd),2,ACC_BUTTON_EFECT_LEN/ACC_BUTTON_EFECT_STEPS,NULL);
            }
        } break;*/
    }

    return CallWindowProc( data->wpEdit, hwnd, msg, wParam, lParam); 
}


