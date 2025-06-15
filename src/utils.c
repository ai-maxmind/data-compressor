#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../include/utils.h"

volatile bool spinnerRunning = false;

long getFileSize(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) return st.st_size;
    return -1;
}
void *spinnerThread(void *arg) {
    const char *message = (const char *)arg;
    const char spinner[] = "|/-\\";
    int i = 0;
    while (spinnerRunning) {
        printf("\r[%c] %s", spinner[i % 4], message);
        fflush(stdout);
        usleep(100000);
        i++;
    }
    printf("\r    \r"); 
    return NULL;
}

void showProgress(const char *operation) {
    printf("[%s] Đang thực hiện...\n", operation);
}

void runCommand(const char *cmd) {
    printf("▶ Chạy: %s\n", cmd);
    int status = system(cmd);
    if (status != 0) {
        fprintf(stderr, "❌ Lỗi khi thực hiện: %s\n", cmd);
        exit(1);
    }
}

void runCommandWithSpinner(const char *cmd, const char *message) {
    pthread_t thread;
    spinnerRunning = true;
    pthread_create(&thread, NULL, spinnerThread, (void *)message);

    int status = system(cmd);

    spinnerRunning = false;
    pthread_join(thread, NULL);

    if (status != 0) {
        fprintf(stderr, "\n❌ Lỗi khi thực hiện: %s\n", cmd);
        exit(1);
    }
}
void printProgressBar(long current, long total, const char *label) {
    int width = 100; 
    float ratio = total > 0 ? (float)current / total : 0;
    int filled = (int)(ratio * width);

    printf("\r%s [", label);
    for (int i = 0; i < width; i++) {
        if (i < filled) printf("=");
        else if (i == filled) printf(">");
        else printf(" ");
    }
    printf("] %3d%%", (int)(ratio * 100));
    fflush(stdout);
}

