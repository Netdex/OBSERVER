//
// Created by netdex on 12/1/2017.
//

#ifndef OBSERVER_TRACKER_H
#define OBSERVER_TRACKER_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void log_init(char* path, uint8_t *key, size_t len, int mode);

void log_close();

void log_write(void *src, size_t size, size_t len);

int log_read(void *dest, size_t size, size_t len);

void print_b(void *c, size_t len);
#endif //OBSERVER_LOG_CONTROL_H
