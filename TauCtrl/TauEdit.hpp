/*
  Name: TauEdit.hpp
  Copyright: Copyright(C) 2003, Jan Ringos, Tringi@Mx-3.cz
  Author: Jan Ringos, http://Tringi.Mx-3.cz
  Description: TauEdit Graphics Control
  
  Posts EN_UPDATE to notify change by user

*/
#ifndef __TAUEDIT_HPP
#define __TAUEDIT_HPP

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "EditReturnSys.hpp"
#include "InvalidateBorder.hpp"

extern int InitTauEditControl(HINSTANCE);

#define TAUE_CLASS   "TauEditClass"

// messages
#define TAUE_SETCOLOR       (WM_USER+1)
#define TAUE_SETFONT        (WM_USER+2)
#define TAUE_SETFCOLOR      (WM_USER+3)

// style
#define TAUES_TEXT          0
#define TAUES_NUMBER        4

#define TAUES_BORDER        8


#endif

