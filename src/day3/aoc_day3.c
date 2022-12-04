#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

#include "aoc_utils.h"

int get_value_of_char(char c){
    int offset;
    if( isupper(c) ){
        offset = 38;
    } else {
        offset = 96;
    }
    return ((int) c - offset);
}

int get_duplicate_value(char *line){
        char *bitmap = malloc(53);
        memset(bitmap, 0, (53 * sizeof(char)));
        size_t len = strlen(line);
        for (size_t i = 0; i < len / 2; ++i) {
            int val = get_value_of_char(line[i]);
            bitmap[val] = 1;
        }
        for (size_t i = len / 2; i < len; ++i) {
            int val = get_value_of_char(line[i]);
            if(bitmap[val]){
                free(bitmap);
                return val;
            }
        }
        free(bitmap);
        printf("Error finding duplicate value, didn't return anything!");
        exit(1);
}

int part1(GArray *input){
    int total = 0;
    for (guint i = 0; i < input->len; ++i) {
        gchar *line = g_array_index(input, char *, i);
        total += get_duplicate_value(line);
    }
    return total;
}

char* bitset(char *line){
        char *bitset = malloc(53);
        memset(bitset, 0, 53);
        size_t len = strlen(line);
        for (size_t i = 0; i < len; ++i) {
            int val = get_value_of_char(line[i]);
            if(bitset[val] == 0) { bitset[val] = 1; }
        }
        return bitset;
}

int part2(GArray *input){
    int total = 0;
    for (guint i = 0; i < input->len; i+=3) {
        gchar *line1 = g_array_index(input, char *, i);
        gchar *line3 = g_array_index(input, char *, i + 2);
        gchar *line2 = g_array_index(input, char *, i + 1);

        char *set1 = bitset(line1);
        char *set2 = bitset(line2);

        size_t len = strlen(line3);
        for (size_t i = 0; i < len; ++i) {
            int val = get_value_of_char(line3[i]);
            if (set1[val] == 1 && set2[val] == 1) {
                total += val;
                free(set1);
                free(set2);
                break;
            }
        }
    }
    return total;
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

    assert(get_value_of_char('a') == 1);
    assert(get_value_of_char('z') == 26);
    assert(get_value_of_char('A') == 27);
    assert(get_value_of_char('Z') == 52);
    assert(get_value_of_char('L') == 38);
    assert(get_value_of_char('p') == 16);

    assert(get_duplicate_value("aa") == 1);
    assert(get_duplicate_value("blaLBa") == 1);
    assert(get_duplicate_value("jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL") == 38);
    assert(get_duplicate_value("vJrwpWtwJgWrhcsFMMfFFhFp") == 16);
    assert(get_duplicate_value("PmmdzqPrVvPwwTWBwg") == 42);
    assert(get_duplicate_value("wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn") == 22);
    assert(get_duplicate_value("ttgJtRGJQctTZtZT") == 20);
    assert(get_duplicate_value("CrZsJsPPZsGzwwsLwLmpwMDw") == 19);
    assert(get_duplicate_value("ftBjzLtptRWdvZlQQZQf") == 6);

    printf("Part 1: Answer is %d\n", part1(input));
    printf("Part 2: Answer is %d\n", part2(input));
    g_array_free(input, TRUE);
    return 0;
}
