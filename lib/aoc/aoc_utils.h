#ifndef __AOC_UTILS_H__
#define __AOC_UTILS_H__

#include <glib.h>
#include "aoc_queue.h"

GArray *get_input_lines(char *);
int int_compare(const void *, const void*);

void aoc_string_slice(const gchar *, gchar *, size_t, size_t);
#endif
