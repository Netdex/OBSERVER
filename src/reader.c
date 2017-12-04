//
// Created by netdex on 12/3/2017.
//


#include "reader.h"

#include <ncurses/ncurses.h>
#include <event.h>
#include <assert.h>
#include <ctype.h>

#define PAIR_ACCENT    1
#define PAIR_TITLE     2
#define PAIR_MSG       3

void init_color_pairs() {
    init_pair(PAIR_ACCENT, COLOR_CYAN, COLOR_BLACK);
    init_pair(PAIR_TITLE, COLOR_BLACK, COLOR_WHITE);
    init_pair(PAIR_MSG, COLOR_GREEN, COLOR_BLACK);
}

int last_print = EVENT_MESSAGE;

void print_event(event_t *e) {
    switch (e->type) {
        case EVENT_KEYBOARD: {
            keyb_ev_t *kbv = e->event;
            switch (kbv->vkCode) {
                case VK_RETURN:
                    printw("\n");
                    break;
                default: {
                    char c;
                    c = (char) MapVirtualKey(kbv->vkCode, MAPVK_VK_TO_CHAR);
                    if (isprint(c)) {
                        if (kbv->msgId == WM_KEYDOWN || kbv->msgId == WM_SYSKEYDOWN)
                            printw("%c", c);
                    } else {
                        int scancode = MapVirtualKey(kbv->vkCode, MAPVK_VK_TO_VSC);
                        char keyname[256];
                        GetKeyNameTextA(scancode << 16, keyname, 256);

                        attron(COLOR_PAIR(PAIR_ACCENT));
                        if (kbv->msgId == WM_KEYDOWN || kbv->msgId == WM_SYSKEYDOWN)
                            printw("[%s]", keyname);
                        else
                            printw("[/%s]", keyname);
                        attroff(COLOR_PAIR(PAIR_ACCENT));
                    }
                    last_print = EVENT_KEYBOARD;
                    break;
                }
            }
            break;
        }
        case EVENT_WINDOW: {
            msg_ev_t *msv = e->event;
            // string is not null terminated
            char msg[msv->len + 1];
            memset(msg, 0, msv->len + 1);
            memcpy(msg, msv->msg, msv->len);
            attron(COLOR_PAIR(PAIR_TITLE));
            if (last_print == EVENT_KEYBOARD)
                printw("\n\n");
            printw("%s\n", msg);
            attroff(COLOR_PAIR(PAIR_TITLE));
            last_print = EVENT_MESSAGE;
            break;
            break;
        }
        case EVENT_MESSAGE: {
            msg_ev_t *msv = e->event;
            // string is not null terminated
            char msg[msv->len + 1];
            memset(msg, 0, msv->len + 1);
            memcpy(msg, msv->msg, msv->len);
            attron(COLOR_PAIR(PAIR_MSG) | A_BOLD);
            if (last_print == EVENT_KEYBOARD)
                printw("\n\n");
            printw("%s\n", msg);
            attroff(COLOR_PAIR(PAIR_MSG) | A_BOLD);
            last_print = EVENT_MESSAGE;
            break;
        }
        default:
            assert(("invalid message id", false));
    }

}


void observer_interface() {
    initscr();                    /* Start curses mode 		*/
    raw();                        /* Line buffering disabled	*/
    keypad(stdscr, TRUE);        /* We get F1, F2 etc..		*/
    noecho();                    /* Don't echo() while we do getch */
    scrollok(stdscr, TRUE);
    start_color();

    init_color_pairs();

    event_t e;
    while (read_event(&e)) {
        print_event(&e);
        free(e.event);
    }
    refresh();                    /* Print it on to the real screen */
    getch();                    /* Wait for user input */
    endwin();                    /* End curses mode		  */
}