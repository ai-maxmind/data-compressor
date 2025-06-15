#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/compress.h"
#include "../include/utils.h"
#include "monitor_progress.c"
#include <pthread.h>

void compressFile(const char *method, const char *input, const char *output) {
    showProgress("Nén");
    char cmd[512];

    long inputSize = getFileSize(input);
    if (inputSize < 0) {
        fprintf(stderr, "❌ Không đọc được file đầu vào: %s\n", input);
        exit(1);
    }
    printf("📄 Kích thước trước khi nén: %.2f MB\n", inputSize / (1024.0 * 1024.0));

    pthread_t monitorThread;
    MonitorArgs *args = NULL;
    int useMonitor = 0;

    if (strcmp(method, "zstd") == 0 || strcmp(method, "cmix") == 0 || strcmp(method, "zpaq") == 0) {
        args = malloc(sizeof(MonitorArgs));
        args->output = output;
        args->expected = inputSize;
        args->label = "Tiến trình nén";
        useMonitor = 1;
        pthread_create(&monitorThread, NULL, monitorProgress, args);
    }

    if (strcmp(method, "zstd") == 0) {
        snprintf(cmd, sizeof(cmd), "zstd -19 --long=31 -v -o \"%s\" \"%s\"", output, input);
        runCommand(cmd);
    } else if (strcmp(method, "paq8px") == 0) {
        snprintf(cmd, sizeof(cmd), "./paq8px \"%s\"", input);
        runCommandWithSpinner(cmd, "Đang nén với paq8px...");
    } else if (strcmp(method, "cmix") == 0) {
        snprintf(cmd, sizeof(cmd), "./cmix c \"%s\"", input);
        runCommand(cmd);
    } else if (strcmp(method, "zpaq") == 0) {
        snprintf(cmd, sizeof(cmd), "./zpaq a \"%s\" \"%s\" -m5", output, input);
        runCommand(cmd);
    } else {
        fprintf(stderr, "❌ Thuật toán không được hỗ trợ: %s\n", method);
        if (args) free(args);
        exit(1);
    }

    if (useMonitor) pthread_join(monitorThread, NULL);

    long outputSize = getFileSize(output);
    if (outputSize < 0) {
        fprintf(stderr, "⚠ Không xác định được kích thước file sau nén.\n");
    } else {
        printf("📦 Kích thước sau khi nén: %.2f MB\n", outputSize / (1024.0 * 1024.0));
        float ratio = 100.0 * (1.0 - (double)outputSize / inputSize);
        printf("📉 Đã giảm: %.2f%% dung lượng\n", ratio);
    }

    printf("✅ Nén xong bằng %s!\n", method);
}
