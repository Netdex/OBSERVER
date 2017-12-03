//
// mostly written by Brad Conte (brad@bradconte.com)
// http://bradconte.com/rc4_c
//

#include "crypto/rc4.h"

void rc4_xor(uint8_t *state, uint8_t *dest, uint8_t *src, size_t len){
    uint8_t keystr[len];
    rc4_prga(state, keystr, len);
    for(int i = 0; i < len; i++){
        dest[i] = src[i] ^ keystr[i];
    }
}

// Key Scheduling Algorithm
// Input: state - the state used to generate the keystream
//        key - Key to use to initialize the state
//        len - length of key in bytes
void rc4_ksa(uint8_t *state, uint8_t *key, size_t len) {
    int i, j = 0, t;

    for (i = 0; i < 256; ++i)
        state[i] = (uint8_t) i;
    for (i = 0; i < 256; ++i) {
        j = (j + state[i] + key[i % len]) % 256;
        t = state[i];
        state[i] = state[j];
        state[j] = (uint8_t) t;
    }
}

// Pseudo-Random Generator Algorithm
// Input: state - the state used to generate the keystream
//        out - Must be of at least "len" length
//        len - number of bytes to generate
void rc4_prga(uint8_t *state, uint8_t *out, size_t len) {
    int i = 0, j = 0, x, t;

    for (x = 0; x < len; ++x) {
        i = (i + 1) % 256;
        j = (j + state[i]) % 256;
        t = state[i];
        state[i] = state[j];
        state[j] = (uint8_t) t;
        out[x] = state[(state[i] + state[j]) % 256];
    }
}
