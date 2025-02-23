/*
  Name: TauList.cpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: TauList, text item list control
*/

#include "TauList.hpp"
#include "TauIndex.hpp"

struct TauList_Data {
    COLORREF color,hicolor;
    
    int top;
    int cur;
    int limit;
    
    HFONT hFont;
    
    int reserved1;
    HWND hIndex;
    char *sz[TAUL_LIMIT];
};

LRESULT CALLBACK TauList_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

bool TauList_already_inited = false;
__export int InitTauListControl(HINSTANCE hInst) {
    if (TauList_already_inited) return 0;
    TauList_already_inited = true;

    WNDCLASSEX wcl;
    wcl.cbSize = sizeof(WNDCLASSEX);
    wcl.style = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
    wcl.cbClsExtra = 0;
    wcl.cbWndExtra = 0;
    wcl.hInstance = hInst;
    wcl.hIcon = NULL;
    wcl.hIconSm = NULL;
    wcl.hCursor = LoadCursor(NULL,IDC_ARROW);
    wcl.hbrBackground = CreateSolidBrush( 0 );
    wcl.lpszMenuName = NULL;

    wcl.lpszClassName = TAUL_CLASS;
    wcl.lpfnWndProc = TauList_Proc;
    RegisterClassEx(&wcl);
    
    InitTauIndexControl(hInst);

    return 0;
}

LRESULT CALLBACK TauList_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
    TauList_Data *data =  (TauList_Data*)GetWindowLong(hwnd,GWL_USERDATA);
    
//    if ((msg == WM_KEYDOWN || msg == WM_KEYUP)  && wParam == VK_TAB)
//        PostMessage( GetParent(hwnd), msg, wParam, lParam );
    
    switch (msg){
        case WM_CREATE:
            data = (TauList_Data*) GlobalAlloc(GPTR,sizeof(TauList_Data));
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)data);
            // contructor
            RECT r;
            GetClientRect(hwnd,&r);

            data->color = 0x00FFFFFF;
            data->hicolor = 0x00FFFFFF;
            data->limit = TAUL_LIMIT-1;
            
            data->hIndex = CreateWindow(TAUI_CLASS,"",WS_CHILD|WS_VISIBLE,
                                        0,3,36,r.bottom,hwnd,NULL,
                                        GetModuleHandle(NULL),NULL);
            SendMessage(data->hIndex,TAUI_SETNDIGITS,4,lParam);
            SendMessage(data->hIndex,TAUI_SETHEIGHT,18,lParam);
            SendMessage(data->hIndex,TAUI_SETTOP,0,lParam);
            SendMessage(data->hIndex,TAUI_SETHILITE,0xFFFFFFFF,lParam);
        return 0;
        case WM_DESTROY:
            DestroyWindow(data->hIndex);
            for (int i=0;i<TAUL_LIMIT;i++) if (data->sz[i]) { GlobalFree(data->sz[i]); data->sz[i]=NULL; }
            GlobalFree(data);
            SetWindowLong(hwnd,GWL_USERDATA,(LONG)NULL);
        break;
        
        case WM_SETFOCUS: {
            SendMessage(data->hIndex,TAUI_SETHILITE,data->cur,lParam);
            RECT r = { 36,0,2048,2048 };
            InvalidateRect(hwnd,&r,0); 
        } break;
        case WM_KILLFOCUS: {
            SendMessage(data->hIndex,TAUI_SETHILITE,0xFFFFFFFF,lParam);
            RECT r = { 36,0,2048,2048 };
            InvalidateRect(hwnd,&r,0); 
            ReleaseCapture();
            
            KillTimer(hwnd,1);
            KillTimer(hwnd,2);
        } break;
        case WM_LBUTTONDOWN: {
            POINT pt = { (signed short)LOWORD(lParam), (signed short)HIWORD(lParam) };
            RECT r; GetClientRect(hwnd,&r);
            if (!PtInRect(&r,pt)) { SetFocus(NULL); break; }
            
            data->cur = data->top + HIWORD(lParam)/18;
            SendMessage(data->hIndex,TAUI_SETHILITE,data->cur,lParam);
            
            RECT br = { 36,0,2048,2048 };
            InvalidateRect(hwnd,&br,0); 
            SetFocus(hwnd);

            PostMessage( GetParent(hwnd), WM_COMMAND, 
                         MAKEWPARAM( GetWindowLong(hwnd,GWL_ID),LBN_SELCHANGE),
                         (LONG)hwnd);
        } break;
