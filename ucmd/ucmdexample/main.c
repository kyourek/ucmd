#include <stdio.h>
#include <string.h>
#include "ucmdexample.h"

static void transmit(const char *response, void *state) {
    printf("%s\n", response);
}

static char *receive(char *buf, size_t buf_size, void *state) {
    return fgets(buf, buf_size, stdin);
}

int main (int argc, const char *argv[]) {
    uc_example(transmit, receive);
    uc_hello_world(transmit, receive);
    return 0;
}
