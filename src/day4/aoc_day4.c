#include <stdlib.h>
#include <stdio.h>
#include "aoc_utils.h"

typedef struct {
    int start;
    int end;
} elf;

typedef struct {
   elf elf1;
   elf elf2;
} elfTuple;

elfTuple parse_line(gchar* line){
    char *token = strtok(line, "-");
    int start1 = atoi(token);
    token = strtok(NULL, ",");
    int end1 = atoi(token);

    token = strtok(NULL, "-");
    int start2 = atoi(token);
    token = strtok(NULL, "-");
    int end2 = atoi(token);

    elf elf1 = { start1, end1 };
    elf elf2 = { start2, end2 };
    elfTuple elves = { elf1, elf2 };
    return elves;
}

gboolean fully_overlap(elfTuple *elves) {
    elf elf1 = elves->elf1;
    elf elf2 = elves->elf2;
    return (elf1.start <= elf2.end && elf1.end <= elf2.start) ||
           (elf2.start <= elf1.end && elf2.end <= elf1.start);
}

gboolean overlap(elfTuple *elves) {
    elf elf1 = elves->elf1;
    elf elf2 = elves->elf2;
    return (elf1.start >= elf2.start && elf1.start <= elf2.end) ||
           (elf2.start >= elf1.start && elf2.start <= elf1.end);
}

int part1(GArray *elves){
    int score = 0;
    for (guint i = 0; i < elves->len; ++i) {
        elfTuple pair = g_array_index(elves, elfTuple, i);
        if (fully_overlap(&pair)) {
            score += 1;
        }
    }
    return score;
}

int part2(GArray *elves){
    int score = 0;
    for (guint i = 0; i < elves->len; ++i) {
        elfTuple pair = g_array_index(elves, elfTuple, i);
        if (overlap(&pair)) {
            score += 1;
        }
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

    GArray *elfpairs = g_array_new(TRUE, FALSE, sizeof(elfTuple));
    for (guint i = 0; i < input->len; ++i) {
        gchar *line = g_array_index(input, char *, i);
        elfTuple elves = parse_line(line);
        g_array_append_val(elfpairs,elves);
    }
    g_array_free(input, TRUE);
    printf("Part 1: Answer is %d\n", part1(elfpairs));
    printf("Part 2: Answer is %d\n", part2(elfpairs));
    return 0;
}
