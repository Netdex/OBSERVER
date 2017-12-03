//
// Created by netdex on 12/2/2017.
//
// Note that this encryption is meant to obscure data, not to protect it.
//

#ifndef OBSERVER_RC4_H
#define OBSERVER_RC4_H

#include <stdlib.h>
#include <stdint.h>

void rc4_xor(uint8_t *state, uint8_t *dest, uint8_t *src, size_t len);
void rc4_ksa(uint8_t *state, uint8_t *key, size_t len);
void rc4_prga(uint8_t *state, uint8_t *out, size_t len);

#endif //OBSERVER_RC4_H
