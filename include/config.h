//
// Created by netdex on 12/3/2017.
//

#ifndef OBSERVER_CONFIG_H
#define OBSERVER_CONFIG_H

#include <stdbool.h>

#define MODE_WRITE  0
#define MODE_READ   1

struct conf {
    char *path;
    char *key;
    int mode;
};

extern struct conf config;

void config_getopt(int argc, char *argv[]);

#endif //OBSERVER_CONFIG_H
