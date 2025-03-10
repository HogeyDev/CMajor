#include "song.h"

#include <stdlib.h>
#include <stdio.h>

Song song_from_yt(const char *url) {
    return (Song){
        .url = url,
    };
}

void download_song(Song s) {
    char command[512];
    snprintf(command, sizeof(command), "yt-dlp --cookies cookies.txt -f bestaudio -x --audio-format wav -o 'audio.%%(ext)s' '%s'", s.url);
    printf("%s\n", command);
    int code = system(command);
}
