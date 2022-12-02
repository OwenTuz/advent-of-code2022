#include <stdlib.h>
#include <stdio.h>
#include "aoc_utils.h"

int part1(GArray *input){
    int most_calories = 0;
    int current_total = 0;
    for (guint i = 0; i < input->len; ++i) {
        gchar *line = g_array_index(input, char *, i);
        int calories = atoi(line);
        if (calories == 0) {
            if (current_total > most_calories) {
                most_calories = current_total;
            }
            current_total = 0;
        } else {
            current_total += calories;
        }
    }
    return most_calories;
}

int part2(GArray *input){
    int current_total = 0;
    GArray *totals = g_array_new(TRUE, FALSE, sizeof(int));
    for (guint i = 0; i < input->len; ++i) {
        gchar *line = g_array_index(input, char *, i);
        int calories = atoi(line);
        if (calories == 0) {
            g_array_append_val(totals, current_total);
            current_total = 0;
        } else {
            current_total += calories;
        }
    }
    g_array_sort(totals, int_compare);
    int first = g_array_index(totals, int, totals->len - 1);
    int second = g_array_index(totals, int, totals->len - 2);
    int third =  g_array_index(totals, int, totals->len - 3);
    return first + second + third;
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
