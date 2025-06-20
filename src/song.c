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

double get_wav_length(const char *filepath) {
    FILE *f = fopen(filepath, "rb");
    if (!f) return -1.0;

    char chunk_id[4];
    uint32_t chunk_size;
    uint16_t audio_format, num_channels;
    uint32_t sample_rate, byte_rate;
    uint16_t block_align, bits_per_sample;

    // Read RIFF header
    fread(chunk_id, 1, 4, f);  // "RIFF"
    fread(&chunk_size, 4, 1, f);  // Total size minus 8
    fread(chunk_id, 1, 4, f);  // "WAVE"

    // Read fmt subchunk
    fread(chunk_id, 1, 4, f);  // "fmt "
    fread(&chunk_size, 4, 1, f);  // Should be 16 for PCM
    fread(&audio_format, 2, 1, f);
    fread(&num_channels, 2, 1, f);
    fread(&sample_rate, 4, 1, f);
    fread(&byte_rate, 4, 1, f);
    fread(&block_align, 2, 1, f);
    fread(&bits_per_sample, 2, 1, f);

    // Skip any extra fmt bytes if chunk_size > 16
    if (chunk_size > 16) {
        fseek(f, chunk_size - 16, SEEK_CUR);
    }

    // Search for "data" chunk
    while (1) {
        if (fread(chunk_id, 1, 4, f) != 4) {
            fclose(f);
            return -1.0;  // Reached EOF without finding "data"
        }

        fread(&chunk_size, 4, 1, f);

        if (memcmp(chunk_id, "data", 4) == 0) {
            break;
        }

        // Skip unknown chunk
        fseek(f, chunk_size, SEEK_CUR);
    }

    fclose(f);

    double duration = (double)chunk_size / byte_rate;
    return duration;
}
