#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <stdint.h>
#include <event.h>
#include <unistd.h>
#include <assert.h>
#include <config.h>
#include <reader.h>
#include <crypto/rc4.h>

#include "crypto/sha1.h"
#include "tracker.h"
#include "util/log.h"
#include "util/win_help.h"

int observer_hook() {
    HookEvents();
    OnLogBegin();

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
    return msg.wParam;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR lpCmdLine,
                     int nCmdShow) {
    int argc;
    PCHAR *argv = CommandLineToArgvA(GetCommandLineA(), &argc);

    config_getopt(argc, argv);

    log_info("path: %s", config.path);
    log_info("key: %s", config.key);

    if (config.key == NULL)
        log_warn("key is null!");
    if (config.key != NULL) {
        uint8_t keyhash[21];
        SHA1(keyhash, config.key, strlen(config.key));
        log_init(config.path, keyhash, 20, config.mode);
    } else {
        log_init(config.path, NULL, 0, config.mode);
    }
    if (config.mode == MODE_READ) {
        observer_interface();
        log_close();
    } else {
        int code = observer_hook();
        log_close();
        return code;
    }
    return 0;
}