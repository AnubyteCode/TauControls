/*
  Name: TauVU.hpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: Simple graphics VU meter
*/
#ifndef __TAUVU_HPP
#define __TAUVU_HPP

#include <windows.h>
#include <stdio.h>
#include <string.h>

extern int InitTauVUControl(HINSTANCE);

#define TAUVU_CLASS   "TauVUClass"

// messages
#define TAUVU_SETCOLOR       (WM_USER+1)
#define TAUVU_SET            (WM_USER+2)

#define NORMALVU    0x0000FF00
#define HIGHVU      0x0000FFFF
#define CRITICALVU  0x000000FF

#endif
