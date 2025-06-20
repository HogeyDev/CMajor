#include "song.h"
#include "io.h"

#include <stdlib.h>
#include <stdio.h>

int system_silent(const char *cmd) {
    char silent[1023];
    snprintf(silent, sizeof(silent), "%s > /dev/null 2>&1", cmd);

    return system(silent);
}

Song song_from_yt(const char *id) {
    return (Song){
        .id = id,
    };
}

void download_song(Song s) {
    char path_buf[128];
    snprintf(path_buf, 128, "songs/%s.wav", s.id);
    if (file_exists(path_buf)) {
        return;
    }

    char command[512];
    snprintf(command, sizeof(command), "yt-dlp --cookies cookies/cookies.txt -f bestaudio -x --audio-format wav -o 'songs/%s.%%(ext)s' 'youtu.be/%s'", s.id, s.id);
    printf("%s\n", command);
    int code = system_silent(command);
    if (code) {
        system("./cookies/collect.sh"); // update cookies.txt
        code = system(command);
    }
}

void download_thumbnail(Song s) {
    char path_buf[128];
    snprintf(path_buf, 128, "songs/%s.jpg", s.id);
    if (file_exists(path_buf)) {
        return;
    }

    char url[512];
    snprintf(url, sizeof(url), "https://img.youtube.com/vi/%s/0.jpg", s.id);
    
    char command[512];
    snprintf(command, sizeof(command), "curl -o songs/%s.jpg %s", s.id, url);
    printf("%s\n", command);
    system_silent(command);
}
