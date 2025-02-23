/*
  Name: MSStatic.hpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: MiltiString Static Control
*/
#ifndef __MSSTATIC_HPP
#define __MSSTATIC_HPP

#include <windows.h>
#include <stdio.h>
#include <string.h>

extern int InitMSStaticControl(HINSTANCE);

#define MSST_CLASS   "MultiStringStaticClass"

// messages
#define MSST_SETCOLOR       (WM_USER+1)
#define MSST_ADDSTRING      (WM_USER+2)
#define MSST_CLEAR          (WM_USER+3)
#define MSST_SETFONT        (WM_USER+4)

#define MSST_SET            (WM_USER+10)

#define MSST_LIMIT 1008

#endif

