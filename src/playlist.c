#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "playlist.h"
#include "io.h"
#include "song.h"

Playlist create_playlist() {
    Playlist p = { 0 };

    p.cap = 8;
    p.size = 0;
    p.songs = malloc(p.cap * sizeof(Song));

    return p;
}

Playlist load_list(const char *path) {
    Playlist p = create_playlist();

    const char *contents = read_file(path);
    const unsigned int len = strlen(contents);
    unsigned int i = 0;
    while (i < len) {
        char id_buf[12] = { 0 }; // 11 character id + null terminator
        for (unsigned int j = 0; j < 11; j++) {
            id_buf[j] = contents[i];
            i++;
        }
        if (strlen(id_buf) != 11) {
            continue;
        }

        char *id = calloc(strlen(id_buf) + 1, sizeof(char));
        strcpy(id, id_buf);
        printf("Found ID: \"%s\"\n", id);

        i++; // skip the space

        unsigned int buf_size = 8;
        const float ratio = 1.5;
        char *name = malloc((buf_size + 1) * sizeof(char));
        unsigned int index = 0;
        while (contents[i] != '\n' && i < len) {
            name[index] = contents[i];
            index++;
            i++;

            if (index >= buf_size) {
                buf_size *= ratio;

                char *tmp = calloc(buf_size + 1, sizeof(char));
                strcpy(tmp, name);
                free(name);
                name = tmp;
            }
        }
        i++;
        // printf("NAME: \"%s\"\n", name);

        if (strlen(id) == 11 && strlen(name) > 0) {
            Song song = { 0 };
            song.id = malloc((strlen(id) + 1) * sizeof(char));
            strcpy((char *)song.id, id);
            song.name = malloc((strlen(name) + 1) * sizeof(char));
            strcpy((char *)song.name, name);
            add_song(&p, song);
        }
    }

    return p;
}

void add_song(Playlist *p, Song s) {
    p->size++;
    // printf("%d / %d\n", p->size, p->cap);
    if (p->size >= p->cap) {
        p->cap *= 2; // idk what ratio to use, so 2 will do

        Song *tmp = calloc(p->cap, sizeof(Song));
        memcpy(tmp, p->songs, p->size * sizeof(Song));
        free(p->songs);

        p->songs = tmp;
    }

    p->songs[p->size - 1] = s;
}

int download_threads = 0;
void wait_for_download_thread() {
    int status;
    wait(&status);
    download_threads--;
}
void download_list(Playlist *p) {
    for (unsigned int i = 0; i < p->size; i++) {
        while (download_threads >= MAX_DOWNLOAD_THREADS) {
            wait_for_download_thread();
        }

        pid_t pid = fork();
        if (pid == 0) {
            // this is a child process and we can now do a lil parallel processing
            download_song(p->songs[i]);
            download_thumbnail(p->songs[i]);
            _exit(0);
        } else if (pid > 0) {
            download_threads++;
        } else {
            perror("fork failed");
            exit(1);
        }
    }
    while (download_threads > 0) {
        wait_for_download_thread();
    }
}
