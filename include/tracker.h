//
// Created by netdex on 12/1/2017.
//

#ifndef OBSERVER_TRACKER_H
#define OBSERVER_TRACKER_H

#include <stdlib.h>
#include <stdint.h>

void log_init(char* path, uint8_t *key, size_t len);

void log_close();

void log_write(void *p, size_t size, size_t len);

void log_b(char c);

void log_w(short c);

void log_d(int c);

void log_q(long long c);

void print_b(void *c, size_t len);
#endif //OBSERVER_LOG_CONTROL_H
