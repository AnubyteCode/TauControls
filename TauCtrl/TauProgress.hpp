/*
  Name: TauProgress.hpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: Progress bar
*/
#ifndef __TAUPROGRESS_HPP
#define __TAUPROGRESS_HPP

#include <windows.h>
#include <stdio.h>
#include <string.h>

extern int InitTauProgressControl(HINSTANCE);

#define TAUP_CLASS   "TauProgressClass"

// messages
#define TAUP_SETCOLOR       (WM_USER+1)
#define TAUP_SETPOS         (WM_USER+2)
#define TAUP_STEP           (WM_USER+3)

#endif
