/*
  Name: TauButton.hpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: aCoolControls library modified for GDI
*/
#ifndef __TAUBUTTON_HPP
#define __TAUBUTTON_HPP

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define PI 3.1415926535897932

extern int InitTauButtonControl(HINSTANCE);

#define ACCTRL_BUTTON   "aCCtrlButtonClass"

// messages
#define ACCM_SETCOLOR   (WM_USER+1)
#define ACCM_SETFONT    (WM_USER+3)

#endif

