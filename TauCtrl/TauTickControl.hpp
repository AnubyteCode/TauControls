/*
  Name: TauTickControl.hpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: Tick-based control
  
  Handling: Posts EN_UPDATE to parent to notify change by user
*/
#ifndef __TAUTICKCONTROL_HPP
#define __TAUTICKCONTROL_HPP

#include <windows.h>
#include <stdio.h>
#include <string.h>

extern int InitTauTickControl(HINSTANCE);

#define TAUT_CLASS   "TauTickClass"

// messages
#define TAUT_SETCOLOR       (WM_USER+1)
#define TAUT_SETDATA        (WM_USER+2)
#define TAUT_GETDATA        (WM_USER+3)
#define TAUT_SETFCOLOR      (WM_USER+4)

// style
#define TAUTS_VOLUME        0
#define TAUTS_PAN           1
#define TAUTS_VERTICAL      2 // no efect with TAUTS_PAN
#define TAUTS_LARGE         4 // only with TAUTS_PAN
#define TAUTS_VISUAL        8 // mouse in and out efect

// to force control not to stole focus on click, use WS_DISABLED style

#endif
