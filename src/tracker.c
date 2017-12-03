//
// Created by netdex on 12/1/2017.
//

#include "tracker.h"

#include <stdio.h>
#include <assert.h>
#include <log.h>

#include "crypto/rc4.h"

FILE *log_file = NULL;
uint8_t log_rc4_state[256];

void log_init(char *path, uint8_t *key, size_t len) {
    log_debug("intializing file log");
    log_file = fopen(path, "ab");
    assert(log_file);
    rc4_ksa(log_rc4_state, key, len);
    uint8_t trash[1024];
    rc4_prga(log_rc4_state, trash, sizeof(trash) / sizeof(uint8_t));
}

void log_close(){
    log_debug("closing file log");
    fclose(log_file);
}

void log_write(void *p, size_t size, size_t len) {
    log_trace("log write of size %d", size);
    size_t bytes = size * len;
    uint8_t crypt[bytes];
    rc4_xor(log_rc4_state, crypt, (uint8_t*) p, bytes);
    fwrite(crypt, bytes, 1, log_file);
//    fwrite((uint8_t*) p, bytes, 1, log_file);
    fflush(log_file);
}

void log_b(char c) {
    log_write(&c, sizeof(c), 1);
}

void log_w(short c) {
    log_write(&c, sizeof(c), 1);
}

void log_d(int c) {
    log_write(&c, sizeof(c), 1);
}

void log_q(long long c) {
    log_write(&c, sizeof(c), 1);
}

void print_b(void *c, size_t len){
    for(int i = 0; i < len; i++){
        printf("%02X ", *((uint8_t*)c + i));
    }
    printf("\n");
}