//
// Created by netdex on 12/1/2017.
//
#include <stdio.h>
#include <stdlib.h>

#include "util/win_help.h"
#include "event.h"

HHOOK hKeyb;
HHOOK hEvent;

char *GetActiveWindowTitle() {
    char *title = malloc(256);
    HWND handle = GetForegroundWindow();
    GetWindowText(handle, title, 256);
    return title;
}

void HookEvents() {
    HINSTANCE instance = GetModuleHandle(NULL);
    hKeyb = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, instance, 0);

    hEvent = SetWinEventHook(EVENT_OBJECT_NAMECHANGE,
                             EVENT_OBJECT_NAMECHANGE, NULL,
                             WinEventProcCallback, 0, 0,
                             WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
}

void UnhookEvents() {
    UnhookWindowsHookEx(hKeyb);
    UnhookWinEvent(hEvent);
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    OnKeyboardEvent(wParam, (KBDLLHOOKSTRUCT *) lParam);
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

char *lastTitle = NULL;

VOID CALLBACK WinEventProcCallback(HWINEVENTHOOK hWinEventHook, DWORD dwEvent,
                                   HWND hwnd, LONG idObject, LONG idChild,
                                   DWORD dwEventThread, DWORD dwmsEventTime) {
    char *title = GetActiveWindowTitle();
    if (lastTitle) {
        if (strcmp(lastTitle, title) == 0) {
            free(title);
        } else {
            free(lastTitle);
            lastTitle = title;
            OnActiveWindowTitleChanged(title);
        }
    } else {
        lastTitle = title;
        OnActiveWindowTitleChanged(title);
    }

}

// http://alter.org.ua/docs/win/args/
PCHAR *CommandLineToArgvA(PCHAR CmdLine, int *_argc) {
    PCHAR *argv;
    PCHAR _argv;
    ULONG len;
    ULONG argc;
    CHAR a;
    ULONG i, j;

    BOOLEAN in_QM;
    BOOLEAN in_TEXT;
    BOOLEAN in_SPACE;

    len = strlen(CmdLine);
    i = ((len + 2) / 2) * sizeof(PVOID) + sizeof(PVOID);

    argv = (PCHAR *) GlobalAlloc(GMEM_FIXED,
                                 i + (len + 2) * sizeof(CHAR));

    _argv = (PCHAR) (((PUCHAR) argv) + i);

    argc = 0;
    argv[argc] = _argv;
    in_QM = FALSE;
    in_TEXT = FALSE;
    in_SPACE = TRUE;
    i = 0;
    j = 0;

    while ((a = CmdLine[i])) {
        if (in_QM) {
            if (a == '\"') {
                in_QM = FALSE;
            } else {
                _argv[j] = a;
                j++;
            }
        } else {
            switch (a) {
                case '\"':
                    in_QM = TRUE;
                    in_TEXT = TRUE;
                    if (in_SPACE) {
                        argv[argc] = _argv + j;
                        argc++;
                    }
                    in_SPACE = FALSE;
                    break;
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                    if (in_TEXT) {
                        _argv[j] = '\0';
                        j++;
                    }
                    in_TEXT = FALSE;
                    in_SPACE = TRUE;
                    break;
                default:
                    in_TEXT = TRUE;
                    if (in_SPACE) {
                        argv[argc] = _argv + j;
                        argc++;
                    }
                    _argv[j] = a;
                    j++;
                    in_SPACE = FALSE;
                    break;
            }
        }
        i++;
    }
    _argv[j] = '\0';
    argv[argc] = NULL;

    (*_argc) = argc;
    return argv;
}
