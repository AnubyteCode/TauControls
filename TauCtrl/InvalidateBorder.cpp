/*
  Name: InvalidateBorder.cpp
  Copyright: Copyright (C) 2004, Jan Ringos
  Author: Jan Ringos, Tringi@mx-3.cz, http://Tringi.Mx-3.cz
  Description: Support function for TauControls
*/

#include "InvalidateBorder.hpp"

int InvalidateBorder(HWND hwnd, int width, BOOL flag) {
    RECT r, s;
    GetClientRect(hwnd, &r);
    
    CopyMemory(&s, &r, sizeof(RECT));
    s.bottom = width;
    InvalidateRect( hwnd, &s, flag );
    
    CopyMemory(&s, &r, sizeof(RECT));
    s.top = s.bottom - width;
    InvalidateRect( hwnd, &s, flag );
    
    CopyMemory(&s, &r, sizeof(RECT));
    s.right = width;
    InvalidateRect( hwnd, &s, flag );
    
    CopyMemory(&s, &r, sizeof(RECT));
    s.left = s.right - width;
    InvalidateRect( hwnd, &s, flag );

    return 0;
}


