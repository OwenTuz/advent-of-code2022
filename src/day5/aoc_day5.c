
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "aoc_utils.h"

void move_stacks(GArray *stacks, GArray *moves, bool part2){
    for (guint i = 0; i < moves-> len; ++i){
        gchar *line = g_array_index(moves, gchar *, i);

        char *token = strtok(line, " ");
        guint64 parsed = 0;
        guint64 num = 0;
        guint64 from = 0;
        guint64 to = 0;

        while (token != NULL) {
            switch(parsed) {
                case 5:
                    to = g_ascii_strtoull(token, NULL, 10) - 1;
                    break;
                case 3:
                    from = g_ascii_strtoull(token, NULL, 10) - 1;
                    break;
                case 1:
                    num = g_ascii_strtoull(token, NULL, 10);
                    break;
            }
            parsed += 1;
            token = strtok(NULL, " ");
        }
        aoc_GCharQueue *from_queue = g_array_index(stacks, aoc_GCharQueue *, from);
        aoc_GCharQueue *to_queue = g_array_index(stacks, aoc_GCharQueue *, to);
        if (!part2) {
            for (guint64 i = 0; i < num; ++i){
                gchar c = aoc_g_char_queue_pop(from_queue);
                aoc_g_char_queue_push(to_queue, c);
            }
        } else {
            aoc_GCharQueue *holding = aoc_new_g_char_queue();
            for (guint64 i = 0; i < num; ++i){
                gchar c = aoc_g_char_queue_pop(from_queue);
                aoc_g_char_queue_push(holding, c);
            }
            for (guint i = 0; i < num; ++i){
                gchar c = aoc_g_char_queue_pop(holding);
                aoc_g_char_queue_push(to_queue, c);
            }
        }
    }
}

char * part1(GArray *stacks, GArray *moves){
    move_stacks(stacks, moves, false);
    char *result = malloc(stacks->len);
    if (!result) {
        printf("FATAL: malloc failed for result string");
        exit(1);
    }
    for (guint i = 0; i < stacks-> len; ++i){
        aoc_GCharQueue *stack = g_array_index(stacks, aoc_GCharQueue *, i);
        gchar val = aoc_g_char_queue_pop(stack);
        strncat(result, &val, 1);
    }
    assert(result[stacks->len] == NULL);
    return result;
}

char * part2(GArray *stacks, GArray *moves){
    move_stacks(stacks, moves, true);
    char *result = malloc(stacks->len);
    if (!result) {
        printf("FATAL: malloc failed for result string");
        exit(1);
    }
    for (guint i = 0; i < stacks-> len; ++i){
        aoc_GCharQueue *stack = g_array_index(stacks, aoc_GCharQueue *, i);
        gchar val = aoc_g_char_queue_pop(stack);
        strncat(result, &val, 1);
    }
    assert(result[stacks->len] == NULL);
    return result;
}

GArray * parse_stacks(GArray *input){
    GArray *rows = g_array_new(TRUE, FALSE, sizeof(gchar *));
    guint64 max_col = 0;
    while (max_col == 0) {
        gchar *line = g_array_index(input, gchar *, 0);
        if (line[0] == '1'){
            max_col = g_ascii_strtoull(&line[strlen(line)-1], NULL, 10);
        } else {
            g_array_append_val(rows, line);
        }
        g_array_remove_index(input, 0);
    }
    // get rid of blank line too
    g_array_remove_index(input, 0);

    GArray *stacks = g_array_new(TRUE, FALSE, sizeof(aoc_GCharQueue *));
    for (guint i = 0; i < max_col; ++i){
        aoc_GCharQueue *stack = aoc_new_g_char_queue();
        g_array_append_val(stacks, stack);
    }
    assert(stacks->len == max_col);

    for (guint c = 1; c <= rows -> len; ++c){
        guint i = rows-> len - c;
        gchar *line = g_array_index(rows, gchar *, i);
        guint n = 1; // hacky var to find the location of a char in string
        for (guint col = 0; col <= stacks->len - 1; ++col){
            GArray *arr = g_array_index(stacks, GArray *, col);
            gchar val = line[n];
            n += 4;
            if (val == ' ') {
                continue;
            }
            aoc_g_char_queue_push(arr, val);
        }
    }
    g_array_free(rows, TRUE);
    return stacks;
}

int main(int argc, char **argv) {
    char *filename;
    if (argc > 1) {
        filename = g_strdup(argv[1]);
    } else {
        filename = g_strdup("./input");
    }
    // copy input twice, because the functions we use do Bad Things and mutate their inputs
    GArray *input_part1 = get_input_lines(filename);
    GArray *input_part2 = get_input_lines(filename);

    g_free(filename);
    if (!input_part1) exit(1);
    if (!input_part2) exit(1);

    GArray *stacks_part1 = parse_stacks(input_part1);
    // input now contains the 'move' instructions
    printf("Part 1: Answer is %s\n", part1(stacks_part1, input_part1));
    GArray *stacks_part2 = parse_stacks(input_part2);
    printf("Part 2: Answer is %s\n", part2(stacks_part2, input_part2));
    return 0;
}
