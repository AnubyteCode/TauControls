/*
  Name: TauDivider.hpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: Great divider efect window
*/
#ifndef __TAUDIVIDER_HPP
#define __TAUDIVIDER_HPP

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define PI 3.1415926535897932

extern int InitTauDividerControl(HINSTANCE);

#define TAUD_CLASS   "TauDividerClass"

// messages
#define TAUD_SETCOLOR   (WM_USER+1)

#endif

