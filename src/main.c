#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <stdint.h>
#include <crypto/sha1.h>
#include <tracker.h>
#include <log.h>

#include "win_help.h"


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR lpCmdLine,
                     int nCmdShow) {
    int argc;
    PCHAR *argv = CommandLineToArgvA(GetCommandLineA(), &argc);

    char* path = argv[1];
    char* key = argv[2];

    printf("PATH: %s\n", path);
    printf("KEY BYTES: ");
    print_b(key, strlen(key));

    uint8_t keyhash[21];
    SHA1(keyhash, key, strlen(key));
    printf("KEY HASH: ");
    print_b(keyhash, 20);

    log_init(path, keyhash, 20);

    HookEvents();

    MSG msg;
    BOOL bRet;

    while (1) {
        bRet = GetMessage(&msg, NULL, 0, 0);

        if (bRet > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else if (bRet < 0) {
        } else {
            break;
        }
    }
    UnhookEvents();
    log_close();
    return msg.wParam;
}