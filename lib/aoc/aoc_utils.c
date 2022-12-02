#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "aoc_utils.h"

GArray *get_input_lines(char *filename){
    FILE *fp;
    errno = 0;

    if (!(fp = fopen(filename, "r"))) {
        printf("Error opening file '%s': %s \n", filename, strerror(errno));
        return NULL;
    }

    GArray *input = g_array_new(TRUE, FALSE, sizeof(char *));
    char line[4096];
    gchar *input_line;

    while (fgets(line, sizeof(line), fp)) {
        input_line = g_strstrip(g_strdup(line));
        g_array_append_val(input, input_line);
    }
    fclose(fp);
    return input;
}

/*
int_compare is a qsort-compatible function for comparison
Accepts:
    const void* a: First array element for comparison
    const void* b: Second array element for comparison
Returns:
    < 0 if a < b
      0 if a == b
    > 0 if a > b
*/
int int_compare(const void *a, const void *b){
    int diff;
    const int *si_a = (const int*)a;
    const int *si_b = (const int*)b;
    if (__builtin_ssub_overflow(*si_a, *si_b, &diff)) {
        printf("Error comparing %d and %d: would overflow", *si_a, *si_b);
        exit(1);
    } else {
        return *si_a - *si_b;
    }
}
