#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "aoc_utils.h"

unsigned int get_score_part1(gchar* choices){
    unsigned int score = 0;
    if (strcmp(choices, "A X") == 0) {
        score = 4;
    } else if (strcmp(choices, "A Y") == 0) {
        score = 8;
    } else if (strcmp(choices, "A Z") == 0) {
        score = 3;
    } else if (strcmp(choices, "B X") == 0) {
        score = 1;
    } else if (strcmp(choices, "B Y") == 0) {
        score = 5;
    } else if (strcmp(choices, "B Z") == 0) {
        score = 9;
    } else if (strcmp(choices, "C X") == 0) {
        score = 7;
    } else if (strcmp(choices, "C Y") == 0) {
        score = 2;
    } else if (strcmp(choices, "C Z") == 0) {
        score = 6;
    } else {
        printf("This shouldn't happen! Bad input? %s", choices);
        exit(1);
    }
    return score;
}

int part1(GArray *input){
    unsigned int score = 0;
    for (guint i = 0; i < input->len; ++i) {
        gchar *line = g_array_index(input, char *, i);
        score += get_score_part1(line);
    }
    return score;
}

unsigned int get_score_part2(gchar* choices){
    unsigned int score = 0;
    if (strcmp(choices, "A X") == 0) {
        score = 3;
    } else if (strcmp(choices, "A Y") == 0) {
        score = 4;
    } else if (strcmp(choices, "A Z") == 0) {
        score = 8;
    } else if (strcmp(choices, "B X") == 0) {
        score = 1;
    } else if (strcmp(choices, "B Y") == 0) {
        score = 5;
    } else if (strcmp(choices, "B Z") == 0) {
        score = 9;
    } else if (strcmp(choices, "C X") == 0) {
        score = 2;
    } else if (strcmp(choices, "C Y") == 0) {
        score = 6;
    } else if (strcmp(choices, "C Z") == 0) {
        score = 7;
    } else {
        printf("This shouldn't happen! Bad input? %s", choices);
        exit(1);
    }
    return score;
}

int part2(GArray *input){
    unsigned int score = 0;
    for (guint i = 0; i < input->len; ++i) {
        gchar *line = g_array_index(input, char *, i);
        score += get_score_part2(line);
    }
    return score;
}

int main(int argc, char **argv) {
    char *filename;
    if (argc > 1) {
        filename = g_strdup(argv[1]);
    } else {
        filename = g_strdup("./input");
    }
    GArray *input = get_input_lines(filename);

    g_free(filename);
    if (!input) exit(1);

    printf("Part 1: Answer is %d\n", part1(input));
    printf("Part 2: Answer is %d\n", part2(input));
    g_array_free(input, TRUE);
    return 0;
}
