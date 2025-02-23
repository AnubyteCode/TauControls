/*
  Name: TauIndex.hpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: TauIndex, simple indexing list
*/
#ifndef __TAUINDEX_HPP
#define __TAUINDEX_HPP

#include <windows.h>
#include <stdio.h>
#include <string.h>

extern int InitTauIndexControl(HINSTANCE);

#define TAUI_CLASS   "TauIndexClass"

// messages
#define TAUI_SETCOLOR       (WM_USER+1)
#define TAUI_SETTOP         (WM_USER+2)
#define TAUI_SETHILITE      (WM_USER+3)
#define TAUI_SETNDIGITS     (WM_USER+4)

#define TAUI_SETFONT        (WM_USER+5)
#define TAUI_SETHEIGHT      (WM_USER+6)

#define TAUI_SETHICOLOR     (WM_USER+7)

#endif

