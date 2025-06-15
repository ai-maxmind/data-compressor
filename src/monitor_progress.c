#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "../include/utils.h"

typedef struct {
    const char *output;
    long expected;
    const char *label;
} MonitorArgs;

void *monitorProgress(void *args) {
    MonitorArgs *m = (MonitorArgs *)args;
    while (1) {
        long current = getFileSize(m->output);
        if (current < 0) current = 0;

        printProgressBar(current, m->expected, m->label);

        if (current >= m->expected) break;

        usleep(200000); 
    }

    printf("\n");
    free(m);
    return NULL;
}
