#include "io.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

const char *read_file(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(length + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}

int file_exists(const char *path) {
    return !access(path, F_OK);
}
