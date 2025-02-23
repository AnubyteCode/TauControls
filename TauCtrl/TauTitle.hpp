/*
  Name: TauTitle.hpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: Graphics title static control
*/
#ifndef __TAUTITLE_HPP
#define __TAUTITLE_HPP

#include <windows.h>
#include <stdio.h>
#include <string.h>

extern int InitTauTitleControl(HINSTANCE);

#define TAUTITLE_CLASS   "TauTitleClass"

// messages
#define TAUTITLE_SETCOLOR       (WM_USER+1)
#define TAUTITLE_SETFONT        (WM_USER+2)

#endif
