#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
