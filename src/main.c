#include "include/song.h"

int main(void) {
    Song song = song_from_yt("https://www.youtube.com/watch?v=x8VYWazR5mE&rco=1");
    download_song(song);
    return 0;
}
