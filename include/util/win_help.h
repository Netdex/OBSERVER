//
// Created by netdex on 12/1/2017.
//

#ifndef OBSERVER_WIN_HELP_H
#define OBSERVER_WIN_HELP_H

#include <windows.h>

char *GetActiveWindowTitle();

void HookEvents();

void UnhookEvents();

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

VOID CALLBACK WinEventProcCallback(HWINEVENTHOOK hWinEventHook, DWORD dwEvent, HWND hwnd, LONG idObject, LONG idChild,
                                   DWORD dwEventThread, DWORD dwmsEventTime);


PCHAR *CommandLineToArgvA(PCHAR CmdLine, int *_argc);

#endif //OBSERVER_WIN_HELP_H
