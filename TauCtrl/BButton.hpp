/*
  Name: BButton.hpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@seznam.cz
  Author: Jan Ringos, http://Tringi.CzWeb.org
  Description: Bitmap Button for simple graphics interface
*/
#ifndef __BBUTTON_HPP
#define __BBUTTON_HPP

#include <windows.h>

extern int InitBButtonControl(HINSTANCE);

#define BBTN_CLASS   "BitmapButtonClass"

// messages
#define BBTN_SETCOLOR        (WM_USER+1)
#define BBTN_SETBITMAP       (WM_USER+2)
#define BBTN_SETHOVERBITMAP  (WM_USER+3)
#define BBTN_SETDOWNBITMAP   (WM_USER+4)

#endif

