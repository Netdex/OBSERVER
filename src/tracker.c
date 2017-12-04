//
// Created by netdex on 12/1/2017.
//

#include "tracker.h"

#include <stdio.h>
#include <assert.h>
#include <util/log.h>
#include <stdbool.h>
#include <config.h>
#include <windows.h>

#include "crypto/rc4.h"

static FILE *log_file = NULL;
static uint8_t log_rc4_state[256];
static uint8_t *state = NULL;

HANDLE ghMutex;

void log_init(char *path, uint8_t *key, size_t len, int mode) {
    assert(path != NULL);
    log_debug("intializing file log");
    if (mode == MODE_READ)
        log_file = fopen(path, "rb");
    else if (mode == MODE_WRITE)
        log_file = fopen(path, "ab");
    assert(log_file);
    if (key != NULL) {
        state = log_rc4_state;
        rc4_ksa(state, key, len);
        uint8_t trash[1024];
        rc4_prga(state, trash, sizeof(trash) / sizeof(uint8_t));
    }
    ghMutex = CreateMutex(
            NULL,              // default security attributes
            FALSE,             // initially not owned
            NULL);             // unnamed mutex
}

void log_close() {
    log_debug("closing file log");
    fclose(log_file);
}

void log_write(void *src, size_t size, size_t len) {
    log_trace("log write of size %d x %d, from thread %d", size, len, GetCurrentThreadId());

    WaitForSingleObject(ghMutex, INFINITE);

    int s = -1;
    if (state == NULL) {
        s = fwrite(src, size, len, log_file);
    } else {
        size_t bytes = size * len;
        uint8_t ciphertext[bytes];
        rc4_xor(state, ciphertext, (uint8_t *) src, bytes);
        s = fwrite(ciphertext, size, len, log_file);
    }
    assert(s == len);
    fflush(log_file);

    ReleaseMutex(ghMutex);
}

int log_read(void *dest, size_t size, size_t len){
    log_trace("log read of size %d x %d", size, len);

    int s = -1;
    if (state == NULL) {
        s = fread((uint8_t *) dest, size, len, log_file);
    } else {
        size_t bytes = size * len;
        uint8_t ciphertext[bytes];
        s = fread(ciphertext, size, len, log_file);
        rc4_xor(state, (uint8_t *) dest, ciphertext, bytes);
    }
    return s;
}

void print_b(void *c, size_t len) {
    for (int i = 0; i < len; i++) {
        printf("%02X ", *((uint8_t *) c + i));
    }
    printf("\n");
}