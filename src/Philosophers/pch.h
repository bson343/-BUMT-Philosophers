#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>
#include <iostream>

#endif //PCH_H
