#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/decompress.h"
#include "../include/utils.h"
#include "monitor_progress.c"
#include <pthread.h>

void decompressFile(const char *method, const char *input) {
    showProgress("Giải nén");
    char cmd[512];

    long inputSize = getFileSize(input);
    if (inputSize < 0) {
        fprintf(stderr, "❌ Không đọc được file nén: %s\n", input);
        exit(1);
    }
    printf("📦 Kích thước file nén: %.2f MB\n", inputSize / (1024.0 * 1024.0));

    char output[512] = {0};
    int useMonitor = 0;
    pthread_t monitorThread;
    MonitorArgs *args = NULL;

    if (strcmp(method, "zstd") == 0) {
        snprintf(output, sizeof(output), "%.*s", (int)(strlen(input) - 4), input); // remove .zst
        args = malloc(sizeof(MonitorArgs));
        args->output = output;
        args->expected = 0; 
        args->label = "Tiến trình giải nén";
        useMonitor = 1;
        pthread_create(&monitorThread, NULL, monitorProgress, args);

        snprintf(cmd, sizeof(cmd), "unzstd -v \"%s\"", input);
        runCommand(cmd);
    } else if (strcmp(method, "paq8px") == 0) {
        snprintf(cmd, sizeof(cmd), "./paq8px \"%s\"", input);
        runCommandWithSpinner(cmd, "Đang giải nén với paq8px...");
    } else if (strcmp(method, "cmix") == 0) {
        snprintf(output, sizeof(output), "%s.out", input); // cmix mặc định thêm .out
        args = malloc(sizeof(MonitorArgs));
        args->output = output;
        args->expected = 0;
        args->label = "Tiến trình giải nén";
        useMonitor = 1;
        pthread_create(&monitorThread, NULL, monitorProgress, args);

        snprintf(cmd, sizeof(cmd), "./cmix d \"%s\"", input);
        runCommand(cmd);
    } else if (strcmp(method, "zpaq") == 0) {
        snprintf(output, sizeof(output), "%s.extracted", input); // giả định tên thư mục hoặc file
        args = malloc(sizeof(MonitorArgs));
        args->output = output;
        args->expected = 0;
        args->label = "Tiến trình giải nén";
        useMonitor = 1;
        pthread_create(&monitorThread, NULL, monitorProgress, args);

        snprintf(cmd, sizeof(cmd), "./zpaq x \"%s\"", input);
        runCommand(cmd);
    } else {
        fprintf(stderr, "❌ Thuật toán không được hỗ trợ: %s\n", method);
        if (args) free(args);
        exit(1);
    }

    if (useMonitor) pthread_join(monitorThread, NULL);

    if (output[0] != '\0') {
        long outputSize = getFileSize(output);
        if (outputSize > 0) {
            printf("📄 Kích thước sau giải nén: %.2f MB\n", outputSize / (1024.0 * 1024.0));
            float increased = 100.0 * ((double)outputSize / inputSize - 1.0);
            printf("📈 Dung lượng tăng thêm: %.2f%%\n", increased);
        } else {
            printf("⚠ Không thể xác định kích thước file sau giải nén (có thể là thư mục).\n");
        }
    }

    printf("✅ Giải nén xong bằng %s!\n", method);
}