/*        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
            SetFocus( GetParent(hwnd) );
        break;*/
        
        case WM_CHAR:
            if (wParam!=13) break;
        case WM_LBUTTONDBLCLK:
            PostMessage( GetParent(hwnd), WM_COMMAND, 
                         MAKEWPARAM( GetWindowLong(hwnd,GWL_ID),LBN_DBLCLK),
                         (LONG)hwnd);
        break;
        
        case TAUL_GETCURSEL:
            return data->cur;
        case TAUL_GETCELLPOS: {
            RECT r,s; 
            GetWindowRect( hwnd,&r );
            GetWindowRect( GetParent(hwnd), &s );
            int x = r.left - s.left;
            int y = r.top - s.top;
            
            return MAKELRESULT(36+x,(data->cur-data->top)*18+y);
        } break;
        
        case WM_MOUSEMOVE: {
            POINT pt = { (signed short)LOWORD(lParam), (signed short)HIWORD(lParam) };
            RECT r; GetClientRect(hwnd,&r);
            if (pt.x >= r.left && pt.x <= r.right){
                if (GetFocus()==hwnd) SetCapture(hwnd);
            }
            
            if (GetCapture()==hwnd) {
                KillTimer(hwnd,1);
                KillTimer(hwnd,2);
                // pokud mys neni v okne
                
                if (pt.x < r.left || pt.x > r.right){
                    ReleaseCapture();
                    break;
                }
                
                if (GetWindowLong(hwnd,GWL_STYLE)&TAULS_NOSLIDE) {
                    if (pt.y < r.top || pt.y > r.bottom){
                        ReleaseCapture();
                        break;
                    }
                }
                
                
                if (pt.y < r.top) {
                    // posunout nahoru
                    SetTimer(hwnd,1,20,NULL);
                }
                if (pt.y > r.bottom) {
                    // posunout dolu
                    SetTimer(hwnd,2,20,NULL);
                }
            }        
        } break;
        
        case WM_MOUSEWHEEL: {
            int z = (signed short)HIWORD(wParam)/WHEEL_DELTA;
            if (z>0) {
                PostMessage(hwnd,WM_KEYDOWN,VK_UP,0);
                PostMessage(hwnd,WM_KEYUP,VK_UP,0);
            } else {
                PostMessage(hwnd,WM_KEYDOWN,VK_DOWN,0);
                PostMessage(hwnd,WM_KEYUP,VK_DOWN,0);
            }        
        } break;
        
        case WM_TIMER:
            switch(wParam) {
                case 1:
                    PostMessage(hwnd,WM_KEYDOWN,VK_UP,0);
                    PostMessage(hwnd,WM_KEYUP,VK_UP,0);
                break;
                case 2:
                    PostMessage(hwnd,WM_KEYDOWN,VK_DOWN,0);
                    PostMessage(hwnd,WM_KEYUP,VK_DOWN,0);
                break;
            }
        break;
        
        case WM_PAINT: {
            HDC hDC;
            PAINTSTRUCT ps;
            hDC = BeginPaint( hwnd , &ps );
            
            RECT r;
            GetClientRect(hwnd,&r);
            HBRUSH hbro,hbr = CreateSolidBrush(
             RGB(GetRValue(data->color)/4,
                 GetGValue(data->color)/4,
                 GetBValue(data->color)/4));
            hbro = (HBRUSH)SelectObject(hDC,hbr);
//            FillRect(hDC,&r,hbr);
            
            int max = (r.bottom-1)/18;
            
            HPEN hpo,hp = CreatePen(PS_SOLID,0,data->color);
            hpo = (HPEN)SelectObject(hDC,hp);
            MoveToEx(hDC,36,1,NULL);
            LineTo(hDC,36,r.bottom-1);
            MoveToEx(hDC,r.right-1,1,NULL);
            LineTo(hDC,r.right-1,r.bottom-1);
            SelectObject(hDC,hpo);
            DeleteObject(hp);
            
            for (int i=0;i<max;i++) {
                HPEN hpo,hp = CreatePen(PS_SOLID,0,0);
                hpo = (HPEN)SelectObject(hDC,hp);
    
                Rectangle(hDC,38,i*18,r.right-2,i*18+19);
                
                SelectObject(hDC,hpo);
                DeleteObject(hp);
            }
            int n = data->cur-data->top;
            if (GetFocus() == hwnd && n>=0 && n<max) {
                HPEN hpo,hp = CreatePen(PS_SOLID,0,data->hicolor);
                hpo = (HPEN)SelectObject(hDC,hp);
    
                Rectangle(hDC,38,n*18,r.right-2,n*18+19);
                
                SelectObject(hDC,hpo);
                DeleteObject(hp);
            }

            SelectObject(hDC,data->hFont);
            SetBkMode(hDC,TRANSPARENT);
            for (int i=0;i<max;i++) {
                SetTextColor(hDC,(GetFocus()==hwnd && i+data->top==data->cur)?data->hicolor:data->color);
                RECT br = {37,i*18+1,r.right-1,i*18+18};
                if (data->sz[i+data->top]!=NULL)
                DrawText(hDC,data->sz[i+data->top],strlen(data->sz[i+data->top]),
                         &br,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
            }
            
            SelectObject(hDC,hbro);
            DeleteObject(hbr);

            EndPaint( hwnd, &ps );
        } break;


        case WM_SETFONT:
            data->hFont = (HFONT)wParam;
            SendMessage(data->hIndex,TAUI_SETFONT,0,wParam);
        break;
        case TAUL_SETFONT:
            data->hFont = (HFONT)lParam;
            SendMessage(data->hIndex,TAUI_SETFONT,0,lParam);
        break;

        case TAUL_SETCOLOR:
            data->color = (COLORREF)lParam;
            SendMessage(data->hIndex,TAUI_SETCOLOR,0,lParam);
        break;
        case TAUL_SETHICOLOR:
            data->hicolor = (COLORREF)lParam;
            SendMessage(data->hIndex,TAUI_SETHICOLOR,0,lParam);
        break;
        case TAUL_CLEAR:
            for (int i=0;i<TAUL_LIMIT;i++) if (data->sz[i]) { GlobalFree(data->sz[i]); data->sz[i]=NULL; }
        break;
        case TAUL_SETTEXT:
            if (wParam<0||wParam>=TAUL_LIMIT) break;
            
            if (data->sz[wParam] == NULL) data->sz[wParam] = (char*)GlobalAlloc(GPTR,1024);
            if (lParam == 0){
                GlobalFree(data->sz[wParam]);
                data->sz[wParam] = NULL;
            } else {
                strncpy(data->sz[wParam],(char*)lParam,1024);
            }            
        break;
        case TAUL_GETTEXT:
            if (wParam<0||wParam>=TAUL_LIMIT||lParam==0) break;
            
            if (data->sz[wParam] == NULL)
                strcpy((char*)lParam,"");
            else
                strcpy((char*)lParam,data->sz[wParam]);
        break;
        case TAUL_SETLIMIT:
            if (wParam<=0 || wParam>=TAUL_LIMIT) break;
            data->limit = wParam;
        break;
        case WM_KEYDOWN:
            switch(wParam) {
                case VK_UP: {
                    if (data->cur){ data->cur--;
                        if (data->cur < data->top) data->top--;
    
                        SendMessage(data->hIndex,TAUI_SETHILITE,data->cur,lParam);
                        SendMessage(data->hIndex,TAUI_SETTOP,data->top,lParam);
                        RECT r = { 36,0,2048,2048 };
                        InvalidateRect(hwnd,&r,0); 
                        PostMessage( GetParent(hwnd), WM_COMMAND, 
                                     MAKEWPARAM( GetWindowLong(hwnd,GWL_ID),LBN_SELCHANGE),
                                     (LONG)hwnd);
                    } else {
                        KillTimer(hwnd,1);
                    }
                } break;
                case VK_DOWN: {
                    RECT r;
                    GetClientRect(hwnd,&r);
                    
                    if (data->cur < data->limit){ data->cur++;
                        int max = (r.bottom-1)/18;
                        if (data->cur >= data->top+max) data->top++;
    
                        SendMessage(data->hIndex,TAUI_SETHILITE,data->cur,lParam);
                        SendMessage(data->hIndex,TAUI_SETTOP,data->top,lParam);
                        RECT br = { 36,0,2048,2048 };
                        InvalidateRect(hwnd,&br,0); 
                        PostMessage( GetParent(hwnd), WM_COMMAND, 
                                     MAKEWPARAM( GetWindowLong(hwnd,GWL_ID),LBN_SELCHANGE),
                                     (LONG)hwnd);
                    } else {
                        KillTimer(hwnd,2);
                    }
                } break;
                case VK_HOME: {
                    if (data->cur){
                        data->cur = 0;
                        data->top = 0;
    
                        SendMessage(data->hIndex,TAUI_SETHILITE,data->cur,lParam);
                        SendMessage(data->hIndex,TAUI_SETTOP,data->top,lParam);
                        RECT r = { 36,0,2048,2048 };
                        InvalidateRect(hwnd,&r,0); 
                        PostMessage( GetParent(hwnd), WM_COMMAND, 
                                     MAKEWPARAM( GetWindowLong(hwnd,GWL_ID),LBN_SELCHANGE),
                                     (LONG)hwnd);
                    }
                } break;
                case VK_END: {
                    if (data->cur < data->limit-1){
                        RECT r1;
                        GetClientRect(hwnd,&r1);
                        int max = (r1.bottom-1)/18;

                        data->cur = data->limit;
                        data->top = data->limit-max+1;
    
                        SendMessage(data->hIndex,TAUI_SETHILITE,data->cur,lParam);
                        SendMessage(data->hIndex,TAUI_SETTOP,data->top,lParam);
                        RECT r = { 36,0,2048,2048 };
                        InvalidateRect(hwnd,&r,0); 
                        PostMessage( GetParent(hwnd), WM_COMMAND, 
                                     MAKEWPARAM( GetWindowLong(hwnd,GWL_ID),LBN_SELCHANGE),
                                     (LONG)hwnd);
                    }
                } break;
                case VK_PRIOR: {
                    RECT r1;
                    GetClientRect(hwnd,&r1);
                    int max = (r1.bottom-1)/18;

                    if (data->cur <= max){
                        data->cur = 0;
                        data->top = 0;
                    } else {
                        data->cur -= max;
                        if ( data->top > max)
                            data->top -= max;
                        else
                            data->top = 0;
                    }
                    
                    SendMessage(data->hIndex,TAUI_SETHILITE,data->cur,lParam);
                    SendMessage(data->hIndex,TAUI_SETTOP,data->top,lParam);
                    RECT r = { 36,0,2048,2048 };
                    InvalidateRect(hwnd,&r,0); 
                    PostMessage( GetParent(hwnd), WM_COMMAND, 
                                 MAKEWPARAM( GetWindowLong(hwnd,GWL_ID),LBN_SELCHANGE),
                                 (LONG)hwnd);
                } break;
                case VK_NEXT: {
                    RECT r1;
                    GetClientRect(hwnd,&r1);
                    int max = (r1.bottom-1)/18;

                    if (data->cur > data->limit-max){
                        data->cur = data->limit;
                        data->top = data->limit+1-max;
                    } else {
                        data->cur += max;
                        data->top += max;
                    }
                    
                    SendMessage(data->hIndex,TAUI_SETHILITE,data->cur,lParam);
                    SendMessage(data->hIndex,TAUI_SETTOP,data->top,lParam);
                    RECT r = { 36,0,2048,2048 };
                    InvalidateRect(hwnd,&r,0); 
                    PostMessage( GetParent(hwnd), WM_COMMAND, 
                                 MAKEWPARAM( GetWindowLong(hwnd,GWL_ID),LBN_SELCHANGE),
                                 (LONG)hwnd);
                } break;

                default:
                    KillTimer(hwnd,1);
                    KillTimer(hwnd,2);
                    ReleaseCapture();
                break;
            
            }
        break;

        default: return DefWindowProc (hwnd, msg, wParam, lParam);
    }
    return 0;
}


