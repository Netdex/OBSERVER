//
// Created by netdex on 12/3/2017.
//

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "config.h"

struct conf config = {NULL, NULL, MODE_WRITE};

void print_usage(char *argv[]) {
    fprintf(stderr, "Usage: %s [-k key] [-r] file\n",
            argv[0]);
    exit(EXIT_FAILURE);
}

void config_getopt(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "k:r")) != -1) {
        switch (opt) {
            case 'k':
                config.key = optarg;
                break;
            case 'r':
                config.mode = MODE_READ;
                break;
            default:
                print_usage(argv);/* '?' */

        }
    }
    if (optind >= argc) {
        print_usage(argv);
    }
    config.path = argv[optind];

    assert(config.path != NULL);
}