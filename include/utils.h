#ifndef UTILS_H
#define UTILS_H

void showProgress(const char *operation);
void runCommand(const char *cmd);
void runCommandWithSpinner(const char *cmd, const char *message);
long getFileSize(const char *filename);
void printProgressBar(long current, long total, const char *label);
#endif
