/*
  Name: TauCheck.hpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: Graphics check box
  
  Handling: When changed, posts BN_CLICKED to parent window
*/
#ifndef __TAUCHECK_HPP
#define __TAUCHECK_HPP

#include <windows.h>
#include <stdio.h>
#include <string.h>

extern int InitTauCheckControl(HINSTANCE);

#define TAUCHECK_CLASS   "TauCheckClass"

// messages
#define TAUCHECK_SETCOLOR       (WM_USER+1)
#define TAUCHECK_SET            (WM_USER+2)
#define TAUCHECK_GET            (WM_USER+3)
#define TAUCHECK_SETHICOLOR     (WM_USER+4)

#endif
