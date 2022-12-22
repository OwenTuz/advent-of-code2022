#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "aoc_utils.h"

typedef struct Tree {
    guint x;
    guint y;

    guint scenic_left;
    guint scenic_right;
    guint scenic_up;
    guint scenic_down;
} Tree;

bool is_visible_from_left(GArray *input, Tree *tree){
    if (tree->x == 0) {
        return true;
    }
    gchar *row = g_array_index(input, gchar *, tree->y);
    for (int i = tree->x - 1; i >= 0; --i){
        tree->scenic_left += 1;
        if(row[i] >= row[tree->x] ){
            return false;
        }
    }
    return true;
}

bool is_visible_from_right(GArray *input, Tree *tree){
    gchar *row = g_array_index(input, gchar *, tree->y);
    guint max_x = strlen(row) - 1;
    if (tree->x == max_x) {
        return true;
    }
    for (guint i = tree->x + 1; i <= max_x; ++i){
        tree->scenic_right += 1;
        if(row[i] >= row[tree->x] ){
            return false;
        }
    }
    return true;
}

bool is_visible_from_above(GArray *input, Tree *tree){
    if (tree->y == 0) {
        return true;
    }
    gchar *row = g_array_index(input, gchar *, tree->y);
    for (int i = tree->y - 1; i >= 0; --i){
        tree->scenic_up += 1;
        gchar *row_above = g_array_index(input, gchar *, i);
        if (row[tree->x] <= row_above[tree->x]) {
            return false;
        }
    }
    return true;
}

bool is_visible_from_below(GArray *input, Tree *tree){
    guint max_y = input->len - 1;
    if (tree->y == max_y) {
        return true;
    }
    gchar *row = g_array_index(input, gchar *, tree->y);
    for (guint i = tree->y + 1; i <= max_y; ++i){
        gchar *row_below = g_array_index(input, gchar *, i);
        tree->scenic_down += 1;
        if (row[tree->x] <= row_below[tree->x]) {
            return false;
        }
    }
    return true;
}

int part1(GArray *input){
    int count = 0;
    for (guint y = 0; y < input->len; ++y){
        gchar *row = g_array_index(input, gchar *, y);
        for (guint x = 0; x < strlen(row); ++x) {
            Tree tree = { x, y, 0, 0, 0, 0 };
            if(is_visible_from_left(input, &tree) ||
               is_visible_from_right(input, &tree) ||
               is_visible_from_above(input, &tree) ||
               is_visible_from_below(input, &tree)
            ){
                ++count;
            }
        }
    }
    return count;
}

guint part2(GArray *input){
    guint most_scenic = 0;
    for (guint y = 0; y < input->len; ++y){
        gchar *row = g_array_index(input, gchar *, y);
        for (guint x = 0; x < strlen(row); ++x) {
            Tree tree = { x, y, 0, 0, 0, 0 };

            is_visible_from_left(input, &tree);
            is_visible_from_right(input, &tree);
            is_visible_from_above(input, &tree);
            is_visible_from_below(input, &tree);

            guint score = tree.scenic_left * tree.scenic_right * tree.scenic_up * tree.scenic_down;
            if (score > most_scenic) {
                most_scenic = score;
            }
        }
    }
    return most_scenic;
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
