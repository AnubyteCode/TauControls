/*
  Name: TauList.hpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: TauList, text item list control
  
  Handling: Posts LBN_DBLCLKE in form of WM_COMMAND on ENTER or LEFT DBLCLK
            Posts LBN_SELCHANGE when selection is changed by user
*/
#ifndef __TAULIST_HPP
#define __TAULIST_HPP

#include <windows.h>

extern int InitTauListControl(HINSTANCE);

#define TAUL_CLASS   "TauListClass"

// styles
#define TAULS_NOSLIDE        1

// messages
#define TAUL_SETCOLOR        (WM_USER+1)
#define TAUL_SETTEXT         (WM_USER+2)
#define TAUL_GETTEXT         (WM_USER+3)
#define TAUL_CLEAR           (WM_USER+4)
#define TAUL_SETFONT         (WM_USER+5)
#define TAUL_SETHICOLOR      (WM_USER+6)
#define TAUL_SETLIMIT        (WM_USER+7)

#define TAUL_GETCURSEL       (WM_USER+10)
#define TAUL_GETCELLPOS      (WM_USER+11)

#define TAUL_LIMIT          65504

#endif

