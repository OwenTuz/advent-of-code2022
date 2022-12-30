#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "aoc_utils.h"

int tick_gen(GArray *input, const int yield_at, bool clean_start){
    static guint rip; // instruction pointer
    static int reg_x; // x register
    static int tick;
    static int ticks_left;

    int return_val;

    if (clean_start) {
        rip = 0;
        reg_x = 1;
        tick = 0;
        ticks_left = 0;
    }

    if (tick > yield_at){
        printf("Can't yield value for tick %d, function state is already at %d. Maybe pass clean_start=TRUE to reset\n", yield_at, tick);
        exit(1);
    }

    while (rip < input->len && tick < yield_at){
        return_val = reg_x;
        gchar *orig_line = g_array_index(input, gchar *, rip);
        gchar *line = strndup(orig_line, strlen(orig_line) + 1);
        strtok(line, " ");
        char *val = strtok(NULL, " ");
        if (!val) {
            // no value: instruction = noop
            if (ticks_left != 0) {
                // Something Has Gone Wrong
                printf("Error: trying to resume in the middle of atomic instruction noop\n");
                exit(1);
            }
            ++tick;
            ++rip;
            free(line);
        } else {
            // addx instruction
            if (ticks_left == 0) {
                ticks_left = 2;
            }
            while (ticks_left > 0){
                if (tick == yield_at) {
                    break;
                }
                ++tick;
                --ticks_left;
            }
            if (ticks_left == 0){
                reg_x += atoi(val);
                ++rip;
            }
            free(line);
        }
        if (tick < yield_at && rip == input->len) {
            printf("FATAL: Reached end of instructions without yielding a value\n");
            exit(1);
        }
    }
    if (tick != yield_at){
        printf("Reached end of instruction loop but not yielding at correct value. Rip: %d, tick: %d, reg_x: %d\n", rip, tick, reg_x);
        exit(1);
    }
   return return_val;
}

int part1(GArray *input){
    return (20 * tick_gen(input, 20, TRUE)) +
           (60 * tick_gen(input, 60, FALSE)) +
           (100 * tick_gen(input, 100, FALSE)) +
           (140 * tick_gen(input, 140, FALSE)) +
           (180 * tick_gen(input, 180, FALSE)) +
           (220 * tick_gen(input, 220, FALSE));
}

void part2(GArray *input){
    char *grid[6] = {
        strndup("                                        ", 41),
        strndup("                                        ", 41),
        strndup("                                        ", 41),
        strndup("                                        ", 41),
        strndup("                                        ", 41),
        strndup("                                        ", 41),
    };
    for (int vpos = 0; vpos < 6; ++vpos){
        for (int hpos = 0; hpos < 40; ++hpos){
            bool clean_start = (vpos == 0 && hpos == 0);
            int current_pixel = tick_gen(input, (vpos * 40) + hpos + 1, clean_start);
            if (hpos - 1 <= current_pixel && current_pixel <= hpos + 1){
                grid[vpos][hpos] = '#';
            }
        }
        printf("%s\n", grid[vpos]);
        free(grid[vpos]);
    }
    return;
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
    printf("Part 2:\n");
    part2(input);
    g_array_free(input, TRUE);
    return 0;
}
