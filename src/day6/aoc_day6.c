#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "aoc_utils.h"

bool is_marker(gchar *seq){
    for (guint64 i = 0; i<strlen(seq); ++i){
        for (guint64 j = i + 1; j <strlen(seq); ++j){
            if(seq[i] == seq[j]) { return false;}
        }
    }
    return true;
}

int get_chars_until_marker(gchar *input, size_t marker_len){
    for (size_t start = 0; start <= strlen(input) - marker_len; ++start){
        gchar *seq = malloc(marker_len + 1);
        aoc_string_slice(input, seq, start, start + marker_len);
        if (is_marker(seq)){
            return start + marker_len;
        }
        g_free(seq);
    }
    printf("No marker found, this shouldn't happen");
    exit(1);
}

int part1(gchar *input){
    return get_chars_until_marker(input, 4);
}

int part2(gchar *input){
    return get_chars_until_marker(input, 14);
}

int main(int argc, char **argv) {
    char *filename;
    if (argc > 1) {
        filename = g_strdup(argv[1]);
    } else {
        filename = g_strdup("./input");
    }

    FILE *fp;
    if (!(fp = fopen(filename, "r"))) {
        printf("Error opening file '%s': %s \n", filename, strerror(errno));
        exit(0);
    }
    char line[8192];
    fgets(line, sizeof(line), fp);
    gchar *input =  g_strstrip(g_strdup(line));
    fclose(fp);

    g_free(filename);

    assert(get_chars_until_marker("mjqjpqmgbljsphdztnvjfqwrcgsmlb", 4) == 7);
    assert(get_chars_until_marker("mjqjpqm", 4) == 7);

    printf("Part 1: Answer is %d\n", part1(input));
    printf("Part 2: Answer is %d\n", part2(input));
    return 0;
}
